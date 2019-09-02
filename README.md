# hsort
API for various sorting algorithms

## Currently supported algorithms
* Insertion Sort
* Selection Sort
* Merge Sort (by iteration and by recursion)

## API

### Sorting Functions
* hsort_return_t [hsort_sort_int_array](hsort_sort_int_array) (void *array, size_t len, size_t size, hsort_options_t options)
* hsort_return_t [hsort_sort_uint_array](hsort_sort_uint_array) (void *array, size_t len, size_t size, hsort_options_t options)
* hsort_return_t [hsort_sort_str](hsort_sort_str) (char *str, hsort_options_t options)
* hsort_return_t [hsort_sort_custom](hsort_sort_custom) (void *array, size_t len, size_t size, hsort_options_t options, bool is_signed, hsort_equality_cb cb)

### Function Descriptions

| Test 1         |                           |                                                                                                     |
| -------------- | ------------------------- | ------------------------------------------------------------------------- |
| hsort_return_t | **hsort_sort_int_array**  | void *array, size_t len, size_t size, hsort_options_t options |
| hsort_return_t | **hsort_sort_uint_array** | void *array, size_t len, size_t size, hsort_options_t options |
| hsort_return_t | **hsort_sort_str**        | char *str, hsort_options_t options |
| hsort_return_t | **hsort_sort_custom**     | void *array, size_t len, size_t size, hsort_options_t options, bool is_signed, hsort_equality_cb cb |


| Return type    | Function name             | Arguments                                                                                           |
| :------------- | ------------------------- | --------------------------------------------------------------------------------------------------- |
| hsort_return_t | **hsort_sort_int_array**  | void *array, size_t len, size_t size, hsort_options_t options                                       |
| hsort_return_t | **hsort_sort_uint_array** | void *array, size_t len, size_t size, hsort_options_t options                                       |
| hsort_return_t | **hsort_sort_str**        | char *str, hsort_options_t options                                                                  |
| hsort_return_t | **hsort_sort_custom**     | void *array, size_t len, size_t size, hsort_options_t options, bool is_signed, hsort_equality_cb cb |
