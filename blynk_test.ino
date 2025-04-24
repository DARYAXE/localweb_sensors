#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6x4aL8hRf"
#define BLYNK_TEMPLATE_NAME "LED"
#define BLYNK_AUTH_TOKEN "heurnSfwYDgjwGvc7m7ptLKzz8H7nkDg"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
char ssid[] = "EYESOFPRIESTESS"; // WiFi SSID
char pass[] = "whitephospor"; // WiFi password
// Servo setup without using the Servo library
#define SERVOPIN 25 // Pin for the servo
int servoMin = 500; // Minimum pulse width for 0 degrees (500μs)
int servoMax = 2400; // Maximum pulse width for 180 degrees (2400μs)
// Sensor Ultrasonik HC-SR04
#define TRIGPIN 27 // Pin TRIG sensor ultrasonic
#define ECHOPIN 26 // Pin ECHO sensor ultrasonic
#define LEDPIN 1 // Pin for LED indicator
// Threshold distance for servo
long thresholdDistance = 10; // If distance is less than 100 cm, the servo will move
// Blynk virtual pin (optional)
int LEDLogic = 0;
BLYNK_WRITE(V1) {
LEDLogic = param.asInt();
Serial.println(LEDLogic);
}
void setup() {
Serial.begin(9600);
pinMode(TRIGPIN, OUTPUT); // Set TRIG pin as output
pinMode(ECHOPIN, INPUT); // Set ECHO pin as input
pinMode(LEDPIN, OUTPUT); // Set LED as output
pinMode(SERVOPIN, OUTPUT); // Start WiFi and Blynk connection
Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
// Initialize servo position to 0 degrees
writeServo(0);
}
// Function to read distance from ultrasonic sensor
long readUltrasonicDistance() {
digitalWrite(TRIGPIN, LOW); // Ensure TRIG is low
delayMicroseconds(2);
digitalWrite(TRIGPIN, HIGH); // Send trigger pulse
delayMicroseconds(10);
digitalWrite(TRIGPIN, LOW);
long duration = pulseIn(ECHOPIN, HIGH); // Measure the echo duration
long distance = duration * 0.034 / 2; // Calculate the distance in cm
return distance;
}
// Function to generate PWM signal for controlling the servo
void writeServo(int angle) {
int pulseWidth = map(angle, 0, 180, servoMin, servoMax);
for (int i = 0; i < 50; i++) { // Kirim sinyal PWM sebanyak 50 siklus (~1 detik)
digitalWrite(SERVOPIN, HIGH);
delayMicroseconds(pulseWidth);
digitalWrite(SERVOPIN, LOW);
delayMicroseconds(20000 - pulseWidth); // Siklus penuh = 20 ms
}
}
void loop() {
Blynk.run();
long distance = readUltrasonicDistance(); // Read distance from ultrasonic sensor
// Print distance to the serial monitor
Serial.print("Distance: ");
Serial.print(distance);
Serial.println(" cm");
// Send distance to Blynk virtual pin V4
Blynk.virtualWrite(V0, distance); // Send distance value to Blynk V4
// Control servo based on distance
if (distance < thresholdDistance) {
writeServo(180); // If distance is less than the threshold, move the servo to 90 degrees
Serial.println("Servo moving to 90 degrees (close object)");
} else {
writeServo(0); // If distance is greater than the threshold, move the servo back to 0 degrees
Serial.println("Servo returning to 0 degrees (safe distance)");
}
// Control LED based on Blynk input
if (LEDLogic == 1) {
digitalWrite(LEDPIN, HIGH);
} else {
digitalWrite(LEDPIN, LOW);
}
delay(500); // Delay for 1 second before the next reading
}
