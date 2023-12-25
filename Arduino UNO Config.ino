#include <LiquidCrystal.h>
#include <Servo.h>
#include <IRremote.hpp>

// LCD configuration
LiquidCrystal lcd(11, 10, 9, 8, 7, 6);

// Constants for different sensors and components
const int trigPin = 13;           // Trigger pin of the ultrasonic sensor
const int echoPin = 12;           // Echo pin of the ultrasonic sensor
const int closingThreshold = 100; // Threshold distance for object detection

const int temp = A0;              // Temperature sensor pin
const int tempThreshold = 25;     // Threshold temperature value

const int pirPin = 5;             // Motion sensor pin
const int ledGreen = 2;           // Green LED pin

const int gasPin = A1;            // Gas sensor pin
const int gasThreshold = 9;       // Threshold gas concentration value
const int ledWhite = 19;          // White LED pin

const int ldrPin = A2;            // Light sensor pin
const int ledRed = 4;             // Red LED pin

const int buzzer = 18;            // Buzzer pin

// Servo and Light configurations
Servo myservo;       // Door servo
const int light = 17; // Lights

int recvPin = 0;
int Switch;
IRrecv irrecv(recvPin); 
decode_results results;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2); // Initialize LCD with 16 columns and 2 rows
  lcd.setCursor(3, 0);
  lcd.print("Smart Home");
  lcd.setCursor(5, 1);
  lcd.print("System");

  // Setting pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(pirPin, INPUT);
  pinMode(gasPin, INPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledWhite, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(light, OUTPUT);
  pinMode(buzzer, OUTPUT);

  myservo.attach(3); // Attaches the servo motor to pin 3
  irrecv.enableIRIn(); // Enables the IR receiver
  
  delay(1000);
  
  lcd.clear();
  
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  
  lcd.setCursor(0, 1);
  lcd.print("Smoke: ");
}

void loop()
{
  
  //...
  //...
  //Gas Detection
  //...
  //...
  
  int gasValue = analogRead(gasPin);// read the gas sensor value
  // Map the sensor range to a 0-100% concentration range
  int gasConcentration = map(gasValue, 0, 1023, 0, 100); 
  
  Serial.print("Gas Concentration: ");
  Serial.print(gasConcentration);
  Serial.println("%");
  
  lcd.setCursor(7,1);
  lcd.print(gasConcentration);
  lcd.print("%");
  
  if(gasConcentration > gasThreshold)
  {
    Serial.println("Gas Concentration is High.");
    digitalWrite(buzzer, HIGH); 
    digitalWrite(ledWhite, HIGH); 
    lcd.print("  HIGH");
  }
  else
  {
    Serial.println("Gas Concentration is Low.");
    digitalWrite(buzzer, LOW); 
    digitalWrite(ledWhite, LOW); 
    lcd.print("    LOW");
  }
  
  
  //..
  //..
  //Light Intensity Detection for Streetlight
  //..
  //..
  
  int ldrValue = analogRead(ldrPin);
  int intensity = map(ldrValue,6,679,255,0);//mapping the photoressistor input value for light brightness
  int intensityPercentage = map(ldrValue, 0, 1023, 0, 100);
  Serial.print("Light Intensity:");
  Serial.print(intensityPercentage);
  Serial.print("%\n");
  
  if (intensityPercentage < 30){
  	digitalWrite(ledRed,HIGH);
  }
  else{
    digitalWrite(ledRed,LOW);
  }
  
  
  //..
  //..
  //IR Remote Control
  //..
  //..
  if (IrReceiver.decode()) {
   	switch(IrReceiver.decodedIRData.command) { 
    // Button 3: Close Lights
        case 18:	
      				Serial.println("Button 3 Pressed. Lights turned off.");
      				digitalWrite(light, LOW);  
          			break;  
    // Button 2: Open Lights
        case 17:	
      				Serial.println("Button 2 Pressed. Lights turned on.");
      				digitalWrite(light, HIGH); 
          			break;  
    // Button 1: Open Door
        case 16:	
      				Serial.println("Button 1 Pressed. Door Opened.");
					myservo.write(165);  
          			break;  
    // Button 0: Close Door
        case 12:	
      				Serial.println("Button 0 Pressed. Door Closed.");
      				myservo.write(90);  
          			break;
        default: Serial.println(IrReceiver.decodedIRData.command);     
    }     
    IrReceiver.resume(); // Receive the next value
  }  
  Serial.println();
  delay(1000);
}

long microsecondsToCentimeters(long microseconds) {

  return microseconds / 29 / 2;//sound travels in micro second per centimeter
}
