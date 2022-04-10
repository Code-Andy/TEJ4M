/* 
Pushbuttons - Written by Eric & Andy
This code is written to display the state of a pushbutton
*/

#include <LiquidCrystal.h> 

LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // initialize the LCD with pin numbers

int button1Pin = 8; //set pin where button 1 is wired
int button1State; //create variable for the state of button 1
int button2Pin = 9; //set pin where button 1 is wired
int button2State; //create variable for the state of button 1

void setup() 
{
  lcd.begin(16, 2); // set up the LCD's number of columns and rows
}

void loop() 
{
  button1State = digitalRead(button1Pin); //read state of pushbutton
  button2State = digitalRead(button2Pin); //read state of pushbutton

  if (button1State == LOW && button2State == LOW) //both buttons are pressed
  {
    lcd.setCursor(0, 0);
    lcd.print("Both Pressed   ");
    lcd.setCursor(0, 1);
    lcd.print("               ");
  }
  
  else if (button1State == LOW) //button is pressed
  {
    lcd.setCursor(0, 0);
    lcd.print("PB1 Pressed    ");
    lcd.setCursor(0, 1);
    lcd.print("PB2 not Pressed    ");
  }
  
  else if (button2State == LOW) //button is pressed
  {
    lcd.setCursor(0, 0);
    lcd.print("PB1 not Pressed    ");
    lcd.setCursor(0, 1);
    lcd.print("PB2 Pressed    ");
  }
  
  else //button is not pressed
  {
    lcd.setCursor(0, 0);
    lcd.print("PB1 not Pressed");
    lcd.setCursor(0, 1);
    lcd.print("PB2 not Pressed");
  }
  
  delay(100); //delay to allow screen to update
}

