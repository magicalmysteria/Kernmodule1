#include<Servo.h>

Servo myservo;

int buttonPin = 4;
int buttonState = 0; 
int ledPin = 11;
int timesUsed = 0;
int timer = 0;
int notifyTimer = 0;
int ledTimer = 0;

bool pressedButton = false; 
bool servoUp = false;
  
void setup() {
  //Attatch 360 degrees servos
  myservo.attach(9);
 

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

 
}
