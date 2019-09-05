# hsort
API for various sorting algorithms

## Currently supported algorithms
* Insertion Sort
* Selection Sort
* Merge Sort (by iteration and by recursion)

## API

### Sorting Functions
* [Sort signed array](#hsort_sort_int_array)
* [Sort unsigned array](#hsort_sort_uint_array)
* [Sort characters in a string](#hsort_sort_str)
* [Sort array with custom comparision callback](#hsort_sort_custom)

### Function Descriptions

| Return         | Function                                                | Arguments                                                                                            |
| :------------- | :------------------------------------------------------ | :--------------------------------------------------------------------------------------------------- |
| hsort_return_t | <a name=hsort_sort_int_array></a>hsort_sort_int_array   | void \*array, size_t len, size_t size, hsort_options_t options                                       |
Description

| Return         | Function                                                | Arguments                                                                                            |
| :------------- | :------------------------------------------------------ | :--------------------------------------------------------------------------------------------------- |
| hsort_return_t | <a name=hsort_sort_uint_array></a>hsort_sort_uint_array | void \*array, size_t len, size_t size, hsort_options_t options                                       |
Description

| Return         | Function                                                | Arguments                                                                                            |
| :------------- | :------------------------------------------------------ | :--------------------------------------------------------------------------------------------------- |
| hsort_return_t | <a name=hsort_sort_str></a>hsort_sort_str               | char \*str, hsort_options_t options                                                                  |
Description

| Return         | Function                                                | Arguments                                                                                            |
| :------------- | :------------------------------------------------------ | :--------------------------------------------------------------------------------------------------- |
| hsort_return_t | <a name=hsort_sort_custom></a>hsort_sort_custom         | void \*array, size_t len, size_t size, hsort_options_t options, bool is_signed, hsort_equality_cb cb |
Description
