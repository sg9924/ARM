#include <Wire.h>

//Arduino: Master, STM32: Slave
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
  Serial.println("Send character \"s\" to trigger I2C communication");

  while(!Serial.available());
  char in_read=Serial.read();

  while(in_read != 's');

  Serial.println("Starting I2C Communication....");

  Wire.beginTransmission(SLAVE_ADDR); //begin I2C communication with STM32 slave
  Wire.write(0X51);                   //Send first command to get string length
  Wire.endTransmission();             //end transmission

  Wire.requestFrom(SLAVE_ADDR,1);     //request the bytes from the slave


  //getting the data bytes of the length which is received
  if(Wire.available()) {  // 
    data_len = Wire.read(); // Reads the data 
  }

  Serial.print("Length of String:");
  Serial.println(data_len);

  Wire.beginTransmission(SLAVE_ADDR);
  Wire.write(0X52);        //send the 2nd command to get the required string
  Wire.endTransmission();

  Wire.requestFrom(SLAVE_ADDR,data_len);

  uint32_t i=0;
  for(i=0; i<=data_len; i++)
  {
    if(Wire.available())
    {
      rcv_buf[i] = Wire.read(); //read the bytes of the string
    }
  }
  rcv_buf[i] = '\0';   //terminate the string

  Serial.print("String Received:");
  Serial.println((char*)rcv_buf);
}