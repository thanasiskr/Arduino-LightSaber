#include <FastLED.h>
#define LED_PIN    5
#define NUM_LEDS    30
#include "SoftwareSerial.h"
# define Start_Byte 0x7E
# define Version_Byte 0xFF
# define Command_Length 0x06
# define End_Byte 0xEF
# define Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]


# define ACTIVATED LOW

CRGB leds[NUM_LEDS];
SoftwareSerial mySerial(10, 11);

                                
                                
int is_on=0;                                
int sel=1;
const int button = 3;        
const int button1=8;


struct colours{
  char *mod_name;
  int red;
  int blue;
  int green;
  
  
};
typedef struct colours colours_t;
colours_t modes[4]={
      {"Sith",255,0,0},
      {"Jedai",0,100,0},
      {"Yoda",0,0,100},
      {"Samuel Jackson",100,100,0}
      
}; 



void setup () {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  lightsOff();                  //lets close all leds
  Serial.begin(9600);
  pinMode(button, INPUT);
  pinMode(button1,INPUT);
  mySerial.begin (9600);
  
  setVolume(20);
  delay(100);
}



void loop () {
  if (digitalRead(button)==HIGH){     //!
    if (is_on==0){
      Serial.println("ON");
      lightsOn();
      OpenSaber();
      is_on=1;
      Hum();  
    }
    else{
      Serial.println("OFF");
      CloseSaber();
      lightsOff();
      
      is_on=0;
    }
  }
  if(digitalRead(button1)==HIGH){
     
    Serial.println("sel++");
    if (sel<3){
      sel++;
      delay(200);   
    }
    else{
      sel=0; 
      delay(200); 
    } 
  }
  
  //delay(100);
 
}

//Lights--------------------------------------------
void lightsOn(){
   for (int i=0;i<NUM_LEDS;i++){
      leds[i] = CRGB(modes[sel].red,modes[sel].green,modes[sel].blue);
      FastLED.show();
      delay(10);  
   }
   is_on=1;
}

void lightsOff(){
  for (int i=NUM_LEDS;i>=0;i--){
      leds[i] = CRGB(0, 0, 0);
      FastLED.show();
      delay(10);  
  }  
   is_on=0;
}
//Sounds---------------------------------------------
void OpenSaber(){
  Serial.println("music");
  execute_CMD(0x03,0,1); //3 hum,//2 open,//1close                                              
  delay(1000);        //wait for sound to end
}

void CloseSaber(){
  setVolume(20); 
  execute_CMD(0x03,0,3); //3 hum,//2 open,//1close                                             
  delay(1000);            //wait for sound to end
}  
void Hum(){
  setVolume(50);                                          
  execute_CMD(0x03,0,2); //3 hum,//2 open,//1close
  delay(500);             
}  
void playFile(int num){
  execute_CMD(0x03,0,num); //3 hum,//2 open,//1close
  delay(500);
  }
void pause()
{
  execute_CMD(0x0E,0,0);
  delay(500);
}

void play()
{
  execute_CMD(0x0D,0,1); 
  delay(500);
}

void playNext()
{
  execute_CMD(0x01,0,1);
  delay(500);
}


void setVolume(int volume)
{
  execute_CMD(0x06, 0, volume); // Set the volume (0x00~0x30)
  delay(500);             
}

void execute_CMD(byte CMD, byte Par1, byte Par2)
// Excecute the command and parameters
{
  // Calculate the checksum (2 bytes)
  word checksum = -(Version_Byte + Command_Length + CMD + Acknowledge + Par1 + Par2);
  // Build the command line
  byte Command_line[10] = { Start_Byte, Version_Byte, Command_Length, CMD, Acknowledge,
  Par1, Par2, highByte(checksum), lowByte(checksum), End_Byte};
  //Send the command line to the module
  for (byte k=0; k<10; k++)
  {
  mySerial.write( Command_line[k]);
  }
}
