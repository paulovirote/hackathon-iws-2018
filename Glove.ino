#include <SoftwareSerial.h>

SoftwareSerial bluetooth(2, 3); // RX, TX

const int motorPin = 4;
const int buzzerPin = 5;
const int ringInterruptPin = 12;

bool interruptButtonState = false;

int ring[][2] = {
               {1000,200},
               {600,200},
               {300,100},
               {200,100},
               {100,100},
               {50,100}};

int intensity = 0;

void setup() {
 pinMode(motorPin, OUTPUT);
 pinMode(buzzerPin, OUTPUT);
 pinMode(ringInterruptPin, INPUT);

 Serial.begin(115200);
 bluetooth.begin(115200);

 digitalWrite(buzzerPin, HIGH);
}

void loop() {
  if (bluetooth.available()) {
    char bluetoothRead = bluetooth.read();
    intensity = bluetoothRead - ‘0’;

    Serial.println(String(intensity));
    updateInterrupState();

    if (intensity <= 5 && intensity >= 0 && !interruptButtonState) {
      ringWithIntensity(intensity);
    } else if (interruptButtonState) {
      bluetooth.write(“Giozinho”);

      while(bluetooth.read() != ‘e’){}

      interruptButtonState = false;
    }
  }
}

void ringWithIntensity(int intensity) {
  Serial.println(String(intensity));
  digitalWrite(motorPin, HIGH);
  slicedDelay(ring[intensity][1]);
  digitalWrite(motorPin, LOW);

  slicedDelay(ring[intensity][0]);
}

void updateInterrupState() {
  if (digitalRead(ringInterruptPin) == HIGH) {
    interruptButtonState = true;
  }
}

void slicedDelay(int timeInterval) {
  int timeSlice = timeInterval/5;

  delay(timeSlice);
  updateInterrupState();

  delay(timeSlice);
  updateInterrupState();

  delay(timeSlice);
  updateInterrupState();

  delay(timeSlice);
  updateInterrupState();

  delay(timeSlice);
  updateInterrupState();
}