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

| Test 1         |                           |
| -------------- | ------------------------- | ------------------------------------------------------------------------- |
| hsort_return_t | **hsort_sort_int_array**  | void *array, size_t len, size_t size, hsort_options_t options
| hsort_return_t | **hsort_sort_uint_array** | void *array, size_t len, size_t size, hsort_options_t options
| hsort_return_t | **hsort_sort_str**        | char *str, hsort_options_t options
| hsort_return_t | **hsort_sort_custom**     | void *array, size_t len, size_t size, hsort_options_t options, bool is_signed, hsort_equality_cb cb


|                   | Normal color     | Warning color        | Error color       |
| ----------------- | ---------------- | -------------------- | ----------------- |
| hsort_return_t | 26 - 100 % left  | 11 - 25 % left       | 0 - 10 % left     |
| hsort_return_t | < 75 °C          | 75 - 100 °C          | > 100 °C          |
| hsort_return_t | 0 - 74 %         | 75 - 89 %            | 90 - 100 %        |
| hsort_return_t | 0 - 74 % full    | 75 - 89 % full       | 90 - 100 % full   |
| Fan routine       | 0 - 74 % max     | 75 - 89 % max        | 90 - 100 % max    |
| Load routine      | all loads < 1    | any load >= 1        | any load >= 2     |
| Network routine   | both speeds <= K | either speed = M     | either speed > M  |
| RAM routine       | 0 - 74 % free    | 75 - 89 % free       | 90 - 100% free    |
| Time routine      | always normal color                                         |
| TODO routine      | always normal color                                         |
| Volume routine²   | 0 - 70 %         | 80 - 90 %            | 100 %             |
| Weather routine   | always normal color                                         |
| Wifi routine      | connected        | on but not connected | Wifi turned off   |
| Delimiter         | not printed, colors not used                                |
