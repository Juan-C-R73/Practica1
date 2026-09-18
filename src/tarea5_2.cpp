#include <Arduino.h>
#include <Wire.h>

/* Declaramos la función ANTES de usarla para que C++ no dé error */
int readSerial(char result[]) {
  int i = 0;
  while (1) {
    while (Serial.available() > 0) {
      char inChar = Serial.read();
      if (inChar == '\n') {
        result[i] = '\0'; // Terminador de string
        Serial.flush();
        return 0;
      }
      if (inChar != '\r') {
        result[i] = inChar;
        i++;
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  while (!Serial); // Espera a que el puerto serie esté listo
  Wire.begin();    // Únete al bus I2C como Maestro
}

void loop() {
  Serial.print("Escribe 0 para apagar el LED, o 1 para encenderlo: ");
  
  // CORRECCIÓN: Le damos un tamaño de 10 caracteres para que no desborde la memoria
  char ledVal[10]; 
  
  readSerial(ledVal);
  Serial.println(ledVal);
  
  Wire.beginTransmission(8); // Inicia transmisión al Esclavo con dirección #8
  Wire.write(ledVal);        // Envía el valor
  Wire.endTransmission();    // Finaliza la transmisión
  
  delay(500);
}