#include <LedControl.h>

const byte rows = 6;
const byte colums = 7;
const byte dispRows = rows + 1; //one row for controlling
const byte emptyBlock = 0;
const byte Player1 = 1;
const byte Player2 = 2;

const byte redDriver = 0;
const byte blueDriver = 1;
const byte brightness = 3;
const byte scanLimit = 6;

const byte rowMap[7] = {0, 1, 2, 3, 4, 5, 6}; // 6th row = control row in unflipped
const byte colMap[7] = {0, 1, 2, 3, 4, 5, 6}; 

const int stepRow[4] = {0, 1, 1, 1}
const int stepCol[4] = {1, 0, 1, -1}

bool flip = false; // game flips depending on whose turn -flip false = player 1
bool cursorOn = false;

unsigned long  lastBlink = 0;

byte grid[rows][columns];
byte currentPlayer;

enum gameState(Playing, Win, Draw);

void setGame()  {
  for (byte i = 0; i < rows; i++) {
    for (byte j = 0; j< columns; j++)  {
      grid[i][j] = emptyBlock;
    }
  }
  currentPlayer = Player1;
  flip = false;
  gameState = Playing;
}

void setCell(byte ledRow, byte ledCol, byte value){
  if(flip){
    ledRow = dispRows - 1 - ledRow;
    ledCol = columns - 1 - ledCol;
  }
  if(value == Player1){
    lc.setLed(redDriver, rowMap[ledRow], colMap[ledCol], true);
  }
  if(value == Player2){
    lc.setLed(blueDriver, rowMap[ledRow], colMap[ledCol], true);
  }    
}

int coinDrop(byte inputCol) {
  for(int r = rows - 1; r >= 0; r--){
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
        for (int i = 1; i < 4, i++){
          if (checker != grid[r + i * stepRow[dir]][c + i * stepCol[dir]]){
            gameWon = false;
          }
        }
      }
      if (gameWon){
        return true;
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

void clearDisplay(){
  lc.clearDisplay(redDriver);
  lc.clearDisplay(blueDriver);
}

void redrawBoard(
  clearDisplay();
  drawBoard();
  blinkCursor();
)


void setup() {
  Serial.begin(9600);

  for (driver = 0; driver < 2; driver++){
    lc.shutdown(driver, false);
    lc.setIntensity(driver, brightness);
    lc.setScanLimit(driver, scanLimit);
    lc.clearDisplay(driver);
  }
  setGame();

}

void loop() {

  if(millis()- lastBlink > 500){
    lastBlink = millis();
    cursorOn = !cursorOn;
    blinkCursor(cursorOn);
  }

}
