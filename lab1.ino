/*
8 LEDs - Modified by Andy/Eric
This code is written to make 8 LEDs light on and off one
after another in a continuous loop using 
variables, functions, arrays, and for loops.
*/

int delayTime[] = {500,400,300,200,100,100,200,300,400,500}; // delay time variable
int ledPins[] = {1,5,2,6,3,7,4,8}; // led pattern
 

void setup()
{
  // set all LED pins as outputs.
  for (int i = 0; i <= 7; i++)
  {  
    pinMode(ledPins[i], OUTPUT);
  }
}
void loop()
{
  // One for loop to iterate through the delay time list, changing the overall time for each set
  for (int m = 0; m <= 9; m++)
  {
    // for loop to iterate through the ledPin[] array left to right
    for (int i = 0; i <= 7; i++)
    {
      digitalWrite(ledPins[i], HIGH);
      delay(delayTime[m]); 
    }
    // for loop to iterate through the ledPin[] array backwards(right to left)
    for (int i = 7; i >= 0; i--)
    {
      digitalWrite(ledPins[i], LOW);
      delay(delayTime[m]);
    }
  }
}

