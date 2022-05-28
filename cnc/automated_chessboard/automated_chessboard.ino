const int ystepsBetweenSquares = 124;
const int xstepsBetweenSquares = 133;
const int stepoff = 2;
const int stepPinX = 3; 
const int dirPinX = 4; 
const int stepPinY = 5;
const int dirPinY = 6;
const int limitSwitchX = 8;
const int limitSwitchY = 9;
const int magnetPinPos = 10;
const int magnetPinNeg = 11;
int x1 = 0; int x2 = 1;
int y1 = 0; int y2 = 1;
int xsteps;
int ysteps;
int x = 0;
int y = 0;

String readString;
String Q;

void setup() {
  Serial.begin(9600);
  pinMode(stepPinX,OUTPUT); 
  pinMode(stepoff,OUTPUT);
  pinMode(dirPinX,OUTPUT);
  pinMode(stepPinY,OUTPUT); 
  pinMode(dirPinY,OUTPUT);
  pinMode(limitSwitchX, INPUT);
  pinMode(limitSwitchY, INPUT);
  pinMode(magnetPinPos, OUTPUT);
  pinMode(magnetPinNeg, OUTPUT);
  

//_________________________________________________________________________________________________
// Zero position to square A1
//--------------------------------------------------------------------------------------------------
  zeroDiagonal();
  digitalWrite(stepoff, HIGH);
}


void loop() {
//___________________________________________________________________________________________________
// Recieve position adress of piece to be moved from Serial monitor (Letter, Number)
//---------------------------------------------------------------------------------------------------
  digitalWrite(stepoff, HIGH);
  Serial.print("Piece to be moved letter adress:  ");
  while (!Serial.available());
    char C = (char)Serial.read();
    readString += C;
    Q = readString;
           
//______________________________________________________________________________________________
// Move to Position of piece to be moved
//-----------------------------------------------------------------------------------------------
  xsteps = xstepsBetweenSquares*(x1-x2);
  ysteps = ystepsBetweenSquares*(y1-y2);
  
  movePieceDiagonal(xsteps, ysteps);
  digitalWrite(stepoff, HIGH);
 
//_______________________________________________________________________________________________
// Recieve position adress of piece new location from Serial Monitor (Letter, Number)
//-----------------------------------------------------------------------------------------------
  delay(1000);

  
//________________________________________________________________________________________________
// Turn on magnet
//------------------------------------------------------------------------------------------------
  digitalWrite(magnetPinPos, HIGH);
  digitalWrite(magnetPinNeg, LOW);

  
//________________________________________________________________________________________________
// Move piece to new location
//-------------------------------------------------------------------------------------------------
  xsteps = xstepsBetweenSquares*(x2-x1);
  ysteps = ystepsBetweenSquares*(y2-y1);
  
  digitalWrite(dirPinX, HIGH); digitalWrite(dirPinY, LOW);
  offsetPiece();
  delay(50);
  movePiece(xsteps, dirPinX, stepPinX);
  movePiece(-ysteps, dirPinY, stepPinY);
  delay(50);
  digitalWrite(dirPinX, LOW); digitalWrite(dirPinY, HIGH);
  digitalWrite(stepoff, HIGH);
  offsetPiece();

//__________________________________________________________________________________________________
// Turn off magnet
//----------------------------------------------------------------------------------------------------
  digitalWrite(magnetPinPos, LOW);
  digitalWrite(magnetPinNeg, LOW);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS ////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////

int readSerialLetter(){
  int i;
  while(i < 1 || i > 8){
    i = Serial.read(); 
    i = i - 64;
    }
  Serial.println(i);
  Serial.parseInt();
  return i;
  }

//////////////////////////////////////////////////////////////////////////////////////////////////////

int readSerialNumber(){
  int i;
  while (i < 1 || i > 8){
    i = Serial.parseInt();
    }
  Serial.println(i);
  Serial.parseInt();
  return i;
  }

////////////////////////////////////////////////////////////////////////////////////////////////////////

void zeroDiagonal(){
  digitalWrite(stepoff, LOW);
  digitalWrite(dirPinX, LOW);
  digitalWrite(dirPinY, HIGH);
  while (digitalRead(limitSwitchX) == HIGH && digitalRead(limitSwitchY) == HIGH){
    moveDiagonal(3.5);
  }
  while (digitalRead(limitSwitchX) == HIGH){
    moveLinear(stepPinX, 3.5);
  }
  while (digitalRead(limitSwitchY) == HIGH){
    moveLinear(stepPinY, 3.5);
  }
  while (digitalRead(limitSwitchX) == LOW){         //Backoff switch
    digitalWrite(dirPinX, HIGH);
    moveLinear(stepPinX, 1);
  }
  while (digitalRead(limitSwitchY) == LOW){         //Backoff switch
    digitalWrite(dirPinY, LOW);
    moveLinear(stepPinY, 1);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void movePiece(int steps, int dirPin, int stepPin){
  digitalWrite(stepoff, LOW);
  if (steps > 0){
    digitalWrite(dirPin,HIGH);
  }
  else{
    steps = abs(steps);
    digitalWrite(dirPin, LOW);
  }
  for(int i = 0; i < steps; i++) {
    moveLinear(stepPin, 3.1);
  }
  }

////////////////////////////////////////////////////////////////////////////////////////////////////////

void movePieceDiagonal(int xsteps, int ysteps){
  digitalWrite(stepoff, LOW);
  if (ysteps > 0){
    digitalWrite(dirPinY, LOW);
  }
  else{
    ysteps = abs(ysteps);
    digitalWrite(dirPinY, HIGH);
  }
  if (xsteps > 0){
    digitalWrite(dirPinX, HIGH);
  } 
  else{
    xsteps = abs(xsteps);
    digitalWrite(dirPinX, LOW);
  }
  x = 0; y = 0; 
  while (x < xsteps && y < ysteps){
    moveDiagonal(3.5);
    x++; y++;
  }
  for(x; x < xsteps; x++){
    moveLinear(stepPinX, 3.5);
  }
  for(y; y < ysteps; y++){
    moveLinear(stepPinY, 3.5);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void offsetPiece(){
  for (int i=0; i < xstepsBetweenSquares/2; i++){
    moveDiagonal(3.1);
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void moveDiagonal(int delayTime){
    digitalWrite(stepoff, LOW);
    digitalWrite(stepPinX, HIGH);
    digitalWrite(stepPinY, HIGH);
    delay(delayTime);
    digitalWrite(stepPinX, LOW);
    digitalWrite(stepPinY, LOW);
    delay(delayTime);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void moveLinear(int stepPin, int delayTime){
  digitalWrite(stepoff, LOW);
  digitalWrite(stepPin, HIGH);
  delay(delayTime);
  digitalWrite(stepPin, LOW);
  delay(delayTime);
}
