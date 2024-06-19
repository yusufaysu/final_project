#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SDA_PIN 21
#define SCL_PIN 22

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define MAX_CHARS_PER_LINE (SCREEN_WIDTH / 6)

// Giriş pinleri
#define KIRMIZI 33
#define SIYAH 25
#define BEYAZ 26
#define SARI 27
#define MAVI 14
#define YESIL 13

// Çıkış pinleri (LED'ler)
#define SUCCESS_LED 16
#define FAIL_LED 4

// Bilmece ve cevap
struct Bilmece {
  const char* soru;
  int dogruPin;
};

// Siyah renk için bilmeceler
Bilmece siyah_bilmeceler[] = {
  {"Gecenin ortusu, her seyi gizler, Gozlerimizi kapattigimizda onunla bulusuruz. Bu karanlik nedir?", SIYAH},
  {"Gun isiginin kayboldugu yerde, Sessizlik hukum surer, yildizlar parlar. Hangi renkli ortu geceyi kaplar?", SIYAH},
  {"Guvercinlerin kanatlarinda, Bir damla gokyuzu karanligi tasiyan. Bu koyu renkteki kus nedir?", SIYAH},
  {"Yaz gecelerinde kamp atesi sonduruldugunde, Gozlerimizi acsak da goremeyiz. Bu koyu renkteki perde nedir?", SIYAH},
  {"Murekkep gibi yogun, gece kadar sessiz, Gozlerimizi kapatiriz ve icine dalariz. Bu renk nedir?", SIYAH}
};

// Kırmızı renk için bilmeceler
Bilmece kirmizi_bilmeceler[] = {
  {"Icinde sicaklik barindirir, yanardag gibi alevlenir, Kalplerin en derin hislerinde bulunur. Bu tutkulu renk nedir?", KIRMIZI},
  {"Gullerin tac yapraklarinda, Aski ve savasi anlatan. Hangi canli renk?", KIRMIZI},
  {"Ilkbaharin habercisi degil, sonbaharin getirdigi degil, Trafikte dur diyen, yanginin uyardigi. Bu belirgin renk nedir?", KIRMIZI},
  {"Parlak bir elma, tatli bir isirik, Hem meyvede hem kalpte bulunur. Hangi cazip renk?", KIRMIZI},
  {"Kanin icinde dolasir, yasami tasir, Hem tehlikenin hem tutkunun simgesi. Bu guclu renk nedir?", KIRMIZI}
};

// Sarı renk için bilmeceler
Bilmece sari_bilmeceler[] = {
  {"Gokyuzunde parlar, isi ve isik verir, Dunya onunla aydinlanir. Bu parlak renk nedir?", SARI},
  {"Altin gibi parlak, yazin habercisi, Her sabah dogar, her aksam batar. Bu sicak renk nedir?", SARI},
  {"Sonbahar yapraklarinin rengi, Goz alici ve parlak. Hangi sicak renk?", SARI},
  {"Civcivlerin tuyleri, balin tatli ozu, Doganin neseli tonu. Hangi parlak renk?", SARI},
  {"Trafikte dikkat der, yavasla der, Kirmizi ile yesilin arasinda. Bu ikaz rengi nedir?", SARI}
};

// Yeşil renk için bilmeceler
Bilmece yesil_bilmeceler[] = {
  {"Ormanlarin derinlerinde, yapraklarin ustunde, Huzurun ve tazeliğin rengi. Bu dogal renk nedir?", YESIL},
  {"Ilkbaharin baslangicinda, cimenlerin renginde, Doganin canlanisini temsil eder. Hangi canli renk?", YESIL},
  {"Trafikte ilerle der, yol acar, Hareket ve ozgurluk sembolu. Bu yonlendirici renk nedir?", YESIL},
  {"Nane yapraklarinin kokusu, zeytin agaclarinin rengi, Hem mutfakta hem dogada bulunur. Bu aromatik renk nedir?", YESIL},
  {"Denizlerin altindaki yosunlar, baharin ilk filizleri, Hem serinletir hem rahatlatir. Hangi taze renk?", YESIL}
};

// Mavi renk için bilmeceler
Bilmece mavi_bilmeceler[] = {
  {"Gozlerimizi yukari cevirdigimizde, Sonsuzluk hissi verir, bulutsuz gunlerde. Bu sakin renk nedir?", MAVI},
  {"Denizlerin ve okyanuslarin derinliklerinde, Goz alici ve serinletici. Bu huzurlu renk nedir?", MAVI},
  {"Gecenin ardindan gelen sabah, Gokyuzunde beliren huzur. Bu dingin renk nedir?", MAVI},
  {"Barisin ve sadakatin simgesi, Hem bayraklarda hem denizlerde gorulur. Hangi renk anlatilir burada?", MAVI},
  {"Aksamustu gunes batarken, Gokyuzunde beliren serinlik. Hangi sakin renk?", MAVI}
};

// Beyaz renk için bilmeceler
Bilmece beyaz_bilmeceler[] = {
  {"Kar taneleri gibi saf ve temiz, Her yeri kaplar, soguk kis gunlerinde. Bu saf renk nedir?", BEYAZ},
  {"Gelinliklerdeki masumiyet, Isigin birlesimiyle olusur. Bu zarif renk nedir?", BEYAZ},
  {"Pamuk tarlalarinda, bulutlarin arasinda, Huzur ve sukunet verir. Bu saf renk nedir?", BEYAZ},
  {"Gokkusaginin tum renkleri birlesince, Ortaya cikar, her seyin baslangici gibi. Bu aydinlik renk nedir?", BEYAZ},
  {"Kisin toprağı orter, safligin simgesi, Hem kagitta hem karda bulunur. Bu sade renk nedir?", BEYAZ}
};

// Bilmece kategorileri ve sayıları
Bilmece* bilmece_kategorileri[] = {siyah_bilmeceler, kirmizi_bilmeceler, sari_bilmeceler, yesil_bilmeceler, mavi_bilmeceler, beyaz_bilmeceler};
int bilmece_sayilari[] = {5, 5, 5, 5, 5, 5};

Bilmece secilenBilmece;
String secilenBilmeceMetin;
int metinIndex = 0;
unsigned long lastUpdateTime = 0;
const int displayInterval = 10; // Kaydırma hızı (ms cinsinden)

void print_display(const char *str, int offset) {
  display.clearDisplay();
  display.setCursor(offset, 16);

  int len = strlen(str);
  if (len > MAX_CHARS_PER_LINE) {
    // Create a temporary buffer for the substring
    char buffer[MAX_CHARS_PER_LINE + 1];
    strncpy(buffer, str, MAX_CHARS_PER_LINE);
    buffer[MAX_CHARS_PER_LINE] = '\0'; // Null-terminate the string
    display.print(buffer);
  } else {
    display.print(str);
  }

  display.display();
}

void scroll_text(const char *str, int y, int delay_ms) {
  int textWidth = strlen(str) * 6; // Approximate text width (assuming 6 pixels per character)
  int scrollWidth = textWidth - SCREEN_WIDTH;

  // Display the start of the text
  char startBuffer[MAX_CHARS_PER_LINE + 1];
  strncpy(startBuffer, str, MAX_CHARS_PER_LINE);
  startBuffer[MAX_CHARS_PER_LINE] = '\0';
  print_display(startBuffer, 0);
  delay(1000);

  // Scroll the text
  for (int offset = 0; offset <= scrollWidth; offset++) {
    char buffer[MAX_CHARS_PER_LINE + 1];
    int startIndex = offset / 6; // Calculate the starting character index based on pixel offset
    strncpy(buffer, str + startIndex, MAX_CHARS_PER_LINE);
    buffer[MAX_CHARS_PER_LINE] = '\0'; // Null-terminate the string

    print_display(buffer, -(offset % 6)); // Pass the substring to print_display with pixel offset
    delay(delay_ms);
  }

  // Display the end of the text
  int endIndex = strlen(str) - MAX_CHARS_PER_LINE;
  strncpy(startBuffer, str + endIndex, MAX_CHARS_PER_LINE);
  startBuffer[MAX_CHARS_PER_LINE] = '\0';
  print_display(startBuffer, 0);
  delay(1000);
}

void checkAnswer(int pin) {
  Serial.println(F("checkAnswer"));
  if (pin == secilenBilmece.dogruPin) {
    // Doğru cevap
    digitalWrite(SUCCESS_LED, HIGH);
    digitalWrite(FAIL_LED, LOW);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Tebrikler! Dogru kabloyu kestiniz.");
    display.display();
    for (;;);
  } else {
    // Yanlış cevap
    digitalWrite(SUCCESS_LED, LOW);
    digitalWrite(FAIL_LED, HIGH);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Yanlis kablo! Tekrar deneyin.");
    display.display();
  }
  delay(3000); // 3 saniye bekle
  digitalWrite(SUCCESS_LED, LOW);
  digitalWrite(FAIL_LED, LOW);
  metinIndex = 0;
  lastUpdateTime = millis();
}

void setup() {
  // Seri iletişimi başlat
  Serial.begin(9600);
  Serial.println(F("setup"));

  // OLED ekranı başlat
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 baslatilamadi. Baglantiyi kontrol edin."));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Pinleri ayarla
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

  // Rastgele bir renk kategorisi ve bilmece seç
  randomSeed(analogRead(0));
  int renk_indeksi = random(0, sizeof(bilmece_kategorileri) / sizeof(bilmece_kategorileri[0]));
  int bilmece_indeksi = random(0, bilmece_sayilari[renk_indeksi]);
  secilenBilmece = bilmece_kategorileri[renk_indeksi][bilmece_indeksi];

  // Seçilen bilmecenin soru kısmını ekrana yaz
  secilenBilmeceMetin = String(secilenBilmece.soru);
  print_display(secilenBilmeceMetin.c_str(), 0);
}

unsigned long lastScrollTime = 0;
const unsigned long scrollInterval = 4; // 10 saniye

void loop() {
  unsigned long currentMillis = millis();
  Serial.println(F("loop"));

  // Her bir giriş pininin durumunu kontrol et
  if (digitalRead(KIRMIZI) == LOW) {
  Serial.println(F("1")); 
    Serial.println(F("KIRMIZI LOW"));
    checkAnswer(KIRMIZI);
  } else if (digitalRead(SIYAH) == LOW) {
  Serial.println(F("2"));
    Serial.println(F("SIYAH LOW"));
    checkAnswer(SIYAH);
  } else if (digitalRead(BEYAZ) == LOW) {
  Serial.println(F("3"));
    Serial.println(F("BEYAZ LOW"));
    checkAnswer(BEYAZ);
  } else if (digitalRead(SARI) == LOW) {
  Serial.println(F("4"));
    Serial.println(F("SARI LOW"));
    checkAnswer(SARI);
  } else if (digitalRead(MAVI) == LOW) {
  Serial.println(F("5"));
    Serial.println(F("MAVI LOW"));
    checkAnswer(MAVI);
  } else if (digitalRead(YESIL) == LOW) {
  Serial.println(F("6"));
    Serial.println(F("YESIL LOW"));
    checkAnswer(YESIL);
  }

  // Metni kaydırma işlemi
  if (currentMillis - lastScrollTime >= scrollInterval) {
    scroll_text(secilenBilmeceMetin.c_str(), 16, 10);
    lastScrollTime = currentMillis;
  }
}