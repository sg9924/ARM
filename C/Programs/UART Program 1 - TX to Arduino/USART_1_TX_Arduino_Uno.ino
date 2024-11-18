//USART communication between STM32 & Arduino Uno

void setup()
{
  //set baud rate
  Serial.begin(115200);
  
  //LED pin as output
  pinMode(13, OUTPUT);
  
  Serial.println("Arduino Uno UART Receiver");
}


void loop() //repeated for each byte of string to be received
{
  //set LED pin as low
  digitalWrite(13, LOW);

  //wait till some data is received
  while(!Serial.available());

  //set led pin as high on data reception
  digitalWrite(13, HIGH);

  //read the incoming data
  char byte_rx = Serial.read();

  Serial.print(byte_rx);
}
