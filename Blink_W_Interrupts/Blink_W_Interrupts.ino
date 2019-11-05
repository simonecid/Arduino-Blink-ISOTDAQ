#include <rgb_lcd.h>

// Libraries used to control the LCD Display
#include <Wire.h>

// Instantiating a "LCD" object
rgb_lcd lcd;

const unsigned char ledPin = 4; // led pin
const unsigned char buttonPin = 2; // button pin
bool wasButtonPreviouslyPressed; // keep status of button at previous cycle
bool isLEDOn; // keeps status of LED light
const long debounceInterval = 200;
long pressedTime; // keeps when is the last time button has been pressed
long sensorReadTime; // keeps when is the last time sensor has been read
const long clearLCDInterval = 1000; // interval before LCD screen clears itself
const long sensorReadingInterval = 200;
long timeSinceLastPressToPrint = 0;

void toggleLed()
{
  long currentTime = millis();
  long timeSinceLastPress = currentTime - pressedTime;
  
  if (timeSinceLastPress > debounceInterval)
  {
    //if LED is currently on, it is going to be turned off
    //I will send how long it has been on for
    if (isLEDOn)
    {
      timeSinceLastPressToPrint = timeSinceLastPress;
    }
    
    isLEDOn = !isLEDOn; //toggle light
    pressedTime = currentTime; 
  }

  digitalWrite(ledPin, isLEDOn);  
  return;
}

// the setup function runs once when you press reset or power the board
void setup() {
  // put your setup code here, to run once:
  Serial.begin( 9600 );  // (9600 is the baudrate).
  
  // initialize digital pin 4 as an output.
  pinMode(ledPin, OUTPUT);
  // initialize digital pin 2 as an input.
  pinMode(buttonPin, INPUT);
  // initialise button status
  wasButtonPreviouslyPressed = digitalRead(buttonPin);
  pressedTime = millis();
  sensorReadTime = pressedTime;

  // Initialize the lcd, 2 rows of 16 chars
  lcd.begin(16, 2);
  // Set the background LED color
  lcd.setRGB(0, 255, 255);
  lcd.clear();

  attachInterrupt( digitalPinToInterrupt( buttonPin ), toggleLed, RISING );
}

// the loop function runs over and over again forever
void loop() {

  //printing the last time it was pressed if non-null
  if (timeSinceLastPressToPrint > 0)
  {
    lcd.setCursor(0, 0);
    lcd.print("On for: "); 
    lcd.print(timeSinceLastPressToPrint);
    lcd.print(" ms         "); // spacing is for clearing line
    Serial.println("Light on for:");
    Serial.println(timeSinceLastPressToPrint);
  }

  long currentTime = millis();
  //printing and sending sensor (light) reading every 200ms
  if (currentTime - sensorReadTime > sensorReadingInterval) 
  {
    unsigned int lightIntensity = analogRead(0);
    lcd.setCursor(0, 1);
    lcd.print("Sensor: ");
    lcd.print(lightIntensity);
    lcd.print("       "); //spacing is for clearing line
    Serial.println("Light intensity:");
    Serial.println(lightIntensity);
    sensorReadTime = currentTime;
  }
}
