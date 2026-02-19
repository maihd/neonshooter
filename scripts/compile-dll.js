const process = require("child_process")

process.execSync("odin build common -build-mode:dll -define:RAYLIB_SHARED=true");
process.execSync("odin build game -build-mode:dll -custom-attribute:entity -define:RAYLIB_SHARED=true -custom-attribute:entity");