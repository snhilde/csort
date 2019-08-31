#include "tests.h"


int main(int argc, char *argv[])
{
	hsort_test(20, sizeof(int), true, HSORT_MERGE_SORT|HSORT_ORDER_ASC|HSORT_PRINT_BEFORE|HSORT_PRINT_AFTER);

	return EXIT_SUCCESS;
}
