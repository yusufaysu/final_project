#include <Arduino.h>

#define LED_COUNT 9
#define BUTTON_COUNT 9

int ledPins[LED_COUNT] = {22, 21, 19, 18, 5, 17, 16, 4, 15};
int buttonPins[BUTTON_COUNT] = {13, 12, 14, 27, 26, 25, 33, 32, 23};

// Pin definitions for start button, success LED and fail LED
const int startButtonPin = 35; // Define a suitable pin for start button
const int successLedPin = 2;  // Define a suitable pin for success LED
const int failLedPin = 0;     // Define a suitable pin for fail LED

int currentLevel = 1;
int levelLEDCount = 3;

int randomIndexes[LED_COUNT];
int currentSequenceIndex = 0;
bool gameInProgress = false;
bool gameStarted = false; // Flag to check if the game has started
volatile bool startButtonPressed = false; // Flag for start button interrupt

bool debounce(int pin) {
  bool state;
  bool previousState = digitalRead(pin);
  for (int i = 0; i < 5; i++) {
    delay(10);
    state = digitalRead(pin);
    if (state != previousState) {
      return false;
    }
    previousState = state;
  }
  return true;
}

void startGame() {
  char temp[20];
  sprintf(temp, "startGame state->%d", currentLevel);
  Serial.println(temp);
  currentSequenceIndex = 0;
  gameInProgress = true;
  gameStarted = true; // Set the flag when game starts

  // Turn off all LEDs at the start of the game
  for (int i = 0; i < LED_COUNT; i++) {
    digitalWrite(ledPins[i], LOW);
  }
  digitalWrite(successLedPin, LOW); // Turn off success LED
  digitalWrite(failLedPin, LOW);    // Turn off fail LED

  // Randomize LED sequence
  for (int i = 0; i < LED_COUNT; i++) {
    randomIndexes[i] = i;
  }
  for (int i = LED_COUNT - 1; i > 0; i--) {
    int j = random(0, i + 1);
    int temp = randomIndexes[i];
    randomIndexes[i] = randomIndexes[j];
    randomIndexes[j] = temp;
  }

  // Light up LEDs in the randomized order according to the current level
  for (int i = 0; i < levelLEDCount; i++) {
    digitalWrite(ledPins[randomIndexes[i]], HIGH);
    delay(1000);
    digitalWrite(ledPins[randomIndexes[i]], LOW);
    delay(500);
  }

  // Print the randomIndexes array for debugging
  for (int i = 0; i < levelLEDCount; i++) {
    sprintf(temp, "%d ", randomIndexes[i]);
    Serial.print(temp);
  }
  Serial.println();

  Serial.println("startGame_bitti");
}

void restartGame() {
  Serial.println("restartGame");

  // Turn off all LEDs
  for (int i = 0; i < LED_COUNT; i++) {
    digitalWrite(ledPins[i], LOW);
  }

  currentLevel = 1;
  levelLEDCount = 3;
  gameStarted = false; // Reset the flag when game restarts
  gameInProgress = false; // Ensure the gameInProgress is also reset
}

void checkButtonPresses() {
  for (int i = 0; i < BUTTON_COUNT; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      if (debounce(buttonPins[i])) {
        if (i == randomIndexes[currentSequenceIndex]) {
          // Correct button pressed
          digitalWrite(ledPins[i], HIGH);
          currentSequenceIndex++;
          if (currentSequenceIndex == levelLEDCount) {
            // Level completed
            gameInProgress = false;
            delay(1000);
            currentLevel++;
            if (currentLevel > 3) {
              // Game finished after 3 levels
              Serial.println("Oyun bitti");
              gameInProgress = false;
              digitalWrite(successLedPin, HIGH); // Turn on success LED
              return;
            }
            levelLEDCount++;
            if (levelLEDCount > LED_COUNT) {
              levelLEDCount = LED_COUNT; // Limit levelLEDCount to the number of available LEDs
            }
            startGame();
          }
        } else {
          // Incorrect button pressed
          digitalWrite(failLedPin, HIGH); // Turn on fail LED
          gameInProgress = false;
          restartGame(); // Call restartGame to reset the game
          return;
        }
        break;
      }
    }
  }
}

void IRAM_ATTR startButtonPress() {
  startButtonPressed = true; // Set the flag in interrupt handler
}

void setup() {
  Serial.begin(9600);
  Serial.println("setup");
  for (int i = 0; i < BUTTON_COUNT; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  for (int i = 0; i < LED_COUNT; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  pinMode(startButtonPin, INPUT_PULLUP);
  pinMode(successLedPin, OUTPUT);
  pinMode(failLedPin, OUTPUT);
  digitalWrite(successLedPin, LOW);
  digitalWrite(failLedPin, LOW);

  attachInterrupt(digitalPinToInterrupt(startButtonPin), startButtonPress, FALLING);
}

void loop() {
  if (startButtonPressed) {
    startButtonPressed = false; // Reset the flag
    if (!gameStarted) {
      startGame();
    }
  }
  
  if (gameInProgress) {
    checkButtonPresses();
  }
  delay(100);
}


/*
int g_array[9];

void makeRandomArr(int size) {
    srand(time(NULL));
    
    for (int i = 0; i < size; i++) {
        int rastgele = rand() % size + 1;
        // Daha önce üretilen bir sayıyla çakışıp çakışmadığını kontrol et
        for (int j = 0; j < i; j++) {
            if (g_array[j] == rastgele) {
                // Eğer çakışma varsa, yeni bir sayı üret
                rastgele = rand() % 9 + 1;
                // Tekrar kontrol etmek için j'yi sıfırla
                j = -1;
            }
        }
        g_array[i] = rastgele;
    }
}

void print_arr(){
    printf("Array: ");
    for (int i = 0; i < 9 ; i++) {
        printf("%d ", g_array[i]);
    }
    printf("\n");
    printf("Success\n");
}

void startGame(){

}

void setup() {
  Serial.begin(9600);
  makeRandomArr(3);
  print_arr();
}

void loop() {
    delay(1000);
    makeRandomArr(5);
    print_arr();
}
*/