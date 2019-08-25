#include "hsort.h"


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


hsort_return_t hsort_custom(void *arr, size_t len, size_t size, hsort_equality_cb cb, hsort_options_t options)
{
	return HSORT_RET_SUCCESS;
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
