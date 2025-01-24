
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppu.h"
#include "cpu.h"
#include "raylib.h" 

#define HEX_CHARS "0123456789ABCDEF"
int drawnsp = 0;
void StartDemo();
static BUS bus = {0};

static char* mapAsm[0xFFFF] = {0};
static CPU *cpu = {0};
static PPU *ppu = {0};
static bool emulationRun = false;
static float residualTime = 0.0f;
static uint8_t selectedPalette = 0;

static char* hex(uint32_t n, uint8_t d, char *dst) {
    strset(dst, 0);
    int i;
    dst[d] = 0;
    for (i = d - 1; i >= 0; i--, n >>= 4) {
        dst[i] = HEX_CHARS[n & 0xF];
    }
    return dst;
}

void DrawRam(int x, int y, uint16_t nAddr, int nRows, int nColumns) {
    printf("DrawRam");
    int nRamX = x, nRamY = y;
    char hex_aux[16];
    char sOffSet[1024];
    for (int row = 0; row < nRows; row++) {
        strset(sOffSet, 0);
        strcat(sOffSet, "$");
        strcat(sOffSet, hex(nAddr, 4, hex_aux));
        strcat(sOffSet, ":");
        for (int col = 0; col < nColumns; col++) {
            strcat(sOffSet, " ");
            strcat(sOffSet, hex((int) cpuRead(nAddr), 2, hex_aux));
            nAddr += 1;
        }
        DrawText(sOffSet, nRamX, nRamY, 4, BLACK);
        nRamY += 10;
    }
}

void DrawCpu(int x, int y) {
    //printf("\nDrawCpuS");
    char hex_aux[16];
    DrawText("STATUS:", x , y , 4, WHITE);
    DrawText("N", x  + 64, y,  4, cpu->F & N ? GREEN : RED);
    DrawText("V",x  + 80, y ,  4, cpu->F & V ? GREEN : RED);
    DrawText("-",x  + 96, y ,  4, cpu->F & U ? GREEN : RED);
    DrawText("B",x  + 112, y ,  4, cpu->F & B ? GREEN : RED);
    DrawText("D",x  + 128, y ,  4, cpu->F & D ? GREEN : RED);
    DrawText("I",x  + 144, y ,  4, cpu->F & I ? GREEN : RED);
    DrawText("Z",x  + 160, y ,  4, cpu->F & Z ? GREEN : RED);
    DrawText("C",x  + 178, y ,  4, cpu->F & C ? GREEN : RED);
    char pc[1024];
    char a[1024];
    char xr[1024];
    char yr[1024];
    char sp[1024];

    strcpy(pc, "");
    strcpy(a, "");
    strcpy(xr, "");
    strcpy(yr, "");
    strcpy(sp, "");

    strcat(pc, "PC: $");
    strcat(pc, hex(cpu->PC, 4, hex_aux));

    strcat(a, "A: $");
    strcat(a, hex(cpu->A, 2, hex_aux));
    strcat(a, "  [");
    char a_str [256];
    sprintf(a_str,"%d", cpu->A);
    strcat(a, a_str);
    strcat(a, "]");

    strcat(xr, "X: $");
    strcat(xr, hex(cpu->X, 2, hex_aux));
    strcat(xr, "  [");
    char x_str [256];
    sprintf(x_str,"%d", cpu->X);
    strcat(xr, x_str);
    strcat(xr, "]");

    strcat(yr, "Y: $");
    strcat(yr, hex(cpu->Y, 2, hex_aux));
    strcat(yr, "  [");
    char y_str [256];
    sprintf(y_str,"%d", cpu->Y);
    strcat(yr, y_str);
    strcat(yr, "]");

    strcat(sp, "SP: $");
    strcat(sp, hex(cpu->SP, 4, hex_aux));

    


    DrawText(pc, x , y + 10, 4, WHITE);
    DrawText(a, x , y + 20, 4, WHITE);
    DrawText(xr, x , y + 30, 4, WHITE);
    DrawText(yr, x , y + 40, 4, WHITE);
    DrawText(sp, x , y + 50, 4, WHITE);
    //printf("\nDrawCpuE");
}

void DrawCode(int x, int y, int nLines) {
    //printf("DrawCode");
    int nLineY = (nLines >> 1) * 10 + y;
    
    uint16_t pc = cpu->PC;
    char *pcLine = mapAsm[pc++];
    //printf("\n%d\n", pc);
    DrawText(pcLine, x, nLineY, 4, GREEN);

    //printf("\nDrawCodeM1\n");
    while (nLineY < (nLines * 10) + y) {
        //printf("\nDrawCodeM2\n");
        pcLine = mapAsm[pc++];
        if (pcLine != NULL) {
            //printf("\nDrawCodeM3\n");
            nLineY += 10;
            
            DrawText(pcLine, x, nLineY, 4, WHITE); 
            //printf("\nDrawCodeM5\n");
        }  
    }
    pc = cpu->PC;
    nLineY = (nLines >> 1) * 10 + y;
    while (nLineY > y) {
        //printf("\nDrawCodeM6\n");
        pcLine = mapAsm[--pc];
        if (pcLine != NULL) {
            nLineY -= 10;
            DrawText(pcLine, x, nLineY, 4, WHITE); 
        }
    }
    
}

void DrawSprite(Sprite *sprite, uint32_t x, uint32_t y, uint32_t scale) {
    // printf("DrawSprite");
    drawnsp++;
    //printf("Drawnsp: %d", drawnsp);
    if (sprite == NULL) return;
    
    int32_t fxs = 0, fxm = 1, fx = 0;
    int32_t fys = 0, fym = 1, fy = 0;
    Color test = (Color){155,155,155,255};
    //SpriteGetPixel(sprite, fx, fy)
    if (scale > 1) {
        // printf("\nDrawSpriteM\n");
        fx = fxs;
        for (int32_t i = 0; i < sprite->width; i++, fx += fxm) {
            
            // printf("sprite->width: %d\n", sprite->width);
            fy = fys;
            for (int32_t j = 0; j < sprite->height; j++, fy += fym){
                
                // printf("sprite->height: %d\n", sprite->height);
                for (int32_t is = 0; is < scale; is++){
                    // printf("scale: %d\n", scale);
                    for (int32_t js = 0; js < scale; js++){
                        // printf("scale: %d\n", scale);
                        DrawPixel(x + (i * scale) + is, y + (j * scale) + js, SpriteGetPixel(sprite, fx, fy));
                    }
                }
                
            }
            
        }
        
    }
    else {
        // printf("\nDrawSpriteElse\n");
        fx = fxs;
        for (int32_t i = 0; i < sprite->width; i++, fx += fxm) {
            fy = fys;
            for (int32_t j = 0; j < sprite->height; j++, fy += fym)
                DrawPixel(x + i, y + j, SpriteGetPixel(sprite, fx, fy));
        }
    }

    // printf("\nDrawSpriteE\n");
}

// void DrawSprite(Sprite *sprite, uint32_t x, uint32_t y, uint32_t scale) {
//     if (sprite == NULL) return;

    
//     // printf("DrawSpriteS\n");

//     // Variables for flipping and scale
//     int32_t fx, fy;

//     // Loop through sprite width and height
//     for (int32_t i = 0; i < sprite->width; i++) {
//         for (int32_t j = 0; j < sprite->height; j++) {
//             // Get the pixel color from the sprite
//             Color pixelColor = SpriteGetPixel(sprite, i, j);

//             // Check if scaling is required
//             if (scale > 1) {
//                 for (uint32_t is = 0; is < scale; is++) {
//                     for (uint32_t js = 0; js < scale; js++) {
//                         DrawPixel(x + (i * scale) + is, y + (j * scale) + js, pixelColor);
//                     }
//                 }
//             } else {
//                 DrawPixel(x + i, y + j, pixelColor);
//             }
//         }
//     }
//     // printf("DrawSpriteE\n");
// }


void SetupDemo() {
    printf("Starting SetupDemo...\n");
    bus_init(&bus);
    cpu_init(&bus);
    PpuInit();
    cpu = malloc(sizeof(CPU));
    cpu = GetCpu();
    
    if (!cpu) {
        printf("Error: cpu is NULL.\n");
        return;
    }
    printf("CPU allocated successfully.\n");

    cpu->bus = malloc(sizeof(BUS));
    
    if (!cpu->bus) {
        printf("Error: bus is NULL.\n");
        return;
    }
    printf("Bus allocated successfully.\n");

    
    ppu = malloc(sizeof(ppu));
    ppu = GetPpu();
    cpu->bus->ppu = ppu;
    if (!ppu) {
        printf("Error: ppu is NULL.\n");
        return;
    }
    printf("ppu allocated successfully.\n");

    
    Cartridge *cartridge = CartridgeCreate("./test/DK.nes");
    if (!cartridge) {
        printf("Error: bus is NULL.\n");
        return;
    }
    printf("cartridge allocated successfully.\n");
    printf("Cartridge Created\n");
    
    cpu->bus->cartridge = malloc(sizeof(cartridge));
    insertCartridge(cpu->bus, cartridge);
    if (!cpu->bus->cartridge) {
        printf("Error: cartridge is NULL.\n");
        return;
    }
    printf("cartridge Inserted successfully.\n");
    
    ppu->cart = malloc(sizeof(cartridge));
    ConnectCartridgePpu(cpu->bus->cartridge);
    if (!ppu->cart) {
        printf("Error: cartridge failed to connect to PPU.\n");
        return;
    }
    
    printf("cartridge connected to PPU successfully.\n");
    printf("Cartridge Loaded\n");

    disassemble(0x0000, 0xFFFF, mapAsm);
    // Reset
    reset(cpu->bus);
    
    printf("\nPC=%x, setupdone\n", cpu->PC);
    
}

void UpdateDemo() {
    //printf("UpdateDemo");
    if (!cpu->bus) {
        printf("Error: bus is NULL.\n");
        return;
    }
    cpu->bus->controller[0] = 0x00;
    cpu->bus->controller[0] |= IsKeyDown(KEY_X) ? 0x80 : 0x00;
    cpu->bus->controller[0] |= IsKeyDown(KEY_Z) ? 0x40 : 0x00;
    cpu->bus->controller[0] |= IsKeyDown(KEY_A) ? 0x20 : 0x00;
    cpu->bus->controller[0] |= IsKeyDown(KEY_S) ? 0x10 : 0x00;
    cpu->bus->controller[0] |= IsKeyDown(KEY_UP) ? 0x08 : 0x00;
    cpu->bus->controller[0] |= IsKeyDown(KEY_DOWN) ? 0x04 : 0x00;
    cpu->bus->controller[0] |= IsKeyDown(KEY_LEFT) ? 0x02 : 0x00;
    cpu->bus->controller[0] |= IsKeyDown(KEY_RIGHT) ? 0x00 : 0x00;
    float elapsedTime = GetFrameTime();
    if (emulationRun) {
        if (residualTime > 0.0f)
            residualTime -= elapsedTime;
        else {
            residualTime += (1.0f / 60.0f) - elapsedTime;
            do { busClock(cpu->bus); } while (!ppu->frame_complete);
            ppu->frame_complete = false;
        }
    }
    else {
        
        // Emulate code step-by-step
        if (IsKeyDown(KEY_C)) {
            // Clock enough times to execute a whole CPU instruction
            do { busClock(cpu->bus); } while (!CpuComplete());
            // CPU clock runs slower than system clock, so it may be
            // complete for additional system clock cycles. Drain
            // those out
            do { busClock(cpu->bus); } while (!CpuComplete());
        }

        // Emulate one whole frame
        if (IsKeyPressed(KEY_F)) {
            // Clock enough times to draw a single frame
            do { busClock(cpu->bus); } while (!ppu->frame_complete);
            // Use residual clock cycles to complete current instruction
            do { busClock(cpu->bus); } while (!CpuComplete());
            // Reset frame completion flag
            ppu->frame_complete = false;
        }
    }

    if (IsKeyPressed(KEY_SPACE)) emulationRun = !emulationRun;
    if (IsKeyPressed(KEY_R)) reset(cpu->bus);

    if (IsKeyPressed(KEY_P)) selectedPalette = (selectedPalette + 1) % 8;	
}

void StartDemo() {
    printf("Hello, Demo!\n");
    InitWindow(780, 480, "NES Instructions Demo");
    
    SetupDemo();
    
    // for(uint32_t i = 0; i < 0xffff; i++ ){
    //     printf("Address[%x]: %x\n", i, read(cpu->bus, i, 0));
    // }
    
    // uint32_t mapped_addr = 0;
    // for(uint32_t i = 0; i < 0xffff; i++ ){
    //     printf("Done? %d\n", cpu->bus->cartridge->pMapper->ppuMapRead(i, &mapped_addr));
    //     printf("mapped_addr: %d\n", mapped_addr);
    // }
    // printf("%x\n", GetPatternTable(0, selectedPalette));
    // printf("%x\n", ppu->sprScreen);
    // printf("%x\n", ppu->sprScreen);
    // printf("%x\n", cpuRead(0x2002));
    const int nSwatchSize = 6;
    char hex_aux[16];
    char s[1024];
    char pOAM_str [256];
    // for(int i = 0; i < 64; i++){
    //     printf("%d\n", bus.ppu->pOAM[i]);
    // }
    while (!WindowShouldClose()) {
        // printf("LOOPStart!\n");
        
        UpdateDemo();
        BeginDrawing();
            ClearBackground(DARKBLUE);
            DrawCpu(516, 2);
            //DrawCode(516, 72, 26);
            
            // for(int i = 0; i < 26; i++){
        
            //     strset(s, 0);
            //     strset(pOAM_str, 0);
            //     strcat(s, hex(i, 2, hex_aux));
            //     strcat(s, ": (");
                
            //     sprintf(pOAM_str,"%d", bus.ppu->pOAM[i * 4 + 3]);
            //     printf("1\n"); 
            //     strcat(s, pOAM_str);
            //     strset(pOAM_str, 0);strcat(s, ",");
            //     sprintf(pOAM_str,"%d", bus.ppu->pOAM[i * 4 + 0]);
            //     strcat(s, pOAM_str); strcat(s, ") ");
            //     strset(pOAM_str, 0);
            //     strcat(s, "ID: ");
            //     strcat(s, hex(bus.ppu->pOAM[i * 4 + 1], 2, hex_aux));
            //     strcat(s, " AT: ");
            //     strcat(s, hex(bus.ppu->pOAM[i * 4 + 2], 2, hex_aux));
                
            //     DrawText(s, 516, 72 + i * 10, 2, WHITE);
            // }
            
            
            for (int p = 0; p < 8; p++) // For each palette
                for(int s = 0; s < 4; s++) // For each index
                    DrawRectangle(516 + p * (nSwatchSize * 5) + s * nSwatchSize, 340, 
                        nSwatchSize, nSwatchSize, GetColorFromPaletteRam(p, s));
            //printf("GetColorFromPaletteRamDEMO!DONE\n");
            // Draw selection reticule around selected palette
            DrawRectangleLines(516 + selectedPalette * (nSwatchSize * 5) - 1, 339, (nSwatchSize * 4) + 2, nSwatchSize + 2, WHITE);
            //printf("DrawRectangleLinesDone!\n");
            DrawSprite(GetPatternTable(0, selectedPalette), 516, 348, 1);
            //printf("DrawSprite1!\n");
            DrawSprite(GetPatternTable(1, selectedPalette), 648, 348, 1);

            DrawSprite(ppu->sprScreen, 0, 0, 2);
        // printf("EndDrawingS!\n");
        EndDrawing();
        //printf("LoopDone!\n");
    }


}

int main(void) {
    
    StartDemo();
    return 0;
}