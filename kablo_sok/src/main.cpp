#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Giriş pinleri
#define KIRMIZI 33
#define SIYAH 25
#define BEYAZ 26
#define SARI 27
#define MAVI 14
#define YESIL 13

// Çıkış pinleri (LED'ler)
#define SUCCESS_LED 0
#define FAIL_LED 4

// Bilmece ve cevap
const char* puzzle = "Beyaz kabloyu kes!";
const int correctPin = BEYAZ; // Doğru kablo

void print_display(const char *str){
  Serial.println(F("print_display"));
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println(str);
  display.display();
}

void checkAnswer(int pin) {
  if (pin == correctPin) {
    // Doğru cevap
    digitalWrite(SUCCESS_LED, HIGH);
    digitalWrite(FAIL_LED, LOW);
    print_display("Tebrikler! Dogru kabloyu kestiniz.");
    for(;;);
  } else {
    // Yanlış cevap
    digitalWrite(SUCCESS_LED, LOW);
    digitalWrite(FAIL_LED, HIGH);
    print_display("Yanlis kablo! Tekrar deneyin.");

  }
  delay(3000); // 3 saniye bekle
  digitalWrite(SUCCESS_LED, LOW);
  digitalWrite(FAIL_LED, LOW);
  print_display(puzzle);
}

void setup() {
  // Seri iletişimi başlat
  Serial.begin(9600);
  Serial.println(F("setup"));

  // OLED ekranı başlat
  Wire.begin(SDA_PIN, SCL_PIN);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 başlatılamadı. Bağlantıyı kontrol edin."));
    for(;;);
  }
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Ekrana bilmeceyi yaz
  print_display(puzzle);

  pinMode(KIRMIZI, INPUT_PULLUP);
  pinMode(SIYAH, INPUT_PULLUP);
  pinMode(BEYAZ, INPUT_PULLUP);
  pinMode(SARI, INPUT_PULLUP);
  pinMode(MAVI, INPUT_PULLUP);
  pinMode(YESIL, INPUT_PULLUP);

  pinMode(SUCCESS_LED, OUTPUT);
  pinMode(FAIL_LED, OUTPUT);

  digitalWrite(SUCCESS_LED, LOW);
  digitalWrite(FAIL_LED, LOW);
}

void loop() {
  // Her bir giriş pininin durumunu kontrol et
  int temp = 0;
  if (digitalRead(KIRMIZI) == LOW) {
    Serial.println(F("34 LOW"));
    int temp = KIRMIZI;
    checkAnswer(temp);
  } else if (digitalRead(SIYAH) == LOW) {
    Serial.println(F("35 LOW"));
    int temp = SIYAH;
    checkAnswer(temp);
  } else if (digitalRead(BEYAZ) == LOW) {
    Serial.println(F("32 LOW"));
    int temp = BEYAZ;
    checkAnswer(temp);
  } else if (digitalRead(SARI) == LOW) {
    Serial.println(F("33 LOW"));
    int temp = SARI;
    checkAnswer(temp);
  } else if (digitalRead(MAVI) == LOW) {
    Serial.println(F("25 LOW"));
    int temp = MAVI;
    checkAnswer(temp);
  } else if (digitalRead(YESIL) == LOW) {
    Serial.println(F("26 LOW"));
    int temp = YESIL;
    checkAnswer(temp);
  }
}