package scripts

import "core:fmt"
import "core:path/slashpath"
import "core:os"

import "core:odin/ast"
import "core:odin/parser"
import "core:odin/tokenizer"

main :: proc() {
    file_path := "game/entities.odin"
    
    // Read file content
    content, ok := os.read_entire_file(file_path)
    if !ok {
        fmt.eprintf("Error: failed to read content from %s\n", file_path)
        return
    }

    // Tokens

    tok: tokenizer.Tokenizer 
    tokenizer.init(&tok, transmute(string)content, file_path)

    // Parse token
    p := parser.default_parser()
    p.tok = tok

    // Generate entity_packed.odin with contains: Entity, Entity_Type, typeid_to_entity_type
}