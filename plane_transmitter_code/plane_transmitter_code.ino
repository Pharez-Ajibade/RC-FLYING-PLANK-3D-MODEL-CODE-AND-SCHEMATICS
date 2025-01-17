#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>
// including necessary libraries 

RF24 radio(8,10); // (CE, CSN)

const byte address[6] = "00001";

struct data{
 int Channel_1;
 int Channel_2;
 int Channel_3;
};
data send_data;

#define X_joystick A0
#define Y_joystick A1
#define Throttle_joystick A3

int U_or_D ;
int L_or_R ;
int Throttle;

void setup() {
radio.begin();
radio.openWritingPipe(address);
radio.setPALevel(RF24_PA_MAX);
// radio.setDataRate(RF24_250KBPS);
radio.stopListening();

}

void loop(){
U_or_D = analogRead(X_joystick);
Serial.print(" Joystick Value F or B :  ");
Serial.print(U_or_D);
L_or_R = analogRead(Y_joystick); 
Serial.print(" Joystick Value L or R :  ");
Serial.print(L_or_R);
Throttle = analogRead(Throttle_joystick); 
Serial.print(" Throttle Value:  ");
Serial.println(Throttle);

if ( U_or_D > 650 ){
  Serial.print ("  up  ");
  }
else if ( U_or_D < 350){
   Serial.print ("   down  ");
  }
else if (L_or_R < 350){
   Serial.print("  yaw left   ");
  }
else if ( L_or_R > 650){
   Serial.print("  yaw right   ");
  }

 send_data.Channel_1 = U_or_D;
 send_data.Channel_2 = L_or_R;
 send_data.Channel_3 = Throttle;

 radio.write(&send_data, sizeof(data));
 delay(1);
}
