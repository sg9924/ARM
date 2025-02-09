#include <Wire.h>

//Master: Arduino, Slave: STM32
//Scenario: Sending large string
//A4: SDA
//A5: SCL


int LED = 13;
uint8_t rcv_buf[512];   //string to be received

uint32_t data_len=0,w_ptr = 0;
#define SLAVE_ADDR 0x20



void setup()
{
  Serial.begin(9600);
    
  //Define the LED pin as Output
  pinMode(LED, OUTPUT);
  
  //initialize I2C
  Wire.begin(); 
}


void loop() 
{
  uint32_t rem_len=0,last_read=0;
  Serial.println("Master: Arduino");
  Serial.println("Send character \"s\" to trigger I2C communicationn");

  while(!Serial.available());
  char in_read=Serial.read();

  while(in_read!='s');

  Serial.println("Starting I2C Communication....");

  Wire.beginTransmission(SLAVE_ADDR); //begin I2C communication with STM32 slave
  
  Wire.write(0X51);                   //Send first command to get string length
  Wire.endTransmission();             //end transmission
  Wire.requestFrom(SLAVE_ADDR,4);     //request the bytes from the slave


//getting the data bytes of the length which is received
 for(uint32_t len=0; len<4; len++)
 {
  if(Wire.available())
  {  
    uint32_t data = (uint32_t)Wire.read();
    data_len |= (data << (8*len));         //read and store the length bytes
  }
 }


  Serial.print("Length of String:");
  Serial.println(data_len);

  Wire.beginTransmission(SLAVE_ADDR);
  
  Wire.write(0X52);        //send the 2nd command to get the required string
  Wire.endTransmission();

  rem_len = data_len;

while(rem_len > 0)
 {
   if(rem_len <= 32)
   {
      Wire.requestFrom(SLAVE_ADDR,rem_len);
      last_read = rem_len;
      rem_len = 0;
   }
   else
   {
    Wire.requestFrom(SLAVE_ADDR,32);
    last_read = 32;
    rem_len -= 32;
   }
   
  uint32_t i=0;
  for(i=0; i<=last_read; i++)
  {
    if(Wire.available())
    { 
      rcv_buf[w_ptr++] = Wire.read(); //read the string bytes 
    }
  }
 }
 
  rcv_buf[w_ptr] = '\0';   //terminate the string
  w_ptr = 0;

  //print the received string
  Serial.print("String Received:");
  Serial.println((char*)rcv_buf);
}