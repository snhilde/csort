#ifndef __HSORT_H_
#define __HSORT_H_

#include <stdlib.h>


enum hsort_options {
	/* Sorting methods */
	HSORT_INSERTION_SORT = 1 << 0, /* Insertion sort */
	HSORT_SELECTION_SORT = 1 << 1, /* Selection sort */
	HSORT_MERGE_SORT     = 1 << 2, /* Merge sort */

	/* Sort orders */
	HSORT_ORDER_ASC      = 1 << 3, /* Ascending order */
	HSORT_ORDER_DESC     = 1 << 4, /* Descending order */
};

enum hsort_equality {
	HSORT_GT = 1, /* Greater than */
	HSORT_LT,     /* Less than */
	HSORT_EQ      /* Equal to */
};


#endif
