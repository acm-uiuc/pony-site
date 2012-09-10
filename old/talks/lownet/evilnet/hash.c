/*
 * 
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU General Public License version 2.
 *
 * 	Copyright (C) 1998 by kra
 * 
 */
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "hash.h"


static int __chose_table_size(int size)
{
	static int ht_size[] = {
		37, 47, 79, 97, 163, 197, 331, 397, 673, 797, 1361, 1597,
		2729, 3203, 5471, 6421, 10949, 12853, 21911, 25717, 43853,
		51437, 87719, 102877, 175447, 205759, 350899, 411527, 701819,
		823117, 1403641, 1646237, 2807303, 3292489, 5614657, 6584983,
		11229331, 13169977, 22458671, 26339969, 44917381, 52679969,
		89834777, 105359939
	};
	int i;
	
	if (size >= ht_size[sizeof(ht_size) / sizeof(ht_size[0]) - 1])
	    return ht_size[sizeof(ht_size) / sizeof(ht_size[0])];
	for (i = 0; ht_size[i] < size; i++)
	    ;
	return ht_size[i];
}

static inline int __when_need_space(int max_items)
{
	return (HASH_MAX_PCT_FULL * max_items) / 100;
}

static inline int __hash_need_space(struct hash *h)
{
	return h->h_items >= h->h_space_after ? 1 : 0;
//	return (h->h_items * 100) / h->h_max_items >= HASH_MAX_PCT_FULL ? 1 : 0;
}

int hash_init(struct hash *h, int max_items, hash_equal_func eqfunc)
{
	if (max_items < HASH_MIN_ITEMS)
		max_items = HASH_MIN_ITEMS;
	max_items = __chose_table_size(max_items);
	h->h_space_after = __when_need_space(max_items);
	h->h_items = 0;
	h->h_max_items = max_items;
	h->h_eqfunc = eqfunc;
	if (!(h->h_table = malloc(max_items * sizeof(struct hash_table_item))))
		return -1;
	memset(h->h_table, 0, max_items * sizeof(struct hash_table_item));
#ifdef _REENTRANT
	h->h_locked = 0;
	pthread_mutex_init(&h->h_mutex, NULL);
#endif
	return 0;
}

void hash_free(struct hash *h)
{
	if (h->h_table)
		free(h->h_table);
	memset(h, 0, sizeof(*h));
}

static unsigned int __hv_hash;
static unsigned int __hv_hash_save;
static unsigned int __hv_max_items;
#ifdef HASH_FUNC_2
static unsigned int __hv_d;
static unsigned int __hv_d_save;
#endif

static inline int __hv(unsigned int key, unsigned int max_items)
{
	__hv_max_items = max_items;
#ifdef HASH_FUNC_2
	__hv_d = 2;
#endif
	return (int)(__hv_hash = key % max_items);
}

static inline int __hv_peek(void)
{
	return __hv_hash;
}

static inline int __hv_next(void)
{
#ifdef HASH_FUNC_2
	__hv_hash = (__hv_hash + __hv_d) % __hv_max_items;
	__hv_d += 2;
	return (int)__hv_hash;
#else
	return __hv_hash = (__hv_hash + 1) % __hv_max_items;
#endif
}

static inline int __hv_peek_next(void)
{
#ifdef HASH_FUNC2
	return (int)((__hv_hash + __hv_d) % __hv_max_items);
#else
	return (__hv_hash + 1) % __hv_max_items;
#endif
}

static inline void __hv_save_position(void)
{
#ifdef HASH_FUNC_2
	__hv_d_save = __hv_d;
#endif
	__hv_hash_save = __hv_hash;
}

static inline int __hv_load_position(void)
{
#ifdef HASH_FUNC_2
	__hv_d = __hv_d_save;
#endif
	return (int)(__hv_hash = __hv_hash_save);
}


static int __hash_find(struct hash *h, unsigned int key, void *arg)
{	
	int retval;
	int hv;
	struct hash_table_item *h_table;
	
	hv = __hv(key, h->h_max_items);
	h_table = h->h_table;
	retval = -1;
	
	while (h_table[hv].ht_data) {
		if (h_table[hv].ht_key == key) {
			if (!h->h_eqfunc || 
			    h_table[__hv_peek_next()].ht_data == NULL) {
				retval = hv;
				break;
			} else {
				if (h->h_eqfunc(key, h_table[hv].ht_data, arg)) {
					retval = hv;
					break;
				}
			}
		}
		hv = __hv_next();
	}
	return retval;
}

static void __hash_put(struct hash *h, unsigned int key, void *data)
{
	int hv;
	struct hash_table_item *h_table;
	
	hv = __hv(key, h->h_max_items);
	h_table = h->h_table;

	while (h_table[hv].ht_data) {
		hv = __hv_next();
	}
	h_table[hv].ht_key = key;
	h_table[hv].ht_data = data;
	h->h_items++;
}

static int __hash_put_check(struct hash *h, unsigned int key,
			     void *data, void *arg)
{
	int retval;
	int hv;
	struct hash_table_item *h_table = h->h_table;
	
	retval = __hash_find(h, key, arg);
	if (retval < 0) {
		hv = __hv_peek();
		h_table[hv].ht_key = key;
		h_table[hv].ht_data = data;
		return 0;
	} else
		return -1;
}

static void __hash_remap(struct hash *h, int hv)
{
	int count, i;
	struct hash_table_item *h_table, *ht_save;
	
	__hv_save_position();
	h_table = h->h_table;
	count = 0;
	while (h_table[hv].ht_data) {
		count++;
		hv = __hv_next();
	}
	if (count) {
		ht_save = alloca(count * sizeof(struct hash_table_item));
		hv = __hv_load_position();
		i = 0;
		while (h_table[hv].ht_data) {
			ht_save[i].ht_key = h_table[hv].ht_key;
			ht_save[i].ht_data = h_table[hv].ht_data;
			h_table[hv].ht_key = 0;
			h_table[hv].ht_data = NULL;
			i++;
			hv = __hv_next();
		}
		h->h_items -= count;
		for (i = 0; i < count; i++)
			__hash_put(h, ht_save[i].ht_key, ht_save[i].ht_data);
	}
}

static void __hash_new_space(struct hash *h)
{
	struct hash_table_item *old_h_table;
	int old_items, old_max_items;
	int i;

	old_items = h->h_items;
	old_max_items = h->h_max_items;
	old_h_table = h->h_table;
	
	h->h_max_items = __chose_table_size(h->h_max_items + 
		(HASH_SPACE_PCT_INC * h->h_max_items) / 100);
	if (!(h->h_table = malloc(h->h_max_items * sizeof(struct hash_table_item)))) {
		h->h_items = old_items;
		h->h_max_items = old_max_items;
		h->h_table = old_h_table;
		return;
	}
	h->h_space_after = __when_need_space(h->h_max_items);
	memset(h->h_table, 0, h->h_max_items * sizeof(struct hash_table_item));
	h->h_items = 0;
	
	for (i = 0; i < old_max_items; i++) {
		if (old_h_table[i].ht_data)
			__hash_put(h, old_h_table[i].ht_key, old_h_table[i].ht_data);
	}
	free(old_h_table);
}

static inline void __lock(struct hash *h)
{
#ifdef _REENTRANT
	if (!h->h_locked || h->h_locked_thr != pthread_self())
		pthread_mutex_lock(&h->h_mutex);
#endif
}

static inline void __unlock(struct hash *h)
{
#ifdef _REENTRANT
	if (!h->h_locked || h->h_locked_thr != pthread_self())
		pthread_mutex_unlock(&h->h_mutex);
#endif
}

void *hash_get(struct hash *h, unsigned int key, void *arg)
{
	int hv;
	void *retval;

	__lock(h);
	if ((hv = __hash_find(h, key, arg)) >= 0)
		retval = h->h_table[hv].ht_data;
	else
		retval = NULL;
	__unlock(h);
	return retval;
}

void *hash_remove(struct hash *h, unsigned int key, void *arg)
{
	void *retval;
	int hv;
	
	__lock(h);
	if ((hv = __hash_find(h, key, arg)) >= 0) {
		retval = h->h_table[hv].ht_data;
		h->h_table[hv].ht_key = 0;
		h->h_table[hv].ht_data = NULL;
		h->h_items--;
		__hash_remap(h, __hv_next());
	} else
		retval = NULL;
	__unlock(h);
	return retval;
}

int hash_put(struct hash *h, unsigned int key, void *data)
{
	int retval;
	
	__lock(h);
	if (__hash_need_space(h))
		__hash_new_space(h);
	if (h->h_items == h->h_max_items || !data)
		retval = -1;
	else
		retval = 0;
	__hash_put(h, key, data);
	__unlock(h);
	return retval;
}

int hash_put_check(struct hash *h, unsigned int key, void *data, void *arg)
{
	int retval;
	
	__lock(h);
	if (__hash_need_space(h))
		__hash_new_space(h);
	if (h->h_items == h->h_max_items || !data)
		retval = -1;
	else
		retval = __hash_put_check(h, key, data, arg);
	__unlock(h);
	return retval;
}


int hash_count(struct hash *h)
{
	int retval;
	
	__lock(h);
	retval = h->h_items;
	__unlock(h);
	return retval;
}

void hash_lock(struct hash *h)
{
#ifdef _REENTRANT
	if (!h->h_locked || h->h_locked_thr != pthread_self()) {
		pthread_mutex_lock(&h->h_mutex);
		h->h_locked_thr = pthread_self();
		h->h_locked = 1;
	} else
		h->h_locked++;
#endif
}

void hash_unlock(struct hash *h)
{
#ifdef _REENTRANT
	if (--h->h_locked == 0)
		pthread_mutex_unlock(&h->h_mutex);
#endif
}

/*
 * 
 * hash_iterator
 * 
 */

void hash_iter_set(struct hash_iterator *i, struct hash *h)
{
	i->i_hash = h;
	i->i_pos = 0;
}

void hash_iter_lock(struct hash_iterator *i)
{
	hash_lock(i->i_hash);
}

void hash_iter_unlock(struct hash_iterator *i)
{
	hash_unlock(i->i_hash);
}

void *hash_iter_get(struct hash_iterator *i, unsigned int *keyptr)
{
	struct hash *h;
	void *data;
	void *retval;
	
	h = i->i_hash;
	__lock(h);
	for ( ;i->i_pos < h->h_max_items; i->i_pos++) {
		data = h->h_table[i->i_pos].ht_data;
		if (data)
			break;
	}
	if (i->i_pos < h->h_max_items && data) {
		retval = data;
		if (keyptr)
			*keyptr = h->h_table[i->i_pos].ht_key;
	} else
		retval = NULL;
	i->i_pos++;
	__unlock(h);
	return retval;
}

void hash_iter_end(struct hash_iterator *i)
{
	i->i_hash = NULL;
	i->i_pos = 0;
}


#ifdef TEST
/*
 * 
 * Test
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <assert.h>
#include <alloca.h>

int MAX_ITEMS = 1000;

static int verbose = 0;

static int verbose_print(int level, char *format, ...)
{
	va_list ap;
	int retval;
	
	if (verbose >= level) {
		va_start(ap, format);
		retval = vprintf(format, ap);
		va_end(ap);
		return retval;
	}
	return 0;
}


int main(int argc, char *argv[])
{
	struct hash h;
	struct hash_iterator hi;
	int i;
	int *t, *t_arr;
	
	while ((i = getopt(argc, argv, "vi:")) != -1) {
		switch (i) {
		    case 'v':
			++verbose;
			break;
		    case 'i':
			MAX_ITEMS = atoi(optarg);
			break;
		    default:
			fprintf(stderr, "bad option\n");
			exit(1);
		}
	}
	
	assert(t_arr = malloc(MAX_ITEMS * sizeof(int)));
	hash_init(&h, 100, NULL);
	
	verbose_print(1, "start\n");
	for (i = 0; i < MAX_ITEMS; i++) {
		t = t_arr + i;
		*t = i;
		assert(hash_put(&h, i * i, t) == 0);
	}
	assert(MAX_ITEMS == hash_count(&h));
	
	verbose_print(1, "\nhash get\n");
	for (i = 0; i < MAX_ITEMS; i++) {
		t = hash_get(&h, i * i, NULL);
		assert(t);
		assert(*t == i);
		verbose_print(2, "%d ", *t);
	}
	
	verbose_print(1, "\niterator test\n");
	hash_iter_set(&hi, &h);
	while ((t = hash_iter_get(&hi, NULL)))
		verbose_print(2, "%d ", *t);
	hash_iter_end(&hi);
	
	verbose_print(1, "\nremove test\n");
	for (i = 0; i < MAX_ITEMS; i += 3) {
		t = hash_remove(&h, i * i, NULL);
		assert(t);
		assert(*t == i);
		verbose_print(2, "%d ", *t);
	}
	
	verbose_print(1, "\nhash_put_check test\n");
	for (i = 0; i < MAX_ITEMS; i += 3) {
		t = t_arr + i;
		assert(hash_put_check(&h, i * i, t, NULL) == 0);
		verbose_print(2, "%d ", *t);
	}
	for (i = 0; i < MAX_ITEMS; i += 3) {
		t = t_arr + i;
		assert(hash_put_check(&h, i * i, t, NULL) < 0);
		verbose_print(2, "%d ", *t);
	}
	for (i = 0; i < MAX_ITEMS; i += 3) {
		t = hash_remove(&h, i * i, NULL);
		assert(t);
		assert(*t == i);
		verbose_print(2, "%d ", *t);
	}
	
	verbose_print(1, "\nhash get\n");
	for (i = 0; i < MAX_ITEMS; i++) {
		t = hash_get(&h, i * i, NULL);
		if ((i % 3) == 0)
			assert(t == NULL);
		else {
			assert(t);
			assert(*t == i);
			verbose_print(2, "%d ", *t);
		}
	}
	
	hash_free(&h);
	verbose_print(1, "\nOK\n");
	return 0;
}

#endif
