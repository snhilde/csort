#include "hsort.h"


/* --- HELPER TYPES --- */
enum hsort_merge_step {
	HSORT_DESCEND_LEFT,
	HSORT_DESCEND_RIGHT,
	HSORT_MERGE_HALVES,
};

typedef struct hsort_data {
	void                  *array;
	size_t                 len;
	size_t                 size;
	hsort_options_t        options;
	bool                   is_signed;
	hsort_equality_cb      cb;

	/* For merge sort only */
	struct hsort_data     *next;
	enum hsort_merge_step  next_step;
} hsort_data_t;


/* --- CALLBACKS --- */
static hsort_equality_t hsort_qsort_signed_cb(const void *left, const void *right, void *thunk)
{
	/* We have to use a special comparison callback for qsort to support sorting in descending order.*/
	hsort_data_t *data = thunk;
	int64_t       a;
	int64_t       b;

	switch (data->size) {
		case 1:
			a = *(int8_t *)left;
			b = *(int8_t *)right;
			break;

		case 2:
			a = *(int16_t *)left;
			b = *(int16_t *)right;
			break;

		case 4:
			a = *(int32_t *)left;
			b = *(int32_t *)right;
			break;

		case 8:
			a = *(int64_t *)left;
			b = *(int64_t *)right;
			break;
	}

	if (a < b)
		return (data->options & HSORT_ORDER_ASC) ? HSORT_LT : HSORT_GT;
	else if (a > b)
		return (data->options & HSORT_ORDER_ASC) ? HSORT_GT : HSORT_LT;

	return HSORT_EQ;
}

static hsort_equality_t hsort_qsort_unsigned_cb(const void *left, const void *right, void *thunk)
{
	/* We have to use a special comparison callback for qsort to support sorting in descending order.*/
	hsort_data_t *data = thunk;
	u_int64_t     a;
	u_int64_t     b;

	switch (data->size) {
		case 1:
			a = *(u_int8_t *)left;
			b = *(u_int8_t *)right;
			break;

		case 2:
			a = *(u_int16_t *)left;
			b = *(u_int16_t *)right;
			break;

		case 4:
			a = *(u_int32_t *)left;
			b = *(u_int32_t *)right;
			break;

		case 8:
			a = *(u_int64_t *)left;
			b = *(u_int64_t *)right;
			break;
	}

	if (a < b)
		return (data->options & HSORT_ORDER_ASC) ? HSORT_LT : HSORT_GT;
	else if (a > b)
		return (data->options & HSORT_ORDER_ASC) ? HSORT_GT : HSORT_LT;

	return HSORT_EQ;
}

static hsort_equality_t hsort_signed_cb(const void *left, const void *right, void *thunk)
{
	hsort_data_t *data = thunk;
	int64_t       a;
	int64_t       b;

	switch (data->size) {
		case 1:
			a = *(int8_t *)left;
			b = *(int8_t *)right;
			break;
		case 2:
			a = *(int16_t *)left;
			b = *(int16_t *)right;
			break;
		case 4:
			a = *(int32_t *)left;
			b = *(int32_t *)right;
			break;
		case 8:
			a = *(int64_t *)left;
			b = *(int64_t *)right;
			break;
	}

	if (a < b)
		return HSORT_LT;
	else if (a > b)
		return HSORT_GT;

	return HSORT_EQ;
}

static hsort_equality_t hsort_unsigned_cb(const void *left, const void *right, void *thunk)
{
	hsort_data_t *data = thunk;
	u_int64_t     a;
	u_int64_t     b;

	switch (data->size) {
		case 1:
			a = *(u_int8_t *)left;
			b = *(u_int8_t *)right;
			break;

		case 2:
			a = *(u_int16_t *)left;
			b = *(u_int16_t *)right;
			break;

		case 4:
			a = *(u_int32_t *)left;
			b = *(u_int32_t *)right;
			break;

		case 8:
			a = *(u_int64_t *)left;
			b = *(u_int64_t *)right;
			break;

		default:
			return HSORT_EQ; /* Discard comparison on invalid use. */
	}

	if (a < b)
		return HSORT_LT;
	else if (a > b)
		return HSORT_GT;

	return HSORT_EQ;
}

static hsort_equality_t hsort_str_cb(const void *left, const void *right, void *thunk)
{
	hsort_data_t *data = thunk;
	char          a    = *(char *)left;
	char          b    = *(char *)right;

	(void)data;

	if (a < b)
		return HSORT_LT;
	else if (a > b)
		return HSORT_GT;

	return HSORT_EQ;
}


/* --- HELPER FUNCTIONS --- */
static void hsort_swap(void *a, void *b, size_t size)
{
	u_int64_t tmp;

	memcpy(&tmp,  a,   size);
	memcpy( a,    b,   size);
	memcpy( b,   &tmp, size);
}

static void hsort_insert(void *insert_this, void *insert_here, size_t size)
{
	u_int64_t tmp = 0;

	/* Store value that we are moving. */
	memcpy(&tmp, insert_this, size);

	/* Shift the value at each index one to the right. */
	memmove(insert_here + size, insert_here, insert_this - insert_here);

	/* Insert value in sorted place. */
	memcpy(insert_here, &tmp, size);
}

static void hsort_merge_subarrays(hsort_data_t *data, void *tmp_array)
{
	void   *head;
	size_t  left_len;
	size_t  right_len;
	void   *left_index;
	void   *right_index;
	size_t  i;

	/* Keep a reference to the beginning of the temporary array. */
	head = tmp_array;

	left_len  = data->len / 2;
	right_len = data->len - left_len;

	left_index  = data->array;
	right_index = data->array + (left_len * data->size);

	for (i = 0; i < data->len; i++) {
		if (left_len == 0) {
			/* Left side is done. Move over the right value. */
			hsort_swap(right_index, tmp_array, data->size);
			right_index += data->size;
			right_len--;

		} else if (right_len == 0) {
			/* Right side is done. Move over the left value. */
			hsort_swap(left_index, tmp_array, data->size);
			left_index += data->size;
			left_len--;

		} else if (data->cb(left_index, right_index, data) != (data->options & HSORT_ORDER_ASC ? HSORT_GT : HSORT_LT)) {
			/* Left value is less than or equal to right value. Move it to the array. */
			hsort_swap(left_index, tmp_array, data->size);
			left_index += data->size;
			left_len--;

		} else {
			/* Right value is less than left value. Move it to the array. */
			hsort_swap(right_index, tmp_array, data->size);
			right_index += data->size;
			right_len--;
		}

		tmp_array += data->size;
	}

	/* Put the now-sorted array back into place. */
	memcpy(data->array, head, data->len * data->size);
}

static void hsort_add_level(hsort_data_t **level, void *array, size_t len)
{
	hsort_data_t *new_level;

	new_level = malloc(sizeof(*new_level));

	new_level->next      = *level;
	new_level->next_step = HSORT_DESCEND_LEFT;
	new_level->array     = array;
	new_level->len       = len;
	new_level->size      = (*level)->size;
	new_level->options   = (*level)->options;
	new_level->is_signed = (*level)->is_signed;
	new_level->cb        = (*level)->cb;

	(*level) = new_level;
}

static void hsort_finish_level(hsort_data_t **level)
{
	hsort_data_t *old_level;

	old_level = *level;
	*level    = (*level)->next;

	free(old_level);
}

static hsort_data_t *hsort_merge_create_stack(hsort_data_t *data)
{
	hsort_data_t *top_node;

	top_node = malloc(sizeof(*top_node));
	if (top_node == NULL)
		return NULL;

	top_node->next      = NULL;
	top_node->next_step = HSORT_DESCEND_LEFT;
	top_node->array     = data->array;
	top_node->len       = data->len;
	top_node->size      = data->size;
	top_node->options   = data->options;
	top_node->is_signed = data->is_signed;
	top_node->cb        = data->cb;

	return top_node;
}

static int hsort_max_length(hsort_data_t *data)
{
	switch (data->size) {
		case 1:
			return 3;
		case 2:
			return 5;
		case 4:
			return 10;
		case 8:
			if (data->is_signed)
				return 19;
			return 20;
		default:
			return 0;
	}
}

static u_int64_t hsort_random_num(hsort_data_t *data)
{
	/* We're going to produce a pseudo-pseudo-random number here. The first pseudo is because
 	 * the generator function we're going to use (lrand48) generates pseudo-random numbers.
	 * The second pseudo is because lrand48(), which produces values of uniform distribution,
	 * coupled with basic statistics will yield more values with higher digit lengths than lower
	 * digit lengths. This makes sense, because there are more numbers with a length of 8 digits
	 * than there are numbers with a length of 2 digits. To test the sorting function's ability to
	 * sort values of all possible lengths, we want uniformity on value as well as digit length.
	 * We're going to follow four steps to produce these pseudo-pseudo-random values:
	 * 1. Calculate a random digit length between 1 and the maximum length we can have.
	 * 2. For each place, calculate a random digit between 0 and 9.
	 * 3. Insert that digit in the appropriate place in the number.
	 * 4. Possibly negate the number.
	 * Note: We don't care about overflow for our purposes.
	 */
	u_int64_t num = 0;
	int       len;    /* 0 - max length of type */
	int       i;
	int       digit;  /* 0 - 9 */

	/* 1. Get number of digits in final number. */
	len = lrand48() % hsort_max_length(data);

	for (i = 0; i < len; i++) {
		/* 2. Calculate value for digit at index i. **/
		digit = lrand48() % 10;

		/* 3. Insert digit at appropriate place. */
		num += (u_int64_t)pow(10, i) * digit;
	}

	/* Possibly negate it. */
	if (lrand48() % 2)
		num *= -1;

	return num;
}

static hsort_return_t hsort_random_array(hsort_data_t *data)
{
	void         *pos;
	unsigned int  i;

	/* Seed */
	srand48(time(NULL));

	data->array = malloc(data->len * data->size);
	if (data->array == NULL)
		return HSORT_RET_ERROR;

	pos = data->array;
	for (i = 0; i < data->len; i++) {
		/* Insert a random number in each position (index). */
		switch (data->size) {
			case 1:
				if (data->is_signed)
					*(int8_t *)pos = (int8_t)hsort_random_num(data);
				else
					*(u_int8_t *)pos = (u_int8_t)hsort_random_num(data);
				break;

			case 2:
				if (data->is_signed)
					*(int16_t *)pos = (int16_t)hsort_random_num(data);
				else
					*(u_int16_t *)pos = (u_int16_t)hsort_random_num(data);
				break;

			case 4:
				if (data->is_signed)
					*(int32_t *)pos = (int32_t)hsort_random_num(data);
				else
					*(u_int32_t *)pos = (u_int32_t)hsort_random_num(data);
				break;

			case 8:
				if (data->is_signed)
					*(int64_t *)pos = (int64_t)hsort_random_num(data);
				else
					*(u_int64_t *)pos = (u_int64_t)hsort_random_num(data);
				break;
		}
		pos += data->size;
	}

	return HSORT_RET_SUCCESS;
}

static hsort_return_t hsort_check(hsort_data_t *data1, hsort_data_t *data2)
{
	/* Check with 2 arrays are equal for every index.
 	 */
	void         *arr_a;
	void         *arr_b;
	unsigned int  i;
	u_int64_t     a;
	u_int64_t     b;

	arr_a = data1->array;
	arr_b = data2->array;

	/* Quick sanity checks */
	if (data1->len != data2->len) {
		printf("Array lengths do not match\n" "Array A: %zu\n" "Array B: %zu", data1->len, data2->len);
		return HSORT_RET_INVALIDUSE;
	}

	if (data1->size != data2->size) {
		printf("Array data sizes do not match\n" "Array A: %zu\n" "Array B: %zu", data1->size, data2->size);
		return HSORT_RET_INVALIDUSE;
	}

	for (i = 0; i < data1->len; i++) {
		/* Get the value at each index. */
		switch (data1->size) {
			case 1:
				if (data1->is_signed) {
					a = *(int8_t *)arr_a;
					b = *(int8_t *)arr_b;
				} else {
					a = *(u_int8_t *)arr_a;
					b = *(u_int8_t *)arr_b;
				}
				break;

			case 2:
				if (data1->is_signed) {
					a = *(int16_t *)arr_a;
					b = *(int16_t *)arr_b;
				} else {
					a = *(u_int16_t *)arr_a;
					b = *(u_int16_t *)arr_b;
				}
				break;

			case 4:
				if (data1->is_signed) {
					a = *(int32_t *)arr_a;
					b = *(int32_t *)arr_b;
				} else {
					a = *(u_int32_t *)arr_a;
					b = *(u_int32_t *)arr_b;
				}
				break;

			case 8:
				if (data1->is_signed) {
					a = *(int64_t *)arr_a;
					b = *(int64_t *)arr_b;
				} else {
					a = *(u_int64_t *)arr_a;
					b = *(u_int64_t *)arr_b;
				}
				break;

			default:
				return HSORT_RET_INVALIDUSE;
		}

		/* Make sure that they're equal to each other. */
		if (a != b)
			return HSORT_RET_ERROR;

		/* Advance to the next index. */
		arr_a += data1->size;
		arr_b += data2->size;
	}

	/* If we made it this far, then all values were equal. */
	return HSORT_RET_SUCCESS;
}

static void hsort_print_time(struct timespec *start_time, struct timespec *end_time)
{
	long start_ms; /* Millisecond when timer was started */
	long end_ms;   /* Millisecond when timer was stopped */
	long elapsed;  /* Elapsed time in milliseconds */
	long seconds;  /* Elapsed time in seconds */

	start_ms = (start_time->tv_sec * 1000) + (start_time->tv_nsec / 1000000);
	end_ms   = (end_time->tv_sec   * 1000) + (end_time->tv_nsec   / 1000000);
	elapsed  = end_ms - start_ms;
	seconds  = elapsed / 1000;

	printf("Time: %ld.%03lds\n", seconds, elapsed % 1000);
}


/* --- SORTING ALGORITHMS --- */
static hsort_return_t hsort_insertion(hsort_data_t *data)
{
	/* Starting with the second item in the array and iterating until the end, insert the item
 	 * in the preceding subarray at the position where the value before is less than (or greater than,
	 * if sorting in descending order) the item's value and the value after is greater (is less).
	 * E.g. j < i < next item
 	 */
	unsigned int  i;     /* Current item being inserted into the preceding subarray */
	unsigned int  j;     /* Item being checked to determine insertion point for i */
	void         *i_ptr; /* Array position for i */
	void         *j_ptr; /* Array position for j */

	for (i = 1; i < data->len; i++) {
		i_ptr = data->array + (data->size * i);
		for (j = 0; j < i; j++) {
			j_ptr = data->array + (data->size * j);
			if (data->cb(i_ptr, j_ptr, data) == (data->options & HSORT_ORDER_ASC ? HSORT_LT : HSORT_GT)) {
				hsort_insert(i_ptr, j_ptr, data->size);
				break;
			}
		}
	}

	return HSORT_RET_SUCCESS;
}

static hsort_return_t hsort_selection(hsort_data_t *data)
{
	/* Starting with the first item and iterating until one from the end, swap the current item
 	 * with the smallest (or largest, if sorting in descending order) item farther on in the array.
 	 */
	unsigned int  i;       /* Current item, might be swapped out if a smaller (or larger) value is found */
	unsigned int  j;       /* Item being compared against smallest (or largest) value found so far */
	void         *i_ptr;   /* Array position for i */
	void         *j_ptr;   /* Array position for j */
	void         *min_ptr; /* Array position for smallest (or largest) value found so far */

	for (i = 0; i < data->len - 1; i++) {
		i_ptr   = data->array + (data->size * i);
		min_ptr = i_ptr;
		for (j = i + 1; j < data->len; j++) {
			j_ptr = data->array + (data->size * j);
			if (data->cb(j_ptr, min_ptr, data) == (data->options & HSORT_ORDER_ASC ? HSORT_LT : HSORT_GT))
				min_ptr = j_ptr;
		}
		hsort_swap(i_ptr, min_ptr, data->size);
	}

	return HSORT_RET_SUCCESS;
}

static hsort_return_t hsort_merge_by_recursion(hsort_data_t *data, void *tmp_array)
{
	/* 1. Divide  - Halve array to the bottom (len = 1).
 	 * 2. Conquer - Sort lone item.
	 * 3. Combine - Work backwards, combining arrays at each step.
	 */
	hsort_data_t data_left;
	hsort_data_t data_right;

	if (data->len == 1)
		/* 2. Conquer: Sort lone item. */
		return HSORT_RET_SUCCESS;

	data_left.array      = data->array;
	data_left.len        = data->len / 2;
	data_left.size       = data->size;
	data_left.options    = data->options;
	data_left.is_signed  = data->is_signed;
	data_left.cb         = data->cb;

	data_right.array     = data->array + (data_left.len * data->size);
	data_right.len       = data->len - data->len / 2;
	data_right.size      = data->size;
	data_right.options   = data->options;
	data_right.is_signed = data->is_signed;
	data_right.cb        = data->cb;

	/* 1. Divide: Recursively divide the array. */
	hsort_merge_by_recursion(&data_left, tmp_array);
	hsort_merge_by_recursion(&data_right, tmp_array);

	/* 3. Combine: Merge back up. */
	hsort_merge_subarrays(data, tmp_array);

	return HSORT_RET_SUCCESS;
}

static hsort_return_t hsort_merge_by_stack(hsort_data_t *data, void *tmp_array)
{
	/* 1. Divide  - Halve array to the bottom (len = 1).
 	 * 2. Conquer - Sort lone item. (Not shown in this implementation.)
	 * 3. Combine - Work backwards, combining arrays at each step.
	 */
	hsort_data_t *current_level = NULL;
	size_t        tmp_len;

	current_level = hsort_merge_create_stack(data);
	if (current_level == NULL)
		return HSORT_RET_ERROR;

	while (current_level != NULL) {
		if (current_level->next_step == HSORT_DESCEND_LEFT) {
			/* 1. Divide: Work down the left. */
			current_level->next_step = HSORT_DESCEND_RIGHT;
			tmp_len                  = current_level->len / 2;
			if (tmp_len > 1)
				hsort_add_level(&current_level, current_level->array, tmp_len);

		} else if (current_level->next_step == HSORT_DESCEND_RIGHT) {
			/* 1. Divide: Work down the right. */
			current_level->next_step = HSORT_MERGE_HALVES;
			tmp_len                  = current_level->len - current_level->len / 2;
			if (tmp_len > 1)
				hsort_add_level(&current_level, current_level->array + ((current_level->len / 2) * current_level->size), tmp_len);

		} else {
			/* 3. Combine: Merge halves and move up a level. */
			hsort_merge_subarrays(current_level, tmp_array);
			hsort_finish_level(&current_level);
		}
	}

	return HSORT_RET_SUCCESS;
}

static hsort_return_t hsort_merge(hsort_data_t *data)
{
	void           *tmp_array;
	hsort_return_t  ret;

	tmp_array = calloc(data->len, data->size);
	if (tmp_array == NULL)
		return HSORT_RET_ERROR;

	if (data->options & HSORT_MERGE_SORT_RECURSE) {
		ret = hsort_merge_by_recursion(data, tmp_array);
	} else {
		ret = hsort_merge_by_stack(data, tmp_array);
	}

	free(tmp_array);
	return ret;
}


/* --- COMMON FUNCTIONS --- */
static void hsort_print_int_array(hsort_data_t *data)
{
	void         *array = data->array;
	unsigned int  i;

	for (i = 0; i < data->len; i++) {
		switch (data->size) {
			case 1:
				printf("%" PRId8, *(int8_t *)array);
				break;

			case 2:
				printf("%" PRId16, *(int16_t *)array);
				break;

			case 4:
				printf("%" PRId32, *(int32_t *)array);
				break;

			case 8:
				printf("%" PRId64, *(int64_t *)array);
				break;

			default:
				return;
		}
		if (i != data->len - 1)
			printf(", ");
		array += data->size;
	}
	printf("\n");
}

static void hsort_print_uint_array(hsort_data_t *data)
{
	void         *array = data->array;
	unsigned int  i;

	for (i = 0; i < data->len; i++) {
		switch (data->size) {
			case 1:
				printf("%" PRIu8, *(u_int8_t *)array);
				break;

			case 2:
				printf("%" PRIu16, *(u_int16_t *)array);
				break;

			case 4:
				printf("%" PRIu32, *(u_int32_t *)array);
				break;

			case 8:
				printf("%" PRIu64, *(u_int64_t *)array);
				break;

			default:
				return;
		}
		if (i != data->len-1)
			printf(", ");
		array += data->size;
	}
	printf("\n");
}

static hsort_return_t hsort_sort_internal(hsort_data_t *data)
{
	struct timespec start_time = {0};
	struct timespec end_time   = {0};
	hsort_return_t  ret;

	if (data == NULL || data->array == NULL || data->len == 0 || data->size == 0 || data->options == 0 || data->cb == NULL)
		return HSORT_RET_INVALIDUSE;

	if (data->options & HSORT_ORDER_DESC)
		/* If Descending is set, make sure Ascending is not also set. */
		data->options &= ~HSORT_ORDER_ASC;
	else
		/* Default to Ascending. */
		data->options |= HSORT_ORDER_ASC;

	if (data->options & HSORT_PRINT_TIME)
		clock_gettime(CLOCK_MONOTONIC_RAW, &start_time);

	if (data->options & HSORT_INSERTION_SORT)
		ret = hsort_insertion(data);
	else if (data->options & HSORT_SELECTION_SORT)
		ret = hsort_selection(data);
	else if (data->options & (HSORT_MERGE_SORT|HSORT_MERGE_SORT_RECURSE))
		ret = hsort_merge(data);
	else
		return HSORT_RET_ERROR;

	if (data->options & HSORT_PRINT_TIME) {
		clock_gettime(CLOCK_MONOTONIC_RAW, &end_time);
		hsort_print_time(&start_time, &end_time);
	}

	return ret;
}


/* --- PUBLIC FUNCTIONS --- */
void hsort_print_array(void *array, size_t len, size_t size, bool is_signed)
{
	hsort_data_t data;

	data.array     = array;
	data.len       = len;
	data.size      = size;
	data.is_signed = is_signed;

	if (data.is_signed)
		return hsort_print_int_array(&data);
	return hsort_print_uint_array(&data);
}

void hsort_print_str(char *str)
{
	printf("%s\n", str);
}

hsort_return_t hsort_test(size_t len, size_t size, bool is_signed, hsort_options_t options)
{
	hsort_data_t   test;  /* Will contain the array that we will sort for the test */
	hsort_data_t   check; /* Will contain the array that qsort will sort for a known-good check */
	hsort_return_t ret;

	test.len        = len;
	test.size       = size;
	test.options    = options;
	test.is_signed  = is_signed;
	test.cb         = is_signed ? hsort_signed_cb : hsort_unsigned_cb;

	check.len       = len;
	check.size      = size;
	check.options   = options;
	check.is_signed = is_signed;
	check.cb        = is_signed ? hsort_signed_cb : hsort_unsigned_cb;

	/* Build up an array of len items with random values of size size and signedness is_signed. */
	if (hsort_random_array(&test) != HSORT_RET_SUCCESS)
		return HSORT_RET_ERROR;

	/* Duplicate array so qsort can sort it as well. */
	check.array = malloc(check.len * check.size);
	if (check.array == NULL) {
		free(test.array);
		return HSORT_RET_ERROR;
	}
	memcpy(check.array, test.array, test.len * test.size);

	if (options & HSORT_PRINT_BEFORE)
		hsort_print_array(test.array, test.len, test.size, test.is_signed);

	/* Sort test array. */
	if (is_signed)
		ret = hsort_sort_internal(&test);
	else
		ret = hsort_sort_internal(&test);

	if (ret != HSORT_RET_SUCCESS) {
		printf("Sorting error\n");
		free(test.array);
		free(check.array);
		return ret;
	}

	if (options & HSORT_PRINT_AFTER)
		hsort_print_array(test.array, test.len, test.size, test.is_signed);

	/* Sort check array. */
	if (is_signed)
		qsort_r(check.array, check.len, check.size, hsort_qsort_signed_cb, &check);
	else
		qsort_r(check.array, check.len, check.size, hsort_qsort_unsigned_cb, &check);

	/* See if arrays match or not. */
	ret = hsort_check(&test, &check);

	free(test.array);
	free(check.array);
	return ret;
}


/* --- API WRAPPERS --- */
hsort_return_t hsort_sort_int_array(void *array, size_t len, size_t size, hsort_options_t options)
{
	hsort_data_t   data;
	hsort_return_t ret;

	data.array     = array;
	data.len       = len;
	data.size      = size;
	data.options   = options;
	data.is_signed = true;
	data.cb        = hsort_signed_cb;

	if (options & HSORT_PRINT_BEFORE)
		hsort_print_array(data.array, data.len, data.size, data.is_signed);

	ret = hsort_sort_internal(&data);

	if (options & HSORT_PRINT_AFTER)
		hsort_print_array(data.array, data.len, data.size, data.is_signed);

	return ret;

}

hsort_return_t hsort_sort_uint_array(void *array, size_t len, size_t size, hsort_options_t options)
{
	hsort_data_t   data;
	hsort_return_t ret;

	data.array     = array;
	data.len       = len;
	data.size      = size;
	data.options   = options;
	data.is_signed = false;
	data.cb        = hsort_unsigned_cb;

	if (options & HSORT_PRINT_BEFORE)
		hsort_print_array(data.array, data.len, data.size, data.is_signed);

	ret = hsort_sort_internal(&data);

	if (options & HSORT_PRINT_AFTER)
		hsort_print_array(data.array, data.len, data.size, data.is_signed);

	return ret;
}

hsort_return_t hsort_sort_str(char *str, hsort_options_t options)
{
	hsort_data_t   data;
	hsort_return_t ret;

	data.array     = (void *)str;
	data.len       = strlen(str);
	data.size      = sizeof(*str);
	data.options   = options;
	data.is_signed = true;
	data.cb        = hsort_str_cb;

	if (options & HSORT_PRINT_BEFORE)
		hsort_print_str(str);

	ret = hsort_sort_internal(&data);

	if (options & HSORT_PRINT_AFTER)
		hsort_print_str(str);

	return ret;
}

hsort_return_t hsort_sort_custom(void *array, size_t len, size_t size, hsort_options_t options, bool is_signed, hsort_equality_cb cb)
{
	hsort_data_t   data;
	hsort_return_t ret;

	data.array     = array;
	data.len       = len;
	data.size      = size;
	data.options   = options;
	data.is_signed = is_signed;
	data.cb        = cb;

	if (options & HSORT_PRINT_BEFORE)
		hsort_print_array(data.array, data.len, data.size, data.is_signed);

	ret = hsort_sort_internal(&data);

	if (options & HSORT_PRINT_AFTER)
		hsort_print_array(data.array, data.len, data.size, data.is_signed);

	return ret;
}
