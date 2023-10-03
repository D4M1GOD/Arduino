//Ejercicio 3. Realizar un código que permita apagar un zumbador a través de un pulsador el cual será manejado a través de una interrupción. El zumbador se activará cuando la alarma de acceso ingresada a través de un teclado sea errónea.
volatile int CONTADOR = 0;
int ANTERIOR = 0;
int pinBuzzer = 12;
int pinButton = 3;
int pinPassword = 2;  // Pin para la entrada de contraseña
unsigned long lastChangeTime = 0;
bool buttonPressed = false;
bool buzzerState = true;  // Estado inicial del buzzer (encendido)
bool passwordVerified = false;

const unsigned long delayTime = 200;

// Define tu contraseña aquí
const int correctPassword = 1234;  // Cambia esto a tu contraseña

void checkPassword() {
  Serial.println("Ingrese la contraseña:");
  while (!passwordVerified) {
    if (Serial.available() > 0) {
      int enteredPassword = Serial.parseInt();
      Serial.println();
      if (enteredPassword != correctPassword) { // Modificado para cambiar la lógica
        Serial.println("Contraseña incorrecta. Ingreso fallido.");
        passwordVerified = true;
      } else {
        Serial.println("Contraseña correcta. Ingreso exitoso.");
        passwordVerified = true;
      }
    }
  }
}

void setup() {
  pinMode(pinBuzzer, OUTPUT);
  pinMode(pinButton, INPUT_PULLUP);
  pinMode(pinPassword, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinPassword), sensor, RISING);  // Configurar la interrupción
  Serial.begin(9600);
}

void loop() {
  if (!passwordVerified) {
    checkPassword();  // Verificar la contraseña si no está verificada
  }

  unsigned long currentTime = millis();

  if (digitalRead(pinButton) == LOW && !buttonPressed) {
    buttonPressed = true;
    buzzerState = !buzzerState;  // Cambia el estado del buzzer
    digitalWrite(pinBuzzer, buzzerState);
    delay(100);
  }
  
  if (digitalRead(pinButton) == HIGH && buttonPressed) {
    buttonPressed = false;
  }

  if (ANTERIOR != CONTADOR) {
    Serial.println(CONTADOR);
    if (buzzerState) {
      digitalWrite(pinBuzzer, LOW);
    }
    lastChangeTime = currentTime;
    ANTERIOR = CONTADOR;
  } else if (buzzerState && currentTime - lastChangeTime >= delayTime) {
    digitalWrite(pinBuzzer, HIGH);
  }
}

void sensor() {
  if (passwordVerified) {
    CONTADOR++;
  }
}
