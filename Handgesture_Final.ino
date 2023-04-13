// ------------------------
// Handgesture source code
// ------------------------

// Importing the libraries required for Serial communication and sensors
#include <AltSoftSerial.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <NewPing.h>

// Creating an Ultrasonic sensor object and setting echo/trig pins and max distance
NewPing sonar(6, 5, 150);

// Creating an accelerometer object
Adafruit_MPU6050 accelerometer;

// Setting a Serial communication line between the two Arduinos
AltSoftSerial altSerial;

// Declaring variables for all the sensors
int indexFinger = 0;
int data;
int X_pin;
int Y_pin;

// Joystick button settings
int button = 0;
int buttonstate = 0;

void setup() {
  // Initiating Serial, wire and the accelorometer
  Serial.begin(38400);
  altSerial.begin(38400);
  Wire.begin();
  accelerometer.begin();

  pinMode(2, INPUT);
  digitalWrite(2, HIGH);

  // Setting the accelerometer range to +-8G
  accelerometer.setAccelerometerRange(MPU6050_RANGE_8_G); // Different ranges: 2_G,4_G,8_G,16_G. The smaller the range, the more sensitive the accelerometer readings will be.

  // Setting gyro range to +- 500 deg/s
  accelerometer.setGyroRange(MPU6050_RANGE_500_DEG); // Different ranges: 250,500,1000,2000. Smaller degrees-per-second range leads to a more sensitive output.

  // Setting the filter bandwidth to 21 Hz
  accelerometer.setFilterBandwidth(MPU6050_BAND_21_HZ); // Different frequencies(Hz): 5, 10, 21, 44, 94, 184, 260. The bandwidth selection allows you to alter the low-pass filter’s
  // cutoff frequency, allowing you to smooth out the signal by removing high-frequency noise.
  
  pinMode(A0, INPUT); // Set A0 - Flexsensor to input
  pinMode(A1, INPUT); // Set A1 - Joystick x to input
  pinMode(A2, INPUT); // Set A2 - Joystick y to input
 
  delay(100);
}

void loop() {
  
  // Getting data from flexsensor and controlling servo
  indexFinger = analogRead(A0);
  indexFinger = map(indexFinger, 256, 59, 700, 2250);
  
  // ----------------------------------------------------------
  
  // Get new sensor events with the readings
  sensors_event_t a, g, temp;
  accelerometer.getEvent(&a, &g, &temp);

  // Get values for x and y from accelerometer
  int ax = a.acceleration.x;
  int ay = a.acceleration.y;

  // Mapping values to so that it controls the servos
  int x = map(ax, -10, 10, 0, 2250);
  int y = map(ay, -10, 10, 1500, 700);

  // ----------------------------------------------------------
  
  // Recieving the distance from the ultrasonic sensor and saving it
  int distance = sonar.ping_cm();
  int ultra_sonic = map(distance, 0, 60, 700, 2000);
  
  // ----------------------------------------------------------
  
  // Recieving the x and y values from Joystick
  X_pin = analogRead(A1);
  Y_pin = analogRead(A2);

  int joy_x = map(X_pin, 0, 1023, 700, 2250);
  int joy_y = map(Y_pin, 0, 1023, 1500, 700);

  // Button presses
  buttonstate = digitalRead(2);
  if (!buttonstate)
  {
    button = 1;
  }
  else
  {
    button = 0;
  }

  // ----------------------------------------------------------

  // Sending all values in one String via bluetooth to the other Arduino
  String data = "#," + String(indexFinger) + "," + String(x) + "," + String(y) + "," + String(joy_x) + "," + String(joy_y) + "," + String(ultra_sonic) + ",";
  altSerial.print(data);
  Serial.println(data);

  delay(10);
}
