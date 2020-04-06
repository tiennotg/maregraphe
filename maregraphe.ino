#include <SPI.h>
#include <SD.h>

const char* fileName = "maree.csv";
const int chipSelect = 4;
const int serialDebugBaudRate = 9600;
const int analogPin = A0;
const long duration = 1000; // in ms

long previousTime = 0;
long currentTime = 0;

#define _DEBUG

void serialPrintDebug(String line) {
  #ifdef _DEBUG
  Serial.println(line);
  #endif
}

void setup() {
  #ifdef _DEBUG
  Serial.begin(serialDebugBaudRate);
  #endif
  
  serialPrintDebug("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    serialPrintDebug("SD initialization failed!");
    while (1)
      delay(1000);
  }

  serialPrintDebug("SD card initialized.");
  
  File dataFile = SD.open(fileName, FILE_WRITE);
    
    if (dataFile) {
      serialPrintDebug("File opened.");
      serialPrintDebug("Writing header...");
      dataFile.println("########## New session ##########");
      dataFile.println("# Format : time (ms);data");
      
      serialPrintDebug("Data written. Closing file.");
      dataFile.close();
    }
    else
      serialPrintDebug("File opening error!");
}

void loop() {
  currentTime = millis();
  if (currentTime - previousTime >= duration)
  {
    previousTime = currentTime;
    serialPrintDebug("Opening file...");
    File dataFile = SD.open(fileName, FILE_WRITE);
    
    if (dataFile) {
      serialPrintDebug("File opened.");
      serialPrintDebug("Reading sensor data...");
  
      int data = analogRead(analogPin);
      
      serialPrintDebug("Writing data...");
      dataFile.println(String(currentTime)+";"+String(data));
      
      serialPrintDebug("Data written. Closing file.");
      dataFile.close();
    }
    else
      serialPrintDebug("File opening error!");
  }
}
