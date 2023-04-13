// ---------------------
// Robotarm source code
// ---------------------

// Importing the libraries required for Serial communication and Servos
#include <ServoTimer2.h>
#include <AltSoftSerial.h>

// Creating a servo object
ServoTimer2 servo_one;
ServoTimer2 servo_two;
ServoTimer2 servo_three;
ServoTimer2 servo_four;
ServoTimer2 servo_five;
ServoTimer2 servo_six;

// Setting a Serial communication line between the two Arduinos
AltSoftSerial altSerial;

// Declaring variables for the recieved data
String tempString;
int servo_values[6];

int i = 0;

void setup() {
  // Initiating both Serials
  Serial.begin(38400);
  altSerial.begin(38400);

  // Attaching the servos to each pin
  servo_one.attach(3);
  servo_two.attach(4);
  servo_three.attach(5);
  servo_four.attach(6);
  servo_five.attach(7);
  servo_six.attach(10);

  // Setting the servos to their starting position
  servo_one.write(1400);
  servo_two.write(0);
  servo_three.write(1500);
  servo_four.write(1500);
  servo_five.write(1600);
  servo_six.write(0);
}

void loop() {
  char data;

  // Writing the values to the servomotors
  servo_one.write(servo_values[0]);    // Gripper - Flexsensor
  servo_two.write(servo_values[1]);    // Head Up/Down - Accelerometer_X
  servo_three.write(servo_values[2]);  // Head Rotation - Accelerometer_Y
  servo_four.write(servo_values[4]);   // Elbow Up/Down - Ultrasonic Sensor
  servo_five.write(servo_values[5]);   // Arm Up/Down - Joystick Forward/Backward
  servo_six.write(servo_values[3]);    // Arm spin - Joystick Right/Left

  // Checks if we have recieve data
  if (altSerial.available()) {
    data = altSerial.read();  // If we do, the charachter will be saved to the data variable
    if (data == ',') {        // Comma is the delimeter which will seperate the data recieved
      if (tempString.length() > 1) {
        servo_values[i] = tempString.toInt();  // The value will then be converted from String to Int and saved into the right position in the array

        // Checking in Serial if the right values are saved to the right array position
        Serial.print(i);
        Serial.println(servo_values[i]);

        i++;  // Incrementing i so that next value is saved in the next position

        tempString = "";  // Clearing out the String for next input
      }
    } else {
      tempString += data;  // If the next charachter is not a delimeter, then save the number in the String.
    }

    // The input String always starts with a "#", this function sets the variable i back to 0 so that we can get the next value from the sensors.
    if (tempString.startsWith("#")) {
      i = 0;
      tempString = "";
    }
  }
}
