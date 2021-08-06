#include<Servo.h>

// activate software serial for DF miniplayer
#include "SoftwareSerial.h"
SoftwareSerial mySerial(10, 11);
# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]
# define ACTIVATED LOW
boolean isPlaying = false;

Servo myservo;

int buttonPin = 4;
int buttonState = 0; 
int ledPin = 12;
int timesUsed = 0;
int timer = 0;
int notifyTimer = 0;
int ledTimer = 0;

bool pressedButton = false; 
bool servoUp = false;
  
void setup() {
  mySerial.begin (9600);
  
  //Attatch 360 degrees servos
  myservo.attach(9);

 // playFirst();
  isPlaying = false;

  //Set pinmode for button
  pinMode(buttonPin, INPUT);
}

void loop() {
  //Do this while brushing
  if (timer < 120&& pressedButton && timesUsed <=4){
    //Increase timers
    timer++;
    notifyTimer++;
    
    //Check status for indication LED
    if(timesUsed == 0 || timesUsed == 1){
      digitalWrite(ledPin, HIGH); 
      }
    else if (timesUsed == 2 || timesUsed == 3) {
      if (ledTimer < 1){
        digitalWrite(ledPin, HIGH); 
        ledTimer++;
      }
      else{
        digitalWrite(ledPin, LOW);
        ledTimer = 0;
      }
    }
    else if (timesUsed >= 4) {
      digitalWrite(ledPin, LOW);
    }
      
    //Move serve. Extra writes to not crash because of power overload
    if(servoUp){ //Move one way
      myservo.write(45); 
      
      delay(150);
      myservo.write(90); 
      
      delay(150);
      myservo.write(135); 
     
      delay(150);
      myservo.write(180); 
      
      servoUp = false;
    }else{ //Move the other way
      myservo.write(135); 

      delay(150);
      myservo.write(90); 
 
      delay(150);
      myservo.write(45); 
  
      delay(150);
      myservo.write(0); 

      servoUp = true;
    }

    //Notify user each 30 seconds. Extra writes to not crash because of power overload
    if(notifyTimer >30){
      myservo.write(45); 
       
      delay(150);
       myservo.write(0); 
    
      delay(150);
      myservo.write(45); 
    
      delay(150);
      myservo.write(90); 
     
      delay(150);
      myservo.write(135); 
   
      delay(150);
      myservo.write(180); 
    
      delay(150);
      myservo.write(135); 
   
      delay(150);
      myservo.write(90); 
    
      delay(150);
      myservo.write(45); 
    
      delay(150);
       myservo.write(0); 
   
      delay(150);
       myservo.write(45); 

      delay(150);
       myservo.write(0); 
  
      delay(150);
      myservo.write(45); 

      delay(150);
      myservo.write(90); 

      delay(150);
      myservo.write(135); 
  
      delay(150);
       myservo.write(180); 

      delay(150);
      //give notice with servo
      notifyTimer = 0;
    }

    //Do when you brushed for 2 minutes
    if(timer >= 120){
      timesUsed++;
      notifyTimer = 0;
      timer = 0;
      pressedButton = false;
      digitalWrite(ledPin, LOW);

      //Stop servo
      myservo.write(90); 
  
    }

    //Delay 1 second, so the loop runs once about a second
    delay(1000);
  }   
  else if(!pressedButton){
    
    
    //Read button when it is not brush
    int buttonState = digitalRead(4);
    if (buttonState == HIGH) {
      pressedButton = true;

     
    }
    //Small delay so it can easily be read, but not overwork the arduino
    delay(10);
  }

   int buttonState = digitalRead(4);
    if (buttonState == HIGH) 
    {
       if(!isPlaying)
      {
        play();
        isPlaying = true;
      }
    }

    else if (buttonState == LOW) 
    {
       
    }

  
 
}

void playFirst()
{
execute_CMD(0x3F, 0, 0);
delay(500);
setVolume(20);
delay(500);
execute_CMD(0x11,0,1);
delay(500);
}
void pause()
{
execute_CMD(0x0E,0,0);
delay(500);
}
void play()
{
execute_CMD(0x0D,0,1);
delay(500);
}
void playNext()
{
execute_CMD(0x01,0,1);
delay(500);
}
void playPrevious()
{
execute_CMD(0x02,0,1);
delay(500);
}
void setVolume(int volume)
{
execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
delay(2000);
}
void execute_CMD(byte CMD, byte Par1, byte Par2)
// Excecute the command and parameters
{
// Calculate the checksum (2 bytes)
word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
// Build the command line
byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
//Send the command line to the module
for (byte k=0; k<10; k++)
{
mySerial.write( Command_line[k]);
}
}
