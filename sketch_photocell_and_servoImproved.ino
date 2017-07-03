    /* Photocell simple testing sketch. 
     
    Connect one end of the photocell to 5V, the other end to Analog 0.
    Then connect one end of a 10K resistor from Analog 0 to ground
     
    For more information see http://learn.adafruit.com/photocells */
    #include <Servo.h>

    Servo myservo;            // create servo object to control a servo
    double val[5] ;           //Array of 5 elements to get the avarage light level in the room

    int readingArrayPointer = 0; //Pointer of the array
    double valReadings[4];            //Last 4 readings;

    int avarageLightLevel = 0; //Avarage of last 4 readings
    
    int placeReading = 0;
    int pos = 0;              // variable to store the servo position
    bool calibrating = true;  //will be set to false and then the game can be used
    bool firstTime= true;
    int initLevel=0;
    int closeLevel =0;
    
    int photocellPin = 0;     // the cell and 10K pulldown are connected to a0
    int photocellReading;     // the analog reading from the analog resistor divider
    bool beingLit = false;
     
    void setup(void) {
      myservo.attach(9);  // attaches the servo on pin 9 to the servo object
      // We'll send debugging information via the Serial monitor
      Serial.begin(9600);   
    }
     
    void loop(void) {

  
      photocellReading = analogRead(photocellPin);  
     
      Serial.print("Analog reading = ");
      Serial.print(photocellReading);     // the raw analog reading

      if (firstTime == true) 
      {
        valReadings[0] = 1;
        valReadings[1] = 1;
        valReadings[2] = 1;
        valReadings[3] = 1;
         Serial.println(" - closing first time");
         Serial.println(pos);
         if(beingLit == false)
          for (pos = 0; pos <= 180; pos += 1) { // goes from 180 degrees to 0 degrees
            myservo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(15);                       // waits 15ms for the servo to reach the position

         }
          beingLit = true;
          firstTime = false;
                
      } 



      if(calibrating == true)
      {
        
      
      //Calculate lightlevel
      if(placeReading < 5)
      {
        delay(100);
        val[placeReading] = photocellReading;
        placeReading ++;
        Serial.print(" placeReading = ");
        Serial.print(placeReading);
        Serial.print(" Added on place | reading =");
        Serial.print(photocellReading);
        if(placeReading == 4)
        {
          double sum;
          sum = val[0] + val[1] + val[2] + val[3] + val[4];
          Serial.print("sum = ");
          Serial.print(sum);
          photocellReading = sum /5;
          Serial.print("Avarage =");
          Serial.print(photocellReading);
          initLevel = photocellReading + 85; //+30 for margin of error and when light is sun bright on the sensor it will jump above 30
          //openLevel = photocellReading + 60;
          calibrating = false;
        }
      }
    }
    else
    {
      //get the avarage light Level
//    int readingArrayPointer = 0; //Pointer of the array
//    double valReadings[4];            //Last 4 readings;
//    int avarageLightLevel = 0; //Avarage of last 4 readings

      valReadings[readingArrayPointer] = photocellReading;
      readingArrayPointer ++;
      double sumAvarage = valReadings[0] + valReadings[1] + valReadings[2] + valReadings[3];
      avarageLightLevel = sumAvarage /4;
      Serial.print("Avarage light level is now = ");
      Serial.print(avarageLightLevel);
      if(readingArrayPointer == 4)
      {
        readingArrayPointer =0;
        closeLevel = initLevel;
      }


      
      // We'll have a few threshholds, qualitatively determined
      if (avarageLightLevel < closeLevel) 
      {
         Serial.println(" - Closed");
         Serial.println(pos);
         if(beingLit == false)
          for (pos = 0; pos <= 180; pos += 1) { // goes from 180 degrees to 0 degrees
            myservo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(15);                       // waits 15ms for the servo to reach the position

         }
          beingLit = true;
                
      } 
      else if (avarageLightLevel > closeLevel) 
      {
        Serial.println(" - Open");
        Serial.println(pos);
        if(beingLit == true)
          for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
            myservo.write(pos);              // tell servo to go to position in variable 'pos'
            delay(15);                       // waits 15ms for the servo to reach the position

          }
          beingLit = false;
      } 
      Serial.print("closeLevel = ");
      Serial.print(closeLevel);
//      else if (photocellReading < 500) {
//        Serial.println(" - Light");
//      } else if (photocellReading < 800) {
//        Serial.println(" - Bright");
//      } else {
//        Serial.println(" - Very bright");
//      }
      delay(1000);
    }
    }
