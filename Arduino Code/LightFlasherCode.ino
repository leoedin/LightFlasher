/* This file runs the light flasher board.
Note: At the time of writing 2 out of 6 outputs are broken.
tr0 (top right terminal) is broken because the transistor may be bad
tr2 (3rd terminal down) is dead because pin 15 has fallen off the uC.
TODO: Fix pin (by either soldering something on or replacing uC)
Fix transistor
*/

/* File by Leo Rampen
Some rights reservered (but I won't say what).
Influenced by: http://nuewire.com/info-archive/msgeq7-by-j-skoba/
*/

int analogPin = 0; // read from multiplexer using analog input 0
int strobePin = 2; // strobe is attached to digital pin 2
int resetPin = 4; // reset is attached to digital pin 3
int spectrumValue[7]; // to hold a2d values

int tr0pin = 11;
int tr1pin = 10;
int tr2pin = 9; //dead
int tr3pin = 6;
int tr4pin = 5;
int tr5pin = 3;

//define a counter for settings
int settingCounter = 0;

//define switch
int switchPin = 7;
boolean switchPinPrevious = false;

//define LED
int ledPin = 13;

void setup()
{
   //define equaliser pins
   pinMode(analogPin, INPUT);
   pinMode(strobePin, OUTPUT);
   pinMode(resetPin, OUTPUT);
   
   
   //define LED
   pinMode(ledPin, OUTPUT);
   digitalWrite(ledPin, LOW);
   delay(500);
   digitalWrite(ledPin, HIGH);
   delay(500);
   digitalWrite(ledPin, LOW);
   
   //define transistor pins
   pinMode(tr0pin, OUTPUT);
   pinMode(tr1pin, OUTPUT);
   pinMode(tr2pin, OUTPUT);
   pinMode(tr3pin, OUTPUT);
   pinMode(tr4pin, OUTPUT);
   pinMode(tr5pin, OUTPUT);
   
   //set analog reference
   analogReference(DEFAULT);
   
   //setup switch interrupts
   pinMode(switchPin, INPUT);
   digitalWrite(switchPin,HIGH);
   
   //reset the equaliser
   digitalWrite(resetPin, LOW);
   digitalWrite(strobePin, HIGH);

}

void loop()
{
  //reset the equaliser
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);
   
  //This deals with the switch for settings. It increments the setting counter and blinks the LED on switch press.
  //Didn't use interrupts because the pins were already used.
  if (digitalRead(switchPin) == LOW) {
    if (switchPinPrevious == false) {
      if (settingCounter <7) settingCounter++;
      else settingCounter = 0;
      
      //blink once
      digitalWrite(ledPin, HIGH);
      delay(350);
      digitalWrite(ledPin,LOW);
      delay(350);
      switchPinPrevious = true;
    }
   } else if (switchPinPrevious == true) {
     switchPinPrevious = false;
   }
   
   //The different flashing modes.
   switch (settingCounter) {
      //Initial modes are just for dimming. The numbers are a bit arbitrary.
      case 1:       
         //dim mode 1
         //30 for ~6V to the transistor
         analogWrite(tr0pin, 100);
         analogWrite(tr1pin, 100);
         analogWrite(tr2pin, 100);
         analogWrite(tr3pin, 100);
         analogWrite(tr4pin, 100);
         analogWrite(tr5pin, 100);
         break;
       
       case 2:
         //dim mode 2
         analogWrite(tr0pin, 60);
         analogWrite(tr1pin, 60);
         analogWrite(tr2pin, 60);
         analogWrite(tr3pin, 60);
         analogWrite(tr4pin, 60);
         analogWrite(tr5pin, 60);
         break;
         
       case 3:
         //dim mode 3
         analogWrite(tr0pin, 15);
         analogWrite(tr1pin, 15);
         analogWrite(tr2pin, 15);
         analogWrite(tr3pin, 15);
         analogWrite(tr4pin, 15);
         analogWrite(tr5pin, 15);
         break;
     
     //Flash modes turn the light on or off depending on whether a threshold value has been reached. Ordered in terms of threshold size.
      case 4: //Flash mode 1

              
         //get equaliser values
         for (int i = 0; i < 7; i++)
         {
           digitalWrite(strobePin, LOW);
           delayMicroseconds(30); // to allow the output to settle
           spectrumValue[i] = analogRead(analogPin);
          
           digitalWrite(strobePin, HIGH);
         }
         
         //need to find a way to filter this so that it yields better results
         //I've set everything but 3 to 400 as 500 seemed a little too thresholdy.
         if (spectrumValue[0] > 300) analogWrite(tr0pin, 255);
         else analogWrite(tr0pin, 0);
         if (spectrumValue[0] > 300) analogWrite(tr1pin, 255);
         else analogWrite(tr1pin, 0);
         if (spectrumValue[1] > 300) analogWrite(tr2pin, 255);
         else analogWrite(tr2pin, 0);
         if (spectrumValue[1] > 300) analogWrite(tr3pin, 255);
         else analogWrite(tr3pin, 0);
         if (spectrumValue[3] > 400) analogWrite(tr4pin, 255);
         else analogWrite(tr4pin, 0);
         if (spectrumValue[5] > 300) analogWrite(tr5pin, 255);
         else analogWrite(tr5pin, 0);
         break;
         
      case 5: //Flash mode 2
       // This flash mode is the on-off one. It sets a threshold and then either flashes the light on or off depending on reading.
              
         //get equaliser values
         for (int i = 0; i < 7; i++)
         {
           digitalWrite(strobePin, LOW);
           delayMicroseconds(30); // to allow the output to settle
           spectrumValue[i] = analogRead(analogPin);
          
           digitalWrite(strobePin, HIGH);
         }
         
         //need to find a way to filter this so that it yields better results
         //I've set everything but 3 to 400 as 500 seemed a little too thresholdy.
         if (spectrumValue[0] > 400) analogWrite(tr0pin, 255);
         else analogWrite(tr0pin, 0);
         if (spectrumValue[0] > 400) analogWrite(tr1pin, 255);
         else analogWrite(tr1pin, 0);
         if (spectrumValue[1] > 400) analogWrite(tr2pin, 255);
         else analogWrite(tr2pin, 0);
         if (spectrumValue[1] > 400) analogWrite(tr3pin, 255);
         else analogWrite(tr3pin, 0);
         if (spectrumValue[3] > 500) analogWrite(tr4pin, 255);
         else analogWrite(tr4pin, 0);
         if (spectrumValue[5] > 400) analogWrite(tr5pin, 255);
         else analogWrite(tr5pin, 0);
         break;
         
      case 6: //Flash mode 3
          //For late night darkness...
              
         //get equaliser values
         for (int i = 0; i < 7; i++)
         {
           digitalWrite(strobePin, LOW);
           delayMicroseconds(30); // to allow the output to settle
           spectrumValue[i] = analogRead(analogPin);
          
           digitalWrite(strobePin, HIGH);
         }
         
         //need to find a way to filter this so that it yields better results
         //I've set everything but 3 to 400 as 500 seemed a little too thresholdy.
         if (spectrumValue[0] > 600) analogWrite(tr0pin, 255);
         else analogWrite(tr0pin, 0);
         if (spectrumValue[0] > 600) analogWrite(tr1pin, 255);
         else analogWrite(tr1pin, 0);
         if (spectrumValue[1] > 600) analogWrite(tr2pin, 255);
         else analogWrite(tr2pin, 0);
         if (spectrumValue[1] > 600) analogWrite(tr3pin, 255);
         else analogWrite(tr3pin, 0);
         if (spectrumValue[3] > 650) analogWrite(tr4pin, 255);
         else analogWrite(tr4pin, 0);
         if (spectrumValue[5] > 600) analogWrite(tr5pin, 255);
         else analogWrite(tr5pin, 0);
         break;
         
      case 7: //Flash mode 4
          //Plain old strobe light
              
       analogWrite(tr0pin, 255);
       analogWrite(tr1pin, 255);
       analogWrite(tr2pin, 255);
       analogWrite(tr3pin, 255);
       analogWrite(tr4pin, 255);
       analogWrite(tr5pin, 255);
       delay(8);
       analogWrite(tr0pin, 0);
       analogWrite(tr1pin, 0);
       analogWrite(tr2pin, 0);
       analogWrite(tr3pin, 0);
       analogWrite(tr4pin, 0);
       analogWrite(tr5pin, 0);
       delay(150);
       break;
         
      default: //Just switch on the lights

       //255 for 5V to the transistor
       analogWrite(tr0pin, 255);
       analogWrite(tr1pin, 255);
       analogWrite(tr2pin, 255);
       analogWrite(tr3pin, 255);
       analogWrite(tr4pin, 255);
       analogWrite(tr5pin, 255);
       break;
   }
}
