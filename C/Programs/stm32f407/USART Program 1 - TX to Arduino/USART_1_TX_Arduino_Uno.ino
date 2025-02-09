void setup() {
  Serial.begin(115200);
  
  // Define the LED pin as Output
  pinMode (13, OUTPUT);
  
  Serial.println("Arduino USART Receiver...");
  Serial.println("-----------------------------");
    
}


void loop() {

  digitalWrite(13, LOW);

  //wait till we start receiving some data
  while(!Serial.available());
  digitalWrite(13, HIGH);

  //start reading the data
  char in_read=Serial.read();

  //print data
  Serial.print(in_read);

}