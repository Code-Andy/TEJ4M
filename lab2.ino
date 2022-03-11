/* US Sensor - Written by Mr.K 2021.10.16
   This code is written to sense distance and sound a buzzer
   "tone" commands */

// declare variables for pins
int GREEN_LED_PIN = 4;
int RED_LED_PIN = 5;
int BUZZER_PIN = 6;
int TRIGGER_PIN = 3;
int ECHO_PIN = 2;

float duration;
float distance;

//MUSIC VALUES, every index is the frequency one note or rest
int MUSIC_FRE[] = {740, 0, 740, 740, 740, 659, 740, 0, 740, 0, 740, 740, 740 , 659, 740, 740, 0, 880, 740, 0, 659, 587, 0, 494, 494, 554, 587, 494, 740};
//NOTE LENGTH, Length of note, 1 * 430ms is one quarter note
float MUSIC_DELAY[] = {1, 1, 0.5, 0.25, 0.25, 0.25, 0.25, 0.5, 1, 1, 0.5, 0.25, 0.25, 0.25, 0.375, 1, 0.5, 1, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1};

//Frequency Table
//F# = 740
//A = 880
//E = 659
//D = 587
//B = 494
//C# = 554

void setup()
{
  // set pins 3 to 6 outputs
  for (int i = 3; i <= 6; i++)
  {
    pinMode(i, OUTPUT);
  }
  pinMode(2, INPUT); // set pin 2 as input
}

void loop()
{
  /************ Start US Measurement Section ***********/
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;
  /************** End US Measurement Section ***********/

  // if distance is less than 50cm
  if (distance < 50)
  {
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);

    for (int i = 0; i <= 27; i++) {
    //For loop to iterate through the 2 arrays, playing the correct note and length of note
      if (MUSIC_FRE[i] != 0) {
        tone(BUZZER_PIN, MUSIC_FRE[i]);
        delay(MUSIC_DELAY[i] * 430);
      }
      else {
        noTone(BUZZER_PIN);
        delay(MUSIC_DELAY[i] * 430);
      }
      //No tone for short period to act like articulation
      noTone(BUZZER_PIN);
      delay(15);
    }
  }

  //Frequency Slider, Low to high based on distance
  else if ((distance >= 50) && (distance <= 110))
  {
    tone(BUZZER_PIN, 22000 / distance);
  }
  
  // distance is equal to or greater than 50cm
  else
  {
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(RED_LED_PIN, LOW);
    noTone(BUZZER_PIN);
  }
  delay(10); // small delay to save system resources
}
