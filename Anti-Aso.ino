// C++ code - Dog Deterrent System
const int ledPin = 3;
const int buzzerPin = 4;
const int pirPin = 5;
const int statusLED = 6;

int sensorState = LOW;
int sensorVal = 0;

// Timing variables for non-blocking operation
unsigned long previousMillis = 0;
const long blinkInterval = 500;  // LED blink interval
bool ledState = LOW;

void setup()
{
  // Configure pins
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(pirPin, INPUT);
  pinMode(statusLED, OUTPUT);
  
  // CRITICAL: Force all outputs to known OFF state
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);
  noTone(buzzerPin);  // Ensure no PWM signal on buzzer pin
  digitalWrite(statusLED, HIGH);
  
  // Initialize serial
  Serial.begin(9600);
  Serial.println("Dog Deterrent System Initialized");
  Serial.println("Calibrating PIR sensor...");
  
  // PIR sensor stabilization period
  delay(2000);  // 2 second calibration
  
  Serial.println("System ready!");
  Serial.println("---");
}

void loop()
{
  sensorVal = digitalRead(pirPin);
  
  if (sensorVal == HIGH) {
    if (sensorState == LOW) {
      Serial.println("Motion detected!");
      Serial.println("Emitting 25 kHz ultrasonic frequency");
      sensorState = HIGH;
    }
    
    tone(buzzerPin, 25000);
    
    // Display frequency status every second
    static unsigned long lastPrint = 0;
    if (millis() - lastPrint >= 1000) {
      Serial.print("Buzzer Active - Frequency: 25000 Hz");
      Serial.println();
      lastPrint = millis();
    }
    
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= blinkInterval) {
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
    }
  }
  else {
    if (sensorState == HIGH) {
      Serial.println("Motion stopped!");
      Serial.println("Frequency output: OFF");
      sensorState = LOW;
    }
    
    noTone(buzzerPin);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
    ledState = LOW;
    previousMillis = millis();
  }
}
