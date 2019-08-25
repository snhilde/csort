#include "hsort.h"


hsort_return_t hsort_custom(void *arr, size_t len, size_t size, hsort_equality_cb cb, hsort_options_t options)
{
	return HSORT_RET_SUCCESS;
}

hsort_return_t hsort_int(int64_t *arr, size_t len, hsort_options_t options)
{
	return HSORT_RET_SUCCESS;
}

hsort_return_t hsort_uint(u_int64_t *arr, size_t len, hsort_options_t options)
{
	return HSORT_RET_SUCCESS;
}

hsort_return_t hsort_str(char *str, hsort_options_t options)
{
	return HSORT_RET_SUCCESS;
}
