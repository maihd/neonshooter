package scripts

import "core:os/os2"
import "core:strings"
import "core:fmt"
import "core:odin/ast"
import "core:odin/parser"


// Proc_Call :: struct {
//     param_types: [dynamic]string,
//     proc_name: string,
// }


main :: proc() {
    // Initialize the Parser with default settings
    p := parser.default_parser()

    pkg, ok := parser.collect_package("game")
    if !ok {
        fmt.eprintf("Failed to open package from `game`\n")
        return
    }

    if !parser.parse_package(pkg, &p) {
        fmt.eprintf("Failed to parse package %s\n", pkg.name)
        return
    }

    entity_types := make([dynamic]string)

    // Find entity type mark with @entity attribute
    for file_path, file in pkg.files {
        if strings.contains(file_path, "/entity_storage.odin") {
            continue
        }

        if strings.contains(file_path, "/entity_processors.odin") {
            continue
        }

        for decl in file.decls {
            if value_decl, ok := decl.derived.(^ast.Value_Decl); ok {
                if len(value_decl.values) != 1 {
                    continue
                }

                if struct_type, ok := value_decl.values[0].derived.(^ast.Struct_Type); ok {
                    has_attr := false

                    for attr in value_decl.attributes {
                        attr := attr.derived.(^ast.Attribute)
                        for elem in attr.elems {
                            elem_indent := elem.derived.(^ast.Ident)
                            if elem_indent.name == "entity" {
                                has_attr = true
                            }
                        }
                    }

                    if !has_attr {
                        continue
                    }

                    if type_name, ok := get_name(value_decl.names); ok {
                        append(&entity_types, type_name)
                    }
                }
            }
                        
            // fmt.printf("   Name: %v\n", type_name)
        }
    }

    // Generate entity_storage.odin
    enum_text := fmt.tprintf("Entity_Type :: enum(u32) {{\n    %s\n}}", strings.join(entity_types[:], ",\n    "))
    union_text := fmt.tprintf("Any_Entity :: union {{\n    %s\n}}", strings.join(entity_types[:], ",\n    "))
    handle_text := fmt.tprintf("Entity_Handle :: bit_field(u64) {{\n    type: Entity_Type | 32,\n    index: u32 | 24,\n    generation: u32 | 8\n}}")
    
    entities_storage_lines := []string{
        "entities: [Entity_Type][dynamic]Any_Entity,",
        "prev_entities: [Entity_Type][dynamic]Any_Entity,",
        "",
        "handles: [Entity_Type][dynamic]Entity_Handle,",
        "sparse_indices: [Entity_Type][dynamic]Sparse_Index,",
        "next_index: [Entity_Type]u32,",
        "",
        "allocator: mem.Allocator,"
    }
    entities_storage_text := fmt.tprintf("Entity_Storage :: struct {{\n    %s\n}}", strings.join(entities_storage_lines, "\n    "))

    import_text := "import \"core:mem\""

    file_text := fmt.tprintf("// This is generated file, DONOT EDIT!\n\npackage neonshooter_game\n\n%s\n\n%s\n\n%s\n\n%s\n\n%s", import_text, enum_text, handle_text, union_text, entities_storage_text)
    write_ok := os2.write_entire_file_from_string("game/entity_storage.odin", file_text)
    if write_ok != nil {
        fmt.eprintf("Error write to file `game/entity_storage.odin`")
    }

    // Find entity processor (a proc) mark with @entity_proc
    // proc_calls := make([dynamic]Proc_Call)
    proc_calls := make([dynamic]string)

    for file_path, file in pkg.files {
        if strings.contains(file_path, "/entity_storage.odin") {
            continue
        }

        if strings.contains(file_path, "/entity_processors.odin") {
            continue
        }

        // fmt.printf("File: %s\n", file_path)
        for decl in file.decls {
            // fmt.printf("   Found declaration at offset: %v\n", decl.pos.offset)

            if value_decl, ok := decl.derived.(^ast.Value_Decl); ok {
                if len(value_decl.values) != 1 {
                    continue
                }

                if proc_lit, ok := value_decl.values[0].derived.(^ast.Proc_Lit); ok {
                    has_attr := false
                    for attr in value_decl.attributes {
                        attr := attr.derived.(^ast.Attribute)
                        for elem in attr.elems {
                            elem_indent, ok := elem.derived.(^ast.Ident)
                            if ok && elem_indent.name == "entity_proc" {
                                has_attr = true
                            }
                        }
                    }

                    if !has_attr {
                        continue
                    }

                    proc_name, proc_name_ok := get_name(value_decl.names)
                    if !proc_name_ok {
                        continue
                    }

                    params := make([dynamic]string)
                    entity_vars := make([dynamic]string)
                    entities_vars := make([dynamic]string)
                    entities_vars_decl := make([dynamic]string)

                    for param, i in proc_lit.type.params.list {
                        type := param.type.derived.(^ast.Pointer_Type)
                        ident := type.elem.derived.(^ast.Ident)
                        name := ident.name
                        
                        append(&params, fmt.tprintf("transmute(^%s)entity_%d", name, i))

                        append(&entity_vars, fmt.tprintf("entity_%d", i))
                        append(&entities_vars, fmt.tprintf("entities_%d", i))
                        append(&entities_vars_decl, fmt.tprintf("entities_%d := storage.entities[.%s]", i, name))
                    }

                    // fmt.printf("%s(%s)\n", proc_name, strings.join(param_types[:], ", "))
                    // append(&proc_calls, Proc_Call {
                    //     proc_name = proc_name,
                    //     param_types = param_types
                    // })

                    open_fors := make([dynamic]string)
                    close_fors := make([dynamic]string)

                    ident_level := 0
                    for entities_var, i in entities_vars {
                        append(&open_fors, fmt.tprintf("%sfor %s in %s {{", strings.repeat("    ", ident_level + 2), entity_vars[i], entities_var))
                        ident_level += 1
                    }

                    proc_call_line := fmt.tprintf("%s%s(%s)", strings.repeat("    ", ident_level + 2), proc_name, strings.join(params[:], ", "))

                    for entity_var in entities_vars {
                        ident_level -= 1
                        append(&close_fors, fmt.tprintf("%s}", strings.repeat("    ", ident_level + 2)))
                    }

                    proc_call := fmt.tprintf("{{\n        %s\n%s\n%s\n%s\n    }}",
                        strings.join(entities_vars_decl[:], "\n        "),
                        strings.join(open_fors[:], "\n"), 
                        proc_call_line, strings.join(close_fors[:], "\n")
                    )
                    
                    append(&proc_calls, proc_call)
                }
            }
        }
    }

    // Generate entity_system.odin
    // update_entities :: proc(storage: ^Entity_Storage) {
    //    handle_procedure:
    //    {
    //       entities_0 := storage.entities[.<Entity_Name_From_Param0>]
    //       entities_1 := storage.entities[.<Entity_Name_From_Param1>]
    //       ...
    //       for entity_0 in entities_0 {
    //          for entity_1 in entities_1 {
    //              ...fors {
    //                  handle_procedure(entity_0, entity_0, ...)
    //              }
    //          }
    //       }
    //    }
    //
    //    ...other handle procedures
    // }
    entity_system_text := fmt.tprintf(`
// This is generated file, DONOT EDIT!

package neonshooter_game

entity_system_process_entities :: proc(storage: ^Entity_Storage) {{
    %s
}}
    `, strings.join(proc_calls[:], "\n\n    "))
    write_ok = os2.write_entire_file_from_string("game/entity_processors.odin", entity_system_text)
    if write_ok != nil {
        fmt.eprintf("Error write to file `game/entity_processors.odin`")
    }
}

get_name :: proc(exprs: []^ast.Expr) -> (string, bool) {
    for expr in exprs {
        if indent, ok := expr.derived.(^ast.Ident); ok {
            return indent.name, true
        }
    }

    return "", false
}