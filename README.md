# hsort
Library for various sorting algorithms

## Currently supported algorithms
* Insertion Sort
* Selection Sort
* Merge Sort (by iteration and by recursion)

## API

### Sorting Functions
* [Sort signed array](#sort_signed_array)
* [Sort unsigned array](#sort_unsigned_array)
* [Sort characters in a string](#sort_characters_in_a_string)
* [Sort array with custom comparison callback](#sort_array_with_custom_comparison_callback)

### Function Descriptions

|  <a name=sort_signed_array></a>Sort signed array
| :------------------------------------------------------------------------------------------------------------------ |
| `hsort_return_t hsort_sort_int_array(void *array, size_t len, size_t size, hsort_options_t options)`
| Description:
| Arguments:<br/>
|   array:<br/>
|   len:<br/>
|   size:<br/>
|   options:<br/>
| Return:<br/>
|   HSORT_RET_SUCCESS on success, other hsort_return_t on failure


|  <a name=sort_unsigned_array></a>Sort unsigned array
| :------------------------------------------------------------------------------------------------------------------ |
| `hsort_return_t hsort_sort_uint_array(void *array, size_t len, size_t size, hsort_options_t options)`

|  <a name=sort_characters_in_a_string></a>Sort characters in a string
| :------------------------------------------------------------------------------------------------------------------ |
| `hsort_return_t hsort_sort_str(char *str, hsort_options_t options)`

|  <a name=sort_array_with_custom_comparison_callback></a>Sort array with custom comparison callback
| :------------------------------------------------------------------------------------------------------------------ |
| `hsort_return_t hsort_sort_custom(void *array, size_t len, size_t size, hsort_options_t options, bool is_signed, hsort_equality_cb cb)`
