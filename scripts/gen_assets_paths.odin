package gen_assets_path

import "core:strings"
import "core:path/slashpath"
import "core:os"
import "core:fmt"

main :: proc() {
    exe_dir := slashpath.dir(os.args[0])
    fmt.printf("exe_dir: %s\n", exe_dir)

    asset_dir := slashpath.join({exe_dir, "assets"})
    fmt.printf("asset_dir: %s\n", asset_dir)

    rel_path := strings.replace(asset_dir, exe_dir, "", 1) or_else asset_dir

    fmt.printf("Prefix: %s\n", strings.to_ada_case(strings.replace(rel_path, "/", "", 1) or_else rel_path) or_else "")
}