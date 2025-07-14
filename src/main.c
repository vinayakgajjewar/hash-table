#include <stdio.h>

#include "hash_table.h"

int main() {
    ht_hash_table *ht = ht_new();

    /*
     * Do some inserts.
     */
    ht_insert(ht, "name", "maya");
    ht_insert(ht, "breed", "labradoodle");

    /*
     * Do some retrievals.
     */
    char *name = ht_search(ht, "name");
    if (name != NULL) {
        printf("name is %s\n", name);
    }
    char *breed = ht_search(ht, "breed");
    if (breed != NULL) {
        printf("breed is %s\n", breed);
    }

    ht_del_hash_table(ht);
}