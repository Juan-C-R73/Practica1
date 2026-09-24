#include <Arduino.h>
#include <Wire.h>

// La caja debe ser exactamente igual a la de la Placa 1
struct CajaDatos {
  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;
};

volatile bool datosRecibidos = false;
CajaDatos cajaRecibida;

unsigned long tiempoEncendidoLED = 0;
bool ledEncendido = false;

// Interrupción: Se ejecuta sola cuando llegan datos por I2C
void receiveEvent(int howMany) {
  if (howMany == sizeof(CajaDatos)) {
    Wire.readBytes((byte*)&cajaRecibida, sizeof(CajaDatos));
    datosRecibidos = true;
  } else {
    while(Wire.available()) Wire.read(); // Limpiar si llega basura
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  
  Wire.begin(8); // Dirección 8
  Wire.onReceive(receiveEvent);
  
  Serial.println("PLACA 2 Lista. Esperando datos...");
}

void loop() {
  // 1. Mostrar datos si acaban de llegar
  if (datosRecibidos) {
    datosRecibidos = false;
    
    Serial.println("\n--- DATO RECIBIDO ---");
    Serial.print("Acc: "); Serial.print(cajaRecibida.ax); Serial.print(", "); Serial.print(cajaRecibida.ay); Serial.print(", "); Serial.println(cajaRecibida.az);
    Serial.print("Gyr: "); Serial.print(cajaRecibida.gx); Serial.print(", "); Serial.print(cajaRecibida.gy); Serial.print(", "); Serial.println(cajaRecibida.gz);
    Serial.print("Mag: "); Serial.print(cajaRecibida.mx); Serial.print(", "); Serial.print(cajaRecibida.my); Serial.print(", "); Serial.println(cajaRecibida.mz);
    
    // Encendemos el LED y arrancamos nuestro cronómetro
    digitalWrite(LED_BUILTIN, HIGH);
    ledEncendido = true;
    tiempoEncendidoLED = millis();
  }

  // 2. Apagar el LED cuando pase 1 segundo exacto
  if (ledEncendido && (millis() - tiempoEncendidoLED >= 1000)) {
    digitalWrite(LED_BUILTIN, LOW);
    ledEncendido = false;
  }
}