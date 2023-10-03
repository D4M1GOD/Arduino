volatile int velocidad = 20; //volatile se usa cuando la variable cambia
const int maxima = 1000;
const int minima = 20;
const int aumenta = 20;
int leds[4] = {8, 9, 10, 11};
void setup() {
 for (int i = 0; i < 4; i++)
 {
 pinMode(leds[i], OUTPUT);
 digitalWrite(leds[i], LOW);
 }
 velocidad = minima;
 attachInterrupt(digitalPinToInterrupt(2), velocidadMenos, RISING);
 attachInterrupt(digitalPinToInterrupt(3), velocidadMas, RISING);
}
void loop() {
 for (int i = 0; i < 4; i++)
 {
 if (i > 0) //Esto se realiza con el PIN 9, 10 y 11
 {
 digitalWrite(leds[i - 1], LOW); // Apagamos el LED a la izquierda
 }
 digitalWrite(leds[i], HIGH); // Encender el LED en el que estamos
 delay(velocidad); // Esperar el tiempo marcado por velocidad
 }
 digitalWrite(leds[3], LOW);
 for (int i = 3; i >= 0; i--)
 {
 if (i < 3)
 {
 digitalWrite(leds[i + 1], LOW); // Apagar el LED a la derecha
 }
 digitalWrite(leds[i], HIGH); // Encender en el LED en el que estamos
 delay(velocidad); // Esperar el tiempo marcado por velocidad
 }
 digitalWrite(leds[0], LOW);
}
void velocidadMenos()
{
 velocidad = velocidad - aumenta; // Disminuir la velocidad
 if (velocidad < minima) // Si se llega a la velocidad mínima no se disminuye más
 {
 velocidad = minima;
 }
}
// Función ISR pin 3, aumenta la velocidad
void velocidadMas()
{
 velocidad = velocidad + aumenta; // Aumentar la velocidad
 if (velocidad > maxima) // Si se llega a la velocidad máxima no se aumenta más
 {
 velocidad = maxima;
 }
}