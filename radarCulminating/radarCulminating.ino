#include <Stepper.h>
#include "LedControl.h"
 
const int DIN = 4; // Led 8x8 Matrix Pins
const int CLK = 3;
const int CS = 2;
const int numOfDisplays = 2; // Number of Displays
 
LedControl lc = LedControl(DIN, CLK, CS, numOfDisplays); // Initialize LC object
 
const int pingPin = 7;       // Trigger Pin of Ultrasonic Sensor
const int echoPin = 6;       // Echo Pin of Ultrasonic Sensor
const float degree = 2.8125; // Degree per step of motor
long duration, cm;           // Ultrasonic sensor variables
 
const double stepsPerRevolution = 2048;              // Number of steps for one full revolution
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11); // Pin inversion to make the library work
 
const int LedPin = 5; // Led Pin(Analog)
 
// 2D arrays to hold positions of the radar animation
int radarScreen[8][15] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};
 
// 2D arrays to hold positions of the radar detection
int detection[8][15] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};
 
void setup()
{
  //LCD setup
  Serial.begin(9600);
  lc.shutdown(0, false); // Starts the matrices
  lc.shutdown(1, false);
  lc.setIntensity(0, 1); // Set Matrix Brightness
  lc.setIntensity(1, 1);
  lc.clearDisplay(0); // Clears the display once setup is done
  lc.clearDisplay(1);
 
  // Motor + Sensor
  myStepper.setSpeed(2);    // Set speed to 2 steps per "step"
  pinMode(pingPin, OUTPUT); // Ultrasonic Sensor pins
  pinMode(echoPin, INPUT);
 
  pinMode(LedPin, OUTPUT);
}
 
void loop()
{
  // Scan 1 animation
  for (int x = 0; x <= 15; x++)
  {
    // Draws a line from left to right(180 degrees to 0)
    drawRadar(200, x * 12);
    draw(radarScreen);
    delay(15);
    clearScreen(radarScreen);
    draw(radarScreen);
  }
  // Scan 1
  for (int x = 0; x <= 64; x++)
  {
    // Turn the stepper motor 64 times, scanning each time
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pingPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    cm = microsecondsToCentimeters(duration);
    float angle = degree * x;
    if (cm <= 200)
    {
      // If the distance is less than 200cm, send the distance and angle to drawdistance()
      analogWrite(LedPin, 10 + cm * 1.2); // Led brightness based on how far objects are until 200cm
      Serial.print(cm);
      Serial.print("cm  ");
      Serial.print(angle);
      Serial.print("degrees");
      Serial.println();
      drawDistance(cm, angle);
    }
    else if (cm > 200)
    {
      // If over 200, print out of range and continue with the loop
      digitalWrite(LedPin, LOW);
      Serial.println("OUT OF RANGE");
    }
    draw(detection);                              // Update the screen with new points
    myStepper.step(-stepsPerRevolution / 64 / 2); // Move the motor halfway with 64 increments
  }
  clearScreen(detection); // Clears the screen
 
  // Scan 2 animation
  for (int x = 15; x >= 0; x--)
  {
    //draws a line from left to right(0 degrees to 180)
    drawRadar(200, x * 12);
    draw(radarScreen);
    delay(15);
    clearScreen(radarScreen);
    draw(radarScreen);
  }
 
  // Scan 2
  for (int x = 64; x >= 0; x--)
  {
    // Turn the stepper motor 64 times, scanning each time
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(pingPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    cm = microsecondsToCentimeters(duration);
    int angle = degree * x;
    if (cm <= 200)
    {
      // If the distance is less than 200cm, send the distance and angle to drawDistance()
      analogWrite(LedPin, 10 + cm * 1.2); // Led brightness based on how far objects are until 200cm
      Serial.print(cm);
      Serial.print("cm  ");
      Serial.print(angle);
      Serial.print("degrees");
      Serial.println();
      drawDistance(cm, angle);
    }
    else if (cm > 200)
    {
      // If over 200, print out of range and continue with the loop
      digitalWrite(LedPin, LOW);
      Serial.println("OUT OF RANGE");
    }
    draw(detection);                              // Update the screen with new points
    myStepper.step(+stepsPerRevolution / 64 / 2); // Move the motor halfway with 64 increments
  }
  clearScreen(detection);
}
 
// Using trig and the distance and angle, we can map the Line of Sight from the US sensor
// This draws the initial animation at the beginning
void drawRadar(int dist, float angle)
{
  float adjacent = dist * cos(angle * 1000 / 57296);
  int ledDist = abs(adjacent / 25);
  if (ledDist == 0)
  {
    for (int vert = 0; vert < dist / 25; vert++)
    {
      addToScreen(0, vert, true, angle, radarScreen);
    }
  }
  for (int x = 0; x < ledDist; x++)
  {
    for (int check = 0; check < 10; check++)
    {
      float mapVal = ((x * 25) + (check * 2.5)) * (tan(angle * 1000 / 57296));
      int ledVal = map(mapVal, 0, 200, 0, 8);
      addToScreen(x, abs(ledVal), true, angle, radarScreen);
    }
  }
  if (angle == 180)
  {
    radarScreen[7][0] = 1;
  }
}
 
// Similar to drawRadar() but only adds one point(furthest detected) instead of a line from center or zero
void drawDistance(int dist, float angle)
{
  float adjacent = dist * cos(angle * 1000 / 57296);
  int ledX = abs(adjacent / 25);
  if (ledX == 0)
  {
    addToScreen(0, (dist / 25) - 1, true, angle, detection);
  }
  float mapVal = (adjacent) * (tan(angle * 1000 / 57296));
  int ledVal = map(mapVal, 0, 200, 0, 8);
  addToScreen(ledX - 1, abs(ledVal), true, angle, detection);
  if (angle == 180)
  {
    addToScreen(ledX, 0, true, angle, detection);
  }
}
 
// Takes in an x,y value and adds it to the 2d array according to true or false it turns it on/off(1/0)
void addToScreen(int x, int y, boolean state, float angle, int screen[8][15])
{
  int newX = 0;
  if (angle < 90)
  {
    newX = x + 7;
  }
  else
  {
    newX = abs(x - 7);
  }
  if (state == true)
  {
    screen[abs(y - 7)][newX] = 1;
  }
  else
  {
    screen[abs(y - 7)][newX] = 0;
  }
}
 
// Iterates through any 2d array and sets all values to zero and then turns all pixels off
void clearScreen(int screen[8][15])
{
  for (int y = 0; y < 8; y++)
  {
    for (int x = 0; x < 15; x++)
    {
      screen[y][x] = 0;
      if (x < 8)
      {
        lc.setLed(1, x, abs(y - 7), false);
      }
      else
      {
        lc.setLed(0, x - 8, abs(y - 7), false);
      }
    }
  }
}
 
// Iterate through the 2D array and turns on all Leds with 1 in the array on the matrix
void draw(int screen[8][15])
{
  for (int y = 0; y < 8; y++)
  {
    for (int x = 0; x < 15; x++)
    {
      if (screen[y][x] == 1)
      {
        if (x < 8)
        {
          lc.setLed(1, x, abs(y - 7), true);
        }
        else
        {
          lc.setLed(0, x - 8, abs(y - 7), true);
        }
      }
    }
  }
}
 
// Simple conversion from microSeconds to cm
long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
 
