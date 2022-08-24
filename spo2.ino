#define __DEBUG__
 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DFRobot_MAX30102.h>

DFRobot_MAX30102 particleSensor;

/*
Macro definition options in sensor configuration
sampleAverage: SAMPLEAVG_1 SAMPLEAVG_2 SAMPLEAVG_4
               SAMPLEAVG_8 SAMPLEAVG_16 SAMPLEAVG_32
ledMode:       MODE_REDONLY  MODE_RED_IR  MODE_MULTILED
sampleRate:    PULSEWIDTH_69 PULSEWIDTH_118 PULSEWIDTH_215 PULSEWIDTH_411
pulseWidth:    SAMPLERATE_50 SAMPLERATE_100 SAMPLERATE_200 SAMPLERATE_400
               SAMPLERATE_800 SAMPLERATE_1000 SAMPLERATE_1600 SAMPLERATE_3200
adcRange:      ADCRANGE_2048 ADCRANGE_4096 ADCRANGE_8192 ADCRANGE_16384
*/
 
// Definir constantes
#define ANCHO_PANTALLA 128 // ancho pantalla OLED
#define ALTO_PANTALLA 64 // alto pantalla OLED
 
// Objeto de la clase Adafruit_SSD1306
Adafruit_SSD1306 display(ANCHO_PANTALLA, ALTO_PANTALLA, &Wire, -1);
 
void setup() {
  #ifdef __DEBUG__
    Serial.begin(115200);
    delay(100);
    Serial.println();
    Serial.println("Iniciando pantalla OLED");
  #endif

  while (!particleSensor.begin()) {
    Serial.println("MAX30102 was not found");
    delay(1000);
  }
  
  particleSensor.sensorConfiguration(/*ledBrightness=*/50, /*sampleAverage=*/SAMPLEAVG_4, \
                        /*ledMode=*/MODE_MULTILED, /*sampleRate=*/SAMPLERATE_100, \
                        /*pulseWidth=*/PULSEWIDTH_411, /*adcRange=*/ADCRANGE_16384);
            
  
  // Iniciar pantalla OLED en la dirección 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    #ifdef __DEBUG__
      Serial.println("No se encuentra la pantalla OLED");
    #endif
    while (true);
  }

  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.print("SPO2: ");
  display.setCursor(0,16);
  display.setTextSize(3); 
  display.print("99 %");
  display.display();
 
}
 
int32_t SPO2; //SPO2
int8_t SPO2Valid; //Flag to display if SPO2 calculation is valid
int32_t heartRate; //Heart-rate
int8_t heartRateValid; //Flag to display if heart-rate calculation is valid 

void loop()
{
  Serial.println(F("Wait about four seconds"));
  particleSensor.heartrateAndOxygenSaturation(/**SPO2=*/&SPO2, /**SPO2Valid=*/&SPO2Valid, /**heartRate=*/&heartRate, /**heartRateValid=*/&heartRateValid);
  //Print result 
  Serial.print(F("heartRate="));
  Serial.print(heartRate, DEC);
  Serial.print(F(", heartRateValid="));
  Serial.print(heartRateValid, DEC);
  Serial.print(F("; SPO2="));
  Serial.print(SPO2, DEC);
  Serial.print(F(", SPO2Valid="));
  Serial.println(SPO2Valid, DEC);

  display.clearDisplay();

  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.print("SPO2: ");
  display.setCursor(0,16);
  display.setTextSize(3); 
  display.print(SPO2);
  display.print(" %");
  display.display();
  
}
