/*


*/

#include <ArduinoBLE.h>
#define SERVICE_UUID        
#define CHARACTERISTIC_UUID_TX 
#define SENSOR_PIN A0
#define LO_PLUS_PIN 10
#define LO_PLUS_MIN 11

BLEService ECGservice(SERVICE_UUID );


BLEIntCharacteristic ECGchar( CHARACTERISTIC_UUID_TX, BLERead | BLENotify );

int EcgValue = 0;
static unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);    // initialize serial communication
  pinMode(LO_PLUS_PIN, INPUT); // Setup for leads off detection LO +
  pinMode(LO_PLUS_MIN, INPUT); // Setup for leads off detection LO -
  while (!Serial);


  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  /* Set a local name for the BLE device
     This name will appear in advertising packets
     and can be used by remote devices to identify this BLE device
     The name can be changed but maybe be truncated based on space left in advertisement packet
  */
  BLE.setLocalName("Ecg Rose");
  BLE.setAdvertisedService(ECGservice); // add the service UUID
  ECGservice.addCharacteristic(ECGchar); // add the ecg  characteristic
  BLE.addService(ECGservice); // Add the battery service
  ECGchar.writeValue( 0 ); // set initial value for this characteristic
  /* Start advertising BLE.  It will start continuously transmitting BLE
     advertising packets and will be visible to remote BLE central devices
     until it receives a new connection */

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // wait for a BLE central
  BLEDevice central = BLE.central();

  // if a central is connected to the peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's BT address:
    Serial.println(central.address());
    // turn on the LED to indicate the connection:


    // while the central is connected:
    while (central.connected()) {

      EcgValue = analogRead(SENSOR_PIN);
      long currentMillis = millis();
      // Let's convert the value to a char array:
      char txString[4]; // make sure this is big enuffz
     // dtostrf(EcgValue, 1, 2, txString); // int_val, min_width, digits_after_decimal, char_buffer      it needs Library , doesnt read it 
      itoa(EcgValue, txString, 10);


      if (currentMillis - previousMillis >= 200) {
        previousMillis = currentMillis;
        ECGchar.writeValue((int)txString);
        Serial.println(txString);

      }
    }
    // when the central disconnects, turn off the LED:
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}