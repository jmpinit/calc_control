//#define DEBUG

#define TI_RING 2  // White
#define TI_TIP  3  // Red

#define MACHINE_ID 0x23 //PC

// Command codes
#define CMD_VAR   0x06  //Includes a std variable header (used in receiving)
#define CMD_CTS   0x09  //Clear to send: OK to send a variable
#define CMD_DATA  0x15  //Xmit Data Packet (pure data)
#define CMD_FLASH 0x2D  //Used for sending a FLASH app
#define CMD_EXIT  0x36  //Skip/exit - a four byte rejection code (*)
#define CMD_ACK   0x56  //Acknowledgment
#define CMD_ERR   0x5A  //Checksum error: send last packet again
#define CMD_RDY   0x68  //Check if ready
#define CMD_SCR   0x6D  //Request screenshot
#define CMD_CMD   0x87  //Direct command (for remote control for instance)
#define CMD_EOT   0x92  //End Of Transmission: no more variables to send
#define CMD_REQ   0xA2  //Request variable - includes a standard variable
#define CMD_RTS   0xC9  //Request to send - includes a padded variable header

void setup() {
  Serial.begin(115200);
  
  tilpHigh(TI_RING);
  tilpHigh(TI_TIP);
  
  pinMode(13, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    char c = Serial.read();
    pressKey(c);
  }
}

void pressKey(unsigned int button) {
  unsigned char response[4];
  unsigned char p[] = {MACHINE_ID, CMD_CMD, 0x00, 0x00};
  
  p[2] = button&0x00FF;
  p[3] = button>>8;
  
  sendPacketRaw(p, 4);
  
  for(unsigned char i=0; i<4; i++) {
    response[i] = rx();
  }
  
  for(unsigned char i=0; i<4; i++) {
    response[i] = rx();
  }
}

unsigned int sendPacketRaw(unsigned char* data, unsigned int len) {
  unsigned int checksum = 0;
  for(unsigned int i=0; i<len; i++) {
    tx(~data[i]);
    checksum += data[i];
  }
  return checksum;
}

void tilpHigh(unsigned int pin) {
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}

void tilpLow(unsigned int pin) {
  digitalWrite(pin, LOW);
  pinMode(pin, OUTPUT);
}

void tilpWaitHigh(unsigned int pin) {
  while(!digitalRead(pin)) { __asm__("nop\n\t"); }
}

void tilpWaitLow(unsigned int pin) {
  while(digitalRead(pin)) { __asm__("nop\n\t"); }
}

unsigned char rx() {
  unsigned char val = 0;
  
  for(unsigned char b=0; b<8; b++) {
    while(digitalRead(TI_RING)&&digitalRead(TI_TIP)) { __asm__("nop\n\t"); }
    
    val = val>>1;
    if(digitalRead(TI_TIP)) {
      //received 1
      val |= 0x80;
      
      tilpLow(TI_TIP);
      tilpWaitHigh(TI_RING);
      tilpHigh(TI_TIP);
    } else {
      //received 0
      tilpLow(TI_RING);
      tilpWaitHigh(TI_TIP);
      tilpHigh(TI_RING);
    }
  }
  
  tilpHigh(TI_RING);
  tilpHigh(TI_TIP);
  
  return val;
}

void txInt(unsigned int data) {
  tx(data&0xFF);
  tx(data>>8);
}

void tx(unsigned char msg) {
  Serial.print("tx: ");
  Serial.println(msg, HEX);
  
  for(int bit=0; bit<8; bit++) {
    if(msg&(1<<bit)) {
      tilpLow(TI_TIP);
      tilpWaitLow(TI_RING);
      tilpHigh(TI_TIP);
      tilpWaitHigh(TI_RING);
    } else {
      tilpLow(TI_RING);
      tilpWaitLow(TI_TIP);
      tilpHigh(TI_RING);
      tilpWaitHigh(TI_TIP);
    }
  }
  
  tilpHigh(TI_RING);
  tilpHigh(TI_TIP);
}

