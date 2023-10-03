#include <Key.h>
#include <Keypad.h>
#include "LedControl.h"  // Librería LedControl
LedControl lc = LedControl(3, 12, 2, 1); // Los números se refieren a los pines de ARDUINO conectados a la matriz LED

byte Cara_datos[] = {
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10100101,
  B10011001,
  B01000010,
  B00111100
};
byte Cara_triste[] = {
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10011001,
  B10100101,
  B01000010,
  B00111100
};

const byte FILAS = 4;  // número de filas
const byte COLUMNAS = 4; // número de columnas
char keys[FILAS][COLUMNAS] = {  // define la distribución de teclas
  { '1', '2', 'A', '3' },
  { '4', '5', 'B', '6' },
  { '7', '8', 'C', '9' },
  { '*', '0', 'D', '#' }
};
byte pinesFilas[FILAS] = { 7, 6, 5, 4 }; // PINES asignados a las filas
byte pinesColumnas[COLUMNAS] = { 11, 10, 9, 8 }; // PINES asignados a las columnas
Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS); // sentencia que permite crear un objeto keypad
char TECLA; // variable que permite almacenar la tecla presionada
char CLAVE[5]; // vector que permite almacenar en un array los 4 dígitos ingresados por teclado
char CLAVE_SECRETA[5] = "1234"; // vector que guarda la clave secreta para la comparación
byte INDICE = 0;



void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 8); // Los valores están entre 1 y 15
  lc.clearDisplay(0);
  Serial.begin(9600);
  Serial.println("Ingrese la clave:");
}

void loop() {
  TECLA = teclado.getKey();
  if (TECLA) {
    CLAVE[INDICE] = TECLA;
    INDICE++;
    Serial.print(TECLA);
  }
  if (INDICE == 4) {
    Serial.println(); // Agrega una nueva línea para separar la clave ingresada
    if (!strcmp(CLAVE, CLAVE_SECRETA)) { // compara la clave ingresada con la clave maestra
      Serial.println("La clave ingresada es correcta");
      Representar(Cara_datos, 3000); // imprime en el monitor serial que es correcta la clave
      lc.clearDisplay(0);
      delay(2000);
    } else {
      Serial.println("La clave ingresada es incorrecta"); // imprime en el monitor serial que es incorrecta la clave
      Representar(Cara_triste, 3000); // imprime en el monitor serial que es correcta la clave
      lc.clearDisplay(0);
    }
    INDICE = 0;
    Serial.println("Ingrese la clave:");
  }
}

void Representar(byte *Datos, int retardo) {
  for (int i = 0; i < 8; i++) {
    lc.setColumn(0, i, Datos[i]);
  }
  delay(retardo);
}
