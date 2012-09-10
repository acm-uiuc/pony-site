/*
 * example1.c 
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

// For hacking sys_call_table
#include <asm-i386/cacheflush.h>  
#include <asm/page.h>  

MODULE_LICENSE("GPL");

#define CALLOFF 100     /* we'll read first 100 bytes of int $0x80*/

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

// function pointer to the original sys_open before it's hijacked
asmlinkage int (*original_sys_open)(const char *, int, int);

// our hijacked sys_open function
asmlinkage int new_sys_open(const char *filename, int flags, int mode)
{
    int ret;
    printk(KERN_ALERT "CALLING SYSOPEN!\n");

    ret = (*original_sys_open)(filename, flags, mode);
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
    printk(KERN_ALERT "Syscall table @ 0x%x\n", sct);
    printk(KERN_ALERT "ORIG: 0x%x \n", original_sys_open);
    printk(KERN_ALERT "addr of __NR_open: 0x%x \n", &sct[__NR_open]);
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

}

module_init(kproxy_init);
module_exit(kproxy_exit);
