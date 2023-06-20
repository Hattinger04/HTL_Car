#include <SoftwareSerial.h>
#include <FahrzeugLib.h>

CMotor MotorLinks(5, 4);
CMotor MotorRechts(3, 2);
CAntrieb Antrieb (&MotorLinks, &MotorRechts);

const int trigPin = 11;
const int echoPin = 12;
const int trigPin2 = 8;
const int echoPin2 = 9;
const int trigPin3 = 6;
const int echoPin3 = 7;

int abstandLaenge = 33;
int motor1PWM = 3;
int motor1nPWM = 2;
int motor2PWM = 5;
int motor2nPWM = 4;

double gesMax = 248;
double gesMin = 120;

double parMax = 5;
double parMin = 0;

float rechts = 50;
float links = 50;
float vorne = 50;

boolean richtung = true;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);

  Serial.begin(9600);
}

void loop() {
  rechts = abstand(trigPin, echoPin);
  delay(5);
  links = abstand(trigPin3, echoPin3);
  delay(5);
  vorne = abstand(trigPin2, echoPin2);
  delay(5);
  int minimal = minimalValue(rechts, links, vorne);
  if (minimal <= 4 && minimal != 0) {
    if (rechts > links) {
      Antrieb.Kurve(2, gesMax, false);
    } else if (rechts < links) {
      Antrieb.Kurve(-2, gesMax, false);
    } else {
      Antrieb.Kurve(0, gesMax, false);
    }
    delay(750);
  } else if (minimal <= abstandLaenge) {
    lenken(getAusweichRichtung(vorne, rechts, links), minimal, true);
  } else if (minimal > abstandLaenge) {
    Antrieb.Kurve(0, gesMax, true);
  }
}

float abstand(int trigerPin, int echosPin) {
  long duration, cm;

  digitalWrite(trigerPin, LOW);
  delayMicroseconds(5);

  digitalWrite(trigerPin, HIGH);
  delayMicroseconds(15);

  digitalWrite(trigerPin, LOW);

  duration = pulseIn(echosPin, HIGH);
  cm = microsecondsToCentimeters(duration);

  return cm;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}


void lenken(int richtung, int value, boolean vor) {
  double ges = (gesMax - gesMin) / abstandLaenge * value + gesMin;
  double par = richtung * (parMax - parMin) / abstandLaenge * value + parMin;
  Antrieb.Kurve(par, ges, vor);
}

int getAusweichRichtung(float vorne, float rechts, float links) {
  boolean ausweichen;
  minimalValue(vorne, rechts, links) < abstandLaenge ? ausweichen = true : ausweichen = false;
  if (ausweichen) {
    if (rechts > links) {
      return -1;
    } else if (rechts <= links) {
      return 1;
    }
  }
  return 0;
}

float minimalValue(float value1, float value2, float value3) {
  int valueMin = min(value1, value2);
  return min(valueMin, value3);
}
