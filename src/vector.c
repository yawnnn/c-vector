#include <stdlib.h>
#include <string.h>
#include "vector.h"

#define GROWTH_FACTOR   (2UL)

/* ======================================================================================== */
/*                                     PRIVATE METHODS                                      */
/* ======================================================================================== */

inline static char *v_at(Vec *v, size_t pos)
{
    return v->ptr + (pos * v->size);
}

inline static void v_set(Vec *v, size_t pos, size_t n)
{
    memset(v_at(v, pos), 0, n * v->size);
}

inline static void v_cpy(Vec *v, size_t pos, void *source, size_t n)
{
    memcpy(v_at(v, pos), source, n * v->size);
}

inline static void v_cpy_to(void *dest, Vec *v, size_t pos, size_t n)
{
    memcpy(dest, v_at(v, pos), n * v->size);
}

inline static void v_move(Vec *v, size_t pos, void *source, size_t n)
{
    memmove(v_at(v, pos), source, n * v->size);
}

inline static int v_cmp(Vec *v, size_t pos, void *ptr2, size_t n)
{
    return memcmp(v_at(v, pos), ptr2, n * v->size);
}

inline static void v_alloc(Vec *v, size_t n)
{
    v->ptr = malloc(n * v->size);
    v->cap = n;
}

inline static void v_realloc(Vec *v, size_t n)
{
    v->ptr = realloc(v->ptr, n * v->size);
    v->cap = n;
}

/* if shrink, realloc by exact number
 * if grow  , realloc by GROWTH_FACTOR when possible, otherwise exact number */
static void v_resize(Vec *v, size_t n) 
{
    if (v->cap) {
        if (n < v->cap || n > v->cap * GROWTH_FACTOR)
            v_realloc(v, n);
        else if (n > v->cap)
            v_realloc(v, v->cap * GROWTH_FACTOR);
    }
    else {
        v_alloc(v, n > GROWTH_FACTOR ? n : GROWTH_FACTOR);
    }
}

/* ======================================================================================== */
/*                                      PUBLIC METHODS                                      */
/* ======================================================================================== */

/* sets the variables for vector with elements of <size>. The vector is still NULL */
void vec_new(Vec *v, size_t size) 
{
    v->cap = 0;
    v->len = 0;
    v->size = size;
}

/* new vector of elements of <size>, with capacity <n>
 * the elements are not initialized and length is 0 */
void vec_new_with(Vec *v, size_t size, size_t n)
{
    vec_new(v, size);
    vec_reserve(v, n);
}

/* new vector with <n> elements of <size>. everything memset to 0 */
void vec_init(Vec *v, size_t size, size_t n)
{
    vec_new_with(v, n, size);
    v_set(v, 0, n);
    v->len = n;
}

/* new vector from a c-style array <arr> with <n> elements of <size> */
void vec_from(Vec *v, size_t size, void *arr, size_t n)
{
    vec_new(v, size);
    vec_insert_n(v, arr, 0, n);
}

/* clear variables, release memory. 
 * if necessary, the single elements need to be freed by the user before this */
void vec_clear(Vec *v) 
{
    if (v->cap)
        free(v->ptr);
    v->cap = 0;
    v->len = 0;
}

/* reserve memory for at least <n> number of elements */
void vec_reserve(Vec *v, size_t n)
{
    if (n > v->cap)
        v_resize(v, n);
}

/* ensures the memory allocated is exactly as needed for the length */
void vec_shrink_to_fit(Vec *v)
{
    if (v->cap)
        v_resize(v, v->len);
}

/* insert <elem> at the end of the vector */
void vec_push(Vec *v, void *elem) 
{
    vec_insert(v, elem, v->len);
}

/* insert <elem> at <pos> */
void vec_insert(Vec *v, void *elem, size_t pos)
{
    vec_insert_n(v, elem, 1, pos);
}

/* insert <n> <elems> starting from <pos> */
void vec_insert_n(Vec *v, void *elems, size_t n, size_t pos)
{
    if (pos <= v->len) {
        vec_reserve(v, v->len + n);
        v_move(v, pos + n, v_at(v, pos), v->len - pos);
        v_cpy(v, pos, elems, n);
        v->len += n;
    }
}

/* remove element from the end of the array
 * if <elem> != NULL the element is copied to it, so that memory it owns can be freed by the caller */
void vec_pop(Vec *v, void *elem) 
{
    if (v->len) {
        if (elem)
            v_cpy_to(elem, v, v->len - 1, 1);
        v->len--;
    }
}

/* remove the element at <pos>. 
 * if <elem> != NULL the element is copied to it, so that memory it owns can be freed by the caller */
void vec_remove(Vec *v, size_t pos, void *elem) 
{
    vec_remove_n(v, pos, elem, 1);
}

/* remove the <n> elements starting at <pos>. 
 * if <elems> != NULL the elements are copied to it, so that memory they own can be freed by the caller */
void vec_remove_n(Vec *v, size_t pos, void *elems, size_t n)
{
    if (pos + n - 1 < v->len) {
        if (elems)
            v_cpy_to(elems, v, pos, n);
        if (pos + n < v->len)
            v_move(v, pos, v_at(v, pos + n), v->len - (pos + n));
        v->len -= n;
    }
}

/* return element at <pos> in <elem> */
void vec_get(Vec *v, size_t pos, void *elem)
{
    if (pos < v->len)
        v_cpy_to(elem, v, pos, 1);
}

/* set element at <pos> equal to <elem> */
void vec_set(Vec *v, void *elem, size_t pos) 
{
    if (pos < v->len)
        v_cpy(v, pos, elem, 1);
}

/* returns the underlying pointer. 
 * if changes to the vector are made, this pointer can become invalid */
inline void *vec_data(Vec *v)
{
    if (v->len)
        return (void *)v->ptr;
    return NULL;
}

/* pointer to element at <pos>. 
 * if changes to the vector are made, this pointer can become invalid */
inline void *vec_at(Vec *v, size_t pos)
{
    if (pos < v->len)
        return v_at(v, pos);
    return NULL;
}

/* length of vector, number of accessible elements */
inline size_t vec_len(Vec *v) 
{
    return v->len;
}

/* capacity of vector, the number of elements it is allocated for (not bytes) */
inline size_t vec_capacity(Vec *v)
{
    return v->cap;
}

/* swap elements at pos <pos1> and <pos2>
 * for simplicity, a pointer to a element <tmp> is required */
void vec_swap(Vec *v, size_t pos1, size_t pos2, void *tmp)
{
    v_cpy_to(tmp, v, pos1, 1);
    v_cpy(v, pos1, v_at(v, pos2), 1);
    v_cpy(v, pos2, tmp, 1);
}

/* sort in <order> [VEC_SORT_ASC|VEC_SORT_DESC] */
/* TODO --- Better algorithm */
void vec_sort(Vec *v, int order)
{
    int i, j;
    char val[256];
    void *pval;
    bool alloc;

    if (v->size < sizeof(val)) {
        pval = val;
        alloc = NO;
    }
    else {
        pval = malloc(v->size);
        alloc = YES;
    }

    for (i = 0; i < v->len; i++) {
        for (j = i + 1; j < v->len; j++) {
            if (v_cmp(v, i, v_at(v, j), 1) * order > 0)
                vec_swap(v, i, j, pval);
        }
    }

    if (alloc)
        free(pval);
}

/* iterate over elements of array.
 * call first with vec_iter_reset() to reset the interal counter
 * read element in <elem>, returns NO if it's done */
bool vec_iter(Vec *v, void *elem) 
{
    static size_t i = 0;

    if (v) {
        if (i < v->len) {
            vec_get(v, i++, elem);
            return YES;
        }
    } else {
        i = 0;
    }

    return NO;
}