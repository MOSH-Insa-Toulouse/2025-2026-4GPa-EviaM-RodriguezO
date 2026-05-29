#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Servo.h>
#include <Wire.h>
#include <SoftwareSerial.h>


/* PINOUT */
#define PIN_OPAMP A0
#define PIN_FLEX  A1
#define ENC_CLK   2
#define ENC_SW    3
#define ENC_DT    4
#define BT_STATE  6  
#define PIN_SERVO 9
#define PIN_CS    10 

#define MCP_WRITE 0b00010001
#define GRAPH_X0 20   // 
#define GRAPH_Y0 0    // Variables nécessaires pour définir les dimensions 
#define GRAPH_W 108   // des graphiques en fonction de la taille de l'écran OLED
#define GRAPH_H 54    
/* ------------------ OBJETS-----------*/
SoftwareSerial mySerial(7,8);
Servo servoMotor;
Adafruit_SSD1306 display(128, 64, &Wire, -1);
String inputBuffer = "";
String serialBuffer = "";

volatile long encoderPos = 0;
volatile int lastStateCLK;
int menuIndex = 0;
int graphX = 0;
int lastY = 32;
int servoPos = 0;
int potValue = 128;  


enum Mode {
  MENU,
  ANALOG_A0,
  ANALOG_A1,
  SERVO_MODE,
  POT_MODE,
  BT_MODE,
  //MORSE_MODE
};

Mode currentMode = MENU;

/* Servo & Pot */
bool btWasEverConnected = false;
bool receivingCommand = false;
bool lastButtonStateMorse = false;
bool resetTriggered = false;
bool btConnected = false;
bool lastBtState = false;
bool buttonState = false;
bool lastButtonState = false;
bool click = false;

unsigned long pressStartTime = 0;
long lastEnc = 0;/* Encoder */

/*-------------Function Potentiomètre------------------*/
void writePot(byte value) {
  digitalWrite(PIN_CS, LOW);
  SPI.transfer(MCP_WRITE);
  SPI.transfer(value);
  digitalWrite(PIN_CS, HIGH);
}
/*AAAAAAAA*/
void handleSerial() {  //Function pour détecter des commands vers l'arduino

  while (Serial.available() || mySerial.available()) {

    char c;

    if (Serial.available()) c = Serial.read();
    else c = mySerial.read();

    /*-------- SERVO COMMANDS FUNCTION --------*/
    if (c == 'S') {

      // Esperar siguiente carácter
      while (!Serial.available() && !mySerial.available());
      if (Serial.available()) c = Serial.read();
      else c = mySerial.read();
      if (c == '1') {
        servoPos -= 5;
      }
      else if (c == '2') {
        servoPos += 5;
      }

      servoPos = constrain(servoPos, 0, 180);
      servoMotor.write(servoPos);
      Serial.print("Servo -> ");
      Serial.println(servoPos);
      continue;
    }

    // -------- MENU MODE --------
    if (c == 'M') {
      inputBuffer = "M";
      continue;
    }

    if (inputBuffer.length() == 1) {
      if (c >= '1' && c <= '5') {

        switch (c) {
          case '1': currentMode = ANALOG_A0; break;
          case '2': currentMode = ANALOG_A1; break;
          case '3': currentMode = SERVO_MODE; break;
          case '4': currentMode = POT_MODE; break;
          case '5': currentMode = BT_MODE; break;
        }

        graphX = 0;
        encoderPos = 0;
        lastEnc = 0;

        Serial.print("Changé à M");
        Serial.println(c);
      }

      inputBuffer = "";
    }
  }
}
/* ---------------------Function ENCODEUR--------------*/
void doEncoder() {      
  int currentStateCLK = digitalRead(ENC_CLK);
  if (currentStateCLK != lastStateCLK) {
    if (digitalRead(ENC_DT) != currentStateCLK) encoderPos++;
    else encoderPos--;
  }
  lastStateCLK = currentStateCLK;
}
void setup() {
  mySerial.begin(9600);
  Serial.begin(9600);
  pinMode(7,INPUT);
  pinMode(8,OUTPUT);

  pinMode(ENC_CLK, INPUT_PULLUP);
  pinMode(ENC_DT, INPUT_PULLUP);
  pinMode(ENC_SW, INPUT_PULLUP);


  lastStateCLK = digitalRead(ENC_CLK);
  attachInterrupt(digitalPinToInterrupt(ENC_CLK), doEncoder, CHANGE);

  servoMotor.attach(PIN_SERVO);
  pinMode(BT_STATE, INPUT);
  pinMode(PIN_CS, OUTPUT);
  digitalWrite(PIN_CS, HIGH);
  SPI.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

/* ===================== LOOP ===================== */
void loop() {
bool currentBtState = digitalRead(BT_STATE); //Function pour détecter si le module BT et connecté à le portable
if (currentBtState && !lastBtState) {
  if (!btConnected) {
    btConnected = true;
  if (!btWasEverConnected) {
    btWasEverConnected = true;
    }
  }
}
// Détecter connexion
if (!currentBtState && lastBtState) {
  btConnected = false;
}

lastBtState = currentBtState;
handleSerial(); //FIN DE DÉTECTION BT

  long currentEnc;
  noInterrupts();
  currentEnc = encoderPos;
  interrupts();
  int delta = currentEnc - lastEnc;
  lastEnc = currentEnc;
  buttonState = digitalRead(ENC_SW) == LOW;
  unsigned long now = millis();
  if (buttonState && !lastButtonState) {
    pressStartTime = now;
    resetTriggered = false;
  }
  if (!buttonState && lastButtonState) {
    if (!resetTriggered) click = true;
  }
  switch (currentMode) {

    case MENU: {
      menuIndex += delta;
      menuIndex = constrain(menuIndex, 0, 4);

      if (click) {
        click = false;
        switch (menuIndex) {
          case 0: currentMode = ANALOG_A0; break;
          case 1: currentMode = ANALOG_A1; break;
          case 2: currentMode = SERVO_MODE; break;
          case 3: currentMode = POT_MODE; break;
          case 4: currentMode = BT_MODE; break;
        }
      }

      display.clearDisplay();
      display.drawRect(0,0,128,64,SSD1306_WHITE);
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      if (btConnected) {
   display.setCursor(60, 5); // esquina superior derecha aprox
    display.print("BT Connect!");
}
else if(!btConnected) {
  display.setCursor(60, 5);
  display.print("No BT !");
}
      const char* items[5] = {
        "A0",
        "A1",
        "Servo",
        "Pot",
        "Bluetooth",
      };

      for (int i = 0; i < 5; i++) {
        display.setCursor(10, 10 + i * 10);
        if (i == menuIndex) display.print(">");
        display.print(items[i]);
      }

      display.display();
      break;
    }

  case ANALOG_A0: {
int raw = analogRead(A0);
  int y = map(raw, 880, 905, GRAPH_Y0 + GRAPH_H, GRAPH_Y0);

  // Démarragesu graph 
  if (graphX == 0) {
    display.clearDisplay();
    // AXE X
    display.drawLine(GRAPH_X0, GRAPH_Y0 + GRAPH_H, GRAPH_X0 + GRAPH_W, GRAPH_Y0 + GRAPH_H, SSD1306_WHITE);
    // AXE Y
    display.drawLine(GRAPH_X0, GRAPH_Y0, GRAPH_X0, GRAPH_Y0 + GRAPH_H, SSD1306_WHITE);
    // TICKS Y
    for (int i = 0; i <= GRAPH_H; i += 10) {
      display.drawPixel(GRAPH_X0, GRAPH_Y0 + i, SSD1306_WHITE);
    }
    // TICKS X
    for (int i = 0; i <= GRAPH_W; i += 10) {
      display.drawPixel(GRAPH_X0 + i, GRAPH_Y0 + GRAPH_H, SSD1306_WHITE);
    }
    // ===== Type du text =====
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    // Caption
    display.setCursor(60, 0);
    display.print("A0");

    // "tiempo" abajo
    display.setCursor(50, 58);
    display.print("tiempo");
    //
    display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("5V");

  display.setCursor(0, 24);
    display.print("2.5V");

    display.setCursor(0, 52);
    display.print("0V");


  }
  // Dibujar señal
  if (graphX > 0) {
    display.drawLine(
      GRAPH_X0 + graphX - 1, lastY,
      GRAPH_X0 + graphX, y,
      SSD1306_WHITE
    );
  }
  lastY = y;
  display.display();
  graphX++;
  if (graphX >= GRAPH_W) {
    graphX = 0;
  }
  if (click) {
    click = false;
    graphX = 0;
    display.clearDisplay();
    currentMode = MENU;
  }
  break;
}
    case ANALOG_A1: {
      const float VCC= 5; 
      const float R_DIV= 10000; 
      const float flatresistance = 25000.0; 
      const float bendresistance = 100000.0;

int boxWidth = 50;
int boxHeight = 16;
int boxX = 128 - boxWidth;  // Boite de text
int boxY = 25;
int raw2 = analogRead(A1);
// Conversion lecture -> Tension
float Vflex = raw2*VCC/1023.0;
float Rflex = R_DIV *(VCC/Vflex-1.0);
float angle = -100*Vflex+391;
  int y = map(raw2, 580, 1003, GRAPH_Y0 + GRAPH_H, GRAPH_Y0);
  // Démarrage du graph
  if (graphX == 0) {
    display.clearDisplay();

    // EJE X
    display.drawLine(GRAPH_X0, GRAPH_Y0 + GRAPH_H, GRAPH_X0 + GRAPH_W, GRAPH_Y0 + GRAPH_H, SSD1306_WHITE);
    // EJE Y
    display.drawLine(GRAPH_X0, GRAPH_Y0, GRAPH_X0, GRAPH_Y0 + GRAPH_H, SSD1306_WHITE);
    // TICKS Y
    for (int i = 0; i <= GRAPH_H; i += 10) {
      display.drawPixel(GRAPH_X0, GRAPH_Y0 + i, SSD1306_WHITE);
    }

    // TICKS X
    for (int i = 0; i <= GRAPH_W; i += 10) {
      display.drawPixel(GRAPH_X0 + i, GRAPH_Y0 + GRAPH_H, SSD1306_WHITE);
    }
    display.drawRect(boxX, boxY, boxWidth, boxHeight, SSD1306_WHITE);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

// "Valeur Actuel du angle"
    display.setCursor(boxX + 3, boxY + 2);
    display.print("A:");
    display.print((int)angle);
        // ===== Propietés du text =====
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);

    // Caption du graph
    display.setCursor(60, 0);
    display.print("A1");

    // "temps"
    display.setCursor(50, 58);
    display.print("tiempo");
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("-90°");
    display.setCursor(0, 24);
    display.print("0°");
    display.setCursor(0, 52);
    display.print("90°");
  }
  // Dibujar señal
  if (graphX > 0) {
    display.drawLine(
      GRAPH_X0 + graphX - 1, lastY,
      GRAPH_X0 + graphX, y,
      SSD1306_WHITE
    );
  }
  lastY = y;
  display.display();
  graphX++;

  if (graphX >= GRAPH_W) {
    graphX = 0;
  }
  if (click) {
    click = false;
    graphX = 0;
    display.clearDisplay();
    currentMode = MENU;
  }
  break;
}
case SERVO_MODE: {

      servoPos += delta * 2;
      servoPos = constrain(servoPos, 0, 180);
      servoMotor.write(servoPos);

      display.clearDisplay();
      display.setCursor(15,30);
      display.print("Servo: ");
      display.print(servoPos);
      display.display();
      break;
    }
    case POT_MODE: {
      potValue += delta;
      potValue = constrain(potValue, 0, 255);
      writePot(potValue);

      display.clearDisplay();
      display.drawRect(0,0,128,64,SSD1306_WHITE);
      display.setCursor(15,30);
      display.print("Pot: ");
      display.println(potValue);
      display.display();
      break;
    }

case BT_MODE: {
  display.clearDisplay();
  display.drawRect(0,0,128,64,SSD1306_WHITE);
  display.setCursor(20,30);
  display.print("Bluetooth");
  int raw = analogRead(A0);
  int raw2 = analogRead(A1); 
    float voltage = raw * (5.0 / 1023.0);
     mySerial.print(raw);
    mySerial.print("=");
     mySerial.println(raw2);
  delay(200); // evita saturar el BT
  display.display();
  break;
}

  }
  if (buttonState && (now - pressStartTime > 1500) && !resetTriggered) {
  currentMode = MENU;
  resetTriggered = true;

  graphX = 0;
  encoderPos = 0;
  lastEnc = 0;
}
  lastButtonState = buttonState;
 
	  int i = 0; 
	char someChar[32] ={0};
	//when characters arrive over the serial port...

/*	while (Serial.available()) {
	   do{
		someChar[i++] = Serial.read();
		delay(3);		
	   }while (Serial.available() > 0);
	   
	   mySerial.println(someChar); 
	   Serial.println(someChar); 
	}
	while (mySerial.available()) {
		Serial.print((char)mySerial.read());
	}*/

  while (mySerial.available()) {
  Serial.print((char)mySerial.read());
}
}
