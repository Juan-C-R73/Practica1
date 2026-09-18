#include <Arduino.h>
#include "mbed.h" // Se importa mbed 
#include "BBTimer.hpp" // Incluir librería creada en el foro

const int potPin = A0;

volatile bool timerFlag = false; // Flag para avisar al programa que se ha activado la interrupción


BBTimer mi_temporizador(BB_TIMER1); //Configurar temporizador 1

// Configuramos el pin 9 usando la clase PwmOut de mbed
mbed::PwmOut salidaPWM(digitalPinToPinName(9)); 

//Interrupción que se ejecutará cada 10 segundos
void interrupcionCallback() {
  
  timerFlag = true; //Se activa la bandera para saber que se ha ejecutado la interrupción
}


void setup() {
  Serial.begin(9600);
  
  // Configuramos la Frecuencia a 5kHz (Periodo = 1 / 5000)
  salidaPWM.period(1.0 / 5000.0); 
  // Inicializamos el PWM apagado (ciclo de trabajo 0)
  salidaPWM.write(0.0f);
}

void loop() {
if (timerFlag == true) {
    timerFlag = false;
    
    int valorADC = analogRead(potPin);
    char mensaje[50];
    sprintf(mensaje, "[Timer] ADC: %d", valorADC);
    Serial.println(mensaje);
  }
  if (Serial.available() > 0) {
    // Leemos todo el texto hasta que el usuario pulsa Enter (\n)
    String comando = Serial.readStringUntil('\n');
    comando.trim(); // Limpiamos espacios en blanco o retornos de carro extra (\r)

    // A. Comando directo: "ADC"
    if (comando == "ADC") {
      int valorADC = analogRead(potPin);
      char mensaje[50];
      sprintf(mensaje, "[Comando] Lectura unica ADC: %d", valorADC);
      Serial.println(mensaje);
    }
    
    // B. Comando periódico: "ADC(x)"
    else if (comando.startsWith("ADC(") && comando.endsWith(")")) {
      // Extraemos solo el número. Empieza en el carácter 4 y termina 1 antes del final
      String numeroString = comando.substring(4, comando.length() - 1);
      int x = numeroString.toInt();

      if (x == 0) {
        mi_temporizador.timerStop();
        Serial.println("Envio automatico de ADC detenido.");
      } else {
        // Configuramos el timer. x está en segundos, multiplicamos por 1.000.000 para microsegundos
        mi_temporizador.setupTimer(x * 1000000, interrupcionCallback);
        mi_temporizador.timerStart();
        
        char msg[50];
        sprintf(msg, "Iniciado envio de ADC cada %d segundos.", x);
        Serial.println(msg);
      }
    }
    
    // C. Comando PWM: "PWM(x)" con valores del 0 al 9
    else if (comando.startsWith("PWM(") && comando.endsWith(")")) {
      String numeroString = comando.substring(4, comando.length() - 1);
      int nivelPWM = numeroString.toInt();

      // Comprobamos que el número introducido esté en el rango permitido (0 a 9)
      if (nivelPWM >= 0 && nivelPWM <= 9) {
        // Dividimos entre 9.0 para obtener un rango entre 0.0 (0%) y 1.0 (100%)
        float proporcion = nivelPWM / 9.0;
        salidaPWM.write(proporcion);
        
        char msg[50];
        sprintf(msg, "PWM ajustado a nivel %d (aprox %d%%)", nivelPWM, (int)(proporcion * 100));
        Serial.println(msg);
      } else {
        Serial.println("Error: El valor de PWM debe estar entre 0 y 9.");
      }
    }
    
    // Comando no reconocido
    else if (comando.length() > 0) {
      Serial.println("Comando desconocido. Usa: ADC, ADC(x) o PWM(x)");
    }
  }

}