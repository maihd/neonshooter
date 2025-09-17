const process = require("child_process")


process.execSync("odin build game -build-mode:dll -define:RAYLIB_SHARED=true");