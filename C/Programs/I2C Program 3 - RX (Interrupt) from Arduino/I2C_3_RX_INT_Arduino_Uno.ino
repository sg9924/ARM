#include <Wire.h>

#define SLAVE_ADDR 0x80 //Arduino slave address

//A4:  SDA
//A5:  SCL


int LED = 13; //LED pin
uint8_t active_command = 0xFF,led_status=0;  //default command code and led status
char name_msg[32] = "Hi There! This is I2C test from STM32 to Arduino Uno\n"; //data to be sent


//function to get len of data to be sent
uint8_t get_len_of_data()
{
    return (uint8_t)strlen(name_msg);
}


//setup
void setup()
{
  pinMode(LED, OUTPUT); //LED pin as output
  
  //Begin I2C communication with Arduino as slave 
  Wire.begin(SLAVE_ADDR); 
  
  //receive command code
  Wire.onReceive(receiveEvent);

  //send the data requested
  Wire.onRequest(requestEvent);
}


//receive data
void receiveEvent(int bytes)
{
  active_command = Wire.read();               // read one byte
}

//send data on request
void requestEvent()
{
  if(active_command == 0X51)
  {
    uint8_t len = get_len_of_data();
    Wire.write(&len,1);                      //send length data which will be sent
    active_command = 0xFF;                   //reset command code
  }
  

  if(active_command == 0x52)
  {
    Wire.write(name_msg, get_len_of_data()); //send the data
    active_command = 0xFF;                   //reset the command code
  }
}
void loop()
{
  //nothing here
}