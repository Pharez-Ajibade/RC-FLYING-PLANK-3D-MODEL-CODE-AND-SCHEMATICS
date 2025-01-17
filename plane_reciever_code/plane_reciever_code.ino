#include<Servo.h>
#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>
// including necessary libraries 

RF24 radio(8, 10); // (CE, CSN)

const byte address[6] = "00001"; 

struct data {
  int Channel_1;
  int Channel_2;
  int Channel_3;
};
data receive_data;

int Throttle = 0; // set throttle joystic value
int U_or_D ; 
int L_or_R ;

int left_servo_val;
int right_servo_val;
int throttle_val;

Servo Throttle_esc; // CREATING SERVO OBJECT FOR ESC LIBRARY
Servo Left_servo; // CREATING SERVO OBJECTS
Servo Right_servo; // CREATING SERVO OBJECTS

unsigned long lastRecvTime = 0;

void setup() {
radio.begin();
radio.openReadingPipe(0,address);
radio.setPALevel(RF24_PA_MAX);
//radio.setDataRate(RF24_250KBPS);
radio.startListening();

Serial.begin(9600);

Left_servo.attach(6);
Right_servo.attach(7); 
Throttle_esc.attach(9,1000,2000); 

 while(radio.available() != true) {
 Serial.println("RADIO NOT CONNECTED");
 }
}

void loop() {
 while(radio.available()) {
    radio.read(&receive_data, sizeof(data));
  Serial.print(receive_data.Channel_1); 
    Serial.print("  "); // l or r
  Serial.print(receive_data.Channel_2); 
    Serial.print("  "); // u or d
  Serial.print(receive_data.Channel_3); 
    Serial.println("  "); // throttle

    lastRecvTime = millis(); 
 }

 unsigned long now = millis();
if ( now - lastRecvTime > 1000 ) {
ResetData(); // Signal lost.. Reset data
}
   U_or_D = receive_data.Channel_1;
   L_or_R = receive_data.Channel_2;
   Throttle = receive_data.Channel_3;

   throttle_val = map(Throttle, 0, 1023, 0, 180); 
   throttle_val = constrain(throttle_val, 0, 180); 

   left_servo_val  = map(U_or_D, 0, 1023, 0, 180); 
   left_servo_val = constrain(left_servo_val, 0, 180);

   right_servo_val = map(U_or_D, 0, 1023, 180, 0);
   right_servo_val = constrain(right_servo_val, 0, 180);
     
   

   if ( L_or_R < 450 || L_or_R > 500){ 
   left_servo_val = map(L_or_R, 0, 1023, 0, 180);
   right_servo_val = map(L_or_R, 0, 1023, 0, 180);
   
   }
   
  
   Throttle_esc.write(throttle_val);
   Left_servo.write(left_servo_val);
   Right_servo.write(right_servo_val);

  Serial.print(" Throttle Value ");
  Serial.print(throttle_val);
  Serial.print(" ");
  Serial.print(" Left angle ");
  Serial.print(left_servo_val);
  Serial.print("  "); 
  Serial.print(" Right angle : ");
  Serial.println(right_servo_val);
 

  
}

void ResetData(){

   U_or_D = 500;
   L_or_R = 500;
   Throttle = 0;  
}
