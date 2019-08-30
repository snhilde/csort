#include "tests.h"


static int check_array(void *arr_a, void *arr_b, size_t arr_a_len, size_t arr_b_len, size_t a_size, size_t b_size, bool is_signed)
{
	unsigned int i;
	u_int64_t    a;
	u_int64_t    b;

	/* Quick sanity check. */
	if (arr_a_len != arr_b_len) {
		printf("Array lengths do not match\n" "Array A: %zu\n" "Array B: %zu", arr_a_len, arr_b_len);
		return 0;
	}

	if (a_size != b_size) {
		printf("Array data sizes do not match\n" "Array A: %zu\n" "Array B: %zu", a_size, b_size);
		return 0;
	}

	for (i = 0; i < arr_a_len; i++) {
		/* Get the value at each index. */
		switch (a_size) {
			case 1:
				if (is_signed) {
					a = *(int8_t *)arr_a;
					b = *(int8_t *)arr_b;
				} else {
					a = *(u_int8_t *)arr_a;
					b = *(u_int8_t *)arr_b;
				}
				break;

			case 2:
				if (is_signed) {
					a = *(int16_t *)arr_a;
					b = *(int16_t *)arr_b;
				} else {
					a = *(u_int16_t *)arr_a;
					b = *(u_int16_t *)arr_b;
				}
				break;

			case 4:
				if (is_signed) {
					a = *(int32_t *)arr_a;
					b = *(int32_t *)arr_b;
				} else {
					a = *(u_int32_t *)arr_a;
					b = *(u_int32_t *)arr_b;
				}
				break;

			case 8:
				if (is_signed) {
					a = *(int64_t *)arr_a;
					b = *(int64_t *)arr_b;
				} else {
					a = *(u_int64_t *)arr_a;
					b = *(u_int64_t *)arr_b;
				}
				break;
		}

		/* Make sure that they're equal to each other. */
		if (a != b)
			return 0;

		/* Advance to the next index. */
		arr_a += a_size;
		arr_b += b_size;
	}

	/* If we made it this far, then all values were equal. */
	return 1;
}

static void print_array(void *arr, size_t len, size_t size, bool is_signed)
{
	unsigned int i;

	for (i=0; i<len; i++) {
		switch (size) {
			case 1:
				if (is_signed)
					printf("%" PRId8 " ", *(int8_t *)arr);
				else
					printf("%" PRIu8 " ", *(u_int8_t *)arr);
				break;

			case 2:
				if (is_signed)
					printf("%" PRId16 " ", *(int16_t *)arr);
				else
					printf("%" PRIu16 " ", *(u_int16_t *)arr);
				break;

			case 4:
				if (is_signed)
					printf("%" PRId32 " ", *(int32_t *)arr);
				else
					printf("%" PRIu32 " ", *(u_int32_t *)arr);
				break;

			case 8:
				if (is_signed)
					printf("%" PRId64 " ", *(int64_t *)arr);
				else
					printf("%" PRIu64 " ", *(u_int64_t *)arr);
				break;
		}
		arr += size;
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	struct {
		void *test;  /* Test array that will be sorted */
		void *check; /* Known-good check array that test array will be checked against */
	} signed_arrays[] = {
		{ int_group_a, int_group_a_sorted_asc },
		{ int_group_b, int_group_b_sorted_asc },
		{ int_group_c, int_group_c_sorted_asc },
		{ int_group_d, int_group_d_sorted_asc },
		{ NULL, NULL }
	}, unsigned_arrays[] = {
		{ uint_group_a, uint_group_a_sorted_asc },
		{ uint_group_b, uint_group_b_sorted_asc },
		{ uint_group_c, uint_group_c_sorted_asc },
		{ uint_group_d, uint_group_d_sorted_asc },
		{ NULL, NULL }
	}, char_array[] = {
		{ char_group_a, char_group_a_sorted_asc },
		{ char_group_b, char_group_b_sorted_asc },
		{ char_group_c, char_group_c_sorted_asc },
		{ char_group_d, char_group_d_sorted_asc },
		{ NULL, NULL }
	};

	printf("Sorting signed arrays\n");
	printf("Sorting unsigned arrays\n");
	printf("Sorting char arrays\n");

	return EXIT_SUCCESS;
}
