# NES Emulator in C

This is a project is my implementaion to of an NES emulator in C. 

## Project Status

Below is a list of features that have been implemented so far and those that are pending:

### CPU (6502)
- [x] Official opcodes
- [ ] Unofficial opcodes (By the moment some NOP)

### RAM
- [x] Mirroring

### PPU (2C02)
- [x] Background
- [ ] Foreground (Sprites)

### Mappers
- [x] Mapper 000 (NROM)
- [ ] Other mappers...

### APU (Audio Processing Unit)
- [ ] APU stuff :D

### Controller
- [x] Keyboard
- [ ] Gamepad

## Compiling and runing

Raylib is used to process the graphics
```shell
gcc -Wall -Wextra -pedantic -O1 -Wunused-parameter -o NES *.c C:/raylib/raylib/src/raylib.rc.data -L. -LC:/raylib/raylib/src -LC:/raylib/raylib/src -I. -IC:/raylib/raylib/src -IC:/raylib/raylib/src/external -lraylib -lopengl32 -lgdi32 -lwinmm -DPLATFORM_DESKTOP
```

## Resources

- [Nesdev Wiki](https://www.nesdev.org/wiki/Nesdev_Wiki)

- [OneLoneCoder (javidx9)](https://onelonecoder.com/)
