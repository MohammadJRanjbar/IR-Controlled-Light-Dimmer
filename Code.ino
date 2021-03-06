/*AC Light Control

 */

#include  <TimerOne.h>          // Avaiable from http://www.arduino.cc/playground/Code/Timer1
int AC_pin =3;                // Output to Opto Triac
volatile int i=0;               // Variable to use as a counter volatile as it is in an interrupt
volatile boolean zero_cross=0;  // Boolean to store a "switch" to tell us if we have crossed zero
int inpPin=4;
int dim = 0;                    // Dimming level (0-128)  0 = on, 128 = 0ff
int inc=1;                      // counting up or down, 1=up, -1=down
int inString ;
int freqStep = 75;    // This is the delay-per-brightness step in microseconds.
bool flag = true;
int stat=10;
volatile unsigned long intCh1Rise; //Time of the rising edge.
volatile unsigned long PulseLength; //Pulselenght in uS.// For 60 Hz it should be 65
int Brightness=10;
int tempo=100;
// It is calculated based on the frequency of your voltage supply (50Hz or 60Hz)
// and the number of brightness steps you want. 
// 
// Realize that there are 2 zerocrossing per cycle. This means
// zero crossing happens at 120Hz for a 60Hz supply or 100Hz for a 50Hz supply. 

// To calculate freqStep divide the length of one full half-wave of the power
// cycle (in microseconds) by the number of brightness steps. 
//
// (120 Hz=8333uS) / 128 brightness steps = 65 uS / brightness step
// (100Hz=10000uS) / 128 steps = 75uS/step

void setup() {              
  Serial.begin(9600);                        // Begin setup
  pinMode(AC_pin, OUTPUT);                          // Set the Triac pin as output
  attachInterrupt(0, zero_cross_detect, RISING);    // Attach an Interupt to Pin 2 (interupt 0) for Zero Cross Detection
  Timer1.initialize(freqStep);                      // Initialize TimerOne library for the freq we need
  Timer1.attachInterrupt(dim_check, freqStep);      
  // Use the TimerOne Library to attach an interrupt
  // to the function we use to check to see if it is 
  // the right time to fire the triac.  This function 
  // will now run every freqStep in microseconds.                                            
}

void zero_cross_detect() {   
  zero_cross = true;               // set the boolean to true to tell our dimming function that a zero cross has occured
  i=0;
  digitalWrite(AC_pin, LOW);       // turn off TRIAC (and AC)
}                                 

// Turn on the TRIAC at the appropriate time
void dim_check() {                   
  if(zero_cross == true) {              
    if(i>=dim) {                     
      digitalWrite(AC_pin, HIGH);  // turn on light       
      i=0;  // reset time step counter                         
      zero_cross = false; //reset zero cross detection
    } 
    else {
      i++; // increment time step counter                     
    }                                
  }                                  
}                                   

void loop() 
{


/*
  
         if (Serial.available())
         {
     inString=Serial.parseInt();
    if(inString>0)
    {
     dim=map (inString,100,1,0,128);
      
    }

         }
         }
          */
    
    if((digitalRead(inpPin)) == LOW && (flag == true))
    {
    flag = false;
    intCh1Rise = micros();
    }
  if((digitalRead(inpPin)) == HIGH && (flag == false))
  {
    flag = true;
    PulseLength = micros() - intCh1Rise;
    if(PulseLength > 30000)
    {
      Serial.println("sum");
      if(stat < 10)
       {
        stat = stat + 1;
        Serial.println("sum");
        tempo=Brightness+tempo;
        Serial.println(tempo,DEC);
        dim=map (tempo,100,1,0,128);
       }
      else
        stat = 10;

    }
    else if(PulseLength > 5000 && PulseLength < 20000)
    { 
      Serial.println("Substract");
      if (stat > 0)
        {
          Serial.println("Substract");
          stat = stat - 1; 
          tempo=tempo-Brightness;
          Serial.println(tempo,DEC);
          dim=map (tempo,100,1,0,128);
        }
      else
        stat = 0;
     }  
     
    }
  }
