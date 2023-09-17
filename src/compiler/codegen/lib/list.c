#include <stdint.h>
#include <stdlib.h>

typedef uint64_t Ptr;

typedef struct list {
    Ptr **items;
    int length;
} list;

list *list_new() {
    list *l = malloc(sizeof(list));
    l->items = malloc(sizeof(Ptr *));
    l->items[0] = 0;
    l->length = 0;
    return l;
}

void list_push(list *l, void *data) {
    l->length++;
    if (l->length % 2) {
        l->items = realloc(l->items, sizeof(Ptr *) * l->length);
    }
    l->items[l->length - 1] = data;
}

void *list_get(list *l, int index) {
    return l->items[index];
}

int list_size(list *l) {
    return l->length;
}

int list_empty(list *l) {
    return l->length == 0;
}

void *list_pop(list *l) {
    void *data = l->items[l->length - 1];
    l->length--;
    return data;
}