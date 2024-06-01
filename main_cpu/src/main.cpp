#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SDA_PIN 21
#define SCL_PIN 22
#define BUZZER_PIN 13
#define LED_PIN 2 // Örnek olarak 2. pine bir LED bağladık
#define MAX_COUNTDOWN 480 // Saniye cinsinden maksimum geri sayım süresi

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

TaskHandle_t CountdownTask;

void CountdownFunction(void *pvParameters) {
  int countdown = MAX_COUNTDOWN;
  while (countdown >= 0) {
    // Ekrana kalan süreyi yaz
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Time left: ");
    display.print(countdown);
    display.println(" s");
    display.display();

    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000 - (MAX_COUNTDOWN - countdown) * 2); // Buzzer süresini azalt
    digitalWrite(BUZZER_PIN, LOW);

    countdown--;
    delay(1000);
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("BOMB EXPLODED!");
  display.display();

  while (1) {
    // loop
  }
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
  pinMode(LED_PIN, OUTPUT);

  display.clearDisplay();
  display.display();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  xTaskCreatePinnedToCore(CountdownFunction, "CountdownTask", 10000, NULL, 1, &CountdownTask, 0);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(500);
  digitalWrite(LED_PIN, LOW);
  delay(500);
}
