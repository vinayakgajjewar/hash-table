#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "hash_table.h"

/*
 * TODO
 * I'm just guessing what these values should be. I'm using them in the
 * ht_get_hash function.
 */
const int HT_PRIME_1 = 131;
const int HT_PRIME_2 = 257;

/*
 * Sentinel value to represent a deleted item.
 */
static ht_item HT_DELETED_ITEM = {NULL, NULL};

static ht_item *ht_new_item(const char *k, const char *v) {
    ht_item *i = malloc(sizeof(ht_item));
    i->key = strdup(k);
    i->value = strdup(v);
    return i;
}

/*
 * Initialize a new hash table. Right now the max size is capped at 53 items.
 */
ht_hash_table *ht_new() {
    ht_hash_table *ht = malloc(sizeof(ht_hash_table));
    ht->size = 53;
    ht->count = 0;

    /*
     * calloc fills the allocated memory with NULL.
     */
    ht->items = calloc(ht->size, sizeof(ht_item *));
    return ht;
}

static void ht_del_item(ht_item *i) {
    free(i->key);
    free(i->value);
    free(i);
}

/*
 * Deallocate all the items in the hash table, and then deallocate the hash
 * table.
 */
void ht_del_hash_table(ht_hash_table *ht) {
    for (int i = 0; i < ht->size; i++) {
        ht_item *item = ht->items[i];
        if (item != NULL) {
            ht_del_item(item);
        }
    }
    free(ht->items);
    free(ht);
}

/*
 * a should be a prime number that is larger than the size of the alphabet.
 * ASCII has an alphabet size of 128, so choose a prime larger than that.
 *
 * m is the number of buckets we have in the hash table.
 *
 * This hash function has two steps:
 * 1. Convert the string into a large integer.
 * 2. Reduce the integer into a fixed range by taking its remainder mod m.
 */
static int ht_hash(const char *s, const int a, const int m) {
    long hash = 0;
    const int len_s = (int) strlen(s);
    for (int i = 0; i < len_s; i++) {
        hash += (long) pow(a, len_s - (i + 1)) * s[i];
        hash = hash % m;
    }
    return (int) hash;
}

/*
 * We'll handle collisions using open addressing with double hashing. We'll use
 * two hash function to calculate the index that an item should be stored at
 * after a given number of attempts.
 */
static int ht_get_hash(const char *s, const int num_buckets, const int attempt) {
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

/*
 * Keep trying different buckets until we find an empty one. When we do, store
 * the data item there.
 */
void ht_insert(ht_hash_table *ht, const char *key, const char *value) {
    ht_item *item = ht_new_item(key, value);
    int index = ht_get_hash(item->key, ht->size, 0);
    ht_item *cur_item = ht->items[index];
    int attempt = 1;
    while (cur_item != NULL) {
        if (cur_item != &HT_DELETED_ITEM) {
            if (strcmp(cur_item->key, key) == 0) {
                ht_del_item(cur_item);
                ht->items[index] = item;
                return;
            }
        }
        index = ht_get_hash(item->key, ht->size, attempt);
        cur_item = ht->items[index];
        attempt++;
    }
    ht->items[index] = item;
    ht->count++;
}

/*
 * Keep trying different buckets until we find the key we're looking for. If we
 * hit an empty bucket, indicate that no value was found.
 */
char *ht_search(ht_hash_table *ht, const char *key) {
    int index = ht_get_hash(key, ht->size, 0);
    ht_item *item = ht->items[index];
    int attempt = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                return item->value;
            }
        }
        index = ht_get_hash(key, ht->size, attempt);
        item = ht->items[index];
        attempt++;
    }
    return NULL;
}

/*
 * If the item to be deleted is part of a collision chain, simply removing it
 * from the table will make finding items in the tail of that chain impossible.
 *
 * Instead, replace it with a pointer to a global hash table item representing a
 * deleted item.
 */
void ht_delete(ht_hash_table *ht, const char *key) {
    int index = ht_get_hash(key, ht->size, 0);
    ht_item *item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                ht_del_item(item);
                ht->items[index] = &HT_DELETED_ITEM;
            }
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    ht->count--;
}