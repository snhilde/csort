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
| Sort an array of signed integers
| **Arguments**<br/><ul><li>array: array to be sorted</li><li>len: len of array (number of members)</li><li>size: size in bytes of each member</li><li>options: hsort_options_t to specify various aspects</li></ul>
| Returns HSORT_RET_SUCCESS on success, other hsort_return_t on failure


|  <a name=sort_unsigned_array></a>Sort unsigned array
| :------------------------------------------------------------------------------------------------------------------ |
| `hsort_return_t hsort_sort_uint_array(void *array, size_t len, size_t size, hsort_options_t options)`
| Sort an array of unsigned integers
| **Arguments**<ul><li>array: array to be sorted</li><li>len: len of array (number of members)</li><li>size: size in bytes of each member</li><li>options: hsort_options_t to specify various aspects</li></ul>
| Returns HSORT_RET_SUCCESS on success, other hsort_return_t on failure

|  <a name=sort_characters_in_a_string></a>Sort characters in a string
| :------------------------------------------------------------------------------------------------------------------ |
| `hsort_return_t hsort_sort_str(char *str, hsort_options_t options)`
| Sort a string of character
| **Arguments**<ul><li> str: string to be sorted by character</li><li>options: hsort_options_t to specify various aspects</li></ul>
| Returns HSORT_RET_SUCCESS on success, other hsort_return_t on failure

|  <a name=sort_array_with_custom_comparison_callback></a>Sort array with custom comparison callback
| :------------------------------------------------------------------------------------------------------------------ |
| `hsort_return_t hsort_sort_custom(void *array, size_t len, size_t size, hsort_options_t options, bool is_signed, hsort_equality_cb cb)`
| Sort an array of any data type using the specified comparison callback
| Returns HSORT_RET_SUCCESS on success, other hsort_return_t on failure
| <table><tbody><tr><th>Arguments</th></tr><tr><td>array</td><td>array to be sorted</td></tr><tr><td>len</td><td>len of array (number of members)</td></tr><tr><td>size</td><td>size in bytes of each member</td></tr><tr><td>options</td><td>hsort_options_t to specify various aspects</td></tr><tr><td>is_signed</td><td>true if data type is signed, otherwise false</td></tr><tr><td>cb</td><td>custom callback to use for comparing members</td></tr></tbody></table>

