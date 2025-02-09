#include <SPI.h>

//here Arduino is slave and stm32 is master
//Arduino is sending data to stm32  

bool msg_complete = false;
uint8_t user_buff[500];
uint32_t count = 0;
#define MAX_LEN 500


//initialize SPI
void SPI_SlaveInit(void) 
{ 
  pinMode(SCK, INPUT);
  pinMode(MOSI, INPUT);
  pinMode(MISO, OUTPUT);
  pinMode(SS, INPUT);

  //enable SPI as slave
  SPCR = (1 << SPE);
}

//receive data
uint8_t SPI_SlaveReceive(void)
{
  //wait for reception to complete
  while(!(SPSR & (1<<SPIF)));

  //data register
  return SPDR;
}


//transmit data
void SPI_SlaveTransmit(uint8_t data)
{
  //start transmission
  SPDR = data;
  
  //wait for TX to complete
  while(!(SPSR & (1<<SPIF)));
}


void setup() 
{
  Serial.begin(9600);
  
  //initialize SPI
  SPI_SlaveInit();

  //pullup in pin 8
  pinMode(8, INPUT_PULLUP);
  digitalWrite(8,LOW);

  Serial.println("Slave Initialized");
}


void notify_controller(void)
{
  // low to high and to low (single pulse is sent as interrupt signal)
  pinMode(8,OUTPUT);
  digitalWrite(8,HIGH);
  delayMicroseconds(50);
  digitalWrite(8,LOW);
}


//loop
void loop() {
  
  Serial.println("Please type any data terminated with carriage return......");

  //reading user data and storing it in buffer
  while(!msg_complete)
  {
    if (Serial.available())
    {
      //read a byte of incoming data
      char readByte = (char)Serial.read();

      //populate the buffer
      user_buff[count++] = readByte;

      if(readByte == '\r' || (count == MAX_LEN)) //carriage return indicates that we have recived all the data
      {
        msg_complete = true;
        user_buff[count-1] = '\0'; //replace carriage return with null character
      }
    }
  }
  

  Serial.println("Message to be sent: ");
  Serial.println((char*)user_buff);

  notify_controller();

  //send user data through SPI
  for(uint32_t i=0 ; i<count; i++)
  {
    SPI_SlaveTransmit(user_buff[i]);
  }

  //reset all the params
  count = 0;
  msg_complete = false;
  Serial.println("Message has been sent successfully");

  //wait till SS becomes high
  while(!digitalRead(SS));
  Serial.println("Master has ended communication");
 
}