#include "cpu.h"
#include <Keypad.h>



int inputpins[] {32, 33, 25, 26,27, 14, 12,13};
int count=0;

const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'C'},
  {'4', '5', '6', 'D'},
  {'7', '8', '9', 'E'},
  {'A', '0', 'B', 'F'}
};
byte rowPins[ROWS] = {32, 33, 25, 26}; /* connect to the row pinouts of the keypad */
byte colPins[COLS] = {27, 14, 12,13 }; /* connect to the column pinouts of the keypad */
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


void setup() {
  for(int i=0;i<sizeof(inputpins)/sizeof(inputpins[0]);i++){
    pinMode(inputpins[i],INPUT);
  }
  pinMode(BUZZER,OUTPUT);
}

void loop() {

  char customKey ='z';
  init_cpu();
  while(1){
    count+=1;
    customKey= customKeypad.getKey();
    if(customKey){
    
    switch (customKey) {
      case '1': key_board[0] = 1; break;
      case '2': key_board[1] = 1; break;
      case '3': key_board[2] = 1; break;
      case 'C': key_board[3] = 1; break;
      case '4': key_board[4] = 1; break;
      case '5': key_board[5] = 1; break;
      case '6': key_board[6] = 1; break;
      case 'D': key_board[7] = 1; break;
      case '7': key_board[8] = 1; break;
      case '8': key_board[9] = 1; break;
      case '9': key_board[10] = 1; break;
      case 'E': key_board[11] = 1; break;
      case 'A': key_board[12] = 1; break;
      case '0': key_board[13] = 1; break;
      case 'B': key_board[14] = 1; break;
      case 'F': key_board[15] = 1; break;
      default: break;
    }
    }
    usleep(100);
    if(count == 100){    // reset after 100 cycles
      memset(key_board,0,sizeof(key_board));
      count=0;
    }
    for(int i=0;i<sizeof(key_board);i++){
      printf("%d, ",key_board[i]);
    }
    printf("    count : %d\n",count);
    cpu_cycle();
    if(draw_flag==1){
      update_display();
    }
  }
}
