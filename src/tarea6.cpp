#include <Arduino.h>
#include <Arduino_LSM9DS1.h> // Librería para el Acelerómetro, Giróscopo y Magnetómetro

unsigned long tiempoAnterior = 0;
int contadorMuestras = 0;

// Arrays para almacenar 10 muestras (10 muestras x 100ms = 1 segundo)
float accX[10], accY[10], accZ[10];
float gyrX[10], gyrY[10], gyrZ[10];
float magX[10], magY[10], magZ[10];

void setup() {
  Serial.begin(9600);
  while (!Serial); // Esperamos a que abras el Monitor Serie
  
  // Inicializamos el chip IMU de la placa
  if (!IMU.begin()) {
    Serial.println("Error fatal: No se pudo iniciar el IMU.");
    while (1); // Si falla, bloqueamos la placa aquí
  }
  
  Serial.println("Sensor IMU iniciado. Esperando datos...");
}

void loop() {
  unsigned long tiempoActual = millis();
  
  // Comprobamos si han pasado 100 milisegundos
  if (tiempoActual - tiempoAnterior >= 100) {
    tiempoAnterior = tiempoActual; // Actualizamos el cronómetro
    
    // Si hay datos listos en los 3 sensores, los leemos y los guardamos en su posición del array
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable() && IMU.magneticFieldAvailable()) {
      IMU.readAcceleration(accX[contadorMuestras], accY[contadorMuestras], accZ[contadorMuestras]);
      IMU.readGyroscope(gyrX[contadorMuestras], gyrY[contadorMuestras], gyrZ[contadorMuestras]);
      IMU.readMagneticField(magX[contadorMuestras], magY[contadorMuestras], magZ[contadorMuestras]);
      
      contadorMuestras++; // Avanzamos a la siguiente celda
    }
    
    // Cuando acumulamos 10 muestras (ha pasado 1 segundo exacto), las enviamos por UART
    if (contadorMuestras >= 10) {
      Serial.println("\n--- PAQUETE DE DATOS (1 Segundo) ---");
      
      for (int i = 0; i < 10; i++) {
        // Usamos Serial.print en cascada. Es mucho más seguro para imprimir números con 
        // decimales (floats) que intentar meterlos todos en un sprintf gigante.
        Serial.print("T+"); Serial.print((i+1)*100); Serial.print("ms | ");
        
        Serial.print("Acc: "); Serial.print(accX[i]); Serial.print(", "); Serial.print(accY[i]); Serial.print(", "); Serial.print(accZ[i]); Serial.print(" | ");
        Serial.print("Gyr: "); Serial.print(gyrX[i]); Serial.print(", "); Serial.print(gyrY[i]); Serial.print(", "); Serial.print(gyrZ[i]); Serial.print(" | ");
        Serial.print("Mag: "); Serial.print(magX[i]); Serial.print(", "); Serial.print(magY[i]); Serial.print(", "); Serial.println(magZ[i]);
      }
      
      Serial.println("------------------------------------");
      
      // Reseteamos el contador a 0 para empezar a grabar el bloque del siguiente segundo
      contadorMuestras = 0;
    }
  }
}