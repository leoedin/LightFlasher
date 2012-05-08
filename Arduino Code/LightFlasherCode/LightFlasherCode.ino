/* This file runs the light flasher board.
Note: At the time of writing 1 out of 6 outputs are broken.
TODO: Fix pin (by either soldering something on or replacing uC)
*/

/* File by Leo Rampen
Feel free to draw from this with your own projects. Let me know what you produce! The MSGEQ7 code is originally by J Skoba. The rest is written by me, but can be considered public domain.
MSGEQ7 reading code from: http://nuewire.com/info-archive/msgeq7-by-j-skoba/
*/

int analog_pin = 0; // read from multiplexer using analog input 0
int strobe_pin = 2; // strobe is attached to digital pin 2
int reset_pin = 4; // reset is attached to digital pin 3
int spectrum_value[7]; // to hold a2d values

int tr0_pin = 11;
int tr1_pin = 10;
int tr2_pin = 9; //dead
int tr3_pin = 6;
int tr4_pin = 5;
int tr5_pin = 3;

//define a counter for settings
int setting_counter = 0;

//define switch
int switch_pin = 7;
boolean switch_pin_pre = false;

//define LED
int led_pin = 13;

void setup()
{
   //define equaliser pins
   pinMode(analog_pin, INPUT);
   pinMode(strobe_pin, OUTPUT);
   pinMode(reset_pin, OUTPUT);
   
   
   //define LED
   pinMode(led_pin, OUTPUT);
   digitalWrite(led_pin, LOW);
   delay(500);
   digitalWrite(led_pin, HIGH);
   delay(500);
   digitalWrite(led_pin, LOW);
   
   //define transistor pins
   pinMode(tr0_pin, OUTPUT);
   pinMode(tr1_pin, OUTPUT);
   pinMode(tr2_pin, OUTPUT);
   pinMode(tr3_pin, OUTPUT);
   pinMode(tr4_pin, OUTPUT);
   pinMode(tr5_pin, OUTPUT);
   
   //set analog reference
   analogReference(DEFAULT);
   
   //setup switch interrupts
   pinMode(switch_pin, INPUT);
   digitalWrite(switch_pin,HIGH);
   
   //reset the equaliser
   digitalWrite(reset_pin, LOW);
   digitalWrite(strobe_pin, HIGH);

}

void loop()
{
  //reset the equaliser
  digitalWrite(reset_pin, HIGH);
  digitalWrite(reset_pin, LOW);
   
  //This deals with the switch for settings. It increments the setting counter and blinks the LED on switch press.
  if (digitalRead(switch_pin) == LOW) {
    if (switch_pin_pre == false) {
      if (setting_counter <7) setting_counter++;
      else setting_counter = 0;
      
      //blink once
      digitalWrite(led_pin, HIGH);
      delay(350);
      digitalWrite(led_pin,LOW);
      delay(350);
      switch_pin_pre = true;
    }
   } else if (switch_pin_pre == true) {
     switch_pin_pre = false;
   }
   
   //The different flashing modes.
   switch (setting_counter) {
      //Initial modes are just for dimming. The numbers are a bit arbitrary.
      case 1:       
         //dim mode 1
         //30 for ~6V to the transistor
         analogWrite(tr0_pin, 100);
         analogWrite(tr1_pin, 100);
         analogWrite(tr2_pin, 100);
         analogWrite(tr3_pin, 100);
         analogWrite(tr4_pin, 100);
         analogWrite(tr5_pin, 100);
         break;
       
       case 2:
         //dim mode 2
         analogWrite(tr0_pin, 60);
         analogWrite(tr1_pin, 60);
         analogWrite(tr2_pin, 60);
         analogWrite(tr3_pin, 60);
         analogWrite(tr4_pin, 60);
         analogWrite(tr5_pin, 60);
         break;
         
       case 3:
         //dim mode 3
         analogWrite(tr0_pin, 15);
         analogWrite(tr1_pin, 15);
         analogWrite(tr2_pin, 15);
         analogWrite(tr3_pin, 15);
         analogWrite(tr4_pin, 15);
         analogWrite(tr5_pin, 15);
         break;
     
     //Flash modes turn the light on or off depending on whether a threshold value has been reached. Ordered in terms of threshold size.
      case 4: //Flash mode 1

              
         //get equaliser values
         for (int i = 0; i < 7; i++)
         {
           digitalWrite(strobe_pin, LOW);
           delayMicroseconds(30); // to allow the output to settle
           spectrum_value[i] = analogRead(analog_pin);
          
           digitalWrite(strobe_pin, HIGH);
         }
         
         //need to find a way to filter this so that it yields better results
         //I've set everything but 3 to 400 as 500 seemed a little too thresholdy.
         if (spectrum_value[0] > 300) analogWrite(tr0_pin, 255);
         else analogWrite(tr0_pin, 0);
         if (spectrum_value[0] > 300) analogWrite(tr1_pin, 255);
         else analogWrite(tr1_pin, 0);
         if (spectrum_value[1] > 300) analogWrite(tr2_pin, 255);
         else analogWrite(tr2_pin, 0);
         if (spectrum_value[1] > 300) analogWrite(tr3_pin, 255);
         else analogWrite(tr3_pin, 0);
         if (spectrum_value[3] > 400) analogWrite(tr4_pin, 255);
         else analogWrite(tr4_pin, 0);
         if (spectrum_value[5] > 300) analogWrite(tr5_pin, 255);
         else analogWrite(tr5_pin, 0);
         break;
         
      case 5: //Flash mode 2
       // This flash mode is the on-off one. It sets a threshold and then either flashes the light on or off depending on reading.
              
         //get equaliser values
         for (int i = 0; i < 7; i++)
         {
           digitalWrite(strobe_pin, LOW);
           delayMicroseconds(30); // to allow the output to settle
           spectrum_value[i] = analogRead(analog_pin);
          
           digitalWrite(strobe_pin, HIGH);
         }
         
         //need to find a way to filter this so that it yields better results
         //I've set everything but 3 to 400 as 500 seemed a little too thresholdy.
         if (spectrum_value[0] > 400) analogWrite(tr0_pin, 255);
         else analogWrite(tr0_pin, 0);
         if (spectrum_value[0] > 400) analogWrite(tr1_pin, 255);
         else analogWrite(tr1_pin, 0);
         if (spectrum_value[1] > 400) analogWrite(tr2_pin, 255);
         else analogWrite(tr2_pin, 0);
         if (spectrum_value[1] > 400) analogWrite(tr3_pin, 255);
         else analogWrite(tr3_pin, 0);
         if (spectrum_value[3] > 500) analogWrite(tr4_pin, 255);
         else analogWrite(tr4_pin, 0);
         if (spectrum_value[5] > 400) analogWrite(tr5_pin, 255);
         else analogWrite(tr5_pin, 0);
         break;
         
      case 6: //Flash mode 3
          //For late night darkness...
              
         //get equaliser values
         for (int i = 0; i < 7; i++)
         {
           digitalWrite(strobe_pin, LOW);
           delayMicroseconds(30); // to allow the output to settle
           spectrum_value[i] = analogRead(analog_pin);
          
           digitalWrite(strobe_pin, HIGH);
         }
         
         //need to find a way to filter this so that it yields better results
         //I've set everything but 3 to 400 as 500 seemed a little too thresholdy.
         if (spectrum_value[0] > 600) analogWrite(tr0_pin, 255);
         else analogWrite(tr0_pin, 0);
         if (spectrum_value[0] > 600) analogWrite(tr1_pin, 255);
         else analogWrite(tr1_pin, 0);
         if (spectrum_value[1] > 600) analogWrite(tr2_pin, 255);
         else analogWrite(tr2_pin, 0);
         if (spectrum_value[1] > 600) analogWrite(tr3_pin, 255);
         else analogWrite(tr3_pin, 0);
         if (spectrum_value[3] > 650) analogWrite(tr4_pin, 255);
         else analogWrite(tr4_pin, 0);
         if (spectrum_value[5] > 600) analogWrite(tr5_pin, 255);
         else analogWrite(tr5_pin, 0);
         break;
         
      case 7: //Flash mode 4
          //Plain old strobe light
              
       analogWrite(tr0_pin, 255);
       analogWrite(tr1_pin, 255);
       analogWrite(tr2_pin, 255);
       analogWrite(tr3_pin, 255);
       analogWrite(tr4_pin, 255);
       analogWrite(tr5_pin, 255);
       delay(8);
       analogWrite(tr0_pin, 0);
       analogWrite(tr1_pin, 0);
       analogWrite(tr2_pin, 0);
       analogWrite(tr3_pin, 0);
       analogWrite(tr4_pin, 0);
       analogWrite(tr5_pin, 0);
       delay(150);
       break;
         
      default: //Just switch on the lights

       //255 for 5V to the transistor
       analogWrite(tr0_pin, 255);
       analogWrite(tr1_pin, 255);
       analogWrite(tr2_pin, 255);
       analogWrite(tr3_pin, 255);
       analogWrite(tr4_pin, 255);
       analogWrite(tr5_pin, 255);
       break;
   }
}
