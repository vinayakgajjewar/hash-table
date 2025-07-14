#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

typedef struct {
    char *key;
    char *value;
} ht_item;

typedef struct {
    int size;
    int count;

    /*
     * An array of pointers to hash table items.
     */
    ht_item **items;
} ht_hash_table;

static ht_item *ht_new_item(const char *k, const char *v);

ht_hash_table *ht_new();

static void ht_del_item(ht_item *i);

void ht_del_hash_table(ht_hash_table *ht);

static int ht_hash(const char *s, int a, int m);

static int ht_get_hash(const char *s, int num_buckets, int attempt);

void ht_insert(ht_hash_table *ht, const char *key, const char *value);

char *ht_search(ht_hash_table *ht, const char *key);

void ht_delete(ht_hash_table *ht, const char *key);

#endif