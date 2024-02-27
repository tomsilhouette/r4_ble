#include <ArduinoBLE.h>
#include <Servo.h>
#include <string>  
#include <vector>  

Servo myServo;
int prevAngle = -1;

const char* deviceServiceUuid = "e2088282-4fde-42f9-bb22-6ec3c7ed8f91";
const char* deviceServiceRequestCharacteristicUuid = "6d92661d-f429-4d67-929b-28e7a9780912";
const char* deviceServiceResponseCharacteristicUuid = "8dcf199a-30e7-4bd4-beb6-beb57dca866c";

BLEService bleService(deviceServiceUuid);
BLEStringCharacteristic bleRequestCharacteristic(deviceServiceRequestCharacteristicUuid, BLEWrite, 4);
BLEStringCharacteristic bleResponseCharacteristic(deviceServiceResponseCharacteristicUuid, BLENotify, 16);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  myServo.attach(9);
  Serial.begin(9600);

  BLE.setDeviceName("TOMS");
  BLE.setLocalName("TOMSLOCALARDUINO");

  if (!BLE.begin()) {
    Serial.println("Starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  BLE.setAdvertisedService(bleService);
  bleService.addCharacteristic(bleRequestCharacteristic);
  bleService.addCharacteristic(bleResponseCharacteristic);
  BLE.addService(bleService);
  // bleResponseCharacteristic.writeValue("0");

  BLE.advertise();

  Serial.println("Arduino R4 WiFi BLE (Peripheral Device)");
  Serial.println(" ");
}

void loop() {
  BLEDevice central = BLE.central();
  Serial.println("- Discovering central device...");
  delay(500);

  if (central) {
    Serial.println("* Connected to central device!");
    Serial.print("* Device MAC address: ");
    Serial.println(central.address());
    Serial.println(" ");

    while (central.connected()) {
      if (bleRequestCharacteristic.written()) {
          digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
          bleRequestCharacteristic.value();

          bleResponseCharacteristic.writeValue("I am an arduino.");
      }
      else
      {
          digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
      }
    }
    Serial.println("* Disconnected to central device!");
  }
}