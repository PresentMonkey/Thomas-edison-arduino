#include <Arduino.h>
#include <WebUSB.h>
#include <Servo.h>

WebUSB WebUSBSerial(1 /* http:// */, "virtual-star-chart.presentmonkey.me");
Servo myservo;

#define ledPin 13
#define buzzer 10
#define servoPin 9
#define servoDelay 50
#define servoAngle 21

int stars = 1; //global val for starts



void nextion(){
  Serial1.write(0xff);
  Serial1.write(0xff);
  Serial1.write(0xff);
}

void giveBall(){
    WebUSBSerial.write("\r\n Gave Ball");
    myservo.write(servoAngle);
    delay(servoDelay);
    myservo.write(0);
}

void draw_star(int star_number, int star_type){
  Serial1.print("p" + String(star_number-1) + ".pic=" + String(star_type));
  nextion();
}


void flash_star(char* swipe_direction, int star_type){
  if(swipe_direction == "right"){
    for(int i=0; i<=5; i++){
      draw_star(i, star_type);
      delay(100);
    }
  }
  if(swipe_direction == "left"){
    for(int i=5; i>=0; i--){
      draw_star(i, star_type);
      delay(100);
    }
  }
  delay(100);
}

void prize_animation(int choice){
  if(choice == 1){
    flash_star("right", 0);
    flash_star("right", 1);
    flash_star("right", 0);
  }
  if(choice == 2){
    draw_star(3, 0);
    delay(250);
    draw_star(4, 0);
    delay(250);
    draw_star(2, 0);
    delay(250);
    draw_star(5, 0);
    delay(250);
    draw_star(1, 0);
    delay(250);
  }
}

void prize_buzzer(){
   tone(buzzer, 293.665);
   delay(125);
   noTone(buzzer);
   delay(125);
   tone(buzzer, 293.665);
   delay(75);
   tone(buzzer, 440);
   delay(500);
   noTone(buzzer);
}

void giveStar(){
  if(stars < 7){
    draw_star(stars, 1);
    stars += 1;
    delay(500);
  }
  if (stars >= 6){
    draw_star(5, 1);
    delay(500);
    prize_animation(2);
    giveBall();
    prize_buzzer();
    stars = 1;
  }
}
void setup() {
  while (!WebUSBSerial) {
    ;
  }
  WebUSBSerial.begin(9600);
  WebUSBSerial.write("Sketch begins.\r\n> ");
  WebUSBSerial.flush();
  pinMode(ledPin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  myservo.attach(servoPin);
  myservo.write(0);
  Serial1.begin(9600);
}

void loop() {
  if (WebUSBSerial && WebUSBSerial.available()) {
    int byte = WebUSBSerial.read();
    WebUSBSerial.write(byte);
    if (byte == 'h') {
      //WebUSBSerial.write("\r\n Stars: " + String(stars));
      digitalWrite(ledPin, HIGH);
      giveStar();
    }
    WebUSBSerial.write("\r\n> ");
    WebUSBSerial.flush();
  }
}
