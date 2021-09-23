// #include < Wire .h> we are removing this because it is already added in liquid crystal library
#include <LiquidCrystal_I2C.h>
#include "HX711.h"

// Create the lcd object address 0x3F and 16 columns x 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2); //

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 4;
const int LOADCELL_SCK_PIN = 2;

HX711 scale;
long reading = 0;
long lastReading = 0;

int counter = 0;

const int numReadings = 10;
int readings[numReadings];
int total = 0;
int average = 0;
int readIndex = 0;
bool messageFinished = false;

void animate();
void calcAverage();
void displayMessage();

void setup()
{
  Serial.begin(115200);
  // Initialize the LCD connected
  lcd.init();

  // Turn on the backlight on LCD.
  lcd.backlight();

  // print the Message on the LCD.
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  for (int thisReading = 0; thisReading < numReadings; thisReading++)
  {
    readings[thisReading] = 0;
  }
}

void loop()
{
  calcAverage();
  if (reading < 120000)
  {
    messageFinished = false;
  }
  if (reading < 120000 || messageFinished || average > 100)
  {
    animate();
  }
  else if (average < 100)
  {
    displayMessage();
  }
  delay(250);
  counter++;
  Serial.println(average);
}

void animate()
{
  if (counter % 4 == 0)
  {
    lcd.setCursor(0, 0);
    lcd.print("------------------------");
    lcd.setCursor(0, 1);
    lcd.print("------------------------");
  }
  else if (counter % 4 == 1)
  {
    lcd.setCursor(0, 0);
    lcd.print("|||||||||||||||||||||");
    lcd.setCursor(0, 1);
    lcd.print("|||||||||||||||||||||");
  }
  else if (counter % 4 == 2)
  {
    lcd.setCursor(0, 0);
    lcd.print("************************");
    lcd.setCursor(0, 1);
    lcd.print("************************");
  }
}

void calcAverage()
{
  if (scale.is_ready())
  {
    reading = scale.read();
    // Serial.print("HX711 reading: ");
    // Serial.println(reading);
  }
  else
  {
    Serial.println("HX711 not found.");
  }

  total = total - readings[readIndex];
  readings[readIndex] = abs(reading - lastReading);
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  if (readIndex >= numReadings)
  {
    readIndex = 0;
  }
  average = total / numReadings;
  lastReading = reading;
  // Serial.print(reading);
  // Serial.print(", ");
  // Serial.println(average);
}

void displayMessage()
{
  int value = random(10);
  if (value = 0)
  {
    lcd.setCursor(0, 0);
    lcd.print("You're doing             ");
    lcd.setCursor(0, 1);
    lcd.print("okay!                    ");
  }

  delay(5000);
  messageFinished = true;
}