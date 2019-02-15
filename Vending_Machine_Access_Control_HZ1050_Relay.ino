#include <SoftwareSerial.h>
const int chipSelect = 4;
long approvedUsers[] = {1597542,16711302,15138400,16711422,16711302,14745088,15138462,15132408,16311936,15138552,10026622,15138558,16317958,14745094,14745118,14745184,15138334,15138430}; //list the ID numbers (decimal) of any card able to open the restocking door
long approvedDevs[] = {16680568,16711302,15138400,16711422,16711302,14745088,15138462,15132408,16318182,16318104,1560}; //list the ID numbers (decomal) of any card able to open the maintnence door. 
//one ID card can be on both lists, it just opens the first door, then after 2500 ms, the second door unlocks. 
int numApprovedUsers = 18; //must be equal to the length of the approvedUsers array.  C doesn't have a way to get the length of an array.
int numApprovedDevs = 11; //must be equal to the length of the approvedDevs array.
SoftwareSerial inputData(7, 6); //RX, TX we need to put a TX pin to meet the softwareSerial requirments, but it goes unused. HZ1050 TX connects to pin 7
const unsigned long TIMEOUT = 300;  // mS
int frontRelay = 9; //pin for the relay that contols the front (restocking) lock
int backRelay = 8; //pin for the relay that controls the back (development) lock.
int openTime = 2000;
//other variables for the code, no need to change these ever.
byte count;
byte backCount;
// the most recent card, zero if none
unsigned long cardCode;
boolean isLocked = true;
unsigned long timeLastSerialInput;

//Determines if a card has access approval, unlocks appropriate door. 
void processNewCard ()
 //Begin front door processing
{
  Serial.println(cardCode);
  for(int i=0;i<numApprovedUsers;i++)
 {
    if(approvedUsers[i]==cardCode)
   {
    Serial.println("**Access Granted**");
      Serial.println("**Unlocking Front Door**");
      digitalWrite(frontRelay, HIGH);
      delay(openTime);
      digitalWrite(frontRelay, LOW);
   }else{
    count++; //counts if not matching approvedUsers[i]
   }
  }
   if(count == numApprovedUsers){
   Serial.println("**Access Denied for Front**"); 
   }


  //Begin back door processing
  //Serial.println(cardCode);
  for(int i=0;i<numApprovedDevs;i++)
  {
    if(approvedDevs[i]==cardCode)
   {
    Serial.println("**Access Granted**");
      Serial.println("**Unlocking Back Door**");
      digitalWrite(backRelay, HIGH);
      delay(openTime);
      digitalWrite(backRelay, LOW);
   }else{
    backCount++; //counts if not matching approvedUsers[i]
   }
   }
   if(backCount == numApprovedUsers){
   Serial.println("**Access Denied for Back**"); 
   }
  }  // end of processNewCard

  //generates the cardCode whenever a card is detected.
void processIncomingByte (const byte inByte)
  {
  unsigned long now = millis ();
  
  // if a long time has elapsed, assume new card
  if (now - timeLastSerialInput >= TIMEOUT)
    {
    cardCode = 0;
    count = 0; 
    }  // end of timeout
  
  timeLastSerialInput = now;
  cardCode <<= 8;
  cardCode |= inByte;
  count++;
  
  if (count >= 4)
    {
    processNewCard ();
    cardCode = 0;
    count = 0; 
    }  // end of having 4 incoming bytes
   
  }  // end of processIncomingByte

  
 //initialises variables and ports
void setup()
  {
  inputData.begin (19200);
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  pinMode(frontRelay, OUTPUT);
  pinMode(backRelay, OUTPUT);
  // end of setup
}

//basically executes the other methods in the code when a card enters the reader's range.s
void loop()
{
  // if serial data available, process it
  while (inputData.available () > 0)
    processIncomingByte (inputData.read ());
}  // end of loop
