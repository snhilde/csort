#include "hsort.h"


struct hsort_merge_node {
	struct hsort_merge_node *next;
	void                    *array;
	size_t                   len;
	bool                     on_left;
	bool                     on_right;
};


/* --- HELPER FUNCTIONS --- */
static void hsort_swap(void *a, void *b, size_t size)
{
	u_int64_t tmp;

	switch (size) {
		case 1:
			tmp            = *(u_int8_t *)b;
			*(u_int8_t *)b = *(u_int8_t *)a;
			*(u_int8_t *)a = tmp;
			break;
		case 2:
			tmp             = *(u_int16_t *)b;
			*(u_int16_t *)b = *(u_int16_t *)a;
			*(u_int16_t *)a = tmp;
			break;
		case 4:
			tmp             = *(u_int32_t *)b;
			*(u_int32_t *)b = *(u_int32_t *)a;
			*(u_int32_t *)a = tmp;
			break;
		case 8:
			tmp             = *(u_int64_t *)b;
			*(u_int64_t *)b = *(u_int64_t *)a;
			*(u_int64_t *)a = tmp;
			break;
	}
}

static void hsort_insert(void *left, void *right, size_t size)
{
	u_int64_t     tmp = 0;

	/* Store value that we are moving down. */
	hsort_swap(right, &tmp, size);

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
	hsort_swap(right, &tmp, size);
}

static void hsort_push(struct hsort_merge_node **top_node, void *array, size_t len)
{
	struct hsort_merge_node *node;

	node = malloc(sizeof(*node));

	node->next     = *top_node;
	node->array    = array;
	node->len      = len;
	node->on_left  = false;
	node->on_right = false;

	*top_node = node;
}

static void hsort_pop(struct hsort_merge_node **top_node)
{
	struct hsort_merge_node *node;

	node      = *top_node;
	*top_node = (*top_node)->next;

	free(node);
}


/* --- CALLBACKS --- */
static hsort_equality_t hsort_int_cb(void *left, void *right, size_t size, hsort_options_t options)
{
	int64_t a;
	int64_t b;

	switch (size) {
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

static hsort_equality_t hsort_uint_cb(void *left, void *right, size_t size, hsort_options_t options)
{
	u_int64_t a;
	u_int64_t b;

	switch (size) {
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
		return HSORT_LT;
	else if (a > b)
		return HSORT_GT;

	return HSORT_EQ;
}

static hsort_equality_t hsort_str_cb(void *left, void *right, size_t size, hsort_options_t options)
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
	void *end;
	void *selection;
	void *test;

	end = arr + (len*size);
	for (selection = arr+size; selection < end; selection += size) {
		for (test = arr; test < selection; test += size) {
			if (cb(selection, test, size, options) != HSORT_GT) {
				hsort_insert(test, selection, size);
				break;
			}
		}
	}

	return HSORT_RET_SUCCESS;
}

static hsort_return_t hsort_selection(void *arr, size_t len, size_t size, hsort_equality_cb cb, hsort_options_t options)
{
	void *end;
	void *current;
	void *selection;
	void *test;

	end = arr + (len*size);
	for (current = arr; current < end-size; current += size) {
		selection = current;
		for (test = current+size; test < end; test += size) {
			if (cb(selection, test, size, options) == HSORT_GT)
				selection = test;
		}
		if (selection != current)
			hsort_swap(current, selection, size);
	}

	return HSORT_RET_SUCCESS;
}

static hsort_return_t hsort_merge(void *arr, size_t len, size_t size, hsort_equality_cb cb, hsort_options_t options)
{
	struct hsort_merge_node *top_node = NULL;
	void                    *tmp_arr;

	tmp_arr = calloc(len, size);

	hsort_push(&top_node, arr, len);

	while (top_node != NULL) {
		if (top_node->len == 1) {
			/* Discard leaf node. */
			hsort_pop(&top_node);

		} else {
			if (top_node->on_right == true) {
				/* Both halves are sorted. Merge them together. */
				merge_stub(top_node, tmp_arr);
				hsort_pop(&top_node);

			} else if (top_node->on_left == true) {
				/* Left half is done. Move to right half, using the smaller portion. */
				top_node->on_right = true;
				hsort_push(&top_node, top_node->array + (top_node->len + 1)/2, (top_node->len)/2);

			} else {
				/* Start working on the left half, using the larger portion. */
				top_node->on_left = true;
				hsort_push(&top_node, top_node->array, (top_node->len + 1)/2);
			}
		}
	}

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
hsort_return_t hsort_int(void *arr, size_t len, size_t size, hsort_options_t options)
{
	return hsort_custom(arr, len, size, hsort_int_cb, options);
}

hsort_return_t hsort_uint(void *arr, size_t len, size_t size, hsort_options_t options)
{
	return hsort_custom(arr, len, size, hsort_uint_cb, options);
}

hsort_return_t hsort_str(char *str, hsort_options_t options)
{
	return hsort_custom(str, strlen(str), sizeof(*str), hsort_str_cb, options);
}
