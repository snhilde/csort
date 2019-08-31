#ifndef __HSORT_H_
#define __HSORT_H_

#define _GNU_SOURCE /* necessary for qsort_r */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>


typedef enum hsort_options {
	/* Sorting methods */
	HSORT_INSERTION_SORT = 1 << 0, /* Insertion sort */
	HSORT_SELECTION_SORT = 1 << 1, /* Selection sort */
	HSORT_MERGE_SORT     = 1 << 2, /* Merge sort */

	/* Sort orders */
	HSORT_ORDER_ASC      = 1 << 3, /* Ascending order */
	HSORT_ORDER_DESC     = 1 << 4, /* Descending order */

	HSORT_PRINT_BEFORE   = 1 << 5, /* Print array before sorting */
	HSORT_PRINT_AFTER    = 1 << 6, /* Print array after sorting */
} hsort_options_t;

typedef enum hsort_return {
	HSORT_RET_SUCCESS    = 0,      /* Success */
	HSORT_RET_ERROR      = 1 << 0, /* General error */
	HSORT_RET_INVALIDUSE = 1 << 1, /* Invalid usage */
} hsort_return_t;

typedef enum hsort_equality {
	HSORT_LT = -1, /* Less than */
	HSORT_EQ = 0,  /* Equal to */
	HSORT_GT = 1   /* Greater than */
} hsort_equality_t;


typedef hsort_equality_t (*hsort_equality_cb)(const void *left, const void *right, void *size);

hsort_return_t hsort_sort_int_array(void *arr, size_t len, size_t size, hsort_options_t options);
hsort_return_t hsort_sort_uint_array(void *arr, size_t len, size_t size, hsort_options_t options);
hsort_return_t hsort_sort_str(char *str, hsort_options_t options);
hsort_return_t hsort_sort_custom(void *arr, size_t len, size_t size, hsort_equality_cb cb, hsort_options_t options);

void           hsort_print_int_array(void *arr, size_t len, size_t size);
void           hsort_print_uint_array(void *arr, size_t len, size_t size);
void           hsort_print_str(char *str);
hsort_return_t hsort_test(size_t len, size_t size, bool is_signed, hsort_options_t options);


#endif
