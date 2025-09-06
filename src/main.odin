package neonshooter

import "core:fmt"

main :: proc() {
    fmt.printf("NeonShooter game with Odin + SDL3 (GPU, TTF, Image)\n");

    print_primitive_sizes();
}

print_primitive_sizes :: proc() {
    fmt.printf("Size of int: %d\n", size_of(int));
    fmt.printf("Size of uint: %d\n", size_of(uint));

    fmt.printf("Size of rune: %d\n", size_of(rune));
    fmt.printf("Size of string: %d\n", size_of(string));
    fmt.printf("Size of cstring: %d\n", size_of(cstring));

    fmt.printf("Size of [2]f32: %d\n", size_of([2]f32));
    fmt.printf("Size of [3]f32: %d\n", size_of([3]f32));
    fmt.printf("Size of [4]f32: %d\n", size_of([4]f32));
}