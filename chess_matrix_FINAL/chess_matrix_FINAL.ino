#define rows 8 // define number of rows
#define columns 8 //define number of columns
 
int rowpins[rows] = {12, 13, 2, 3, 4, 5, 6, 7} ; // set row pin numbers
int columnpins[columns] = {8, 0, 10, A0, A1, A2, A3, A4} ; // set column pins
int currentState[rows][columns]; // array for current state during pick
int previousState[rows][columns]; // array for comparing state during pick
int reading[rows][columns]; // array for pin read value
int pick_i = 0;
int pick_j = 0;
int place_i = 0;
int place_j = 0;
int capturePick_i = 0;
int capturePick_j = 0;
 
unsigned long debounceDelay = 50;
unsigned long lastDebounceTime = 0;
 
const char* positionArray [rows][columns] ={{"a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8"},
                                            {"b1", "b2", "b3", "b4", "b5", "b6", "b7", "b8"},
                                            {"c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8"},
                                            {"d1", "d2", "d3", "d4", "d5", "d6", "d7", "d8"},
                                            {"e1", "e2", "e3", "e4", "e5", "e6", "e7", "e8"},
                                            {"f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8"},
                                            {"g1", "g2", "g3", "g4", "g5", "g6", "g7", "g8"},
                                            {"h1", "h2", "h3", "h4", "h5", "h6", "h7", "h8"}};
                                            
 
void setup () {
 
  Serial.begin (9600); // Start serial comms
  delay (2000);
  initialise (); // initialise function
 
}
 
void loop () {
 
  pickFunction();
 
}
 
void initialise () {

  Serial.println ("Board initialising");
 
  for (int i = 0; i < rows; i++) { // Set rows to LOW INPUT
    pinMode (rowpins[i], INPUT);
    digitalWrite (rowpins[i], LOW);
  }
 
  for (int j = 0; j < columns; j++) { // set columns to INPUT with PULLUP
    pinMode (columnpins[j], INPUT);
    digitalWrite (columnpins[j], HIGH);
  }
 
  for (int i = 0; i < rows; i++) {
    pinMode (rowpins[i], OUTPUT); // set row to LOW OUTPUT
    for (int j = 0; j < columns; j++) {
    reading[i][j] = digitalRead (columnpins[j]); // read board positions and set for initialise
    currentState[i][j] = reading[i][j];
    previousState[i][j] = reading[i][j];
    }
    pinMode (rowpins[i], INPUT); // set row back to INPUT
  }

  delay (200);
  Serial.println ("Current Layout:");

  for (int a = 0; a < rows; a++) {
    for (int b = 0; b < columns; b++) {
      Serial.print (reading[a][b]);
      Serial.print ("  ");
    }
    Serial.println (" ");
  }
}
 
void pickFunction () { // Search for a pick position
 
  for (int i = 0; i < rows; i++) {
    pinMode (rowpins[i], OUTPUT); // set row to LOW OUTPUT
    for (int j = 0; j < columns; j++) {
      reading[i][j] = digitalRead (columnpins[j]); // read the pin state
      if (reading[i][j] != previousState[i][j]) { // if the reading is different to the last reading
        lastDebounceTime = millis(); // reset the debounce timer
      }
      if ((millis() - lastDebounceTime) > debounceDelay) { // if the reading has stayed the same for the debounce time
        if (reading[i][j] != currentState[i][j]) { // if the reading is different to last saved state
          currentState[i][j] = reading[i][j]; // save the current reading as the current state
          if (currentState[i][j] == 1) { // if part not present
            pick_i = i; // save pick position
            pick_j = j;
            pinMode (rowpins[i], INPUT); // set row back to INPUT
            placeFunction(); // go to place function
          }
          if (currentState[i][j] == 0) { // if part present
            Serial.print ("invalid move "); // invalid move
            Serial.println (positionArray[i][j]);
          }
        }
      }
      previousState[i][j] = reading[i][j]; // set previous state as reading
    }
    pinMode (rowpins[i], INPUT); // set row back to INPUT
  }
}
 
void placeFunction () { // Search for place position
 
previousState[pick_i][pick_j] = reading[pick_i][pick_j]; // set previous state as reading
 
  while (pick_j != 0) { // loop to prevent exit while there is a pick position
    for (int i = 0; i < rows; i++) {
      pinMode (rowpins[i], OUTPUT); // set row to LOW OUTPUT
      for (int j = 0; j < columns; j++) {
        reading[i][j] = digitalRead (columnpins[j]); // read the pin state
        if (reading[i][j] != previousState[i][j]) { // if the reading is different to the last reading
          lastDebounceTime = millis(); // reset the debounce timer
        }
        if ((millis() - lastDebounceTime) > debounceDelay) { // if the reading has stayed the same for the debounce time
          if (reading[i][j] != currentState[i][j]) { // if the reading is different to last saved state
            currentState[i][j] = reading[i][j]; // save the current reading as the current state
            if (currentState[i][j] == 1) { // if part not present, it is a capture move
              capturePick_i = i; // save capture pick position
              capturePick_j = j;
              pinMode (rowpins[i], INPUT); // set row back to INPUT
              captureFunction(); // go to capture function
            }
            if (currentState[i][j] == 0) { // if part present
              place_i = i; // save place position
              place_j = j;
              pinMode (rowpins[i], INPUT); // set row back to INPUT
              moveFunction(); // go to movefunction
            } 
          }
        }
        previousState[i][j] = reading[i][j]; // set previous state as reading
      }
      pinMode (rowpins[i], INPUT); // set row back to INPUT
    }
  }
}
 
void captureFunction () { // Check place position is same as capture position
 
previousState[capturePick_i][capturePick_j] = reading[capturePick_i][capturePick_j]; // set previous state as reading
 
  while (pick_j != 0) { // loop to prevent exit while there is a pick position
    for (int i = 0; i < rows; i++) {
      pinMode (rowpins[i], OUTPUT); // set row to LOW OUTPUT
      for (int j = 0; j < columns; j++) {
        reading[i][j] = digitalRead (columnpins[j]); // read the pin state
        if (reading[i][j] != previousState[i][j]) { // if the reading is different to the last reading
          lastDebounceTime = millis(); // reset the debounce timer
        }
        if ((millis() - lastDebounceTime) > debounceDelay) { // if the reading has stayed the same for the debounce time
          if (reading[i][j] != currentState[i][j]) { // if the reading is different to last saved state
            currentState[i][j] = reading[i][j]; // save the current reading as the current state
            if (currentState[i][j] == 0 && pick_i == i && pick_j == j) { // if part present and same as original pick position
              place_i = i; // save place position
              place_j = j;
              pinMode (rowpins[i], INPUT); // set row back to INPUT
              captureMoveFunction(); // go to capturemovefunction
            } 
          }
        }
        previousState[i][j] = reading[i][j]; // set previous state as reading
      }
      pinMode (rowpins[i], INPUT); // set row back to INPUT
    }
  }
}
 
void moveFunction () { // serial print move position
 
  Serial.print ("m");
  Serial.print (positionArray[pick_i][pick_j]);
  Serial.println (positionArray[place_i][place_j]); // serial print position, i.e. ma1c1
  previousState[place_i][place_j] = reading[place_i][place_j]; // set previous state as reading
  pick_i = 0; // reset pick positions
  pick_j = 0;
}
 
void captureMoveFunction () { // serial print move position
 
  Serial.print ("m");
  Serial.print (positionArray[capturePick_i][capturePick_j]);
  Serial.println (positionArray[place_i][place_j]); // serial print position, i.e. ma1c1
  previousState[place_i][place_j] = reading[place_i][place_j]; // set previous state as reading
  currentState[place_i][place_j] = reading[place_i][place_j]; // set current state as reading
  capturePick_i = 0; // reset pick positions
  capturePick_j = 0;
  pick_i = 0;
  pick_j = 0;
}
 
void testFunction () { // Test Function
 
  for (int i = 0; i < rows; i++) {
    pinMode (rowpins[i], OUTPUT); // set row to LOW OUTPUT
    for (int j = 0; j < columns; j++) {
      reading[i][j] = digitalRead (columnpins[j]); // read the pin state
      if (reading[i][j] != previousState[i][j]) { // if the reading is different to the last reading
        lastDebounceTime = millis(); // reset the debounce timer
      }
      if ((millis() - lastDebounceTime) > debounceDelay) { // if the reading has stayed the same for the debounce time
        if (reading[i][j] != currentState[i][j]) { // if the reading is different to last saved state
          currentState[i][j] = reading[i][j]; // save the current reading as the current state
          if (currentState[i][j] == 1 || currentState[i][j] == 0) { // 1 for no part present, 0 for part present
            Serial.println (positionArray[i][j]); // print position
            for (int a = 0; a < rows; a++) {
              for (int b = 0; b < columns; b++) {
                Serial.print (reading[a][b]);
              }
              Serial.println (" ");
            }
          } 
        }
      }
      previousState[i][j] = reading[i][j]; // set previous state as reading
    }
    pinMode (rowpins[i], INPUT); // set row back to INPUT
  }
}
