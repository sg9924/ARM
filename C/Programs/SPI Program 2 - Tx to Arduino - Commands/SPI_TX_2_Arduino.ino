
//SPI pin numbers:
//SCK   13   Serial Clock.
//MISO  12   Master In Slave Out.
//MOSI  11   Master Out Slave In.
//SS    10   Slave Select

#include <SPI.h>

const byte led = 9;           //LED digital I/O pin

boolean ledState = HIGH;      //LED state

uint8_t dataBuff[255];        //data buffer

uint8_t board_id[10] = "ARDUINOUNO";   //slave board name

#define NACK    0xA5   //NACK code
#define ACK     0xF5   //ACK code


//command codes
#define COMMAND_LED_CTRL          0x50
#define COMMAND_LED_READ          0x51

#define LED_ON     1
#define LED_OFF    0

//arduino analog pins
#define ANALOG_PIN0   0
#define ANALOG_PIN1   1
#define ANALOG_PIN2   2
#define ANALOG_PIN3   3
#define ANALOG_PIN4   4


//initialize SPI
void SPI_SlaveInit(void) 
{ 
  pinMode(SCK, INPUT);
  pinMode(MOSI, INPUT);
  pinMode(MISO, OUTPUT);
  pinMode(SS, INPUT);
  
  // Enable SPI as slave
  SPCR = (1 << SPE);
}

//recieve data
uint8_t SPI_SlaveReceive(void)
{
  //wait for reception complete
  while(!(SPSR & (1<<SPIF)));

  // data register
  return SPDR;
}


//send data
void SPI_SlaveTransmit(uint8_t data)
{
  //start TX
  SPDR = data;
  
  //wait for TX to complete
  while(!(SPSR & (1<<SPIF)));
}
  

//setup
void setup() 
{
  Serial.begin(9600);
  
  //initialize LED pin
  pinMode(led, OUTPUT);
  
  //make LEd low
  digitalWrite(led,LOW);
  
  //initialize SPI as slave
  SPI_SlaveInit();
  
  Serial.println("Arduino Slave Initialized");
}

//check data and return acknowledge
byte checkData(byte command)
{
  return ACK;
}

//loop
void loop() 
{
  byte data, command, len, ackornack=NACK;
  
  //1. wait till SS becomes low
  Serial.println("Slave waiting for SS to go Low");
  while(digitalRead(SS));
  
  //2. wait for a data byte to be received
  command = SPI_SlaveReceive();
  ackornack = checkData(command);
  
  //3. send ACK after receiving data
  SPI_SlaveTransmit(ackornack);
  
  len = SPI_SlaveReceive(); //read a dummy byte
  
  if(command == COMMAND_LED_CTRL)
  {
    //read 2 bytes for Pin number and state
    uint8_t pin = SPI_SlaveReceive(); 
    uint8_t value = SPI_SlaveReceive();

    Serial.println("Received Command: COMMAND_LED_CTRL");

    if(value == (uint8_t)LED_ON)
    {
      digitalWrite(pin,HIGH);
    }
    else if (value == (uint8_t)LED_OFF)
    {
      digitalWrite(pin,LOW);
    }
  
  }
  else if (command == COMMAND_LED_READ)
  {
    //read pin number
    uint8_t pin = SPI_SlaveReceive();
    Serial.println("Received Command: COMMAND_LED_READ");

    uint8_t val = digitalRead(pin);
    SPI_SlaveTransmit(val);
    val = SPI_SlaveReceive(); //dummy read
  }
}