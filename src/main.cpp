#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


#define LED_BLUETOOTH     13
#define LED_POWER         12
#define SEED_COUNT_SENSOR 18

#define serviceID BLEUUID((uint16_t)0x9951)
#define DEVICE_NAME "SeedMonitoring"

#define SEED_SCAN_DEBOUNCE 10

uint32_t seedInterval = 0;



/* This function handles the server callbacks */
bool deviceConnected = false;
class ServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* MyServer) {
      deviceConnected = true;
      digitalWrite(LED_BLUETOOTH, HIGH);
    };

    void onDisconnect(BLEServer* MyServer) {
      deviceConnected = false;
      MyServer->getAdvertising()->start();
      digitalWrite(LED_BLUETOOTH, LOW);
    }
};


// Define characteristics
BLECharacteristic * seedintervalCharacteristic;




void initseedintervalCharacteristic(BLEService * service)
{
  seedintervalCharacteristic = new BLECharacteristic
  (
    BLEUUID((uint16_t)0x1B01),
    BLECharacteristic::PROPERTY_NOTIFY
  );
  service->addCharacteristic(seedintervalCharacteristic);
  seedintervalCharacteristic->setValue(seedInterval);
  seedintervalCharacteristic->addDescriptor(new BLE2902());

}




volatile unsigned long last_seed_time;

void detectSeeds()
{
  volatile unsigned long current_time = millis();
  seedInterval = current_time - last_seed_time;
  if(seedInterval > SEED_SCAN_DEBOUNCE)
  {
    seedintervalCharacteristic->setValue(seedInterval);
    seedintervalCharacteristic->notify();
    last_seed_time = current_time;
  }
}

void initBluetooth()
{
  pinMode(LED_BLUETOOTH, OUTPUT);
  digitalWrite(LED_BLUETOOTH, LOW);

  // Create and name the BLE Device
  BLEDevice::init(DEVICE_NAME);

  /* Create the BLE Server */
  BLEServer *MyServer = BLEDevice::createServer();
  MyServer->setCallbacks(new ServerCallbacks());  
  BLEService *meteringService = MyServer->createService(serviceID);
 
 // Start rpm characteristic
 initseedintervalCharacteristic(meteringService);

  // Satrt service
  MyServer->getAdvertising()->addServiceUUID(serviceID);
  meteringService->start();
  
  // Start the Server/Advertising
  MyServer->getAdvertising()->start();

  Serial.println("Waiting for a Client to connect...");
}

void setup() {
  pinMode(LED_POWER, OUTPUT);
  pinMode(LED_BLUETOOTH, OUTPUT);
  digitalWrite(LED_POWER, HIGH);
  digitalWrite(LED_BLUETOOTH, LOW);
  pinMode(SEED_COUNT_SENSOR, INPUT_PULLDOWN);
  attachInterrupt(SEED_COUNT_SENSOR, detectSeeds, RISING);

  initBluetooth();
  

  last_seed_time = millis();
}

void loop() {}