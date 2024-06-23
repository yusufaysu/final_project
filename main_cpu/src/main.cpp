#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SDA_PIN 21
#define SCL_PIN 22
#define BUZZER_PIN 13
#define BUTTON1_PIN 32
#define BUTTON2_PIN 33
#define BUTTON3_PIN 25
#define BUTTON4_PIN 26
#define INPUT1_PIN 27
#define INPUT2_PIN 14
#define INPUT3_PIN 12
#define MAX_COUNTDOWN 480 // Saniye cinsinden maksimum geri sayım süresi

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

TaskHandle_t CountdownTask;

volatile bool input1_triggered = false;
volatile bool input2_triggered = false;
volatile bool input3_triggered = false;
bool num1_displayed = false;
bool num2_displayed = false;
bool num3_displayed = false;
bool exploded = false;
int numbers[4];

void IRAM_ATTR onInput1() {
  input1_triggered = true;
}

void IRAM_ATTR onInput2() {
  input2_triggered = true;
}

void IRAM_ATTR onInput3() {
  input3_triggered = true;
}

void displayNumber(int number) {
  Serial.println(F("displayNumber"));
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(number); 
  display.display();
  delay(3000);
  display.clearDisplay();
  display.display();
}

void CountdownFunction(void *pvParameters) {
  int countdown = MAX_COUNTDOWN;
  while (countdown >= 0) {

    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000 - (MAX_COUNTDOWN - countdown) * 2); // Buzzer süresini azalt
    digitalWrite(BUZZER_PIN, LOW);

    countdown--;
    delay(1000 - (MAX_COUNTDOWN - countdown) * 2);
  }

  digitalWrite(BUZZER_PIN, HIGH);
  delay(1000);
  digitalWrite(BUZZER_PIN, LOW);
  
  exploded = true;
}

void setup() {
  Serial.begin(9600);

  // I2C
  Wire.begin(SDA_PIN, SCL_PIN);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 başlatılamadı. Bağlantıyı kontrol edin."));
    for(;;);
  }

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BUTTON3_PIN, INPUT_PULLUP);
  pinMode(BUTTON4_PIN, INPUT_PULLUP);
  pinMode(INPUT1_PIN, INPUT);
  pinMode(INPUT2_PIN, INPUT);
  pinMode(INPUT3_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(INPUT1_PIN), onInput1, HIGH);
  attachInterrupt(digitalPinToInterrupt(INPUT2_PIN), onInput2, HIGH);
  attachInterrupt(digitalPinToInterrupt(INPUT3_PIN), onInput3, HIGH);

  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  numbers[0] = random(1, 100);
  numbers[1] = random(1, 100);
  numbers[2] = random(1, 100);
  numbers[3] = random(1, 100);

  xTaskCreatePinnedToCore(CountdownFunction, "CountdownTask", 10000, NULL, 1, &CountdownTask, 0);
}

void loop() {
  if (exploded)
  {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("BOMB EXPLODED!");
    display.display();
    while (1);
  }
  Serial.println((int)numbers[0]);
  Serial.println((int)numbers[1]);
  Serial.println((int)numbers[2]);
  Serial.println((int)numbers[3]);

  while (!num1_displayed || !num2_displayed || !num3_displayed) {
    if (input1_triggered && !num1_displayed) {
      displayNumber(numbers[0]);
      num1_displayed = true;
    }

    if (input2_triggered && !num2_displayed) {
      displayNumber(numbers[1]);
      num2_displayed = true;
    }

    if (input3_triggered && !num3_displayed) {
      displayNumber(numbers[2]);
      num3_displayed = true;
    }
    delay(100);
  }

  // Doğru cevabı rastgele bir şıkka yerleştir
  int correctIndex = random(0, 4);
  int options[4];
  for (int i = 0, j = 0; i < 4; i++) {
    if (i == correctIndex) {
      options[i] = numbers[3];
    } else {
      options[i] = numbers[j++];
    }
  }
  Serial.println((int)correctIndex);

  // Şıkları yazdır
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Hangi sayiyi gormedin");
  display.setCursor(0, 15);
  display.print("A:");
  display.println(options[0]);
  display.setCursor(30, 15);
  display.print("B:");
  display.println(options[1]);
  display.setCursor(60, 15);
  display.print("C:");
  display.println(options[2]);
  display.setCursor(90, 15);
  display.print("D:");
  display.println(options[3]);
  display.display();

  // Kullanıcı girdisini kontrol et
  bool correct = false;
  while (!correct) {
    if (digitalRead(BUTTON1_PIN) == LOW) {
      if (correctIndex == 0) {
        correct = true;
        Serial.println("BUTTON1_PIN doğru");
      } else {
        Serial.println("BUTTON1_PIN yanlış");
        break;
      }
    }
    if (digitalRead(BUTTON2_PIN) == LOW) {
      if (correctIndex == 1) {
        correct = true;
        Serial.println("BUTTON2_PIN doğru");
      } else {
        Serial.println("BUTTON2_PIN yanlış");
        break;
      }
    }
    if (digitalRead(BUTTON3_PIN) == LOW) {
      if (correctIndex == 2) {
        correct = true;
        Serial.println("BUTTON3_PIN doğru");
      } else {
        Serial.println("BUTTON3_PIN yanlış");
        break;
      }
    }
    if (digitalRead(BUTTON4_PIN) == LOW) {
      if (correctIndex == 3) {
        correct = true;
        Serial.println("BUTTON4_PIN doğru");
      } else {
        Serial.println("BUTTON4_PIN yanlış");
        break;
      }
    }
    delay(100);
  }

  if (correct) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Tebrikler bombayi imha ettiniz");
    display.display();
    vTaskDelete(CountdownTask); // Stop the countdown
    digitalWrite(BUZZER_PIN, LOW);
  } else {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("BOMB EXPLODED!");
    display.display();
    vTaskDelete(CountdownTask); // Stop the countdown
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);
  }

  while (1) {
    // loop
  }
}
