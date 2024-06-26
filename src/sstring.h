/**
 * @file sstring.h
 */

#ifndef __VSTR_H__
#define __VSTR_H__

#include <stdbool.h>

/**
 * @brief Dynamic string
 */
typedef struct SString {
    char *ptr; /**< underlying c-style string (access through SString_data()) */
    size_t cap; /**< capacity allocated */
    size_t len; /**< length of the SString */
} SString;

/**
 * @brief new SString
 *
 * the SString is not allocated, therefore SString_data() returns NULL
 *
 * @param s SString
 */
void SString_new(SString *s);

/**
 * @brief new SString with reserved space
 *
 * the SString has 0 length but is allocated, therefore SString_data() is valid
 *
 * @param s SString
 * @param len minimum number of characters to reserve memory for
 */
void SString_new_with(SString *s, size_t len);

/**
 * @brief new SString from c-style string
 *
 * @param s SString
 * @param source source c-style string
 */
void SString_from(SString *s, const char *source);

/**
 * @brief clear variables, release memory
 *
 * @param s SString
 */
void SString_clear(SString *s);

/**
 * @brief reserve memory ahead of time
 *
 * @param s SString
 * @param len minimum number of characters to reserve memory for
 */
void SString_reserve(SString *s, size_t len);

/**
 * @brief shrink allocated memory to what is exactly needed for length
 *
 * @param s SString
 */
void SString_shrink_to_fit(SString *s);

/**
 * @brief strcpy for SString
 *
 * @param dest SString
 * @param source source c-style string
 * @return the underlying c-style string of @p dest
 */
char *SString_cpy(SString *dest, const char *source);

/**
 * @brief strncpy for SString
 *
 * @param dest SString
 * @param source source c-style string
 * @param num max number of characters to copy
 * @return the underlying c-style string of @p dest
 */
char *SString_ncpy(SString *dest, const char *source, size_t num);

/**
 * @brief strcat for SString
 *
 * @param dest SString
 * @param source source c-style string
 * @return the underlying c-style string of @p dest
 */
char *SString_cat(SString *dest, const char *source);

/**
 * @brief strncat for SString
 *
 * @param dest SString
 * @param source source c-style string
 * @param num max number of characters to concat
 * @return the underlying c-style string of @p dest
 */
char *SString_ncat(SString *dest, const char *source, size_t num);

/**
 * @brief merge two SString
 *
 * merge @p source into @p dest with c-style string @p sep in between. consumes @p source
 *
 * @param dest SString dest
 * @param source SString source
 * @return the underlying c-style string of @p dest
 */
char *SString_merge(SString *dest, SString *source, const char *sep);

/**
 * @brief return the underlying c-style string, or NULL
 *
 * @param s SString
 * @return c-style string or NULL
 */
inline char *SString_data(SString *s) {
    if (s->cap)  // len could be 0, but still allocated because of the null-terminating character
        return s->ptr;
    return (char *)0;
}

/**
 * @brief if SString is empty
 *
 * @param s SString
 * @return boolean
 */
inline bool SString_is_empty(SString *s) {
    return s->len == 0;
}

/**
 * @brief reset iterator
 */
#define SString_iter_reset() SString_iter(NULL, NULL)

#endif /* __VSTR_H__ */