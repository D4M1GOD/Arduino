#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <Key.h>
#include <Keypad.h>
#include "LedControl.h"

// Configura los pines para la librería LedControl
LedControl lc = LedControl(D8, D7, D6, 1);

DHT dht(D1, DHT11);
const char* ssid = "CELERITY SUAREZ"; // Cambia esto a tu SSID de Wi-Fi
const char* password = "1708Suarez2022"; // Cambia esto a tu contraseña de Wi-Fi
const int irPin = D5; // Pin del sensor infrarrojo

ESP8266WebServer server(80);

const int redPin = D2;
const int greenPin = D3;
const int bluePin = D4;

float temp, hume, temp_f, h1;
int updateCount = 0; // Variable para llevar el registro de las actualizaciones
volatile bool irSensorActivated = false; // Bandera para indicar la activación del sensor IR

byte sunPattern[8] = {
  B10001001,
  B01010010,
  B00111100,
  B10111110,
  B01111101,
  B00111100,
  B01001010,
  B10010001
};

byte snowflakePattern[8] = {
  B00101000,
  B00010100,
  B00101000,
  B01000010,
  B10000001,
  B01000010,
  B00101000,
  B00010100
};

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

void updateMatrix() {
  byte* pattern = (temp >= 23.0) ? sunPattern : snowflakePattern;
  for (int row = 0; row < 8; row++) {
    lc.setColumn(0, row, pattern[7 - row]);
  }
}

void handleRoot() {
  String html = "<html><body>";
  html += "Temperatura: " + String(temp) + "°C<br>";
  html += "Humedad: " + String(hume) + "%<br>";
  html += "Temperatura: " + String(temp_f) + "°F<br>";
  html += "Sensacion Térmica: " + String(h1) + "<br>";
  html += "Actualizaciones: " + String(updateCount) + "<br>"; // Muestra el número de actualizaciones
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void ICACHE_RAM_ATTR handleIRInterrupt() {
  // Marca la bandera de activación del sensor IR
  irSensorActivated = true;
}

void setup() {
  Serial.begin(9600);
  
  // Configura los pines del LED RGB como salidas
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  
  // Configura el pin del sensor infrarrojo como entrada
  pinMode(irPin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(irPin), handleIRInterrupt, RISING); // Configura la interrupción del IR en el espacio IRAM

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
    setColor(255, 165, 0); // Color naranja (R, G, B)
  }

  Serial.print("Connected to WiFi. Access the web page at the following IP address: ");
  Serial.println(WiFi.localIP());

  
  Serial.println("Connected to WiFi");
  server.on("/", HTTP_GET, handleRoot);
  server.begin();
  dht.begin();
 
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
}

void loop() {
  // Verifica si se activó el sensor IR
  if (irSensorActivated) {
    // Lee los datos del sensor de temperatura DHT11
    hume = dht.readHumidity();
    temp = dht.readTemperature();
    temp_f = dht.readTemperature(true);
    h1 = dht.computeHeatIndex(temp, hume);

    // Incrementa el contador de actualizaciones
    updateCount++;

    // Resetea la bandera del sensor IR
    irSensorActivated = false;

    // Actualiza la matriz 8x8
    updateMatrix();
  }

  // Verifica el estado de la conexión Wi-Fi y ajusta el color del LED RGB
  if (WiFi.status() == WL_CONNECTED) {
    setColor(0, 255, 0); // Color verde (R, G, B)
  } else {
    setColor(255, 165, 0); // Color naranja (R, G, B)
  }

  // Maneja las solicitudes del servidor web
  server.handleClient();
}
