#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SERVICE_UUID        "12345678-1234-5678-1234-56789abcdef0"  // Define a custom service UUID
#define CHARACTERISTIC_UUID "12345678-1234-5678-1234-56789abcdef1"  // Define a custom characteristic UUID

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;
uint32_t activePin = 0;

// Callback to handle BLE client connection and disconnection
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;
        Serial.println("BLE Client Connected");
    }

    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;
        Serial.println("BLE Client Disconnected");
        pServer->startAdvertising();  // Restart advertising after client disconnects
    }
};

// Function to perform actions based on received message
void performAction(String action) {
  Serial.println(action);
  if(action=="up"){
    activePin = 25;
    digitalWrite(26,LOW);
    digitalWrite(18,LOW);
    digitalWrite(19,LOW);
  }else if(action=="down"){
    activePin = 26;
    digitalWrite(25,LOW);
    digitalWrite(18,LOW);
    digitalWrite(19,LOW);
  }else if(action=="left"){
    activePin = 18;
    digitalWrite(26,LOW);
    digitalWrite(25,LOW);
    digitalWrite(19,LOW);
  }else if(action=="right") {
    activePin = 19;
    digitalWrite(26,LOW);
    digitalWrite(18,LOW);
    digitalWrite(25,LOW);
  }
}

void setup() {
    Serial.begin(115200);
    pinMode(25,OUTPUT);
    pinMode(26,OUTPUT);
    pinMode(18,OUTPUT);
    pinMode(19,OUTPUT);
    // Initialize BLE
    BLEDevice::init("ESP32_BLE_Server");  // Name of BLE device
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    // Create a custom service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create a characteristic that is readable, writable, and can notify
    pCharacteristic = pService->createCharacteristic(
        CHARACTERISTIC_UUID,
        BLECharacteristic::PROPERTY_READ |
        BLECharacteristic::PROPERTY_WRITE |
        BLECharacteristic::PROPERTY_NOTIFY
    );

    pCharacteristic->addDescriptor(new BLE2902());

    // Start the service
    pService->start();

    // Start advertising
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->start();

    Serial.println("ESP32 BLE Server Running...");
    
    // Set GPIO pins (example: LED and Buzzer)
    pinMode(2, OUTPUT);  // LED pin
}

void loop() {
    if (deviceConnected) {
        // Check if there is incoming data
        if (pCharacteristic->getValue().length() > 0) {
            String receivedData = pCharacteristic->getValue().c_str();
            // Perform actions based on the received data
            performAction(receivedData);

            // Clear the value after processing
            pCharacteristic->setValue("");
        }
    }
    if(activePin != 0) {
      digitalWrite(activePin,HIGH);
    }
}
