#define TI_RING 2  //white
#define TI_TIP  3  //red

#define MACHINE_ID 0x23 //PC

//command codes
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

//rejection codes
#define REJ_EXIT  0x01  //EXIT, the entire transfer has been cancelled
#define REJ_SKIP  0x02  //SKIP, the current var has been skipped by the user
#define REJ_MEM   0x03  //MEM, out of memory

//keys
#define KEY_GRAPH  0x44
#define KEY_DRAW   0x2F
#define KEY_ENTER  0x05

#define KEY_RIGHT  0x01
#define KEY_LEFT   0x02
#define KEY_UP     0x03
#define KEY_DOWN   0x04

//variable types
#define TYPE_PRGM  0x06
#define TYPE_PIC   0x07

unsigned char info[768];
unsigned char packet[4];
unsigned int len = 0;

#define LEN_PROG 222

unsigned char rawData[222] = {

	0xDC, 0x00, 0x2A, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49,

	0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55,

	0x56, 0x57, 0x58, 0x59, 0x5A, 0x2A, 0x04, 0xAA, 0x00, 0x3F, 0x31, 0x04,

	0x53, 0x3F, 0x31, 0x04, 0x54, 0x3F, 0xE1, 0x3F, 0xD6, 0x31, 0x3F, 0xBB,

	0x6A, 0x5F, 0x52, 0x58, 0x3F, 0x72, 0x04, 0x41, 0x3F, 0x41, 0x04, 0x42,

	0x3F, 0xCE, 0x41, 0x6E, 0x36, 0x35, 0x40, 0x41, 0x6D, 0x39, 0x30, 0x3F,

	0x41, 0x71, 0x36, 0x34, 0x04, 0x42, 0x3F, 0xCE, 0x41, 0x6E, 0x39, 0x37,

	0x40, 0x41, 0x6D, 0x31, 0x32, 0x32, 0x3F, 0x41, 0x71, 0x39, 0x36, 0x04,

	0x42, 0x3F, 0xD6, 0x32, 0x3F, 0xCE, 0x42, 0x6A, 0x33, 0x32, 0x3C, 0x42,

	0x6A, 0x31, 0x32, 0x37, 0x3F, 0xCF, 0x3F, 0xCE, 0x42, 0x6A, 0x33, 0x32,

	0x3F, 0x53, 0x70, 0x31, 0x04, 0x53, 0x3F, 0xCE, 0x42, 0x6A, 0x31, 0x32,

	0x37, 0x3F, 0xCF, 0x3F, 0x53, 0x71, 0x31, 0x04, 0x53, 0x3F, 0xCE, 0x53,

	0x6A, 0x30, 0x3F, 0xCF, 0x3F, 0x31, 0x36, 0x04, 0x53, 0x3F, 0x54, 0x71,

	0x31, 0x04, 0x54, 0x3F, 0xD4, 0x3F, 0xE0, 0x54, 0x2B, 0x53, 0x2B, 0x2A,

	0x29, 0x2A, 0x11, 0x3F, 0xD4, 0x3F, 0xD0, 0x3F, 0xE0, 0x54, 0x2B, 0x53,

	0x2B, 0xBB, 0x0C, 0xAA, 0x00, 0x2B, 0x42, 0x2B, 0x31, 0x11, 0x11, 0x3F,

	0x53, 0x70, 0x31, 0x04, 0x53, 0x3F, 0xD4, 0x3F, 0xCE, 0x53, 0x6A, 0x31,

	0x37, 0x3F, 0xCF, 0x3F, 0x31, 0x04, 0x53, 0x3F, 0x54, 0x70, 0x31, 0x04,

	0x54, 0x3F, 0xD4, 0x3F, 0xD7, 0x31

};

#define DEBUG

void setup()
{
  Serial.begin(115200);
  
  tilpHigh(TI_RING);
  tilpHigh(TI_TIP);
  
  pinMode(13, OUTPUT);
}

void loop() {
  if(Serial.available()>0) {
    char c = Serial.read();
    
    //send a program
    unsigned char name[8] = {'T', 'E', 'R', 'M', 0x00, 0x00, 0x00, 0x00};
    unsigned int chk = 0;
    tx(MACHINE_ID);
    tx(CMD_VAR);
    tx(13);  //packet len
    tx(0);
    tx(11);  //data len
    chk += 11;
    tx(0);
    tx(5);   //var type
    chk += 5;
    chk += sendPacketRaw(name, 8);
    tx(0);
    tx(0);
    txInt(chk);//checksum
    
    //sendPacket(CMD_VAR, 13, request);
    
    receivePacket();
    //receivePacket();
    //rx(); rx(); rx(); rx();
    
    //sendPacket(CMD_ACK, 0, 0);
    
    //PACKET HEADER
    tx(MACHINE_ID);
    tx(CMD_DATA);
    txInt(LEN_PROG+2);
    
    tx(0xDE);
    tx(0x00);
    unsigned int check = sendPacketRaw(rawData, LEN_PROG)+0xDE;
    
    //send checksum
    txInt(check);
    
    receivePacket();
    
    delay(5000);
    
    sendPacket(CMD_RDY, 0, 0);
    receivePacket(); //hopefully hear that it went okay?
    
    sendPacket(CMD_EOT, 0, 0);
  }
}

/*
void sendPic() {
  tx(MACHINE_ID);
    tx(CMD_DATA);
    tx((57+758+17)&0xFF); tx((57+758+17)>>8);
  //BEGIN FILE HEADER
    unsigned int outsum = 0;  //the checksum for the whole message
    
    //sig
    chkTx('*', &outsum);
    chkTx('*', &outsum);
    chkTx('T', &outsum); chkTx('I', &outsum); chkTx('8', &outsum); chkTx('3', &outsum); chkTx('F', &outsum);
    chkTx('*', &outsum);
    //magic sig
    chkTx(0x1A, &outsum); chkTx(0x0A, &outsum); chkTx(0x00, &outsum);
    //comment
    for(unsigned int i=0; i<42; i++) {
      tx(0x00);
    }
    //length of variable section
    chkTx((758+17)&0xFF, &outsum); chkTx((758+17)>>8, &outsum);
    
    //VARIABLE SECTION!
    unsigned int sum = 0; //inner checksum
    
    chkTx(0x11, &sum); chkTx(0x00, &sum); //idk wtf this is, but we need it
    
    //len of variable data
    chkTx(758&0xFF, &sum); chkTx(758>>8, &sum);
    
    //variable type id
    chkTx(0x07, &sum);
    
    //variable name
    chkTx(0x06, &sum); chkTx(0x08, &sum);
    tx(0x00); tx(0x00); tx(0x00); tx(0x00); tx(0x00); tx(0x00);
    
    //version
    tx(0x00);
    
    //flag (archived/not)
    tx(0x00);
    
    //len of variable data again
    chkTx(758&0xFF, &sum); chkTx(758>>8, &sum);
    
    //len of an image
    chkTx(756&0xFF, &sum); chkTx(756>>8, &sum);
    
    //the image
    for(unsigned int i=0; i<756; i++) {
      chkTx(0xAA, &sum);
    }
    
    //END VARIABLE SECTION
    
    tx(sum&0xFF); tx(sum>>8);
    
    //END FILE
    
    outsum += sum;
    
    //send checksum
    tx(outsum&0xFF); tx(outsum>>8);
}*/

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

void receivePacket() {
  Serial.println("receiving packet");
  for(unsigned char i=0; i<4; i++) {
    packet[i] = rx();
    Serial.println(packet[i], HEX);
  }
  
  #ifdef DEBUG
  Serial.print("packet received with data of len ");
  #endif
  
  len = (packet[3]<<8)|packet[2];
  
  #ifdef DEBUG
  Serial.print(len, DEC);
  Serial.println();
  #endif
  
  for(unsigned char i=0; i<len; i++) {
    info[i] = rx();
    Serial.println(info[i], HEX);
  }
  if(len>0) {
    #ifdef DEBUG
    Serial.println("data read");
    #endif
    
    unsigned char CRCL = rx();
    unsigned char CRCH = rx();
  }
}

unsigned int sendPacketRaw(unsigned char* data, unsigned int len) {
  unsigned int checksum = 0;
  for(unsigned int i=0; i<len; i++) {
    tx(data[i]);
    checksum += data[i];
  }
  return checksum;
}

void sendPacket(unsigned char cmd, unsigned int len, unsigned char* data) {
  tx(MACHINE_ID);
  tx(cmd);
  
  //tell calc how much data to expect
  tx(len&0x0F);  //LSB
  tx(len>>8);    //MSB
  
  //send data and calc checksum
  unsigned int checksum = 0;
  for(unsigned int i=0; i<len; i++) {
    checksum += data[i];
    tx(data[i]);
  }
  
  //send checksum
  if(len>0) {
    tx(checksum&0xFF);  //LSB
    tx(checksum>>8);    //MSB
  }
}

void chkTx(unsigned char data, unsigned int* checksum) {
  tx(data);
  (*checksum) += data;
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

#define DELAY 50
unsigned char rx() {
  unsigned char val = 0;
  
  for(unsigned char b=0; b<8; b++) {
    while(digitalRead(TI_RING)&&digitalRead(TI_TIP)) { __asm__("nop\n\t"); }
    
    val = val>>1;
    if(digitalRead(TI_TIP)) {
      //received 1
      delayMicroseconds(DELAY);
      
      tilpLow(TI_TIP);
      tilpWaitHigh(TI_RING);
      tilpHigh(TI_TIP);
    } else {
      //received 0
      val |= 0x80;
      
      delayMicroseconds(DELAY);
      
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
      delayMicroseconds(DELAY);
      tilpWaitLow(TI_RING);
      delayMicroseconds(DELAY);
      tilpHigh(TI_TIP);
      delayMicroseconds(DELAY);
      tilpWaitHigh(TI_RING);
      delayMicroseconds(DELAY);
    } else {
      tilpLow(TI_RING);
      delayMicroseconds(DELAY);
      tilpWaitLow(TI_TIP);
      delayMicroseconds(DELAY);
      tilpHigh(TI_RING);
      delayMicroseconds(DELAY);
      tilpWaitHigh(TI_TIP);
      delayMicroseconds(DELAY);
    }
  }
  
  tilpHigh(TI_RING);
  tilpHigh(TI_TIP);
}


