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
    fmt.fprintf(f, "package neonshooter_game\n\n")

    gen_asset_path(f, asset_dir, exe_dir)
    gen_load_all_assets_proc(f, asset_dir, exe_dir)
}

is_ignore :: proc(path: string) -> bool {
    return slice.any_of([]string{ ".DS_Store", ".ds_Store"}, slashpath.ext(path))
}

var_name_from_path :: proc(path, root: string, allocator := context.allocator, loc := #caller_location) -> string {
    var_name, _ := strings.replace(path, root, "", 1, allocator, loc)
    var_name, _ = strings.replace(var_name, "/", "", 1, allocator, loc)
    var_name, _ = strings.replace_all(var_name, "/", "_", allocator)
    var_name, _ = strings.replace_all(var_name, ".", "_", allocator)
    result, _ := strings.to_ada_case(var_name, allocator)
    return result
}

gen_asset_path :: proc(output: os.Handle, path: string, root: string) -> os.Error {
    if is_ignore(path) {
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
        var_name := var_name_from_path(path, root)

        fmt.fprintf(output, "%s :: `%s`\n", var_name, filepath.rel(root, path) or_else path)
    }

    return nil
}

gen_load_all_assets_proc :: proc(output: os.Handle, path: string, root: string) -> os.Error {
    if is_ignore(path) {
        return nil
    }

    fmt.fprintf(output, "\nload_all_textures :: proc() -> bool {{\n")
    defer fmt.fprintf(output, "}}")

    visit :: proc(output: os.Handle, path: string, root: string) -> os.Error {
        stat := os.stat(path) or_return
        if stat.is_dir {
            dir_handle := os.open(path) or_return
            defer os.close(dir_handle)

            dir_entries := os.read_dir(dir_handle, -1) or_return
            defer delete(dir_entries)

            for entry in dir_entries {
                visit(output, entry.fullpath, root)
            }
        } else {
            var_name := var_name_from_path(path, root)
            fmt.fprintf(output, "    _ = load_texture(%s)\n", var_name)
        }

        return nil
    }
    
    return visit(output, path, root)
}