#include "hsort.h"


/* --- HELPER FUNCTIONS --- */
static void hsort_insert(void *left, void *right, size_t size)
{
	u_int64_t     tmp = 0;

	/* Store value that we are moving down. */
	switch (size) {
		case 1:
			tmp = *(u_int8_t *)right;
			break;
		case 2:
			tmp = *(u_int16_t *)right;
			break;
		case 4:
			tmp = *(u_int32_t *)right;
			break;
		case 8:
			tmp = *(u_int64_t *)right;
			break;
	}

	/* Shift the value at each index one to the right. */
	while (right > left) {
		switch (size) {
			case 1:
				*(u_int8_t *)right = *((u_int8_t *)(right) - 1);
				break;
			case 2:
				*(u_int16_t *)right = *((u_int16_t *)(right) - 1);
				break;
			case 4:
				*(u_int32_t *)right = *((u_int32_t *)(right) - 1);
				break;
			case 8:
				*(u_int64_t *)right = *((u_int64_t *)(right) - 1);
				break;
		}
		right -= size;
	}

	/* Insert value in sorted place. */
	switch (size) {
		case 1:
			*(u_int8_t *)right = tmp;
			break;
		case 2:
			*(u_int16_t *)right = tmp;
			break;
		case 4:
			*(u_int32_t *)right = tmp;
			break;
		case 8:
			*(u_int64_t *)right = tmp;
			break;
	}
}


/* --- CALLBACKS --- */
static hsort_equality_t hsort_int_cb(void *left, void *right, hsort_options_t options)
{
	int64_t a = *(int64_t *)left;
	int64_t b = *(int64_t *)right;

	if (a < b)
		return HSORT_LT;
	else if (a > b)
		return HSORT_GT;

	return HSORT_EQ;
}

static hsort_equality_t hsort_uint_cb(void *left, void *right, hsort_options_t options)
{
	u_int64_t a = *(u_int64_t *)left;
	u_int64_t b = *(u_int64_t *)right;

	if (a < b)
		return HSORT_LT;
	else if (a > b)
		return HSORT_GT;

	return HSORT_EQ;
}

static hsort_equality_t hsort_str_cb(void *left, void *right, hsort_options_t options)
{
	char a = *(char *)left;
	char b = *(char *)right;

	if (a < b)
		return HSORT_LT;
	else if (a > b)
		return HSORT_GT;

	return HSORT_EQ;
}


/* --- SORTING ALGORITHMS --- */
static hsort_return_t hsort_insertion(void *arr, size_t len, size_t size, hsort_equality_cb cb, hsort_options_t options)
{
	unsigned int i;
	unsigned int j;
	void *left;
	void *right;

	for (i = 0; i+1 < len; i++) {
		right = arr + (i+1)*size;
		for (j = 0; j <= i; j++) {
			left = arr + j*size;
			if (cb(left, right, options) != HSORT_LT) {
				hsort_insert(left, right, size);
				break;
			}
		}
	}

	return HSORT_RET_SUCCESS;
}

static hsort_return_t hsort_selection(void *arr, size_t len, size_t size, hsort_equality_cb cb, hsort_options_t options)
{

	return HSORT_RET_SUCCESS;
}

static hsort_return_t hsort_merge(void *arr, size_t len, size_t size, hsort_equality_cb cb, hsort_options_t options)
{

	return HSORT_RET_SUCCESS;
}


/* --- COMMON FUNCTION --- */
hsort_return_t hsort_custom(void *arr, size_t len, size_t size, hsort_equality_cb cb, hsort_options_t options)
{
	if (arr == NULL || len == 0 || size == 0 || cb == NULL || options == 0)
		return HSORT_RET_INVALIDUSE;

	if (options & HSORT_ORDER_DESC)
		/* If Descending is set, make sure Ascending is not also set. */
		options &= ~HSORT_ORDER_ASC;
	else
		/* Default to Ascending. */
		options |= HSORT_ORDER_ASC;

	if (options & HSORT_INSERTION_SORT)
		return hsort_insertion(arr, len, size, cb, options);
	else if (options & HSORT_SELECTION_SORT)
		return hsort_selection(arr, len, size, cb, options);
	else if (options & HSORT_MERGE_SORT)
		return hsort_merge(arr, len, size, cb, options);

	return HSORT_RET_ERROR;
}


/* --- API WRAPPERS --- */
hsort_return_t hsort_int(int64_t *arr, size_t len, hsort_options_t options)
{
	return hsort_custom(arr, len, sizeof(*arr), hsort_int_cb, options);
}

hsort_return_t hsort_uint(u_int64_t *arr, size_t len, hsort_options_t options)
{
	return hsort_custom(arr, len, sizeof(*arr), hsort_uint_cb, options);
}

hsort_return_t hsort_str(char *str, hsort_options_t options)
{
	return hsort_custom(str, strlen(str), sizeof(*str), hsort_str_cb, options);
}
