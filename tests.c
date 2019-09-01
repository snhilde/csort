#include <stdio.h>
#include <inttypes.h>

#include "hsort.h"


int main(int argc, char *argv[])
{
	hsort_return_t ret;

	ret = hsort_test(20000, sizeof(int), true, HSORT_INSERTION_SORT|HSORT_ORDER_DESC|HSORT_PRINT_TIME);
	if (ret == HSORT_RET_SUCCESS)
		printf("Insertion sort was successful\n");
	else if (ret == HSORT_RET_INVALIDUSE)
		printf("Check arguments\n");
	else
		printf("Insertion sort failed\n");

	ret = hsort_test(20000, sizeof(int), true, HSORT_SELECTION_SORT|HSORT_ORDER_DESC|HSORT_PRINT_TIME);
	if (ret == HSORT_RET_SUCCESS)
		printf("Selection sort was successful\n");
	else if (ret == HSORT_RET_INVALIDUSE)
		printf("Check arguments\n");
	else
		printf("Selection sort failed\n");

	ret = hsort_test(20000, sizeof(int), true, HSORT_MERGE_SORT|HSORT_ORDER_DESC|HSORT_PRINT_TIME);
	if (ret == HSORT_RET_SUCCESS)
		printf("Merge sort was successful\n");
	else if (ret == HSORT_RET_INVALIDUSE)
		printf("Check arguments\n");
	else
		printf("Merge sort failed\n");

	ret = hsort_test(20000, sizeof(int), true, HSORT_MERGE_SORT_RECURSE|HSORT_ORDER_DESC|HSORT_PRINT_TIME);
	if (ret == HSORT_RET_SUCCESS)
		printf("Recursive merge sort was successful\n");
	else if (ret == HSORT_RET_INVALIDUSE)
		printf("Check arguments\n");
	else
		printf("Recursive merge sort failed\n");

	return EXIT_SUCCESS;
}
