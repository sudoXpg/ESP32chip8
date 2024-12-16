#include<string.h>

#define PIXEL_SIZE 2

#include <Wire.h>

#include <Adafruit_GFX.h>

#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define BUZZER 2

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET - 1 // Reset pin
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, & Wire, OLED_RESET);

// LANDING.ch8
const uint8_t bindata[] = {0x00,0xE0,0xA3,0x0A,0x60,0x00,0x61,0x1F,0xD0,0x11,0x70,0x08,0x40,0x40,0x12,0x12,0x12,0x08,0xA3,0x00,0x64,0x09,0x74,0x01,0x44,0x36,0x12,0x50,0xC5,0x0B,0x86,0x50,0x76,0x0F,0x46,0x18,0xD4,0x61,0x46,0x17,0xD4,0x62,0x46,0x16,0xD4,0x63,0x46,0x15,0xD4,0x64,0x46,0x14,0xD4,0x65,0x46,0x13,0xD4,0x66,0x46,0x12,0xD4,0x67,0x46,0x11,0xD4,0x68,0x46,0x10,0xD4,0x69,0x46,0x0F,0xD4,0x6A,0x66,0x19,0xD4,0x66,0x12,0x16,0x6A,0x00,0x6B,0x00,0x69,0x02,0xA2,0xFE,0xDA,0xB1,0x60,0x14,0xF0,0x18,0x60,0x14,0xF0,0x15,0xF0,0x07,0x30,0x00,0x12,0x62,0x60,0x01,0xF0,0x18,0xDA,0xB1,0x7A,0x01,0x4A,0x3E,0x22,0x90,0xDA,0xB1,0x4F,0x01,0x12,0x9E,0x60,0x08,0xE0,0xA1,0x22,0xBE,0x48,0x01,0x22,0xD6,0x60,0x02,0xF0,0x15,0xF0,0x07,0x30,0x00,0x12,0x88,0x12,0x68,0x79,0xFF,0x39,0x00,0x12,0xFA,0x6A,0x00,0x7B,0x01,0x69,0x02,0x00,0xEE,0x60,0x0A,0xF0,0x18,0xA3,0x0E,0xF5,0x33,0xF2,0x65,0x63,0x19,0x64,0x00,0xF0,0x29,0xD3,0x45,0x73,0x05,0xF1,0x29,0xD3,0x45,0x73,0x05,0xF2,0x29,0xD3,0x45,0x12,0xBC,0x48,0x01,0x00,0xEE,0x68,0x01,0x60,0x03,0xF0,0x18,0x8D,0xA0,0x8E,0xB0,0x7D,0x01,0x7E,0x01,0xA3,0x0C,0xDD,0xE1,0x00,0xEE,0xA3,0x0C,0xDD,0xE1,0x7E,0x01,0xDD,0xE1,0x4F,0x01,0x12,0xEA,0x60,0x01,0xF0,0x18,0xA2,0xFE,0x00,0xEE,0x75,0x01,0x60,0x05,0xF0,0x18,0x68,0x00,0x4E,0x1F,0xDD,0xE1,0xA2,0xFE,0x00,0xEE,0x6A,0x00,0x00,0xEE,0xE0,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xFF,0x00,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

#define START_ADDRESS 0x200
/*
Memory Map:
+---------------+= 0xFFF (4095) End of Chip-8 RAM
|               |
|               |
|               |
|               |
|               |
| 0x200 to 0xFFF|
|     Chip-8    |
| Program / Data|
|     Space     |
|               |
|               |
|               |
+- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
|               |
|               |
|               |
+---------------+= 0x200 (512) Start of most Chip-8 programs
| 0x000 to 0x1FF|
| Reserved for  |
|  interpreter  |
+---------------+= 0x000 (0) Start of Chip-8 RAM
*/

/*
Keyboard:
    1	2	3	C
    4	5	6	D
    7	8	9	E
    A	0	B	F
*/

/*
Diaplay:
    (0,0)	(63,0)
    (0,31)	(63,31)
*/

unsigned short opcode;
unsigned char memory[4096];
unsigned char V[16];
unsigned int index_reg;
unsigned int pc;
unsigned char gfx[2048];
unsigned char scaled_gfx[4096];
unsigned char delay_timer;
unsigned char sound_timer;
unsigned int stack[16];
unsigned short sp;
unsigned char key_board[16];
//unsigned char fontset[80];
unsigned char draw_flag;
unsigned char sound_flag;

short unsigned height;
short unsigned px;

void init_cpu() {

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  // Font set >> devernay.free.fr/hacks/chip8/C8TECH10.HTM#0.1
 unsigned char fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};


  memset(memory, 0, sizeof(memory)); // initialise memory
  memcpy(memory + 0x200, bindata, sizeof(bindata));
  memset(V, 0, sizeof(V)); // initialise registers 
  index_reg = 0; // initialise index_reg register
  pc = START_ADDRESS; // initialise PC at 0x200
  sp = 0; // initialsie SP at 0x00
  memset(stack, 0, sizeof(stack)); // initialise stack 
  memset(key_board, 0, sizeof(key_board)); // initialise keyboard
  memset(gfx, 0, sizeof(gfx)); // initialise

  // set timers/flags
  delay_timer = 0;
  sound_timer = 0;
  sound_flag = 0;
  draw_flag = 0;

  memcpy(memory, fontset, sizeof(fontset)); // copy the fonts into memory
}

// int load_rom(char *rom){
//     FILE *fp = fopen(rom,"rb");
//     if(!fp){
//         fprintf(stderr,"Error in opening %s\n",rom);
//         return -1;
//     }

//     struct stat file_nfo;                                   // struct for file information
//     stat(rom,&file_nfo);                                    // populate the struct
//     size_t file_size=file_nfo.st_size;                      // get the file size

//     size_t bytes_read=fread(memory+0x200,1,sizeof(memory)-0x200,fp);    // copy the contents from the rom onto the memory
//     fclose(fp);

//     if(bytes_read!=file_size){
//         fprintf(stderr,"read error\n");
//         return -1;
//     }

//     return 0;
// }

void update_display() {
  display.clearDisplay();
  for (int y = 0; y < 32; y++) {
    for (int x = 0; x < 64; x++) {
      if (gfx[y * 64 + x]) {
        display.fillRect(x * 2, y * 2, 2, 2, WHITE);
      }
    }
  }
  display.display(); // Refresh the screen
}

void cpu_cycle() {
  draw_flag = 0;
  sound_flag = 0;

  opcode = memory[pc] << 8 | memory[pc + 1];
  unsigned short x = (opcode & 0x0F00) >> 8; // get Vx reg from the opcode [()XNN ] 
  unsigned short y = (opcode & 0x00F0) >> 4; // get Vy reg from the opcode [()NYN ]
  switch (opcode & 0xF000) {
  case 0x0000:
    switch (opcode & 0x00FF) {
    case 0x00E0:
      memset(gfx, 0, sizeof(gfx));
      pc += 2;
      break;
    case 0x00EE:
      pc = stack[sp];
      sp--;
      pc += 2;
      break;
    default:
      break;
    }
    break;

  case 0x1000:
    pc = opcode & 0x0FFF;
    break;

  case 0x2000:
    sp++;
    stack[sp] = pc;
    pc = opcode & 0x0FFF;
    break;

  case 0x3000:
    if (V[x] == (opcode & 0x00FF)) {
      pc += 2;
    }
    pc += 2;
    break;

  case 0x4000:
    if (V[x] != (opcode & 0x00FF)) {
      pc += 2;
    }
    pc += 2;
    break;

  case 0x5000:
    if (V[x] == V[y]) {
      pc += 2;
    }
    pc += 2;
    break;

  case 0x6000:
    V[x] = (opcode & 0x00FF);
    pc += 2;
    break;

  case 0x7000:
    V[x] += (opcode & 0x00FF);
    pc += 2;
    break;

  case 0x8000:
    switch (opcode & 0x000F) {
    case 0x0000:
      V[x] = V[y];
      pc += 2;
      break;
    case 0x0001:
      V[x] = (V[x] | V[y]);
      pc += 2;
      break;
    case 0x0002:
      V[x] = (V[x] & V[y]);
      pc += 2;
      break;
    case 0x0003:
      V[x] = (V[x] ^ V[y]);
      pc += 2;
      break;
    case 0x0004:
      V[x] = (V[x] + V[y]);
      pc += 2;
      break;
    case 0x0005:
      V[0xF] = (V[x] > V[y]) ? 1 : 0;
      V[x] = (V[x] - V[y]);
      pc += 2;
      break;
    case 0x0006:
      V[0xF] = V[x] & 0x0F;
      V[x] = (V[x] >> 1);
      pc += 2;
      break;
    case 0x0007:
      V[0xF] = (V[y] > V[x]) ? 1 : 0;
      V[x] = (V[y] - V[x]);
      pc += 2;
      break;
    case 0x000E:
      V[0xF] = (V[x] >> 7) & 0x1;
      V[x] = (V[x] << 1);
      pc += 2;
      break;
    default:
      break;
    }
    break;

  case 0x9000:
    if (V[x] != V[y]) {
      pc += 2;
    }
    pc += 2;
    break;

  case 0xA000:
    index_reg = opcode & 0x0FFF;
    pc += 2;
    break;

  case 0xB000:
    pc = (opcode & 0x0FFF) + V[0];
    pc += 2;
    break;

  case 0xC000:
    V[x] = (rand() % 256) & (opcode & 0x00FF);
    pc += 2;
    break;

  case 0xD000:
    draw_flag = 1;
    height = opcode & 0x000F;
    V[0xF] = 0;

    for (int yline = 0; yline < height; yline++) {
      px = memory[index_reg + yline];
      for (int xline = 0; xline < PIXEL_SIZE; xline++) {
        if ((px & (0x80 >> xline)) != 0) {
          if (gfx[(V[x] + xline + ((V[y] + yline) * 64))] ==
            1) {
            V[0xF] = 1;
          }
          gfx[V[x] + xline + ((V[y] + yline) * 64)] ^= 1;
        }
      }
    }
    pc += 2;
    break;

  case 0xE000:
    switch (opcode & 0x00FF) {
    case 0x009E:
      if (key_board[V[x]]) {
        pc += 2;
      }
      pc += 2;
      break;
    case 0x00A1:
      if (!key_board[V[x]]) {
        pc += 2;
      }
      pc += 2;
      break;
    }
    break;

  case 0xF000:
    switch (opcode & 0x00FF) {
    case 0x0007:
      V[x] = delay_timer;
      pc += 2;
      break;

    case 0x000A:
      for (int i = 0; i < sizeof(key_board); i++) {
        if (key_board[i]) {
          V[x] = i;
          pc += 2;
          break;
        }
      }
      break;

    case 0x0015:
      delay_timer = V[x];
      pc += 2;
      break;

    case 0x0018:
      sound_timer = V[x];
      pc += 2;
      break;

    case 0x001E:
      index_reg += V[x];
      pc += 2;
      break;

    case 0x0029:
      index_reg = V[x] * 5;
      pc += 2;
      break;

    case 0x0033:
      memory[index_reg] = (V[x] / 100);
      memory[index_reg + 1] = (V[x] % 100) / 10;
      memory[index_reg + 2] = (V[x] % 10);
      pc += 2;
      break;

    case 0x0055:
      for (int i = 0; i < sizeof(V); i++) {
        memory[i + index_reg] = V[i];
      }
      pc += 2;
      break;

    case 0x0065:
      for (int i = 0; i < sizeof(V); i++) {
        V[i] = memory[i + index_reg];
      }
      pc += 2;
      break;

    default:
      break;
    }
    break;

  default:
    break;
  }

  // update timers
  if (delay_timer > 0) delay_timer--;
  if (sound_timer > 0) {
    sound_flag = 1;
    if (sound_timer == 1) {
      tone(BUZZER, 60);
      usleep(20);
      noTone(BUZZER);
    }
    sound_timer--;
  }
  return;
}