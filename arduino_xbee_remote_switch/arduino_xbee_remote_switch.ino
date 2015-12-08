//ref https://www.youtube.com/watch?v=CzH146rR-7I
/*
(1) Arduino --- cordinator XBee (APT mode) sends AT command D4 (Pin# 11)
When Arduino's D2 is state (HIGH or LOW)
(2) End device XBee (Receive AT command D4)
(3) XBee (ON/OFF) control a SSR
*/
const byte LED = 13;
const byte BUTTON = 2;
int commandStatus = 5;

// Interrupt Service Routine (ISR)
void switchPressed ()
{
  if (digitalRead (BUTTON) == HIGH){
    setRemoteState(0x4);  // Function send commant to Xbee OFF
  }
  else{
    setRemoteState(0x5);  // Function send commant to Xbee ON
  }
}  // end of switchPressed

void setup ()
{
  pinMode (LED, OUTPUT);  // so we can update the LED
  digitalWrite (LED, LOW);
  digitalWrite (BUTTON, HIGH);  // internal pull-up
  Serial.begin(9600);
}  // end of setup

void setRemoteState(char value) {
  //Remote AT Command Request
  Serial.write(0x7E);       // start byte
  Serial.write(0x00);  // high part of length (always zero)
  Serial.write(0x10);       // lop part of length
  Serial.write(0x17);       // 0x17 is a remote AT command
  Serial.write(0x01);       // if frame id set to zero, no reply
  // ID of recipient, or use 0xFFFF for broadcast
  Serial.write((byte)00);
  Serial.write((byte)00);
  Serial.write((byte)00);
  Serial.write((byte)00);
  Serial.write((byte)00);
  Serial.write((byte)00);
  Serial.write(0xFF);   // 0xFF for Broadcast
  Serial.write(0xFF);   // 0xFF for Broadcast

  // 16 bit of recipient or 0xFFFE
  Serial.write(0xFF);
  Serial.write(0xFE);

  Serial.write(0x02);        // 0x02 to apply these chages
  
  // command name in ASCII character
  Serial.write('D');          // AT commnad D4
  Serial.write('4');

  // command data in as many bytes as needed
  Serial.write(value);

  // checksum is all bytes after length bytes
  long sum = 0x17 + 0x01 + 0xFF + 0xFF + 0xFF + 0xFE + 0x02 + 'D' + '4' + value;
  Serial.write( 0xFF - ( sum & 0xFF) );  // calculate the proper checlsum
  
}

void loop ()
{
  delay(10
  00);
  switchPressed ();
  // AT Command Response
  do{
    // do nothing
    //digitalWrite (LED, HIGH);
  } while (Serial.available() < 8);  //Make sure the frame is all there
  
  if (Serial.read()==0x7E){     // 0-th offset, 7E is the start byte
    for (int i = 1; i<7; i++){
      byte discardByte = Serial.read();      
    }
    int commandStatus = Serial.read(); // Read 7-th offset
  }
  if (commandStatus != 0){
    for (int i=1; i<=5; i++){
      digitalWrite (LED, HIGH);
      delay(500);
      digitalWrite (LED, LOW);
      delay(500);
    }
  }
  delay(5000);
} 
