#include <CurieBLE.h>
#include <Servo.h>

const int servoPin1 = 9; // set servoPin1 to use on-board servo1 
const int piezoPin = 12;
Servo servo1;
BLEPeripheral blePeripheral; // create peripheral instance

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // create service

// create switch characteristic and allow remote device to read and write
BLECharCharacteristic switchChar1("19B10010-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo1.attach(9);
  pinMode(servoPin1, OUTPUT); // use the LED on pin 13 as an output
  //pinMode(piezoPin, OUTPUT);

  // set the local name peripheral advertises
  blePeripheral.setLocalName("SERVOS");
  // set the UUID for the service this peripheral advertises
  blePeripheral.setAdvertisedServiceUuid(ledService.uuid());

  // add service and characteristic
  blePeripheral.addAttribute(ledService);
  blePeripheral.addAttribute(switchChar1);

  // assign event handlers for connected, disconnected to peripheral
  blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // assign event handlers for characteristic
  switchChar1.setEventHandler(BLEWritten, switchCharacteristicWritten);
// set an initial value for the characteristic
  switchChar1.setValue(0);

  // advertise the service
  blePeripheral.begin();
  Serial.println(("Bluetooth device active, waiting for connections..."));
}

void loop() {
  // put your main code here, to run repeatedly:
  // poll peripheral
  blePeripheral.poll();
}

void blePeripheralConnectHandler(BLECentral& central) {
  // central connected event handler
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

void blePeripheralDisconnectHandler(BLECentral& central) {
  // central disconnected event handler
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}

void switchCharacteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  // central wrote new value to characteristic, update LED
  Serial.print("Characteristic event, written: ");

  unsigned long seconds = 1000L; //Notice the L 
  unsigned long minutes = seconds * 60;
  unsigned long hours = minutes * 60;
  unsigned long days = hours * 24;
  unsigned long week = days * 7;

  if (switchChar1.value()) {  // if value != 1, open pill box then close it and sets delay to a week
    //tone(piezoPin, 3000, 500);
    servo1.write(180);
    Serial.println(servo1.read());
    //delay(2000);
    //servo1.write(-180);
    //delay(week);
  } else {
    servo1.write(0);
    Serial.println(servo1.read());
  }
}
