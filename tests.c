#include "hsort.h"
#include <stdio.h>
#include <inttypes.h>

enum types {
	INT,
	UINT,
	CHAR
};


static void print_arr(void *arr, size_t len, enum types type)
{
	unsigned int i;

	for (i=0; i<len; i++) {
		if (type == INT) {
			printf("%" PRId64 " ", *(int64_t *)arr);
			arr += sizeof(int64_t);
		} else if (type == UINT) {
			printf("%" PRIu64 " ", *(u_int64_t *)arr);
			arr += sizeof(u_int64_t);
		} else if (type == CHAR) {
			printf("%c ", *(char *)arr);
			arr += sizeof(char);
		} else {
			printf("Unknown type\n");
			break;
		}
	}

	printf("\n");
}

int main(int argc, char *argv[])
{
	int64_t   nums[]   = {234, 2832, 293, -10, 139, 19, 0, -129, -19, 4, 94, 2, 34, 50, 125, 253};
	u_int64_t u_nums[] = {185, 1284, 288, 23, 90, 12, 1, 985, 38, 235, 267, 23, 1, 1, 3945, 95};
	char      str[]    = "aTj94adaHae43JHaKEh5gajDmv9AjrrR";

	printf("Integers:\n");
	print_arr(nums, sizeof(nums)/sizeof(*nums), INT);
	hsort_int(nums, sizeof(nums)/sizeof(*nums), HSORT_INSERTION_SORT|HSORT_ORDER_ASC);
	printf("\n");

	printf("Unsigned integers:\n");
	print_arr(u_nums, sizeof(u_nums)/sizeof(*u_nums), UINT);
	printf("\n");

	printf("Characters:\n");
	print_arr(str, sizeof(str)/sizeof(*str), CHAR);

	return EXIT_SUCCESS;
}
