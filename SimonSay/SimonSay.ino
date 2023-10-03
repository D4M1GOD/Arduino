// LEDs
const int RGBLED1 = 13;
const int YLED = 12;
const int BLED = 11;

// BUTTONs
const int RBUT = 7;
const int YBUT = 6;
const int BBUT = 5;
const int TWORBUT = 4;
const int TWOYBUT = 3;
const int TWOBBUT = 2;

// SPEAKER
const int SPK = 8;

// STATUS
int STATUSRBUT = LOW;
int STATUSYBUT = LOW;
int STATUSBBUT = LOW;
int TWOSTATUSRBUT = LOW;
int TWOSTATUSYBUT = LOW;
int TWOSTATUSBBUT = LOW;

// TONES
const int RTone = 120;
const int YTone = 220;
const int BTone = 320;

// ARRAY DE 6 VALORES
int sequenceIndex = 0;
int twosequenceIndex = 0;
const int sequenceLength = 6;
const int twosequenceLength = 6;
int sequence[sequenceLength]; // This array will store the button press order
int twosequence[twosequenceLength]; // This array will store the button press order
int contIndex = 0;
int twocontIndex = 0;
// COLORES
int RED = 1;
int YELLOW = 2;
int BLUE = 3;
int ARRAYCOLOR[6] = {0, 0, 0, 0, 0, 0};
int TWOARRAYCOLOR[6] = {0, 0, 0, 0, 0, 0};

void setup() {
  // LED AND BUTTON PLAYER ONE
  pinMode(RBUT, INPUT);
  pinMode(RGBLED1, OUTPUT);
  pinMode(YBUT, INPUT);
  pinMode(YLED, OUTPUT);
  pinMode(BBUT, INPUT);
  pinMode(BLED, OUTPUT);
  // LED AND BUTTON PLAYER TWO
  pinMode(TWORBUT, INPUT);
  pinMode(TWOYBUT, INPUT);
  pinMode(TWOBBUT, INPUT);
  // SPEAKER
  pinMode(SPK, OUTPUT);
  // SERIAL
  Serial.begin(9600);
}

void loop() {
  Serial.println("PLAY GAME");
  playgame();
  delay(2000); // Espera 2 segundos antes de reiniciar el juego
  Serial.println("RESET GAME");
  resetGame(); // Reinicia el juego para comenzar de nuevo
}

void resetGame() {
  contIndex = 0;
  twocontIndex = 0;
  sequenceIndex = 0;
  twosequenceIndex = 0;
  for (int i = 0; i < sequenceLength; i++) {
    sequence[i] = 0;
    twosequence[i] = 0;
    ARRAYCOLOR[i] = 0;
    TWOARRAYCOLOR[i] = 0;
  }
}


void playgame() {
  while (contIndex < 5) {
    checkButton(STATUSRBUT, RBUT, RGBLED1, RTone, "BUTTON RED", RED);
    checkButton(STATUSYBUT, YBUT, YLED, YTone, "BUTTON YELLOW", YELLOW);
    checkButton(STATUSBBUT, BBUT, BLED, BTone, "BUTTON BLUE", BLUE);

    // Mostrar la secuencia en la consola serial
    // Serial.println("Secuencia total:");
    for (int i = 0; i < sequenceLength; i++) {
      if (sequence[i] == RED) {
        ARRAYCOLOR[i] = RED;
        contIndex = i;
      } else if (sequence[i] == YELLOW) {
        ARRAYCOLOR[i] = YELLOW;
        contIndex = i;
      } else if (sequence[i] == BLUE) {
        ARRAYCOLOR[i] = BLUE;
        contIndex = i;
      }
    }
  }

  while (twocontIndex < 5) {
    twocheckButton(TWOSTATUSRBUT, TWORBUT, RGBLED1, RTone, "TWOBUTTON RED", RED);
    twocheckButton(TWOSTATUSYBUT, TWOYBUT, YLED, YTone, "TWOBUTTON YELLOW", YELLOW);
    twocheckButton(TWOSTATUSBBUT, TWOBBUT, BLED, BTone, "TWOBUTTON BLUE", BLUE);

    for (int i = 0; i < sequenceLength; i++) {
      if (twosequence[i] == RED) {
        TWOARRAYCOLOR[i] = RED;
        twocontIndex = i;
      } else if (twosequence[i] == YELLOW) {
        TWOARRAYCOLOR[i] = YELLOW;
        twocontIndex = i;
      } else if (twosequence[i] == BLUE) {
        TWOARRAYCOLOR[i] = BLUE;
        twocontIndex = i;
      }
    }
  }

    // Comparar los arreglos ARRAYCOLOR y TWOARRAYCOLOR
  bool arraysAreEqual = true;
  for (int i = 0; i < sequenceLength; i++) {
    if (ARRAYCOLOR[i] != TWOARRAYCOLOR[i]) {
      arraysAreEqual = false;
      break;
    }
  }
  
   if (arraysAreEqual) {
    // Si los arreglos son iguales, tocar tono de ganador
    Serial.print("WIN");
    digitalWrite(RGBLED1, HIGH);
    digitalWrite(YLED, HIGH);
    digitalWrite(BLED, HIGH);
    tone(SPK, 500);
    delay(1000);
    noTone(SPK);
    digitalWrite(RGBLED1, LOW);
    digitalWrite(YLED, LOW);
    digitalWrite(BLED, LOW);
  } else {
    // Si los arreglos no son iguales, tocar tono de tristeza
    for (int i = 0; i < 3; i++) {
      Serial.print("LOSER");
      digitalWrite(RGBLED1, HIGH);
      digitalWrite(YLED, HIGH);
      digitalWrite(BLED, HIGH);
      tone(SPK, 100);
      delay(1000);
      noTone(SPK);
      digitalWrite(RGBLED1, LOW);
      digitalWrite(YLED, LOW);
      digitalWrite(BLED, LOW);
      delay(200);
    }
  }
  
    for (int k = 0; k < 6; k++) {
    Serial.print("ARRAYCOLOR[");
    Serial.print(k);
    Serial.print("] = ");
    Serial.print(ARRAYCOLOR[k]);
    Serial.print(" - ");
    Serial.println(TWOARRAYCOLOR[k]);
  } 
}

void checkButton(int &buttonStatus, int buttonPin, int ledPin, int toneFrequency, const char *message, int color) {
  int currentStatus = digitalRead(buttonPin);
  if (currentStatus == HIGH && buttonStatus == LOW) {
    buttonStatus = HIGH;
    pushButton(buttonPin, ledPin, toneFrequency);
    Serial.println(message);
    sequence[sequenceIndex] = color; // Store the button press in the sequence array
    sequenceIndex = (sequenceIndex + 1) % sequenceLength; // Move to the next index, looping back if needed
  }
  if (currentStatus == LOW && buttonStatus == HIGH) {
    buttonStatus = LOW;
  }
}

void twocheckButton(int &buttonStatus, int buttonPin, int ledPin, int toneFrequency, const char *message, int color) {
  int currentStatus = digitalRead(buttonPin);
  if (currentStatus == HIGH && buttonStatus == LOW) {
    buttonStatus = HIGH;
    pushButton(buttonPin, ledPin, toneFrequency);
    Serial.println(message);
    twosequence[twosequenceIndex] = color; // Store the button press in the sequence array
    twosequenceIndex = (twosequenceIndex + 1) % twosequenceLength; // Move to the next index, looping back if need
  }
  if (currentStatus == LOW && buttonStatus == HIGH) {
    buttonStatus = LOW;
  }
}

void pushButton(int button, int led, int atone) {
  digitalWrite(led, HIGH);
  tone(SPK, atone);
  delay(600);
  digitalWrite(led, LOW);
  noTone(SPK);
}
