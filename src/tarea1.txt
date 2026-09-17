#include <Arduino.h>

const int potPin = A0;  // Pin analógico para leer el potenciómetro

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  int valorADC = analogRead(potPin); // Leer el valor del ADC

  
  
  char mensaje[50]; // Buffer para guardar el texto del monitor
  
  sprintf(mensaje, "Valor del ADC leido: %d", valorADC); // Mensaje que mostrará el monitor

  
  Serial.println(mensaje); //Mostrar mensaje

 
  delay(1000); // 1 segundo de espera 
}