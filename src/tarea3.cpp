#include <Arduino.h>
#include "mbed.h" // Se importa mbed 

const int potPin = A0;

// Configuramos el pin 9 usando la clase PwmOut de mbed
mbed::PwmOut salidaPWM(digitalPinToPinName(9)); 

void setup() {
  Serial.begin(9600);
  
  // Configuramos la Frecuencia a 5kHz (Periodo = 1 / 5000)
  salidaPWM.period(1.0 / 5000.0); 
}

void loop() {
  // Leemos el valor del potenciómetro (de 0 a 1023)
  int valorADC = analogRead(potPin);
  
  // Calculamos el Duty Cycle entre 0.0 y 1.0
  float proporcion = valorADC / 1023.0; 
  
  // Aplicamos la salida PWM al pin 9
  salidaPWM.write(proporcion);
  
  // Mostrar los datos para comprobar
  char mensaje[50];
  sprintf(mensaje, "ADC: %d -> PWM: %d%%", valorADC, (int)(proporcion * 100));
  Serial.println(mensaje);
  
  delay(100);
}