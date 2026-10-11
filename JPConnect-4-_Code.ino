#include <LedControl.h>

#define DIN 11
#define CLK 13
#define LOAD 10
#define P1_R 2
#define P1_O 3
#define P1_L 4
#define P2_L 5
#define P2_O 6
#define P2_R 7
#define BZ1 1
#define P1_LED 9
#define P2_LED 8

LedControl lc(DIN, CLK, LOAD, 2);

const byte rows = 6;
const byte columns = 7;
const byte dispRows = rows + 1; //one row for controlling
const byte emptyBlock = 0;
const byte Player1 = 1;
const byte Player2 = 2;
const byte moveLeft = 0;
const byte moveRight = 1;
const byte drop = 2;

const byte redDriver = 0;
const byte blueDriver = 1;
const byte brightness = 3;
const byte scanLimit = 6;

const byte rowMap[7] = {0, 1, 2, 3, 4, 5, 6}; // 6th row = control row in unflipped
const byte colMap[7] = {1, 2, 3, 4, 5, 6, 7};
const byte buttons[6] = {P1_L, P1_R, P1_O, P2_L, P2_R, P2_O}; 

const int stepRow[4] = {0, 1, 1, 1};
const int stepCol[4] = {1, 0, 1, -1};

bool flip = false; // game flips depending on whose turn -flip false = player 1
bool cursorOn = false;
bool endFlash = false;

unsigned long lastFlash = 0;
unsigned long  lastBlink = 0;

byte grid[rows][columns];
byte currentPlayer;
byte cursorCol;
byte winner;
byte winRows[4];
byte winCols[4];

bool lastRead[6] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
bool currentRead[6] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
unsigned long lastChange[6] = {0, 0, 0, 0, 0, 0};

enum GameState {Playing, Won, Draw};
GameState state;

void setGame()  {
  for (byte i = 0; i < rows; i++) {
    for (byte j = 0; j< columns; j++)  {
      grid[i][j] = emptyBlock;
    }
  }
  currentPlayer = Player1;
  cursorCol = 0;
  flip = false;
  state = Playing;
  digitalWrite(BZ1, LOW);
}

void setCell(byte ledRow, byte ledCol, byte value){
  if(flip){
    ledRow = dispRows - 1 - ledRow;
    ledCol = columns - 1 - ledCol;
  }
  lc.setLed(redDriver, rowMap[ledRow], colMap[ledCol], value == Player1);
  lc.setLed(blueDriver, rowMap[ledRow], colMap[ledCol], value == Player2);
}

int readButton() {
  for(byte i = 0; i < 6; i++){
    bool val = digitalRead(buttons[i]);

    if(val != lastRead[i]){
      lastRead[i] = val;
      lastChange[i] = millis();
    }

    if(millis() - lastChange[i] > 50 && val != currentRead[i]){
      currentRead[i] = val;
      if(currentRead[i] == LOW){
        return i;
      }
    }
  }
  return -1;
}

int coinDrop(byte inputCol) {
  for(int r = 0; r < rows; r++){
    if(grid[r][inputCol] == emptyBlock){
      grid[r][inputCol] = currentPlayer;
      return r;
    }
  }
  return -1;
}

bool checkWin() {
  for(int c = 0; c < columns; c++){
    for(int r = 0; r < rows; r++){
      int checker = grid[r][c];
      if (checker == emptyBlock){
        continue;
      }

      for(byte dir = 0; dir < 4; dir++){
        int endRow = r + 3 * stepRow[dir];
        int endCol = c + 3 * stepCol[dir];
        if (endRow < 0 || endCol < 0 || endRow >= rows || endCol >= columns) {
          continue;
        }

        bool gameWon = true;
        for (int i = 1; i < 4; i++){
          if (checker != grid[r + i * stepRow[dir]][c + i * stepCol[dir]]){
            gameWon = false;
          }
        }

        if (gameWon){
          for(int j = 0; j < 4; j++){
            winRows[j] = r + j * stepRow[dir];
            winCols[j] = c + j * stepCol[dir];
          }
        return true;
      }
      }
    }
  }
  return false;
}

bool gameFull(){
  for (byte c = 0; c < columns; c++){
    if(grid[rows - 1][c] == emptyBlock){
      return false;
    }
  }
  return true;
}

void drawBoard(){
  for (byte c = 0; c < columns; c++){
    for(byte r = 0; r < rows; r++){
      setCell(r, c, grid[r][c]); 
    }
  }
}

void blinkCursor(bool on){
  cursorOn = on;
  lastBlink = millis();
  setCell(6, cursorCol, on ? currentPlayer: emptyBlock);
  digitalWrite((currentPlayer == Player1) ? P1_LED : P2_LED, on ? HIGH : LOW);
}

void clearDisplay(){
  lc.clearDisplay(redDriver);
  lc.clearDisplay(blueDriver);
}

void redrawBoard(){
  clearDisplay();
  drawBoard();
  blinkCursor(true);
}


void setup() {
  Serial.begin(9600);

  for(byte driver = 0; driver < 2; driver++){
    lc.shutdown(driver, false);
    lc.setIntensity(driver, brightness);
    lc.setScanLimit(driver, scanLimit);
    lc.clearDisplay(driver);
  }
  for(byte i = 0; i < 6; i++){
    pinMode(buttons[i], INPUT_PULLUP);
  }
  pinMode(P1_LED, OUTPUT);
  pinMode(P2_LED, OUTPUT);
  pinMode(BZ1, OUTPUT);

  setGame();
  redrawBoard();
}

void loop() {

  int pressed = readButton();
  byte whoPress = 0;
  byte whatPress = 0;
  if(pressed != -1){
    if (pressed == 0 || pressed == 1 || pressed == 2){
      whoPress = Player1;
      whatPress = pressed;
    } else{
      whoPress = Player2;
      whatPress = pressed - 3;
    }
  }

  if(state == Playing){ 
    if(millis()- lastBlink > 300){
      blinkCursor(!cursorOn);
    }

    if(pressed != -1 && whoPress == currentPlayer){
      blinkCursor(false);
      switch(whatPress) {
        case moveLeft: 
          if(cursorCol > 0){
            cursorCol --;
            blinkCursor(true);
          } else{
            cursorCol = columns - 1;
            blinkCursor(true);
          }
          break;

        case moveRight:
          if(cursorCol < columns - 1){
            cursorCol ++;
            blinkCursor(true);
          } else{
            cursorCol = 0;
            blinkCursor(true);
          }
          break;
        
        case drop: {
          int row = coinDrop(cursorCol);
          if(row != -1){
            setCell(row, cursorCol, currentPlayer);
            if(checkWin()){
              state = Won;
              winner = currentPlayer;
              endFlash = true;
              lastFlash = millis();
              digitalWrite(BZ1, HIGH);
            } else if(gameFull()){
              state = Draw;
              endFlash = true;
              lastFlash = millis();
              digitalWrite(BZ1, HIGH);
            } else{
              currentPlayer = 3 - currentPlayer; //if P1 -- 3-1 =2, if P2 -- 3-2 =1, so it changes the the nxt player
              flip = !flip;
              redrawBoard();
            }
          } else blinkCursor(true);
          break;
        }
      }
    }
  } else {
      if(millis() - lastFlash > 300){
        lastFlash = millis();
        endFlash = !endFlash;

        if(state == Won){
          for(byte i = 0; i < 4; i++){
            setCell(winRows[i], winCols[i], endFlash ? winner : emptyBlock);
          }
          digitalWrite(BZ1, endFlash ? HIGH : LOW);
        }

        if(state == Draw){
          if(endFlash)  drawBoard();
          else  clearDisplay();
        }
      }

      if(whatPress == 2){
        setGame();
        redrawBoard();
      }
  }
}
