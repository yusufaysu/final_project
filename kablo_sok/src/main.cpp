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

// Bilmece kategorileri
Bilmece siyah_bilmeceler[] = {
  {"Gecenin ortusu, her seyi gizler, Gozlerimizi kapattigimizda onunla bulusuruz. Bu karanlik nedir?", SIYAH},
  {"Gun isiginin kayboldugu yerde, Sessizlik hukum surer, yildizlar parlar. Hangi renkli ortu geceyi kaplar?", SIYAH},
  {"Guvercinlerin kanatlarinda, Bir damla gokyuzu karanligi tasiyan. Bu koyu renkteki kus nedir?", SIYAH},
  {"Yaz gecelerinde kamp atesi sonduruldugunde, Gozlerimizi acsak da goremeyiz. Bu koyu renkteki perde nedir?", SIYAH},
  {"Murekkep gibi yogun, gece kadar sessiz, Gozlerimizi kapatiriz ve icine dalariz. Bu renk nedir?", SIYAH}
};

Bilmece kirmizi_bilmeceler[] = {
  {"Icinde sicaklik barindirir, yanardag gibi alevlenir, Kalplerin en derin hislerinde bulunur. Bu tutkulu renk nedir?", KIRMIZI},
  {"Gullerin tac yapraklarinda, Aski ve savasi anlatan. Hangi canli renk?", KIRMIZI},
  {"Ilkbaharin habercisi degil, sonbaharin getirdigi degil, Trafikte dur diyen, yanginin uyardigi. Bu belirgin renk nedir?", KIRMIZI},
  {"Parlak bir elma, tatli bir isirik, Hem meyvede hem kalpte bulunur. Hangi cazip renk?", KIRMIZI},
  {"Kanin icinde dolasir, yasami tasir, Hem tehlikenin hem tutkunun simgesi. Bu guclu renk nedir?", KIRMIZI}
};

Bilmece sari_bilmeceler[] = {
  {"Gokyuzunde parlar, isi ve isik verir, Dunya onunla aydinlanir. Bu parlak renk nedir?", SARI},
  {"Altin gibi parlak, yazin habercisi, Her sabah dogar, her aksam batar. Bu sicak renk nedir?", SARI},
  {"Sonbahar yapraklarinin rengi, Goz alici ve parlak. Hangi sicak renk?", SARI},
  {"Civcivlerin tuyleri, balin tatli ozu, Doganin neseli tonu. Hangi parlak renk?", SARI},
  {"Trafikte dikkat der, yavasla der, Kirmizi ile yesilin arasinda. Bu ikaz rengi nedir?", SARI}
};

Bilmece yesil_bilmeceler[] = {
  {"Ormanlarin derinlerinde, yapraklarin ustunde, Huzurun ve tazeliğin rengi. Bu dogal renk nedir?", YESIL},
  {"Ilkbaharin baslangicinda, cimenlerin renginde, Doganin canlanisini temsil eder. Hangi canli renk?", YESIL},
  {"Trafikte ilerle der, yol acar, Hareket ve ozgurluk sembolu. Bu yonlendirici renk nedir?", YESIL},
  {"Nane yapraklarinin kokusu, zeytin agaclarinin rengi, Hem mutfakta hem dogada bulunur. Bu aromatik renk nedir?", YESIL},
  {"Denizlerin altindaki yosunlar, baharin ilk filizleri, Hem serinletir hem rahatlatir. Hangi taze renk?", YESIL}
};

Bilmece mavi_bilmeceler[] = {
  {"Gozlerimizi yukari cevirdigimizde, Sonsuzluk hissi verir, bulutsuz gunlerde. Bu sakin renk nedir?", MAVI},
  {"Denizlerin ve okyanuslarin derinliklerinde, Goz alici ve serinletici. Bu huzurlu renk nedir?", MAVI},
  {"Gecenin ardindan gelen sabah, Gokyuzunde beliren huzur. Bu dingin renk nedir?", MAVI},
  {"Barisin ve sadakatin simgesi, Hem bayraklarda hem denizlerde gorulur. Hangi renk anlatilir burada?", MAVI},
  {"Aksamustu gunes batarken, Gokyuzunde beliren serinlik. Hangi sakin renk?", MAVI}
};

Bilmece beyaz_bilmeceler[] = {
  {"Kar taneleri gibi saf ve temiz, Her yeri kaplar, soguk kis gunlerinde. Bu saf renk nedir?", BEYAZ},
  {"Gelinliklerdeki masumiyet, Isigin birlesimiyle olusur. Bu zarif renk nedir?", BEYAZ},
  {"Pamuk tarlalarinda, bulutlarin arasinda, Huzur ve sukunet verir. Bu saf renk nedir?", BEYAZ},
  {"Gokkusaginin tum renkleri birlesince, Ortaya cikar, her seyin baslangici gibi. Bu aydinlik renk nedir?", BEYAZ},
  {"Kisin toprağı orter, safligin simgesi, Hem kagitta hem karda bulunur. Bu sade renk nedir?", BEYAZ}
};

Bilmece* bilmece_kategorileri[] = {siyah_bilmeceler, kirmizi_bilmeceler, sari_bilmeceler, yesil_bilmeceler, mavi_bilmeceler, beyaz_bilmeceler};
int bilmece_sayilari[] = {5, 5, 5, 5, 5, 5};

Bilmece secilenBilmece;
String secilenBilmeceMetin;

unsigned long lastScrollTime = 0;
const unsigned long scrollInterval = 4; // 10 saniye

void checkAnswer(int pin) {
  if (pin == secilenBilmece.dogruPin) {
    digitalWrite(SUCCESS_LED, HIGH);
    digitalWrite(FAIL_LED, LOW);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Tebrikler! Dogru kabloyu kestiniz.");
    display.display();
    for (;;);
  } else {
    digitalWrite(SUCCESS_LED, LOW);
    digitalWrite(FAIL_LED, HIGH);
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Yanlis kablo! Tekrar deneyin.");
    display.display();
  }
  delay(3000);
  digitalWrite(SUCCESS_LED, LOW);
  digitalWrite(FAIL_LED, LOW);
}

void print_display(const char *str, int offset) {
  display.clearDisplay();
  display.setCursor(offset, 16);

  int len = strlen(str);
  if (len > MAX_CHARS_PER_LINE) {
    char buffer[MAX_CHARS_PER_LINE + 1];
    strncpy(buffer, str, MAX_CHARS_PER_LINE);
    buffer[MAX_CHARS_PER_LINE] = '\0'; 
    display.print(buffer);
  } else {
    display.print(str);
  }

  display.display();
}

void scroll_text(const char *str, int y, int delay_ms) {
  int textWidth = strlen(str) * 6;
  int scrollWidth = textWidth - SCREEN_WIDTH;

  char startBuffer[MAX_CHARS_PER_LINE + 1];
  strncpy(startBuffer, str, MAX_CHARS_PER_LINE);
  startBuffer[MAX_CHARS_PER_LINE] = '\0';
  print_display(startBuffer, 0);
  delay(1000);

  for (int offset = 0; offset <= scrollWidth; offset++) {
    char buffer[MAX_CHARS_PER_LINE + 1];
    int startIndex = offset / 6;
    strncpy(buffer, str + startIndex, MAX_CHARS_PER_LINE);
    buffer[MAX_CHARS_PER_LINE] = '\0';

    print_display(buffer, -(offset % 6));

    if (digitalRead(KIRMIZI) == LOW) {
      checkAnswer(KIRMIZI);
    } else if (digitalRead(SIYAH) == LOW) {
      checkAnswer(SIYAH);
    } else if (digitalRead(BEYAZ) == LOW) {
      checkAnswer(BEYAZ);
    } else if (digitalRead(SARI) == LOW) {
      checkAnswer(SARI);
    } else if (digitalRead(MAVI) == LOW) {
      checkAnswer(MAVI);
    } else if (digitalRead(YESIL) == LOW) {
      checkAnswer(YESIL);
    }

    delay(delay_ms);
  }

  int endIndex = strlen(str) - MAX_CHARS_PER_LINE;
  strncpy(startBuffer, str + endIndex, MAX_CHARS_PER_LINE);
  startBuffer[MAX_CHARS_PER_LINE] = '\0';
  print_display(startBuffer, 0);
  delay(1000);
}

void setup() {
  Serial.begin(9600);

  Wire.begin(SDA_PIN, SCL_PIN);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 baslatilamadi. Baglantiyi kontrol edin."));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

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

  randomSeed(analogRead(0));
  int renk_indeksi = random(0, sizeof(bilmece_kategorileri) / sizeof(bilmece_kategorileri[0]));
  int bilmece_indeksi = random(0, bilmece_sayilari[renk_indeksi]);
  secilenBilmece = bilmece_kategorileri[renk_indeksi][bilmece_indeksi];

  secilenBilmeceMetin = String(secilenBilmece.soru);
  print_display(secilenBilmeceMetin.c_str(), 0);
}

void loop() {
  unsigned long currentMillis = millis();

  if (digitalRead(KIRMIZI) == LOW) {
    checkAnswer(KIRMIZI);
  } else if (digitalRead(SIYAH) == LOW) {
    checkAnswer(SIYAH);
  } else if (digitalRead(BEYAZ) == LOW) {
    checkAnswer(BEYAZ);
  } else if (digitalRead(SARI) == LOW) {
    checkAnswer(SARI);
  } else if (digitalRead(MAVI) == LOW) {
    checkAnswer(MAVI);
  } else if (digitalRead(YESIL) == LOW) {
    checkAnswer(YESIL);
  }

  if (currentMillis - lastScrollTime >= scrollInterval) {
    scroll_text(secilenBilmeceMetin.c_str(), 16, 10);
    lastScrollTime = currentMillis;
  }
}
