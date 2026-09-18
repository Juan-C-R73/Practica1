#include <Arduino.h>
#include "BBTimer.hpp" // Incluir librería creada en el foro

const int potPin = A0;


volatile bool timerFlag = false; // Flag para avisar al programa que se ha activado la interrupción


BBTimer mi_temporizador(BB_TIMER1); //Configurar temporizador 1

//Interrupción que se ejecutará cada 10 segundos
void interrupcionCallback() {
  
  timerFlag = true; //Se activa la bandera para saber que se ha ejecutado la interrupción
}

void setup() {
  Serial.begin(9600);
  
   
  
  mi_temporizador.setupTimer(10000000, interrupcionCallback);// 10 segundos = 10.000.000 microsegundos
  mi_temporizador.timerStart();
}

void loop() {
  //Se revisa constantemente si la bandera está activa
  if (timerFlag == true) {
    timerFlag = false; // Se desactiva la bandera
    
    // Se lee el ADC y se muestra el mensaje
    int valorADC = analogRead(potPin);
    char mensaje[50];
    sprintf(mensaje, "Temporizador: 10s - ADC leido: %d", valorADC);
    Serial.println(mensaje);
  }
}