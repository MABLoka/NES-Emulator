NES Emulator in C

This project is my implementation of an NES emulator in C.

Below is a list of features that have been implemented so far and those that are pending:
CPU (6502)

    Official opcodes
    Unofficial opcodes

RAM

    Mirroring

PPU (2C02)

    Background
    Foreground (Sprites)

Mappers

    Mapper 000 (NROM)
    Other mappers...

APU (Audio Processing Unit)

    APU stuff :D

Controller

    Keyboard
    Gamepad

Compiling and runing

Raylib is necessary to run this project

gcc -Wall -Wextra -pedantic -O1 -Wunused-parameter -o NES *.c C:/raylib/raylib/src/raylib.rc.data -L. -LC:/raylib/raylib/src -LC:/raylib/raylib/src -I. 
-IC:/raylib/raylib/src -IC:/raylib/raylib/src/external -lraylib -lopengl32 -lgdi32 -lwinmm -DPLATFORM_DESKTOP

Resources

    Nesdev Wiki, OneLoneCoder (javidx9)
