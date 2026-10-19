@echo off

odin build game -out:temp/game.dll -build-mode:dll -define:RAYLIB_SHARED=true -custom-attribute:entity -custom-attribute:entity_proc -debug
