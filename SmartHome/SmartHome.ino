#include <Servo.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Key.h>
#include <Keypad.h>
Servo myServo;

// Creación del objeto LCD en la dirección 0x27
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);
int LEDRED = 13;
int LED = 3;
int BRILLO;
int POTENCIOMETRO = 0;
int zumbadorPin = 12;
int SensorPin = A0;
int cont=0;

const byte FILAS = 4;           // número de filas
const byte COLUMNAS = 4;        // número de columnas
char keys[FILAS][COLUMNAS] = {  // define la distribucion de teclas
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};
byte pinesFilas[FILAS] = { 7, 6, 5, 4 };                                                // PINES asignados a las filas
byte pinesColumnas[COLUMNAS] = { 11, 10, 9, 8 };                                        // PINES asignados a las columnas
Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS);  // sentencia que permite crear un objeto keypad
char TECLA;                                                                             // variable que permite almacenae la tecla presionada
char CLAVE[5];                                                                          // vector que permite almacenae en un array los 4 digitos ingresados por teclado
char CLAVE_SECRETA[5] = "1234";                                                         // vector que guarda la clave secreta para la comparación
byte INDICE = 0;                                                                        // indice del arreglo
byte INTENTOS = 0;                                                                      // contador de intentos

void setup() {
  Serial.begin(9600);                // inicializa comunicacion serial
  lcd.setBacklightPin(3, POSITIVE);  // se habilita que el PIN 3 del LCD
  lcd.setBacklight(HIGH);            // encender la iluminacion posterior del LCD
  lcd.begin(20, 2);                  // Inicialización del módulo 2 líneas y 16 colum
  lcd.clear();                       // limpiar pantalla
  lcd.setCursor(0, 0);               // ubicar el cursor en la línea 0 y columna 0
  lcd.print("INGRESAR PASSWORD");    // impresión del mensaje texto
  pinMode(LEDRED, OUTPUT);
  pinMode(LED, OUTPUT);
  myServo.attach(2);
}

void loop() {

  TECLA = teclado.getKey();  // obtiene la tecla presionada y asigna a variable
  if (TECLA)                 // comprueba que se haya presionado una tecla
  {
    CLAVE[INDICE] = TECLA;     // almacena en array la tecla presionada
    lcd.setCursor(INDICE, 1);  // ubicar el cursor columna 0 y la línea 1.
    lcd.print(TECLA);          // escribe texto
    lcd.display();             // función para mostrar el texto
    INDICE++;                  // incrementa indice en uno
    Serial.print(TECLA);       // impresión en monitor serial la tecla presionada
  }

  if (INDICE == 4)  // Condición para comprobar si ya se almacenaron los 4 digitos
  {
    if (!strcmp(CLAVE, CLAVE_SECRETA)) {  // compara la clave ingresada con la clave maestra
      //Serial.println(" La clave ingresada es correcta"); // imprime en monitor serial que es correcta la clave
      lcd.setCursor(0, 1);    // ubicar el cursor columna 0 y la línea 1.
      lcd.print("CORRECTO");  // escribe texto
      lcd.display();          // función para mostrar el texto
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("INGRESASTE!!");  // escribe texto

      INDICE = 0;
      lcd.clear();  // limpiar pantalla
      do {
        myServo.write(0);
        // wait for the servo to get there
        delay(2000);
        if (cont == 0) {
          myServo.write(90);
          delay(2000);
          myServo.write(60);
          delay(2000);
          myServo.write(30);
          delay(2000);
          myServo.write(0);
          delay(2000);
          cont = 2;
        }
        int v = analogRead(A3);
         Serial.println(v);
        if (v < 30) {
          BRILLO = analogRead(POTENCIOMETRO) / 4;  // valor leído de entrada
          analogWrite(LED, BRILLO);                // El brillo del LED es controlado a través
        } else digitalWrite(LED, LOW);
        int humedad = map(analogRead(SensorPin), 0, 1023, 100, 0);
        lcd.setCursor(0, 0);   // ubicar el cursor en la línea 0 y columna 0
        lcd.print("HUMEDAD");  // impresión del mensaje texto
        //Serial.println(humedad);
        lcd.setCursor(8, 0);  // ubicar el cursor columna 0 y la línea 1.}
        lcd.print(humedad);
        if (humedad < 9) {
          lcd.setCursor(9, 0);
          lcd.print(" ");
        }
        lcd.setCursor(10, 0);
        lcd.print("%");
        delay(100);


        if (humedad < 60 && humedad > 40) {
          lcd.setCursor(0, 1);
          lcd.print("NORMAL ");
        } else if (humedad < 40) {
          lcd.setCursor(0, 1);
          lcd.print("BAJA  ");
        } else {
          lcd.setCursor(0, 1);
          lcd.print("ALTA  ");
        }

      } while (INDICE == 0);

    } else {
      //Serial.println(" La clave ingresada es incorrecta"); // imprime en monitor serial que es incorrecta la clave
      lcd.setCursor(0, 1);      // ubicar el cursor columna 0 y la línea 1.
      lcd.print("INCORRECTO");  // escribe texto
      lcd.display();            // función para mostrar el texto
      delay(1000);
      lcd.noDisplay();
      INDICE = 0;
      INTENTOS++;
      if (INTENTOS < 4) {
        int CONT = 3 - INTENTOS;
        lcd.clear();
        lcd.setCursor(0, 0);      // ubicar el cursor columna 0 y la línea 1.
        lcd.print("TE QUEDAN ");  // impresión del mensaje texto
        lcd.print(CONT);          // impresión del mensaje texto
        lcd.print(" DE 3");       // impresión del mensaje texto
        lcd.setCursor(0, 1);      // ubicar el cursor columna 0 y la línea 1.
        lcd.print("INTENTOS");    // impresión del mensaje texto
        lcd.display();
        delay(1500);
        lcd.clear();
        if (CONT > 0) {
          lcd.setCursor(0, 0);             // ubicar el cursor columna 0 y la línea 1.
          lcd.print("INGRESAR PASSWORD");  // impresión del mensaje texto
          lcd.display();                   // función para mostrar el texto
        }
      }
      if (INTENTOS == 3) {
        do {
          lcd.setCursor(0, 0);   // ubicar el cursor columna 0 y la línea 1.
          lcd.print("ALARMA!");  // impresión del mensaje texto
          lcd.display();         // función para mostrar el texto
          digitalWrite(LEDRED, HIGH);
          tone(zumbadorPin, 1000);
          delay(300);  // Mantén el tono durante 1 segundo
          digitalWrite(LEDRED, LOW);
          noTone(zumbadorPin);  // Detén el tono
          delay(300);           // Espera 1 segundo antes de repetir
          INDICE = 0;
        } while (INDICE == 0);
      }
    }
  }
}
