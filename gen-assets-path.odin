package gen_assets_path

import "core:fmt"
import "core:os"
import "core:path/slashpath"
import "core:strings"

main :: proc() {
	fmt.printf("Generate assets path definitions\n")

	if stat, err := os.stat("assets"); err != nil && !stat.is_dir {
		fmt.fprintf(
			os.stderr,
			"Failed to open assets folder. Error: %s\n",
			err,
		)
		return
	}

	dir, err := os.open("assets")
	if err != nil {
		fmt.fprintf(
			os.stderr,
			"Failed to read files from assets folder. Error: %s\n",
			err,
		)
		return
	}

	dir_files, read_err := os.read_dir(dir, -1)
	if read_err != nil {
		fmt.fprintf(
			os.stderr,
			"Failed to read files from assets folder. Error: %s\n",
			read_err,
		)
		return
	}

	f, _ := os.open(
		"src/assets.odin",
		os.O_WRONLY | os.O_CREATE | os.O_TRUNC,
		0o644,
	)
	defer os.close(f)

	fmt.fprintf(f, "// Generated file. Donot editting.\n\n")
	fmt.fprintf(f, "package neonshooter\n\n")

	// fmt.fprintf(f, "")

	for file in dir_files {
		print_file_path_definition(f, file)
	}
}

print_file_path_definition :: proc(
	f: os.Handle,
	file: os.File_Info,
	prefix := "",
) {
	file_name, _ := strings.replace_all(file.name, ".", "_")
	file_name = strings.to_ada_case(file_name)

	params: []string =
		len(prefix) == 0 ? {prefix, file_name} : {prefix, "_", file_name}

	name := strings.concatenate(params)

	if file.is_dir {
		dir, err := os.open(file.fullpath)
		if err != nil {
			return
		}

		dir_files, dir_err := os.read_dir(dir, -1)
		if dir_err != nil {
			return
		}

		for child_file in dir_files {
			print_file_path_definition(f, child_file, name)
		}

		return
	}

	fmt.fprintf(f, "%s :: `%s`\n", name, file.fullpath)
}
