#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED ekran ayarları
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

// I2C pin tanımları
#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin tanımlamaları
const int success_led_pin = 12;
const int fail_led_pin = 14;
const int yellow_led_pin = 13; // Sarı LED pin tanımı
const int button_1_pin = 25;
const int button_2_pin = 26;
const int button_3_pin = 27;

// Değişkenler
int correctAnswer;
int consecutiveCorrectCount = 0; // Ardışık doğru cevap sayacı

// OLED ekrana soruyu ve şıkları yazdırma fonksiyonu
void displayQuestion(String question, int correctAnswer, int correctPos) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(question);

  int options[3];
  options[correctPos] = correctAnswer; // Doğru cevabı ata

  // Diğer iki seçeneği rasgele belirle ve tekrar kontrolü yap
  for (int i = 0; i < 3; i++) {
    if (i != correctPos) {
      int temp = 0;
      do {
        temp = random(correctAnswer - 5, correctAnswer + 5); // Rasgele bir sayı aralığı seçebilirsiniz, burada 1 ile 20 arasını seçtim
      } while (temp == correctAnswer || temp == options[0] || temp == options[1]); // Tekrar kontrolü

      options[i] = temp;
    }
  }

  // Şıkları ekrana yazdır
  for (int i = 0; i < 3; i++) {
    display.setCursor(0 + (i * 40), 10);
    display.println(String((char)('A' + i)) + ": " + String(options[i]));
  }

  display.display();
}

// Rastgele işlem ve şıklar oluşturma fonksiyonu
void generateQuestion() {
  int num1 = random(1, 10);
  int num2 = random(1, 10);
  int operation = random(0, 3);
  int answer;
  String question;

  switch (operation) {
    case 0:
      question = String(num1) + " + " + String(num2) + " = ?";
      answer = num1 + num2;
      break;
    case 1:
      question = String(num1) + " - " + String(num2) + " = ?";
      answer = num1 - num2;
      break;
    case 2:
      question = String(num1) + " * " + String(num2) + " = ?";
      answer = num1 * num2;
      break;
  }

  correctAnswer = random(0, 3);
  displayQuestion(question, answer, correctAnswer);
}

// Buton durumu kontrol fonksiyonu
void checkAnswer() {
  if (digitalRead(button_1_pin) == LOW) {
    if (correctAnswer == 0) {
      digitalWrite(yellow_led_pin, HIGH); // Sarı LED'i yak
      delay(500); // Kısa bir süre sarı LED'i yakık tut
      digitalWrite(yellow_led_pin, LOW); // Sarı LED'i söndür
      consecutiveCorrectCount++;
      if (consecutiveCorrectCount >= 5) {
        digitalWrite(success_led_pin, HIGH); // Başarı LED'i yak
        for(;;){}
      }
    } else {
      digitalWrite(fail_led_pin, HIGH);
      delay(1000);
      digitalWrite(fail_led_pin, LOW);
      consecutiveCorrectCount = 0; // Yanlış cevap alındığında sayaçı sıfırla
    }
    generateQuestion();
  } else if (digitalRead(button_2_pin) == LOW) {
    if (correctAnswer == 1) {
      digitalWrite(yellow_led_pin, HIGH); // Sarı LED'i yak
      delay(500); // Kısa bir süre sarı LED'i yakık tut
      digitalWrite(yellow_led_pin, LOW); // Sarı LED'i söndür
      consecutiveCorrectCount++;
      if (consecutiveCorrectCount >= 5) {
        digitalWrite(success_led_pin, HIGH); // Başarı LED'i yak
        for(;;){}
      }
    } else {
      digitalWrite(fail_led_pin, HIGH);
      delay(1000);
      digitalWrite(fail_led_pin, LOW);
      consecutiveCorrectCount = 0; // Yanlış cevap alındığında sayaçı sıfırla
    }
    generateQuestion();
  } else if (digitalRead(button_3_pin) == LOW) {
    if (correctAnswer == 2) {
      digitalWrite(yellow_led_pin, HIGH); // Sarı LED'i yak
      delay(500); // Kısa bir süre sarı LED'i yakık tut
      digitalWrite(yellow_led_pin, LOW); // Sarı LED'i söndür
      consecutiveCorrectCount++;
      if (consecutiveCorrectCount >= 5) {
        digitalWrite(success_led_pin, HIGH); // Başarı LED'i yak
        for(;;){}
      }
    } else {
      digitalWrite(fail_led_pin, HIGH);
      delay(1000);
      digitalWrite(fail_led_pin, LOW);
      consecutiveCorrectCount = 0; // Yanlış cevap alındığında sayaçı sıfırla
    }
    generateQuestion();
  }
}

void setup() {
  // I2C başlatma
  Wire.begin(SDA_PIN, SCL_PIN);

  // OLED ekranı başlatma
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000); // Ekran başlangıcı için bekleme süresi
  display.clearDisplay();
  
  // Pin modları
  pinMode(success_led_pin, OUTPUT);
  pinMode(fail_led_pin, OUTPUT);
  pinMode(yellow_led_pin, OUTPUT); // Sarı LED için pin modu
  pinMode(button_1_pin, INPUT_PULLUP);
  pinMode(button_2_pin, INPUT_PULLUP);
  pinMode(button_3_pin, INPUT_PULLUP);

  // Seri iletişim başlatma (hata ayıklama için)
  Serial.begin(9600);

  // İlk soruyu oluştur
  generateQuestion();
}

void loop() {
  checkAnswer();
  delay(100);
}
