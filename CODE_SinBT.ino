#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Servo.h>
#include <Wire.h>

/* ===================== PINES ===================== */
#define PIN_OPAMP A0
#define PIN_FLEX  A1
#define PIN_SERVO 9
#define PIN_CS    10
#define ENC_CLK   2
#define ENC_DT    4
#define ENC_SW    3

/* ===================== OBJETOS ===================== */
Servo servoMotor;
Adafruit_SSD1306 display(128, 64, &Wire, -1);

/* ===================== VARIABLES ===================== */
volatile long encoderPos = 0;
volatile int lastStateCLK;

int menuIndex = 0;

enum Mode {
  MENU,
  ANALOG,
  SERVO_MODE,
  POT_MODE,
  BT_MODE,
  MORSE_MODE
};

Mode currentMode = MENU;

/* Servo & Pot */
int servoPos = 0;
int potValue = 128;

/* Morse */
String morseBuffer = "";
String translatedText = "";
bool buttonState = false;
bool lastButtonState = false;
bool lastButtonStateMorse = false;

unsigned long pressStartTime = 0;
unsigned long lastMorseInputTime = 0;
bool resetTriggered = false;

/* Encoder control */
long lastEnc = 0;

/* ===================== MCP ===================== */
#define MCP_WRITE 0b00010001
void writePot(byte value) {
  digitalWrite(PIN_CS, LOW);
  SPI.transfer(MCP_WRITE);
  SPI.transfer(value);
  digitalWrite(PIN_CS, HIGH);
}

/* ===================== ENCODER ISR ===================== */
void doEncoder() {
  int currentStateCLK = digitalRead(ENC_CLK);
  if (currentStateCLK != lastStateCLK) {
    if (digitalRead(ENC_DT) != currentStateCLK) encoderPos++;
    else encoderPos--;
  }
  lastStateCLK = currentStateCLK;
}

/* ===================== MORSE ===================== */
char morseToChar(String code) {
 if (code == ".-") return 'A';
  if (code == "-...") return 'B';
  if (code == "-.-.") return 'C';
  if (code == "-..") return 'D';
  if (code == ".") return 'E';
  if (code == "..-.") return 'F';
  if (code == "--.") return 'G';
  if (code == "....") return 'H';
  if (code == "..") return 'I';
  if (code == ".---") return 'J';
  if (code == "-.-") return 'K';
  if (code == ".-..") return 'L';
  if (code == "--") return 'M';
  if (code == "-.") return 'N';
  if (code == "---") return 'O';
  if (code == ".--.") return 'P';
  if (code == "--.-") return 'Q';
  if (code == ".-.") return 'R';
  if (code == "...") return 'S';
  if (code == "-") return 'T';
  if (code == "..-") return 'U';
  if (code == "...-") return 'V';
  if (code == ".--") return 'W';
  if (code == "-..-") return 'X';
  if (code == "-.--") return 'Y';
  if (code == "--..") return 'Z';
  if (code == "-----") return '0';
  if (code == ".----") return '1';
  if (code == "..---") return '2';
  if (code == "...--") return '3';
  if (code == "....-") return '4';
  if (code == ".....") return '5';
  if (code == "-....") return '6';
  if (code == "--...") return '7';
  if (code == "---..") return '8';
  if (code == "----.") return '9';
  return '?';
  return '?';
}

/* ===================== SETUP ===================== */
void setup() {
  Serial.begin(9600);

  pinMode(ENC_CLK, INPUT_PULLUP);
  pinMode(ENC_DT, INPUT_PULLUP);
  pinMode(ENC_SW, INPUT_PULLUP);

  lastStateCLK = digitalRead(ENC_CLK);
  attachInterrupt(digitalPinToInterrupt(ENC_CLK), doEncoder, CHANGE);

  servoMotor.attach(PIN_SERVO);

  pinMode(PIN_CS, OUTPUT);
  digitalWrite(PIN_CS, HIGH);
  SPI.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

/* ===================== LOOP ===================== */
void loop() {

  /* ===== ENCODER ===== */
  long currentEnc;
  noInterrupts();
  currentEnc = encoderPos;
  interrupts();

  int delta = currentEnc - lastEnc;
  lastEnc = currentEnc;

  /* ===== BOTÓN GLOBAL ===== */
  buttonState = digitalRead(ENC_SW) == LOW;
  unsigned long now = millis();
  static bool click = false;

  if (buttonState && !lastButtonState) {
    pressStartTime = now;
    resetTriggered = false;
  }

  if (!buttonState && lastButtonState) {
    if (!resetTriggered) click = true;
  }

  // CLICK LARGO → salir a menú
  if (buttonState && (now - pressStartTime > 1500)) {
    currentMode = MENU;

    // limpiar morse al salir
    morseBuffer = "";
    translatedText = "";

    resetTriggered = true;
  }

  /* ===================== MENU ===================== */
  if (currentMode == MENU) {

    menuIndex += delta;
    menuIndex = constrain(menuIndex, 0, 4);

    if (click) {
      currentMode = (Mode)(menuIndex + 1);
      click = false;
    }

    display.clearDisplay();
    display.drawRect(0,0,128,64,SSD1306_WHITE);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    const char* items[5] = {
      "1.Analog",
      "2.Servo",
      "3.Pot",
      "4.Bluetooth",
      "5.Morse"
    };

    for (int i=0; i<5; i++) {
      display.setCursor(10, 10 + i*10);
      if (i == menuIndex) display.print(">");
      display.print(items[i]);
    }

    display.display();
  }

  /* ===================== ANALOG ===================== */
  else if (currentMode == ANALOG) {

    int a0 = analogRead(A0);
    int a1 = analogRead(A1);

    display.clearDisplay();
    display.drawRect(0,0,128,64,SSD1306_WHITE);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(15,20);
    display.print("A0: "); display.println(a0);
    display.setCursor(15,35);
    display.print("A1: "); display.println(a1);

    display.display();
  }

  /* ===================== SERVO ===================== */
  else if (currentMode == SERVO_MODE) {

    servoPos += delta * 2;
    servoPos = constrain(servoPos, 0, 180);
    servoMotor.write(servoPos);

    display.clearDisplay();
    display.drawRect(0,0,128,64,SSD1306_WHITE);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(15,30);
    display.print("Servo: ");
    display.print(servoPos);
    display.print(" deg");

    display.display();
  }

  /* ===================== POT ===================== */
  else if (currentMode == POT_MODE) {

    potValue += delta;
    potValue = constrain(potValue, 0, 255);
    writePot(potValue);

    display.clearDisplay();
    display.drawRect(0,0,128,64,SSD1306_WHITE);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(15,30);
    display.print("Pot: ");
    display.println(potValue);

    display.display();
  }

  /* ===================== BLUETOOTH ===================== */
  else if (currentMode == BT_MODE) {

    display.clearDisplay();
    display.drawRect(0,0,128,64,SSD1306_WHITE);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(20,30);
    display.print("Bluetooth");

    display.display();
  }

  /* ===================== MORSE ===================== */
  else if (currentMode == MORSE_MODE) {

    if (!buttonState && lastButtonStateMorse) {
      unsigned long duration = now - pressStartTime;
      if (duration < 300) morseBuffer += ".";
      else morseBuffer += "-";
      lastMorseInputTime = now;
    }

    if (morseBuffer.length() > 0 && now - lastMorseInputTime > 1000) {
      translatedText += morseToChar(morseBuffer);
      morseBuffer = "";
      lastMorseInputTime = now;
    }

    static unsigned long lastWordCheck = 0;
    if (now - lastWordCheck > 100) {
      if (translatedText.length() > 0 && morseBuffer.length() == 0 &&
          now - lastMorseInputTime > 2000 &&
          translatedText.charAt(translatedText.length()-1) != ' ') {
        translatedText += ' ';
        lastMorseInputTime = now;
      }
      lastWordCheck = now;
    }

    display.clearDisplay();
    display.drawRect(0,0,128,64,SSD1306_WHITE);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(10,30);
    display.print(translatedText);

    display.display();

    lastButtonStateMorse = buttonState;
  }

  /* ===== ACTUALIZACIÓN GLOBAL ===== */
  lastButtonState = buttonState;
}