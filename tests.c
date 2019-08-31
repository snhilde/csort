#include "tests.h"


int main(int argc, char *argv[])
{
	hsort_return_t ret;

	ret = hsort_test(20, sizeof(int), true, HSORT_MERGE_SORT|HSORT_ORDER_ASC|HSORT_PRINT_BEFORE|HSORT_PRINT_AFTER);
	if (ret == HSORT_RET_SUCCESS)
		printf("Sort was successful\n");
	else if (ret == HSORT_RET_INVALIDUSE)
		printf("Check arguments\n");
	else
		printf("Sort failed\n");

	return EXIT_SUCCESS;
}
