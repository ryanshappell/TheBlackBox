#include <Adafruit_NeoPixel.h>
#include <Servo.h>

//Fan
const int fanPin = 4;
long onTime = 0;
long lastOn = 0;
long offTime = 0;
long lastOff = 0;

//LED strip
const int LEDPin = 5;
#define LED_COUNT 5
Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_COUNT, LEDPin, NEO_RGB + NEO_KHZ800);
int red = 0;
int green = 0;
int blue = 0;

//Two ultrasonic sensors
const int echoPin1 = 6;
const int trigPin1 = 7;
const int echoPin2 = 8;
const int trigPin2 = 9;
unsigned int distance1;
unsigned int distance2;

//Servo motor
Servo servo;
const int servoPin = 10;
boolean canMove = false;
long downTime = 0;
long lastDown = 0;
long upTime = 0;
long lastUp = 0;

//Touch sensor
const int touchPin = A0;
int touchReading;

void setup() {
  Serial.begin(9600);
  leds.begin();

  servo.attach(servoPin);
  pinMode(echoPin1, INPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(fanPin, OUTPUT);
  servo.write(0);
}

void loop() {
  //First ultrasonic sensor reading
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  distance1 = pulseIn(echoPin1, HIGH) / 58.2;
  delay(50);
  
  //Second ultrasonic sensor reading
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  distance2 = pulseIn(echoPin2, HIGH) / 58.2;
  delay(50);

  //Change leds color
  red = map(constrain(distance1, 0, 25), 0, 25, 255, 0);
  blue = map(constrain(distance2, 0, 25), 0, 25, 255, 0);
  
  leds.setPixelColor(0, green, red, blue);
  leds.setPixelColor(1, green, red, blue);
  leds.setPixelColor(2, green, red, blue);
  leds.setPixelColor(3, green, red, blue);
  leds.setPixelColor(4, green, red, blue);
  leds.show();

  //Touch and fan
  touchReading = analogRead(touchPin);
  if (touchReading < 10) {
    if(millis() - lastOff >= offTime){
      lastOff = millis();
      digitalWrite(fanPin, HIGH);
      offTime = random(2000, 5000);
    }
    if (millis() - lastOn >= onTime) {
      lastOn = millis();
      digitalWrite(fanPin, LOW);
      onTime = random(2000, 5000);
    }
  } else {
    if (millis() - lastOn >= onTime) {
      lastOn = millis();
      digitalWrite(fanPin, LOW);
      onTime = random(2000, 5000);
    }
  }

  //Servo to touch hand
  if (distance1 < 25 || distance2 < 25) {
    canMove = true;
  } else {
    canMove = false;
  }
  if (canMove) {
    if(millis() - lastUp >= upTime){
      lastUp = millis();
      servo.write(50);
      upTime = random(3000, 10000);
    }
    if (millis() - lastDown >= downTime) {
      lastDown = millis();
      servo.write(0);
      downTime = random(1000, 2000);
    }
  }
}
