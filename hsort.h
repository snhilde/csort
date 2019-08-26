#ifndef __HSORT_H_
#define __HSORT_H_

#include <stdlib.h>
#include <string.h>


typedef enum hsort_options {
	/* Sorting methods */
	HSORT_INSERTION_SORT = 1 << 0, /* Insertion sort */
	HSORT_SELECTION_SORT = 1 << 1, /* Selection sort */
	HSORT_MERGE_SORT     = 1 << 2, /* Merge sort */

	/* Sort orders */
	HSORT_ORDER_ASC      = 1 << 3, /* Ascending order */
	HSORT_ORDER_DESC     = 1 << 4, /* Descending order */
} hsort_options_t;

typedef enum hsort_return {
	HSORT_RET_SUCCESS    = 0,      /* Success */
	HSORT_RET_ERROR      = 1 << 0, /* General error */
	HSORT_RET_INVALIDUSE = 1 << 1, /* Invalid usage */
} hsort_return_t;

typedef enum hsort_equality {
	HSORT_GT = 1, /* Greater than */
	HSORT_LT,     /* Less than */
	HSORT_EQ      /* Equal to */
} hsort_equality_t;


typedef hsort_equality_t (*hsort_equality_cb)(void *left, void *right, size_t size, hsort_options_t options);

hsort_return_t hsort_int(void *arr, size_t len, size_t size, hsort_options_t options);
hsort_return_t hsort_uint(void *arr, size_t len, size_t size, hsort_options_t options);
hsort_return_t hsort_str(char *str, hsort_options_t options);
hsort_return_t hsort_custom(void *arr, size_t len, size_t size, hsort_equality_cb cb, hsort_options_t options);


#endif
