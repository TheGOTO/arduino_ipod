

#include <ipod.h>




//#define DEBUG
//#define DEBUG2



// ipod mode 2 commands that we use
byte cmd_switchMode2[2]= {1,2};
byte  cmd_switchMode4[2] = {1, 4};
const byte cmd_get_current_mode_status = 0x03;

//command 2
byte cmd_playPause[2]= {0,1};
byte cmd_buttonRelease[2]= {0,0};

//command 4
const byte  cmd_playback = 0x29;
const byte track_back = 0x04;
const byte track_play = 0x01;
const byte track_stop = 0x02;
const byte track_next = 0x03;
const byte fr_start = 0x06;
const byte ff_start = 0x05;
const byte ff_stop = 0x07;
const byte arduino_reset=0x0F;

const byte  cmd_get_ipodname =0x14;
const byte cmd_pollingmode =0x26;
const byte cmd_get_playlist_pos =0x1E;
const byte cmd_get_time_and_status =0x1c;
const byte cmd_get_shuffle_mode=0x2c;
const byte cmd_get_repeat_mode=0x2f;
const byte cmd_get_total_in_playlist=0x35;
const byte cmd_get_song_title =0x20;
const byte cmd_get_artist =0x22;
const byte cmd_get_album =0x24;

const int lf = 10;

const byte rows = 4; //four rows
const byte cols = 4; //three columns
const int polling_period=5000;//millis
unsigned long lastMillis=0;
unsigned long lastMillis_display=0;



const byte cmd_header_byte0 = 0xff;
const byte cmd_header_byte1 = 0x55;

bool firstConnected=true;
bool isPlaying=false;
bool isStoped=false;
bool isFForFR = false;



String ipodname;
byte currentmode;


byte track_time[size_int_32];
byte track_length[size_int_32];
byte track_status;
byte shuffle_mode;
byte repeate_mode;

byte input_array[255 + 7];// 2 for header + 1 for length + 1 for mode + 2 for command + up to 255 for params + 1 checkSum
byte output_array[255];// with the params needed and send it to the iPod





void setup() {
  // setup serial port
  Serial.begin(9600);
  display_setup();
  
}

void loop()
{
  init_ipod();
  read_ipod();

  poll_ipod();
  update_display(playlist_pos,playlist_total);
}

void read_ipod()
{
  
  int amount = Serial.readBytesUntil(lf, input_array, sizeof(input_array));

  if (amount > 0)
  {   
    #ifdef DEBUG   
    String response="#"+String(amount)+" <-";
    
    for (int k = 0; k < amount; k++)
    {        
      response += String(input_array[k],HEX)+":";
    }         
     write_to_display(response);
    
    #endif
    
    byte current_cmd=input_array[pos_cmd]-1;//response id is command id +1
    switch(current_cmd)
    {
      case cmd_get_ipodname:
        ipodname=result_toASCII(input_array,amount);      
        write_to_display(ipodname);
        break;   
      case cmd_pollingmode:
        write_to_display(result_toString(input_array,pos_data,amount));
        break;
      case cmd_get_playlist_pos:
        memcpy(playlist_pos, input_array+pos_data, size_int_32);//copy playlist position

        sendToPod(4,cmd_get_song_title, playlist_pos, size_int_32);
        sendToPod(4,cmd_get_artist, playlist_pos, size_int_32);
        sendToPod(4,cmd_get_album, playlist_pos, size_int_32);

        //write_to_display(result_toString(input_array,amount));
        break;
      case cmd_get_song_title:
    	write_to_display(result_toASCII(input_array,amount));
        break;
      case cmd_get_artist:
    	write_to_display(result_toASCII(input_array,amount));
    	break;
      case cmd_get_album:
        //write_to_display(result_toASCII(input_array,amount));
        break;
      case cmd_get_time_and_status:
        //write_to_display(result_toASCII(input_array,amount));
    	memcpy(track_length, input_array+pos_data, size_int_32);//copy length position
    	memcpy(track_time, input_array+pos_data+size_int_32, size_int_32);//copy time position
    	track_status=input_array[pos_data+size_int_32+1];
        break;
      case cmd_get_shuffle_mode:
        //write_to_display(result_toASCII(input_array,amount));
    	shuffle_mode=input_array[pos_data];
        break;
      case cmd_get_repeat_mode:
        //write_to_display(result_toASCII(input_array,amount));
    	repeate_mode=input_array[pos_data];
        break;
      case cmd_get_total_in_playlist:
        //write_to_display(result_toASCII(input_array,amount));
		memcpy(playlist_total, input_array+pos_data, size_int_32);//copy length position
        break;
    }    

  if(input_array[pos_mode]==0x00)//mode 0 cmd
  {
    //specal case command get mode
	  if (input_array[pos_cmd - 1] == 0x04 && input_array[pos_cmd] == 0x01)
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




void poll_ipod()
{
	 if (millis() - lastMillis_display > polling_period)
	 {
		 sendToPod(4,cmd_get_playlist_pos, output_array, 0);
		 sendToPod(4,cmd_get_time_and_status,output_array, 0);
		 sendToPod(4,cmd_get_shuffle_mode,output_array, 0);
		 sendToPod(4,cmd_get_repeat_mode,output_array, 0);
		 sendToPod(4,cmd_get_total_in_playlist,output_array, 0);
		 sendToPod(4,cmd_get_ipodname,output_array, 0);

	     lastMillis_display = millis();
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
        sendToPod(0, cmd_switchMode4, output_array, 0);// setup the ipod for mode 4
      }    
     sendGetModeToPod();     
     lastMillis = millis();
  } 
}

//0xFF, 0x55, 0x02, 0x00, 0x03, 0xFB
// builds the msg that we want to send to the ipod
void sendGetModeToPod()
{
	input_array[0] = cmd_header_byte0;
	input_array[1] = cmd_header_byte1;
	input_array[2] = 0x02; // SIZE
	input_array[3] = 0x00; //mode
	input_array[4] = cmd_get_current_mode_status; //command
	input_array[5] = 0xFB;


  String response="-> ";
  // send the message to the ipod!
  for (int j = 0; j < 6; j++)
  {    
	  response += String(input_array[j],HEX)+":";
      Serial.write(input_array[j]);    
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
  input_array[0] = cmd_header_byte0;
  input_array[1] = cmd_header_byte1;
  input_array[2] = (byte)1 + 2 + paramLength; // SIZE
  input_array[3] = mode; //mode
  input_array[4] = cmd[0]; //command
  input_array[5] = cmd[1]; //command

  // for now, length will always be zero.  if we entered mode 4 this might change...
  if (paramLength == 0)
  {
    input_array[6] = 0;
  }
  else
  {
    for (int j = 0; j < paramLength; j++)
    {
      input_array[6 + j] = parameter[j];
    }
  }

  String response="-> ";
  // load up the checksum
  input_array[6 + paramLength] = getCheckSum(paramLength);



  // send the message to the ipod!
  for (int j = 0; j < 7 + paramLength; j++)
  {
	  response += String(input_array[j],HEX)+":";
      Serial.write(input_array[j]);
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
    checkSum = checkSum + input_array[k];

  return 0x100 - (checkSum & 0xFF);

}


//When the iPod is first recognized by the Arduino, this function puts the iPod into a ready to play state
void kickStart(){
  
    
  //In order to get into a play state, Mode 2 is first enabled then the play command for mode 2 is sent
  //Sending the play command while in Mode4 does nothing if the iPod has no song that it is currently playing
  //After sending the Mode2 Play command, the iPod is then switched back into Mode4 for the advanced controls
  //After this switch up, the begin polling command is sent. Polling data is only sent when playing
  sendToPod(0, cmd_switchMode2, output_array, 0);// setup the ipod for mode 4
  
  
  //The delay times were "calculated" from trial and error
  //With these delays the iPod is able to handle switching modes and accepting commands
  delay(50);
  sendToPod(2, cmd_playPause, output_array, 0);// setup the ipod for mode 4
  sendToPod(2, cmd_buttonRelease, output_array, 0);// setup the ipod for mode 4
  delay(1000);
  sendToPod(0, cmd_switchMode4, output_array, 0);// setup the ipod for mode 4
  delay(100);


 output_array[0] = 0x01; //start polling
 sendToPod(4, cmd_pollingmode, output_array, 1);
 delay(2000);
 
  //Polling was enabled so the boolean check bit pollingOn is set to true

  
  //Set first connected to false to prevent the kickstart function from being performed again 
  firstConnected = false;
}


void reset()
{
	asm volatile ("  jmp 0");
}









