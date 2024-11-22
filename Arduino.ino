#include <Servo.h>  // Include the Servo library

Servo myServo;  // Create a Servo object to control the servo motor

int mq2Pin = A0;     // Analog pin connected to MQ2 sensor output
int servoPin = 9;    // Digital pin connected to Servo control wire
int fanPin = 10;     // Digital pin connected to the fan's control (via transistor/relay)
int buzzerPin = 8;   // Digital pin connected to the buzzer
int gasThreshold = 60;  // Threshold value for gas detection
unsigned long fanTurnOffTime = 0;  // Variable to track when to turn off the fan
unsigned long fanRunDuration = 10000;  // Keep the fan on for 10 seconds after the gas level drops below the threshold

void setup() {
  pinMode(mq2Pin, INPUT);  // Set the MQ2 sensor pin as input
  pinMode(fanPin, OUTPUT); // Set the fan control pin as output
  pinMode(buzzerPin, OUTPUT); // Set the buzzer pin as output

  myServo.attach(servoPin); // Attach the Servo object to pin 9
  myServo.write(0);  // Initialize the servo position (0 degrees)

  Serial.begin(115200);  // Start the serial communication for debugging
}

void loop() {
  int gasValue = analogRead(mq2Pin);  // Read the analog value from the MQ2 sensor
  
  Serial.print("Gas Value: ");
  Serial.println(gasValue);  // Print the gas value to the serial monitor

  if (gasValue > gasThreshold) {
    myServo.write(180);  // Rotate the servo to 180 degrees (simulating valve open)
    digitalWrite(fanPin, HIGH);  // Turn on the fan (ventilation)
    digitalWrite(buzzerPin, HIGH);  // Turn on the buzzer to sound an alarm
    fanTurnOffTime = millis() + fanRunDuration;  // Set the time to turn off the fan after 10 seconds
    
    Serial.println("Gas leak detected! Servo activated, fan turned on, and buzzer sounding.");
  } else {
    myServo.write(0);  // Rotate the servo back to 0 degrees (simulating valve closed)
    digitalWrite(buzzerPin, LOW);  // Turn off the buzzer

    // Only turn off the fan if the 10-second delay has passed
    if (millis() > fanTurnOffTime) {
      digitalWrite(fanPin, LOW);  // Turn off the fan
      Serial.println("Fan turned off after delay.");
    }
    
    Serial.println("No gas leak detected. Servo deactivated, buzzer turned off.");
  }
  
  delay(1000);  // Wait for 1 second before reading the sensor again
}
