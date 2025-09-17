package gen_assets_path

import "core:path/filepath"
import "core:slice"
import "core:strings"
import "core:path/slashpath"
import "core:os"
import "core:fmt"

import vmem "core:mem/virtual"

main :: proc() {
    exe_dir := slashpath.dir(os.args[0])
    fmt.printf("exe_dir: %s\n", exe_dir)

    asset_dir := slashpath.join({exe_dir, "assets"})
    fmt.printf("asset_dir: %s\n", asset_dir)

    f, _ := os.open(
        path = slashpath.join({exe_dir, "game", "assets_path.odin"}), 
        flags = os.O_CREATE | os.O_TRUNC | os.O_WRONLY, 
        mode = 0o644,
    )
    defer os.close(f)

    fmt.fprintf(f, "// Generated file. Donot editing.\n")
    fmt.fprintf(f, "package game\n\n")

    gen_asset_path(f, asset_dir, exe_dir)
}

gen_asset_path :: proc(output: os.Handle, path: string, root: string) -> os.Error {
    if slice.any_of([]string{ ".DS_Store", ".ds_Store" }, slashpath.ext(path)) {
        return nil
    }

    stat := os.stat(path) or_return
    if stat.is_dir {
        fmt.fprintf(output, "\n")
        
        dir_handle := os.open(path) or_return
        defer os.close(dir_handle)

        dir := os.read_dir(dir_handle, -1) or_return
        defer delete(dir)

        arena: vmem.Arena; 
        vmem.arena_init_growing(&arena) or_return
        defer vmem.arena_destroy(&arena)
        
        allocator := vmem.arena_allocator(&arena)

        for entry in dir {
            entry_abs_path := slashpath.join({ path, entry.name }, allocator)
            gen_asset_path(output, entry_abs_path, root)
        }
    } else {
        var_name, _ := strings.replace(path, root, "", 1)
        var_name, _ = strings.replace(var_name, "/", "", 1)
        var_name, _ = strings.replace_all(var_name, "/", "_")
        var_name, _ = strings.replace_all(var_name, ".", "_")

        fmt.fprintf(output, "%s :: `%s`\n", strings.to_ada_case(var_name), filepath.rel(root, path) or_else path)
    }

    return nil
}