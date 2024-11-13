#include <Wire.h> // package to use for I2C

#define MY_ADDR   0x68

int LED = 13;                //LED Pin
char rx_buffer[32];          //received data
uint32_t cnt =0;             //count
uint8_t message[50];         //message


void setup()
{

Serial.begin(9600);

//configure LED as output
pinMode(LED,OUTPUT);
  
//start I2C with Arduino as slave for address 0x68
Wire.begin(MY_ADDR); 
  
//provide fucntion to be executed on reception of data
Wire.onReceive(receiveEvent);

sprintf(message,"Slave is ready : Address 0x%x",MY_ADDR);
Serial.println((char*)message);

Serial.println("Waiting for data from master");  
}

void loop(void)
{
//empty
}

//function which is executed when data is received on Arduino I2C bus
void receiveEvent(int bytes) 
{
  while(Wire.available())
  {
   rx_buffer[cnt++] = Wire.read();
  }

  rx_buffer[cnt] = '\0'; //terminate the received string
  cnt=0; //reset the count

  //Logging
  Serial.print("Received:");  
  Serial.println((char*)rx_buffer);  
}