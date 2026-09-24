#include <Arduino.h>
#include <Wire.h>
#include <Arduino_LSM9DS1.h>

// Caja de datos
struct CajaDatos {
  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;
};

CajaDatos muestras[5]; // Array para guardar las 5 cajas (1 segundo / 200ms = 5)
bool capturando = false;
int contador = 0; //contador de muestras tomadas
unsigned long tiempoAnterior = 0; //contador de cuantos ms han pasado

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  while (!Serial); // Esperar al monitor serie
  if (!IMU.begin()) {
    Serial.println("Error al iniciar IMU");
    while (1);
  }
  Serial.println("PLACA 1 Lista. Envia 'S' por el monitor serie para empezar.");
}

void loop() {
  // 1. Esperar comando del usuario
  if (!capturando && Serial.available() > 0) {
    char comando = Serial.read();
    if (comando == 'S' || comando == 's') {
      capturando = true;
      contador = 0;
      tiempoAnterior = millis(); // función que lee los ms,solo se actualiza al mandar S 
  
      Serial.println("Capturando datos (1 segundo)...");
    }
  }

  // 2. Capturar datos cada 200 ms
  if (capturando) {
    unsigned long tiempoActual = millis();
    if (tiempoActual - tiempoAnterior >= 200) {
      tiempoAnterior = tiempoActual;
      //Guardar los datos en el array
      if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable() && IMU.magneticFieldAvailable()) {
        IMU.readAcceleration(muestras[contador].ax, muestras[contador].ay, muestras[contador].az);
        IMU.readGyroscope(muestras[contador].gx, muestras[contador].gy, muestras[contador].gz);
        IMU.readMagneticField(muestras[contador].mx, muestras[contador].my, muestras[contador].mz);
        contador++;
      }
      
      // 3. Si ya tenemos las 5 muestras, las enviamos por I2C
      if (contador >= 5) {
        capturando = false; //para no capturar mas datos hasta volver a pulsar S
        Serial.println("Captura terminada. Enviando por I2C a PLACA 2...");
        
        for (int i = 0; i < 5; i++) {
          Wire.beginTransmission(8); // se envía por la dirección 8
          Wire.write((byte*)&muestras[i], sizeof(CajaDatos)); // Enviamos la caja entera en bytes
          Wire.endTransmission();
          delay(50); // Pausa de seguridad para no saturar al esclavo
        }
        Serial.println("Envio completado. Esperando nueva orden ('S').");
      }
    }
  }
}