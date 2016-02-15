#include <Adafruit_SSD1306.h>
#include <Keypad.h>

#define DEBUG

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

// ipod mode 2 commands that we use
byte cmd_switchMode2[2]= {1,2};
byte  cmd_switchMode4[2] = {1, 4};
const byte cmd_getcurrentmodestatus = 0x03;

//command 2
byte cmd_playPause[2]= {0,1};
byte cmd_buttonRelease[2]= {0,0};

//command 4
const byte  cmd_playback = 0x29;
//0x01 = Play/Pause
//0x02 = Stop
//0x03 = Skip++
//0x04 = Skip--
//0x05 = FFwd
//0x06 = FRwd
//0x07 = StopFF/RW
//byte switchtoMainPL[2]{0x00,0x16};
const byte  cmd_ipodname =0x14;
//byte timeelapsed[2]= {0x00,0x27};
//byte getstatus[2]={0x00,0x1C};
const byte cmd_pollingmode =0x26;
const byte cmd_playlistpos =0x1E;
//byte getshufflemode[2]={0x00,0x2C};
const byte  cmd_switchto = 0x17;
const byte  cmd_getSongTitle =  0x20;


const int lf = 10;
const byte length_pos=2;
const byte mode_pos=3;
const byte cmd_pos=5;
const byte rows = 4; //four rows
const byte cols = 4; //three columns
const int polling_period=5000;//millis
unsigned long lastMillis=0;
byte linecount=0;


const int track_back = 0x04;
const int track_play = 0x01;
const int track_stop = 0x02;
const int track_next = 0x03;
const int ff_stop = 0x07;
const int fr_start = 0x06;
const int ff_start = 0x05;
const int cmd_header_byte0 = 0xff;
const int cmd_header_byte1 = 0x55;

bool firstConnected=true;
bool isPlaying=false;
bool isStoped=false;
bool isFForFR = 0;


//tool -> http://javl.github.io/image2cpp/
//Brightness =50
//Canvas 64*32
//inver colors
const unsigned char bmw_logo [] PROGMEM = {
0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x1f, 0xf8, 0x18, 0x00, 0x00,
0x00, 0x01, 0x8f, 0xf3, 0xc7, 0xf1, 0x80, 0x00, 0x00, 0x0c, 0xff, 0xf1, 0x87, 0xff, 0x30, 0x00,
0x00, 0x23, 0xff, 0xf4, 0x27, 0xff, 0xc4, 0x00, 0x00, 0x9f, 0xff, 0xf6, 0x67, 0xff, 0x99, 0x00,
0x03, 0x60, 0xff, 0xff, 0xff, 0xfe, 0x3e, 0xc0, 0x04, 0x9c, 0x1f, 0x1f, 0x00, 0xfc, 0x03, 0x20,
0x09, 0x13, 0x31, 0xff, 0x00, 0x08, 0x47, 0x90, 0x17, 0xc2, 0x4f, 0xff, 0x00, 0x03, 0x80, 0xe8,
0x27, 0xf9, 0xbf, 0xff, 0x00, 0x01, 0x8f, 0xe4, 0x4f, 0xfe, 0x7f, 0xff, 0x00, 0x00, 0x7f, 0xf2,
0x5f, 0xfc, 0xff, 0xff, 0x00, 0x00, 0x3f, 0xfa, 0x9f, 0xfd, 0xff, 0xff, 0x00, 0x00, 0x3f, 0xf9,
0x9f, 0xf9, 0xff, 0xff, 0x00, 0x00, 0x1f, 0xf9, 0xbf, 0xfb, 0xff, 0xff, 0x00, 0x00, 0x1f, 0xfd,
0xbf, 0xf8, 0x00, 0x00, 0xff, 0xff, 0xdf, 0xfd, 0x9f, 0xf8, 0x00, 0x00, 0xff, 0xff, 0x9f, 0xf9,
0x9f, 0xfc, 0x00, 0x00, 0xff, 0xff, 0xbf, 0xf9, 0x5f, 0xfc, 0x00, 0x00, 0xff, 0xff, 0x3f, 0xfa,
0x4f, 0xfe, 0x00, 0x00, 0xff, 0xfe, 0x7f, 0xf2, 0x27, 0xff, 0x00, 0x00, 0xff, 0xfc, 0xff, 0xe4,
0x17, 0xff, 0xc0, 0x00, 0xff, 0xf3, 0xff, 0xe8, 0x09, 0xff, 0xf0, 0x00, 0xff, 0x8f, 0xff, 0x90,
0x04, 0xff, 0xfe, 0x00, 0xf8, 0x7f, 0xff, 0x20, 0x03, 0x7f, 0xff, 0xfe, 0x7f, 0xff, 0xfe, 0xc0,
0x00, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xf9, 0x00, 0x00, 0x27, 0xff, 0xff, 0xff, 0xff, 0xe4, 0x00,
0x00, 0x0c, 0xff, 0xff, 0xff, 0xff, 0x30, 0x00, 0x00, 0x01, 0x8f, 0xff, 0xff, 0xf1, 0x80, 0x00,
0x00, 0x00, 0x18, 0x3f, 0xfc, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00,

};




byte msg[255 + 7];// 2 for header + 1 for length + 1 for mode + 2 for command + up to 255 for params + 1 checkSum
byte params[255];// with the params needed and send it to the iPod
int incomingByte = 0;
String ipodname;
byte currentmode;
char  keys[rows][cols] = {
  {'0', '1', '2', '3'},
  {'4', '5', '6', '7'},
  {'8', '9', 'a', 'b'},
  {'c', 'd', 'e', 'f'}
};
byte colPins[cols] = {9, 10, 11, 12}; //connect to the row pinouts of the keypad
byte rowPins[rows] = {5, 6, 7, 8}; //connect to the column pinouts of the keypad


Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );


void setup() {
  // setup serial port
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.clearDisplay();
  display.drawBitmap(30, 0,  bmw_logo, 64, 32, 1);
  display.display();
  delay(2000);
  display.clearDisplay();
  
}

void loop()
{
  init_ipod();
  read_from_ipod();
  read_keypad();  
}

void read_from_ipod()
{
  
  int amount = Serial.readBytesUntil(lf, msg, sizeof(msg));

  if (amount > 0)
  {   
    #ifdef DEBUG   
    String response="#"+String(amount)+" <-";
    
    for (int k = 0; k < amount; k++)
    {        
      response += String(msg[k],HEX)+":";
    }         
     write_to_display(response);
    
    #endif
    
    byte current_cmd=msg[cmd_pos]-1;//response id is command id +1
    switch(current_cmd)
    {
      case cmd_ipodname:
        ipodname=result_toASCII(msg,amount);      
        write_to_display(ipodname);
        break;   
      case cmd_pollingmode:
        write_to_display(result_toString(msg,amount));
        break;
      case cmd_playlistpos:
        write_to_display(result_toString(msg,amount));
        break;      
    }    

  if(msg[mode_pos]==0x00)//mode 0 cmd
  {
    //specal case command get mode
			if (msg[cmd_pos - 1] == 0x04 && msg[cmd_pos] == 0x01)
      {      
        currentmode=cmd_switchMode4[1];
      }
      else
      {
        currentmode=0x00;//reset mode 
        firstConnected=false;//reset connection status       
      }
  }   
  }
}


void read_keypad()
{
  char key = keypad.getKey();

  if (key != NO_KEY)
  {

    switch (key)
    {
      case '0':
			params[0] = track_back; //track--
        sendToPod(4, cmd_playback, params, 1);
        break;
      case '1':       
        if(isStoped)
        {
          kickStart();
        }        
        params[0] = track_play; //play/pause
        sendToPod(4, cmd_playback, params, 1);
        isPlaying=true;
        break;
      case '2':
			params[0] = track_stop; //stop
        sendToPod(4, cmd_playback, params, 1);
        isStoped=true;
        break;
      case '3':
			params[0] = track_next; //track++
        sendToPod(4, cmd_playback, params, 1);

        sendToPod(4, cmd_playlistpos, params, 0);
        
        break;
      case '4':
        if (isFForFR)
        {
				params[0] = ff_stop; //stop FF
          isFForFR = false;
        }
        else
        {
				params[0] = fr_start; //FRwd
          isFForFR = true;
        }
        sendToPod(4, cmd_playback, params, 1);
        break;
      case '5':
        break;
      case '6':       
        sendToPod(4, cmd_ipodname, params, 0);
        break;
      case '7':
        if (isFForFR)
        {
				params[0] = ff_stop; //stop
          isFForFR = false;
        }
        else
        {
				params[0] = ff_start; //FFwd
          isFForFR = true;
        }
        sendToPod(4, cmd_playback, params, 1);
        break;
    }    
  }

}


/*
 * poll the ipod every polling_period
 */
void init_ipod()
{

  if(firstConnected)
  {
   kickStart();
  }

  
  if (millis() - lastMillis > polling_period)
  {

      if(currentmode!=cmd_switchMode4[1])
      {  
        sendToPod(0, cmd_switchMode4, params, 0);// setup the ipod for mode 4
      }    
     sendGetModeToPod();     
     lastMillis = millis();
  } 
}

//0xFF, 0x55, 0x02, 0x00, 0x03, 0xFB
// builds the msg that we want to send to the ipod
void sendGetModeToPod()
{
	msg[0] = cmd_header_byte0;
	msg[1] = cmd_header_byte1;
	msg[2] = 0x02; // SIZE
  msg[3] = 0x00; //mode
  msg[4] = cmd_getcurrentmodestatus; //command
  msg[5] = 0xFB; 


  String response="-> ";
  // send the message to the ipod!
  for (int j = 0; j < 6; j++)
  {    
	  response += String(msg[j],HEX)+":";
      Serial.write(msg[j]);    
  }

	#ifdef DEBUG
    write_to_display(response);
   	#endif

  
}

// builds the msg that we want to send to the ipod
void sendToPod(byte mode, byte cmd, byte parameter[], byte paramLength)
{
  byte cmdarray[2]={0x00,cmd};
  
  sendToPod(mode,cmdarray,parameter,paramLength);
}

// builds the msg that we want to send to the ipod
void sendToPod(byte mode, byte cmd[], byte parameter[], byte paramLength)
{
	msg[0] = cmd_header_byte0;
	msg[1] = cmd_header_byte1;
  msg[2] = (byte)1 + 2 + paramLength; // SIZE
  msg[3] = mode; //mode
  msg[4] = cmd[0]; //command
  msg[5] = cmd[1]; //command

  // for now, length will always be zero.  if we entered mode 4 this might change...
  if (paramLength == 0)
  {
    msg[6] = 0;
  }
  else
  {
    for (int j = 0; j < paramLength; j++)
    {
      msg[6 + j] = parameter[j];
    }
  }

  String response="-> ";
  // load up the checksum
  msg[6 + paramLength] = getCheckSum(paramLength);



  // send the message to the ipod!
  for (int j = 0; j < 7 + paramLength; j++)
  {
	  response += String(msg[j],HEX)+":";
      Serial.write(msg[j]);
  }
	#ifdef DEBUG
    write_to_display(response);
   	#endif


}

// calculates and returns the checksum of what is currently in the msg[] buffer
byte getCheckSum(byte paramLength)
{
  byte checkSum = 0;


  for (int k = 2; k < 6 + paramLength; k++)
    checkSum = checkSum + msg[k];

  return 0x100 - (checkSum & 0xFF);

}


//When the iPod is first recognized by the Arduino, this function puts the iPod into a ready to play state
void kickStart(){
  
    
  //In order to get into a play state, Mode 2 is first enabled then the play command for mode 2 is sent
  //Sending the play command while in Mode4 does nothing if the iPod has no song that it is currently playing
  //After sending the Mode2 Play command, the iPod is then switched back into Mode4 for the advanced controls
  //After this switch up, the begin polling command is sent. Polling data is only sent when playing
  sendToPod(0, cmd_switchMode2, params, 0);// setup the ipod for mode 4
  
  
  //The delay times were "calculated" from trial and error
  //With these delays the iPod is able to handle switching modes and accepting commands
  delay(50);
   sendToPod(2, cmd_playPause, params, 0);// setup the ipod for mode 4
   sendToPod(2, cmd_buttonRelease, params, 0);// setup the ipod for mode 4
  delay(1000);
   sendToPod(0, cmd_switchMode4, params, 0);// setup the ipod for mode 4
  delay(100);


 params[0] = 0x01; //start polling
 sendToPod(4, cmd_pollingmode, params, 1);
delay(2000);

   
 
  //Polling was enabled so the boolean check bit pollingOn is set to true

  

  
  //Set first connected to false to prevent the kickstart function from being performed again 
  firstConnected = false;
}

void write_to_display(String value) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  if(linecount<=4)
  {
  display.setCursor(0, 0);
  display.clearDisplay();
  }
  display.setTextWrap(true);
  display.println(value);
  display.display();
  //  delay(1);

  //  display.startscrollright(0x00, 0x0F);
  //  delay(2000);
  //  display.stopscroll();
  //  delay(1000);
  //  display.startscrollleft(0x00, 0x0F);
  //  delay(2000);
  //  display.stopscroll();
  linecount++;
}

String result_toASCII(byte msg[], short array_size)
{
 String response;
   
    for (int k = cmd_pos+1; k < array_size - 1; k++) //ignore headers and checksum   
    {
      response += String((char)msg[k]);     
    }  
    return response;  
}

String result_toString(byte msg[], short array_size)
{   
   String response;
   
    for (int k = cmd_pos+1; k < array_size - 1; k++) //ignore headers and checksum   
    {
      response += String(msg[k]);     
    }  
    return response;  
}

//    byte[] array_copy(byte array[], short startpos, short amount)
//    {
//      byte ret_array[amount];
//      short count=0;
//      
//       for (int k = startpos; k < amount; k++) //ignore headers and checksum   
//        {
//            ret_array[count]=array[k];
//            count++;   
//        }  
//      
//    }



