package neonshooter

import "core:fmt"
import "core:mem"
main :: proc() {
    array := make([dynamic]int, 1000)
    slice: []int = array[:]

    index := mem.ptr_sub(&slice[10], cast(^int)raw_data(slice))
    fmt.printf("index = %v\n", index)
}