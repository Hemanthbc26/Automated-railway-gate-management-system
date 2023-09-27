#include <Servo.h>
#include <HX711.h>

const int trigPin1 = 2; //ulatrasonic sensor pins
const int echoPin1 = 3;  //ulatrasonic sensor pins
const int trigPin2 = 4;  //ulatrasonic sensor pins
const int echoPin2 = 5;  //ulatrasonic sensor pins
const int gateServoPin = 8 ; //servo motor pin
const int armServoPin = 9;  //servo motor pin
const int gateopenPosition = 90;  //initial position of the gate
const int gateclosePosition = 0;  //closing position of the gate
const int armopenPosition = 90;  //initial position of the gate
const int armclosePosition = 180; //closing position of the gate
const int buzzerPin =12;  //buzzer pin
const int irSensorPin = 13;  //IR sensor pin
const int loadCellDoutPin1 = 6; // HX711 data pin for load cell 1
const int loadCellSckPin1 = 7;  // HX711 clock pin for load cell 1
const int loadCellDoutPin2 = 10; // HX711 data pin for load cell 2
const int loadCellSckPin2 = 11;  // HX711 clock pin for load cell 2
const int ledPin = A0;
const int ledPin1 = A1;
const int servoDelay = 10;
Servo gateServo;
Servo armServo;
HX711 scale1;
HX711 scale2;

// Calibration values for converting raw readings to grams
const float calibration_factor1 = 696.50; // Replace with your calibration factor for load cell 1
const float calibration_factor2 = 696.50; // Replace with your calibration factor for load cell 2

void setup() {
  Serial.begin(9600);  // Initialize serial communication for debugging
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(irSensorPin, INPUT);
  pinMode(loadCellDoutPin1, INPUT);
  pinMode(loadCellSckPin1, OUTPUT);
  pinMode(loadCellDoutPin2, INPUT);
  pinMode(loadCellSckPin2, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  gateServo.attach(gateServoPin);
  armServo.attach(armServoPin);
  gateServo.write(gateopenPosition);  // Set the initial position of the gate servo
  armServo.write(armopenPosition);   // Set the initial position of the arm servo


  scale1.begin(loadCellDoutPin1, loadCellSckPin1); // Initialize load cell 1
  scale1.set_scale(calibration_factor1);           // Set the calibration factor
  scale1.tare();                                   // Tare the scale


  scale2.begin(loadCellDoutPin2, loadCellSckPin2); // Initialize load cell 2
  scale2.set_scale(calibration_factor2);           // Set the calibration factor
  scale2.tare();                                   // Tare the scale
}

void loop() {
  // Ultrasonic sensor 1: Measure distance
  long duration1, distance1;
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;

  
   // Ultrasonic sensor 2: Measure distance

  long duration2, distance2;
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;

   // Read the state of the IR sensor
  int irSensorState = digitalRead(irSensorPin);

  Serial.print("Distance1: ");
  Serial.print(distance1);
  Serial.println(" cm");

  Serial.print("Distance2: ");
  Serial.print(distance2);
  Serial.println(" cm");

// Read weight from load cell 1
  float weight1 = (scale1.get_units() ); // Read weight from load cell 1
  Serial.print("Weight1: ");
  Serial.print(weight1);
  Serial.println(" g");

// Read weight from load cell 2
  float weight2 = (scale2.get_units() ); // Read weight from load cell 2
  Serial.print("Weight2: ");
  Serial.print(weight2);
  Serial.println(" g");
  
// Check if a train is detected based on IR sensor and weight conditions
  if (irSensorState == LOW && ((weight1 > 40 && weight1 < 100) || (weight2 > 40 && weight2 < 100))) 
  {
    Serial.println("TRAIN DETECTED");
  }
  else{
   Serial.println("TRAIN  NOT DETECTED");
  }

  Serial.println("\n");
 
 // Perform actions based on sensor readings and conditions
  if (distance1 < 5 && irSensorState == LOW && weight1 > 40 &&  weight1 < 100 ) {
    turnOnBuzzer();
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin, HIGH);
    
    closeGate();
    moveArmDown();
    
  }


  if (distance2 < 5 && irSensorState == LOW && weight2 > 40  && weight2 <100) {
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin1, HIGH);
    openGate();
    moveArmUp();
    turnOffBuzzer();
  }
  delay(500);
  
 
}
// Custom function to open the gate
void openGate() {
  gateServo.write(gateopenPosition);
}

// Custom function to open the gate
void closeGate() {
  gateServo.write(gateclosePosition);
}

// Custom function to raise the arm
void moveArmUp() {
  armServo.write(armopenPosition);
}

// Custom function to lower the arm
void moveArmDown() {
  armServo.write(armclosePosition);
}

// Custom function to turn on the buzzer
void turnOnBuzzer() {
  digitalWrite(buzzerPin, HIGH);
}

// Custom function to turn off the buzzer
void turnOffBuzzer() {
  digitalWrite(buzzerPin, LOW);
}
