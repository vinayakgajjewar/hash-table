/*
 * TODO
 * I'm just guessing what these values should be. I'm using them in the
 * ht_get_hash function.
 */
const int HT_PRIME_1 = 131;
const int HT_PRIME_2 = 257;

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