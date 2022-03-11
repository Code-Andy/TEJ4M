//Washing Machine written by Andy Zhang Jan 9th 2022
 
#include <LiquidCrystal.h> //Utilizes the LCD library and Servo Library
#include <Servo.h>
 
const int rs = A0, en = A1, d4 = A2, d5 = A3, d6 = A4, d7 = A5; //LCD display pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); //Initialize a LCD object
Servo myservo; //Initialize a servo object
 
//Pinouts for LEDs, Buttons, Data pins, etc.
const int BUZZER_PIN = 12;
const int READY_LED_PIN = 7;
const int WASH_LED_PIN = 8;
const int RINSE_LED_PIN = 9;
const int DONE_LED_PIN = 10;
const int HEAVY_DUTY_PIN = 11;
const int doorSwitch_PIN = 6;
const int stopPB_PIN = 5;
const int rinsePB_PIN = 4;
const int washPB_PIN = 3;
const int heavyPB_PIN = 2;
const int soap_pump = 1;
const int soap_valve = 0;
 
//Music list for notes and note times
const int music_fre[] = {277, 740, 698, 622, 554, 466, 494, 554, 622, 415, 466, 494, 466, 554, 554, 740, 698, 622, 554, 740, 740, 830, 740, 698, 622, 698, 740, 0};
const float music_delay[] = {1.5, 0.5, 0.5, 0.5, 1.5, 1.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.5, 1.5, 1.5, 0.5, 0.5, 0.5, 1.5, 1.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 2, 1};
boolean music_played = false; //Boolean variable to store whether the music has been played
 
//Boolean vars of all the switches
bool doorState = digitalRead(doorSwitch_PIN);
bool stopPBState = digitalRead(stopPB_PIN);
bool rinsePBState = digitalRead(rinsePB_PIN);
bool washPBState = digitalRead(washPB_PIN);
bool heavyPBState = digitalRead(heavyPB_PIN);
 
int pos = 0; //Servo Position in degrees
int mode = 0; //Switch case value
 
void setup() {
  //Setup LCD screen size & and attach servo object to correct pin
  lcd.begin(16, 2);
  myservo.attach(soap_valve);
 
  //Set all LEDs as output pins
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(READY_LED_PIN, OUTPUT);
  pinMode(WASH_LED_PIN, OUTPUT);
  pinMode(RINSE_LED_PIN, OUTPUT);
  pinMode(DONE_LED_PIN, OUTPUT);
  pinMode(HEAVY_DUTY_PIN, OUTPUT);
  pinMode(soap_pump, OUTPUT);
  //Set all switches and buttons as inputs
  pinMode(doorSwitch_PIN, INPUT);
  pinMode(stopPB_PIN, INPUT);
  pinMode(rinsePB_PIN, INPUT);
  pinMode(washPB_PIN, INPUT);
  pinMode(heavyPB_PIN, INPUT);
}
 
void loop() {
  //read button states
  doorState = digitalRead(doorSwitch_PIN);
  stopPBState = digitalRead(stopPB_PIN);
  rinsePBState = digitalRead(rinsePB_PIN);
  washPBState = digitalRead(washPB_PIN);
  heavyPBState = digitalRead(heavyPB_PIN);
 
  switch (mode) {
    case 0: //standby
  	//Turn all LEDs off
  	digitalWrite(READY_LED_PIN, LOW);
      digitalWrite(WASH_LED_PIN, LOW);
      digitalWrite(RINSE_LED_PIN, LOW);
      digitalWrite(DONE_LED_PIN, LOW);
      digitalWrite(HEAVY_DUTY_PIN, LOW);
      if (doorState == LOW) //If Door is closed move to ready state
  	{
    	mode = 1;
      }
      if (rinsePBState == LOW && doorState == LOW) //If the rinse button is pressed and the door is closed begin rinse state
  	{
    	mode = 3;
    	//Play a short tone
    	tone(BUZZER_PIN, 800);
    	delay(125);
    	noTone(BUZZER_PIN);
      }
      if (washPBState == LOW && doorState == LOW) //If the wash button is pressed and the door is closed begin wash state
  	{
    	mode = 2;
    	//Play a short tone
    	tone(BUZZER_PIN, 800);
    	delay(125);
    	noTone(BUZZER_PIN);
      }
      if (heavyPBState == LOW && doorState == LOW)//If the heavyduty button is pressed and the door is closed begin heavyduty state
  	{
    	mode = 5;
    	//Play a short tone
    	tone(BUZZER_PIN, 800);
    	delay(125);
    	noTone(BUZZER_PIN);
      }
      break;
    case 1: //ready LED
  	//Prints on the first line "READY" and turns all LEDs off except the ready LED
  	lcd.print("READY");
      digitalWrite(READY_LED_PIN, HIGH);
      digitalWrite(WASH_LED_PIN, LOW);
      digitalWrite(RINSE_LED_PIN, LOW);
      digitalWrite(DONE_LED_PIN, LOW);
      mode = 0; //Loops back to mode 0 to check for button presses
  	delay(125); //Delay for button debounce
  	lcd.clear();
      break;
    case 2: //Wash LED
  	//Prints on the first line "WASH" and turns all LEDs off except the Wash LED
  	lcd.print("WASH");
      music_played = false; //Resets music played counter
  	dispenseSoap(); //Dispense Soap
  	digitalWrite(READY_LED_PIN, LOW);
      digitalWrite(WASH_LED_PIN, HIGH);
      washDelay(8000, "WASH"); //Waits 8 seconds whilst checking for the stop button or open door
  	//Prints on the first line "RINSE" and turns all LEDs off except the RINSE LED
  	lcd.print("RINSE");
      digitalWrite(RINSE_LED_PIN, HIGH);
      digitalWrite(WASH_LED_PIN, LOW);
      washDelay(3000, "RINSE"); //Waits 3 seconds whilst checking for the stop button or open door
  	delay(125); //Delay for button debounce
  	lcd.clear();
      mode = 4; //Moves to "done" state
  	break;
    case 3: //Rinse LED
  	//Prints on the first line "RINSE" and turns all LEDs off except the RINSE LED
  	lcd.print("RINSE");
      digitalWrite(READY_LED_PIN, LOW);
      digitalWrite(WASH_LED_PIN, LOW);
      digitalWrite(RINSE_LED_PIN, HIGH);
      digitalWrite(DONE_LED_PIN, LOW);
      washDelay(3000, "RINSE"); //Waits 3 seconds whilst checking for the stop button or open door
  	lcd.clear();
      mode = 4; //Moves to "done" state
  	music_played = false; //Resets music played counter
  	break;
    case 4: //Done LED
  	//Prints on the first line "DONE" and turns all LEDs off except the DONE LED
  	lcd.print("DONE");
      digitalWrite(READY_LED_PIN, LOW);
      digitalWrite(WASH_LED_PIN, LOW);
      digitalWrite(RINSE_LED_PIN, LOW);
      digitalWrite(DONE_LED_PIN, HIGH);
      if (music_played == false) //If music has not been played, play music once
  	{
    	music();
    	music_played = true;
    	delay(100); //Debounce
  	}
      // Check door/stop button state
  	doorState = digitalRead(doorSwitch_PIN);
      stopPBState = digitalRead(stopPB_PIN);
      if ((doorState == HIGH) || (stopPBState == LOW)) // If door is open or stop is pressed go to standby mode
  	{
    	tone(BUZZER_PIN, 800); //Short button tone
    	delay(125);
    	lcd.clear();
    	noTone(BUZZER_PIN);
    	mode = 0; //Reset to standby mode
    	break;
      }
      else {
    	//loop itself with a debounce delay
    	delay(125);
        lcd.clear();
    	mode = 4;
    	break;
      }
    case 5: //Heavyduty LED
  	//Prints on the first line "HEAVYDUTY" and turns all LEDs off except the HEAVYDUTY LED
  	dispenseSoap(); //Dispense soap
  	lcd.print("HEAVYDUTY");
      digitalWrite(READY_LED_PIN, LOW);
      digitalWrite(HEAVY_DUTY_PIN, HIGH);
      washDelay(10000, "HEAVYDUTY"); //Waits 10 seconds whilst checking for the stop button or open door
  	lcd.print("RINSE");
      digitalWrite(HEAVY_DUTY_PIN, LOW);
      digitalWrite(RINSE_LED_PIN, HIGH);
      washDelay(3000, "RINSE"); //Waits 3 seconds whilst checking for the stop button or open door
  	lcd.clear();
      mode = 4; //Moves to "done" state
  	break;
  }
}
 
void music() {
  //Plays a tone 28 times depending on frequency and time from the 2 lists
  for (int i = 0; i <= 27; i++) {
    if (music_fre[i] != 0) {
      tone(BUZZER_PIN, music_fre[i]);
      delay(music_delay[i] * 400);
    }
    else {
      noTone(BUZZER_PIN);
      delay(music_delay[i] * 400);
    }
    // Check door/stop button state
    stopPBState = digitalRead(stopPB_PIN);
    doorState = digitalRead(doorSwitch_PIN);
    if (stopPBState == LOW || doorState == HIGH) //If door is open or stop is pressed stop the music
    {
      mode = 4;
      music_played = true;
      noTone(BUZZER_PIN);
      break;
    }
    noTone(BUZZER_PIN);
    delay(15);
  }
}
 
void washDelay(int delayTime, String washType) {
  lcd.clear();
  int counter = 0;
  // Print which Wash it is
  int timeleft = delayTime / 1000; //Milliseconds to seconds conversion
  lcd.print(washType);
  //Loops a simple check button state and delay of 100ms to add up to total delay needed
  for (int i = 0; i < delayTime / 100; i++) {
    // Check door/stop button state
    stopPBState = digitalRead(stopPB_PIN);
    doorState = digitalRead(doorSwitch_PIN);
    if (stopPBState == LOW || doorState == HIGH) //If door is open or stop is pressed break out of the delay
    {
      mode = 4;
      music_played = true;
      delay(100);
      break;
    }
    //Print on the second line
    lcd.setCursor(0, 1);
    if (counter % 5 == 0) //Print the timeleft as the initial time - a counter counting every 0.5 seconds
    {
      lcd.print("Time left " + String(timeleft - (counter * 0.1)));
    }
    delay(100);
    counter++;
  }
}
 
void dispenseSoap() {
  //Turn on motor for 2 seconds
  digitalWrite(soap_pump, HIGH);
  delay(2000);
  digitalWrite(soap_pump, LOW);
  //Open gate and wait half a second
  for (pos = 90; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(15);
  }
  delay(500);
  //Closed gate and wait half a second
  for (pos = 0; pos <= 90; pos += 1) {
    myservo.write(pos);
    delay(15);
  }
}