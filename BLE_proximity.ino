#include "BLEDevice.h"
int LED =2;
int accountant = 0;
static BLEAddress *pServerAddress;
BLEScan* pBLEScan;
BLEClient*  pClient;
bool deviceFound = false;
bool lit = false;
bool BotonOff = false;
String knownAddresses[] = { "fe:58:8a:5b:c7:4a"};
/*******************************************************************
you can add multiple known device's addresses separated by comma like:
String knownAddresses[] = { "fe:58:8a:5b:c7:4a","ee:f5:8b:44:cd:2a"};
********************************************************************/

unsigned long entry;


static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
  Serial.print("Notify callback for characteristic ");
  Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
  Serial.print(" of data length ");
  Serial.println(length);
}


class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice Device){
      //Serial.print("BLE Advertised Device found: ");
      //Serial.println(Device.toString().c_str());
      pServerAddress = new BLEAddress(Device.getAddress());
      bool known = false;
      bool Master = false;
      for (int i = 0; i < (sizeof(knownAddresses) / sizeof(knownAddresses[0])); i++) {
        if (strcmp(pServerAddress->toString().c_str(), knownAddresses[i].c_str()) == 0)
          known = true;
      }
      if (known) {
        Serial.print("Device found: ");
        Serial.println(Device.getRSSI());
        if (Device.getRSSI() > -85) {        //You can set the threshold here
          deviceFound = true;
        }
        else {
          deviceFound = false;
        }
        Device.getScan()->stop();
        delay(100);
      }
    }
};



void setup() {
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
  BLEDevice::init("");
  pClient  = BLEDevice::createClient();
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  Serial.println("Done");
}



void Bluetooth() {
  Serial.println();
  Serial.println("BLE Scan restarted.....");
  deviceFound = false;
  BLEScanResults scanResults = pBLEScan->start(5);
  if (deviceFound) {
    Serial.println("Found");
    lit = true;
    digitalWrite(LED,HIGH);
    accountant = 0;
    delay(1000);
  }
  else{
    digitalWrite(LED,LOW);        
    delay(1000);
  }
}

void loop() {
  Bluetooth();
}
