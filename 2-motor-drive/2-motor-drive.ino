/*************************************************************
Motor Shield 2-Channel DC Motor Demo
by Randy Sarafan

For more information see:
https://www.instructables.com/id/Arduino-Motor-Shield-Tutorial/

A = venstre motor
B = højre motor inverted

*************************************************************/
boolean start = true;
int speedVariable;
void left(){

  digitalWrite(9, HIGH);   //Engage the Brake for Channel A - venstre
  
  //Motor B - højre forward @ hald speed
  digitalWrite(13, LOW); //Establishes forward direction of Channel B - højre
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B - højre
  analogWrite(11, speedVariable);   //Spins the motor on Channel B - højre at full speed
}
void right(){
   //Motor A - venstre forward @ half speed
  digitalWrite(12, HIGH);  //Establishes forward direction of Channel A - venstre
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A - venstre
  analogWrite(3, speedVariable);    //Spins the motor on Channel A - venstre at half speed
  
  //Motor B forward @ full speed
  digitalWrite(8, HIGH);   //Engage the Brake for Channel B - højre
  
}
void hardleft(){

  digitalWrite(9, HIGH);   //Engage the Brake for Channel A - venstre
  
  //Motor B - højre forward @ full speed
  digitalWrite(13, LOW); //Establishes forward direction of Channel B - højre
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B - højre
  analogWrite(11, 255);   //Spins the motor on Channel B - højre at full speed
}
void hardright(){
   //Motor A - venstre forward @ full speed
  digitalWrite(12, HIGH);  //Establishes forward direction of Channel A - venstre
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A - venstre
  analogWrite(3, 255);    //Spins the motor on Channel A - venstre at half speed
  
  //Motor B forward @ full speed
  digitalWrite(8, HIGH);   //Engage the Brake for Channel B - højre
  
}

void forward(){
   //Motor A - venstre forward @ full speed
  digitalWrite(12, HIGH);  //Establishes forward direction of Channel A - venstre
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A - venstre
  analogWrite(3, speedVariable);    //Spins the motor on Channel A - venstre at half speed
  
  //Motor B - højre forward @ full speed
  digitalWrite(13, LOW); //Establishes forward direction of Channel B - højre
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B - højre
  analogWrite(11, speedVariable);   //Spins the motor on Channel B - højre at full speed
}

void backwards(){
  //Motor A - venstre backward @ full speed
  digitalWrite(12, LOW);  //Establishes backward direction of Channel A - venstre
  digitalWrite(9, LOW);   //Disengage the Brake for Channel A - venstre
  analogWrite(3, speedVariable);    //Spins the motor on Channel A - venstre at half speed
  
  //Motor B - højre backward @ full speed
  digitalWrite(13, HIGH); //Establishes backward direction of Channel B - højre
  digitalWrite(8, LOW);   //Disengage the Brake for Channel B - højre
  analogWrite(11, speedVariable);   //Spins the motor on Channel B - højre at full speed
}

void stopMotor(){
  digitalWrite(9, HIGH);   //Engage the Brake for Channel A - venstre
  digitalWrite(8, HIGH);   //Engage the Brake for Channel B - højre
  start = false;
}
#include <RPLidar.h>


RPLidar lidar;

#define RPLIDAR_MOTOR 7 // The PWM pin for control the speed of RPLIDAR's motor.
// This pin should connected with the RPLIDAR's MOTOCTRL signal
int forwardanglereadings = 0;
int leftanglereadings = 0;
int rightanglereadings = 0;
int forwarddistancereadings = 0;
int leftdistancereadings = 0;
int rightdistancereadings = 0;

void LIDAR_GO(){
  if (IS_OK(lidar.waitPoint())) {
    //perform data processing here... 
    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle;
    int quality = lidar.getCurrentPoint().quality;
    int rounded = round(angle);
    int roundedDistance = round(distance);
    if (lidar.getCurrentPoint().startBit) {
      int forwardreadings;
      int leftreadings;
      int rightreadings;
      forwardreadings = forwarddistancereadings / forwardanglereadings;
      leftreadings = leftdistancereadings / leftanglereadings;
      rightreadings = rightdistancereadings / rightanglereadings;
      if (forwardreadings > 300){
        forward();
        delay(1000);
      }else{
        if(leftreadings<rightreadings){
          right();
          delay(1000);
        }else if(rightreadings<leftreadings){
          left();
          delay(1000);
        }else if((leftreadings && rightreadings)< 200){
          backwards();
        } else{
          right();
        }
      }
      forwardanglereadings = 0;
      leftanglereadings = 0;
      rightanglereadings = 0;
      forwarddistancereadings = 0;
      leftdistancereadings = 0;
      rightdistancereadings = 0;
    }else {
      if(quality > 14){
        if(((angle > 0 && angle < 45)|| (angle > 360-45 && angle < 360))){
          forwardanglereadings = forwardanglereadings +1;
          forwarddistancereadings = forwarddistancereadings + roundedDistance;
        }else if((angle > 45 && angle < 135) && (distance > 150 && distance < 300 )){
          rightanglereadings = rightanglereadings +1;
          rightdistancereadings = rightdistancereadings + roundedDistance;  
        }else if((angle > 225 && angle < 360-45) && (distance > 150 && distance < 300 )){
          leftanglereadings = leftanglereadings +1;
          leftdistancereadings = leftdistancereadings + roundedDistance;  
  
        }
      }
    }
  }else {
    analogWrite(RPLIDAR_MOTOR, 0); //stop the rplidar motor
    
    // try to detect RPLIDAR... 
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
       //detected...
       lidar.startScan();
       analogWrite(RPLIDAR_MOTOR, 255);
       delay(1000);
    }
  }
}
void setup() {
  
  //Setup Channel A
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin

  //Setup Channel B
  pinMode(13, OUTPUT); //Initiates Motor Channel B pin
  pinMode(8, OUTPUT);  //Initiates Brake Channel B pin


  lidar.begin(Serial);
  lidar.startScan();
  // set pin modes
  pinMode(RPLIDAR_MOTOR, OUTPUT);
  analogWrite(RPLIDAR_MOTOR, 255);
  
}

void loop(){
  speedVariable = (((analogRead(A0)/2)/10));
  LIDAR_GO();
  }
