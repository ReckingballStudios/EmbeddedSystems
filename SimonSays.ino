/*
 * Mason Reck
 * reckingballstudios@gmail.com
 * A fun little simon says game on the breadboard
 */
 
const int NUM_COLORS = 3;
const int LED[3] = {11, 12, 13};
const int BUTTON[3] = {8, 9, 10};
const int RED = 0;
const int GREEN = 1;
const int BLUE = 2;

String gameState = "blinking";
int score = 0;
const int MAX_GAME_LENGTH = 20;
int sequence[MAX_GAME_LENGTH];
int guess[MAX_GAME_LENGTH];
int guessPosition = 0;
int timer = 0;

bool ledOn[3] = {false, false, false};
bool isButtonPressed[3] = {false, false, false};


/**SETUP FUNCTION**/
void setup() {
  for(int i = 0; i < NUM_COLORS; i ++){
    pinMode(LED[i], OUTPUT);
    pinMode(BUTTON[i], INPUT_PULLUP);
  }
  Serial.begin(9600);
  randomSeed(analogRead(0));
  createSequence();
}
// Function randomly assembles a brand new sequence each game
void createSequence(){
  for(int i = 0; i < MAX_GAME_LENGTH; i ++){
    sequence[i] = random(0, 3);
    guess[i] = -1;
  }
}


/**MAIN LOOP**/
void loop() {
  
  if(gameState == "blinking"){
    blinkSequence();
  }

  if(gameState == "waiting"){
    if(timer > 0){
      buttonCheck();
      timer -= 1;
      delay(1);
    } else {
      gameState = "blinking";
      timer = 0;
    }
  }

  if(gameState == "guessing"){
    buttonCheck();
  }

  if(gameState == "gameOver"){
    gameOver();
  }

  if(gameState == "victory"){
    victory();
  }
}

// A function that blinks the pattern the user is to repeat
void blinkSequence(){
  for(int i = 0; i < score + 1; i ++){
    int color = sequence[i];
    ledOn[color] = true;
    switchLights();
    delay(700);
    ledOn[color] = false;
    switchLights();
    delay(300);
  }
  gameState = "waiting";
  timer = 4000;
  guessPosition = 0;
}

// Function checks for button presses, and formulates the user's guess
void buttonCheck(){
  //Check for button presses
  for(int i = 0; i < NUM_COLORS; i ++){
    isButtonPressed[i] = digitalRead(BUTTON[i]);
    if(isButtonPressed[i]){
      //Have LED on only when you are holding the button
      ledOn[i] = true;
      switchLights();
      waitForButtonUp(BUTTON[i]);
      ledOn[i] = false;
      switchLights();
      makeGuess(i);
    }
  }
}

void makeGuess(int color){
  gameState = "guessing";
  guess[guessPosition] = color;
  //Check to see if the guess is correct
  if(guess[guessPosition] != sequence[guessPosition] or gameState == "gameOver"){
    gameState = "gameOver";
    Serial.println("Score: " + (String)score);
    return;
  }
  guessPosition ++;
  // If the guessPosition is greater than user's score, then the user has finished their guess
  if(guessPosition > score){
    score ++;
    gameState = "blinking";
    if(score == MAX_GAME_LENGTH){
      gameState = "victory";
    }
    delay(1000);
  }
}

// Function to tell the user that the game is over, and they lost
void gameOver(){
  ledOn[RED] = true;
  switchLights();
  delay(50);
  ledOn[RED] = false;
  switchLights();
  delay(50);
}

// Function to tell the user that the game is over, and they won
void victory(){
  ledOn[BLUE] = true;
  ledOn[RED] = false;
  switchLights();
  delay(50);
  ledOn[BLUE] = false;
  ledOn[GREEN] = true;
  switchLights();
  delay(50);
  ledOn[GREEN] = false;
  ledOn[RED] = true;
  switchLights();
  delay(50);
}

// Function turns the lights on and off
void switchLights(){
  //Switch the lights on and off
  for(int i = 0; i < NUM_COLORS; i ++){
    if(ledOn[i]){
      digitalWrite(LED[i], HIGH);
    } else {
      digitalWrite(LED[i], LOW);
    }
  }
}

// Simple method that waits for a button to be released
void waitForButtonUp(int pinNumber){
  while(digitalRead(pinNumber)){
    
  }
  delay(100);
}
