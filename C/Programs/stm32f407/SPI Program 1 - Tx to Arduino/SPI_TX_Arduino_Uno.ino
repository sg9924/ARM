#include<SPI.h>
#include<stdint.h>  
#define SPI_SCK    13  //SCLK
#define SPI_MOSI   11  //MOSI
#define SPI_SS     10  //SS

char data[500];

//initialize Arduino as slave for SPI
void SPI_SlaveInit(void) 
{ 
//initialize Arduino SPI pins.
pinMode(SCK, INPUT);
pinMode(MOSI, INPUT);
pinMode(SS, INPUT);

//enable Arduino SPI as slave
SPCR = (1 << SPE);
}

//receive data to Arduino - 8 bit
uint8_t SPI_SlaveReceive(void)
{
//wait till data reception is complete
while(!(SPSR & (1<<SPIF)));

return SPDR; //return data
}



//setup
void setup() 
{
//intialize serial communications
Serial.begin(9600);

//intialize Arduino as SPI slave
SPI_SlaveInit();
Serial.println("Slave Initialized");
}

//main program
void loop() 
{
uint32_t i;
uint16_t dataLen = 0; //data length

Serial.println("Slave waiting for ss to go low");
while(digitalRead(SS));//read SS till it goes low

i = 0;
dataLen = SPI_SlaveReceive(); //recive length
for(i=0; i<dataLen; i++)
{
  data[i] = SPI_SlaveReceive(); //receive each data byte for the received length
}

data[i] = '\0'; //terminate the recieved data string

//Print Results
Serial.println("Received Data:");
Serial.println(data);
Serial.print("Data Length:");
Serial.println(dataLen);
}