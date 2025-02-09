void setup() {
  Serial.begin(115200);
  
  //LED pin as Output
  pinMode(13, OUTPUT);
  
 //Serial.println("Arduino Case Converter program.......");
 //Serial.println("-------------------------------------"); 
}



char changeCase(char ch)
{
  if (ch >= 'A' && ch <= 'Z')
    ch = ch + 32;
  else if (ch >= 'a' && ch <= 'z')
    ch = ch - 32;  

  return ch;
}


void loop()
{

  digitalWrite(13, LOW);

  //wait until some data is received
  while(!Serial.available());

  //write LEd Pin HIGH
  digitalWrite(13, HIGH);

  //read the received data
  char in_read=Serial.read();

  //print the data 
  Serial.print(changeCase(in_read));
}