#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

//Adafruit_PCD8544 display = Adafruit_PCD8544(SCLK, DIN, D/C, CS, RST);
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);
//TMP36 Pin Variables
int sensorPin = 0; //the analog pin the TMP36's Vout (sense) pin is connected to
//the resolution is 10 mV / degree centigrade with a
//500 mV offset to allow for negative temperatures
void setup() {
  display.begin();                          //uruchom ekran
  display.setContrast(40);                  //ustaw kontrast
  display.clearDisplay();                   //wyczyść bufor ekranu
}

double readTermistor(int val) {
  /** Steinhart-Hart equation
     1/T =crea A+ B*ln(R) + C*[ln(R)]^3
     T = temperatura
     R  = rezystancja
     A, B, C = są to współczynniki Steinharta-harta zależne od modelu termistora oraz zakresu interesujących nas temperatur.
  */
  double resistance = ((1024.0 / val) - 1);
  double Temp = log(resistance);
  Temp = 1 / (0.003354016 + 0.0002569850 * Temp + 0.000002620131 * Temp * Temp + 0.00000006383091 * Temp * Temp * Temp);
  return  Temp - 273.15;
}

double readTMP36() {
  //getting the voltage reading from the temperature sensor
  int reading = analogRead(sensorPin);
  // converting that reading to voltage, for 3.3v arduino use 3.3
  float voltage = reading * 5.0;
  voltage /= 1024.0;
  // now print out the temperature
  float temperatureC = (voltage - 0.5) * 100 ;  //converting from 10 mv per degree wit 500 mV offset
  return temperatureC;
}

void printToSerial() {
  String empty = ",";
  String A = empty + readTermistor(analogRead(A1));
  String B = empty + readTermistor(analogRead(A2));
  String C = empty + readTermistor(analogRead(A3));
  String D = empty + readTMP36();
  Serial.print(millis() + A + B + C + D );
}

void loop() {
  printToSerial();
  display.setTextSize(1);                   //ustaw rozmiar czcionki na 1
  display.setTextColor(BLACK);              //ustaw kolor tekstu na czarny
  display.clearDisplay();                   //wyczyść bufor ekranu
  display.setCursor(1, 1);                                              //to degrees ((voltage - 500mV) times 100)
  display.print("Daniel Slaby");
  display.setCursor(0, 10);
  display.print("NTC 1k:"); display.print(readTermistor(analogRead(A1)));
  display.setCursor(0, 20);
  display.print("NTC 10k:"); display.print(readTermistor(analogRead(A2)));
  display.setCursor(0, 30);
  display.print("NTC 100k:"); display.print(readTermistor(analogRead(A3)));
  display.setCursor(0, 40);                                              //to degrees ((voltage - 500mV) times 100)
  display.print("TMP36: "); display.print(readTMP36());
  display.display();
  delay(100);                                     //waiting a second
}