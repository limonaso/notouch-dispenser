/**####################################################
#            *              *                    *    #
#   *           dispenser_anticovid19.ino   *      *  #
#            Using ultrasonic sensor HC-SR04          #
#      *        and micro servos SG90.                #
#            *      *                    *            #
#               Copyright: Limonaso 2020  *       *   #                        
#     *        *          *                 *         #
####################################################**/


#include <Servo.h>                              //include the library of the servos to makwe easier their use

const int EchoPin = 5;                           //emitter pin from HC-SR04
const int TrigPin = 6;                       //receiver pin from HC-HCSR04
int pos = 1500;                                 //entire value position in microseconds for servos

Servo dispenser;                                    //create a different object to control each servo
Servo dispenser2;

void setup()                                    //function of starting pre-config, only run once
{
  pinMode(TrigPin, OUTPUT);                 //emitter pin HC-SR04 as output 
  pinMode(EchoPin, INPUT);                      //receiver pin HC-SR04 as input 
  dispenser.attach(9,1000,2000);                             //link pin 9 to servo 1, min value 1000 microsegs to 2000 microsegs(0-180º)
  dispenser2.attach(10,1000,2000);                           //link pin 10 to servo 2, min value 1000 microsegs to 2000 microsegs(0-180º)
  Serial.begin(9600);                           //Start serial communication with 9600 baudios speed data.
  dispenser.write(pos);                             //initial central position for both servos
  dispenser2.write(pos);                            
}

void loop()                                      
{
  int cm = ping(TrigPin, EchoPin);             //making entire value for centimeters and linking the function ping value with the HC-SR04 pines
  Serial.print("Distance: ");                   //print the header of the centimeters value in serial com
  Serial.println(cm);                            //print the decimal value to show the centimeters distance

  if (cm <= 6)                                   //if distance from hand to sensor is less or same than 6cm, activate dispenser
  {
        for (pos = 1500; pos <= 2000; pos += 500)                //position value goes from 90 to 180 degrees in a step of 500 microseconds
      { 
        dispenser.writeMicroseconds(pos);                                //positioning servo 1 with pos value
        dispenser2.writeMicroseconds(pos);                               //positioning servo 2 with pos value too
        delay(15);                                              //waiting 15 milliseconds for servo reaches the position
      } 
  }
  
  else                                                       //if not detects proximity, start position for servos
  {
        for (pos = 2000; pos >= 1500; pos -= 500)               //position value goes from 2000 to 1500 in a step of 500 microseconds
      { 
        dispenser.writeMicroseconds(pos);                               //positioning servo 2 with pos value
        dispenser2.writeMicroseconds(pos);                              //positioning servo 2 with pos value too
        delay(15);                                      //position value goes from 2000 to 1500 in a step of 500 microseconds
      }
  }
                            
  delay(250);                                    
}

int ping(int TrigPin, int EchoPin)            //making entire ping for calculate time between sending and receiving ultrasonic pulse to know distance of hand
{
  long duration, distanceCm;                   //making two long values to host the result of measuring the duration wave & physical distance in cm
  
  digitalWrite(TrigPin, LOW);                 //to generate a clean pulse, putting emitter to LOW for 4 microsegundos
  delayMicroseconds(4);
  digitalWrite(TrigPin, HIGH);                //generating pulse during 10 microseconds
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);                 //shutdown the emitter
  
  duration = pulseIn(EchoPin, HIGH);              //measuring time between pulses, in microseconds
  
  distanceCm = duration * 10 / 292/ 2;             //converting distance in cm
  return distanceCm;                               //returning up to distanceCm value to repeat the process and refresh it
}
