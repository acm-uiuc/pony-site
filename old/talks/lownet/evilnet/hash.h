/*
 * 
 *	This is free software. You can redistribute it and/or modify under
 *	the terms of the GNU General Public License version 2.
 *
 * 	Copyright (C) 1998 by kra
 * 
 */
#ifndef __HASH_H
#define __HASH_H

#ifdef _REENTRANT
#include <pthread.h>
#endif

#define HASH_MIN_ITEMS		131
#define HASH_MAX_PCT_FULL	80
#define HASH_SPACE_PCT_INC	100

#if 0
#define HASH_FUNC_2
#endif

typedef int (*hash_equal_func)(unsigned int key, void *data_ht, void *arg);

struct hash_table_item {
	unsigned int ht_key;
	void 	     *ht_data;
};

struct hash {
	int h_items;
	int h_max_items;
	int h_space_after;
	struct hash_table_item *h_table;
	hash_equal_func h_eqfunc;
#ifdef _REENTRANT
	int h_locked;
	pthread_t h_locked_thr;
	pthread_mutex_t h_mutex;
#endif
};

struct hash_iterator {
	struct hash *i_hash;
	int 	    i_pos;
};


int  hash_init(struct hash *h, int max_items, hash_equal_func eqfunc);

void *hash_get(struct hash *h, unsigned int key, void *arg);
void *hash_remove(struct hash *h, unsigned int key, void *arg);
int  hash_put(struct hash *h, unsigned int key, void *data);
int  hash_put_check(struct hash *h, unsigned int key, void *data, void *arg);

void hash_lock(struct hash *h);
void hash_unlock(struct hash *h);
int  hash_count(struct hash *h);

void hash_free(struct hash *h);

/*
 * hash_iter
 */
void hash_iter_set(struct hash_iterator *i, struct hash *h);
void *hash_iter_get(struct hash_iterator *i, unsigned int *keyptr);
void hash_iter_end(struct hash_iterator *i);

void hash_iter_lock(struct hash_iterator *i);
void hash_iter_unlock(struct hash_iterator *i);


#endif
