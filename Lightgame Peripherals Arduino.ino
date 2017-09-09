/*
Copyright 2014 Efstathios Lyberidis & Thodoris Bais

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/


/////////////// Slave Arduino /////////



#include "pitches.h" 
#include <SPI.h>
#include <TFT.h>
#include <Wire.h>  // Arduino serial connection

// lcd pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8  
// pin for speaker, randomly at 7
int speaker_pin=7;  

char getPlayerNumber[5] = {-1,-1,-1,-1,-1} ;

// create an instance of the library
TFT TFTscreen = TFT(cs, dc, rst);

// char array to print to the screen
int counter = 0;
char give_to_lcd[4];

// x,y screen coordinates
int x = 0;
int y = 0;

// can be used as signals to begin playing the appropriate music
volatile int play_music=0;
int correct =0;
int fault =0;
int tension =0;
int outro =0;
int intro =0;

//5 notes
int melody_correct[] = {NOTE_AS6,NOTE_F7,NOTE_AS7,NOTE_DS8,NOTE_D8};

//5 notes
int melody_fault[] = {NOTE_E6,NOTE_A5,NOTE_AS5,NOTE_E5,NOTE_F5};

//8 notes
int melody_tension[] = {NOTE_G6,NOTE_DS7,NOTE_B6,NOTE_G6,NOTE_DS6,NOTE_C6,NOTE_A5,NOTE_G5};

//64 notes
int melody_outro[] = { 
  NOTE_G5,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_G4,NO_SOUND,NOTE_G5,NO_SOUND,
  NOTE_D5,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_D4,NO_SOUND,NOTE_D5,NO_SOUND,
  NOTE_E5,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_E4,NO_SOUND,NOTE_E5,NO_SOUND,
  NOTE_B4,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_B3,NO_SOUND,NOTE_B4,NO_SOUND,
  NOTE_G5,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_C4,NO_SOUND,NOTE_G5,NO_SOUND,
  NOTE_G4,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_G3,NO_SOUND,NOTE_G4,NO_SOUND,
  NOTE_C5,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_C4,NO_SOUND,NOTE_C5,NO_SOUND,
  NOTE_D5,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_D4,NO_SOUND,NOTE_D5,NO_SOUND
};

// 320 notes
int melody_intro[] = {
  NOTE_C5,NOTE_C5,NOTE_C7,NOTE_C6,NOTE_C5,NOTE_C5,NOTE_C7,NOTE_C6,
  NOTE_C5,NOTE_C5,NOTE_C7,NOTE_C6, NOTE_C5,NOTE_C5,NOTE_C7,NOTE_C6,
  NOTE_CS5,NOTE_CS5,NOTE_CS7,NOTE_CS6,NOTE_CS5,NOTE_CS5,NOTE_CS7,NOTE_CS6,
  NOTE_CS5,NOTE_CS5,NOTE_CS7,NOTE_CS6,NOTE_CS5,NOTE_CS5,NOTE_CS7,NOTE_CS6,
  NOTE_F4,NOTE_F4,NOTE_F6,NOTE_F5,NOTE_F4,NOTE_F4,NOTE_F6,NOTE_F5,
  NOTE_F4,NOTE_F4,NOTE_F6,NOTE_F5,NOTE_F4,NOTE_F4,NOTE_F6,NOTE_F5,
  NOTE_GS4,NOTE_GS4,NOTE_GS6,NOTE_GS5,NOTE_GS4,NOTE_GS4,NOTE_GS6,NOTE_GS5,
  NOTE_GS4,NOTE_GS4,NOTE_GS6,NOTE_GS5,NOTE_GS4,NOTE_GS4,NOTE_GS6,NOTE_GS5,
  
  NOTE_C5,NOTE_C5,NOTE_C7,NOTE_C7,NOTE_C5,NOTE_C5,NOTE_C7,NOTE_C7,
  NOTE_C5,NOTE_C5,NOTE_C7,NOTE_C7,NOTE_C5,NOTE_C5,NOTE_C7,NOTE_C7,
  NOTE_CS5,NOTE_CS5,NOTE_CS7,NOTE_CS7,NOTE_CS5,NOTE_CS5,NOTE_CS7,NOTE_CS7,
  NOTE_CS5,NOTE_CS5,NOTE_CS7,NOTE_CS7,NOTE_CS5,NOTE_CS5,NOTE_CS7,NOTE_CS7,
  NOTE_F4,NOTE_F4,NOTE_F6,NOTE_F6,NOTE_F4,NOTE_F4,NOTE_F6,NOTE_F6,
  NOTE_F4,NOTE_F4,NOTE_F6,NOTE_F6,NOTE_F4,NOTE_F4,NOTE_F6,NOTE_F6,
  NOTE_GS4,NOTE_GS4,NOTE_GS6,NOTE_GS6,NOTE_GS4,NOTE_GS4,NOTE_GS6,NOTE_GS6,
  NOTE_GS4,NOTE_GS4,NOTE_GS6,NOTE_GS6,NOTE_GS4,NOTE_GS4,NOTE_GS6,NOTE_GS6,
  
  NOTE_C5,NOTE_C5,NOTE_C7,NO_SOUND,NOTE_C5,NOTE_C5,NOTE_C7,NO_SOUND,
  NOTE_C5,NOTE_C5,NOTE_C7,NO_SOUND,NOTE_C5,NOTE_C5,NOTE_C7,NO_SOUND,
  NOTE_CS5,NOTE_CS5,NOTE_CS7,NO_SOUND,NOTE_CS5,NOTE_CS5,NOTE_CS7,NO_SOUND,
  NOTE_CS5,NOTE_CS5,NOTE_CS7,NO_SOUND,NOTE_CS5,NOTE_CS5,NOTE_CS7,NO_SOUND,
  NOTE_F4,NOTE_F4,NOTE_F6,NO_SOUND,NOTE_F4,NOTE_F4,NOTE_F6,NO_SOUND,
  NOTE_F4,NOTE_F4,NOTE_F6,NO_SOUND,NOTE_F4,NOTE_F4,NOTE_F6,NO_SOUND,
  NOTE_GS4,NOTE_GS4,NOTE_GS6,NO_SOUND,NOTE_GS4,NOTE_GS4,NOTE_GS6,NO_SOUND,
  NOTE_GS4,NOTE_GS4,NOTE_GS6,NO_SOUND,NOTE_GS4,NOTE_GS4,NOTE_GS6,NO_SOUND,
  
  NOTE_C5,NO_SOUND,NOTE_C7,NO_SOUND,NOTE_C5,NO_SOUND,NOTE_C7,NO_SOUND,
  NOTE_C5,NO_SOUND,NOTE_C7,NO_SOUND,NOTE_C5,NO_SOUND,NOTE_C7,NO_SOUND,
  NOTE_CS5,NO_SOUND,NOTE_CS7,NO_SOUND,NOTE_CS5,NO_SOUND,NOTE_CS7,NO_SOUND,
  NOTE_CS5,NO_SOUND,NOTE_CS7,NO_SOUND,NOTE_CS5,NO_SOUND,NOTE_CS7,NO_SOUND,
  NOTE_F4,NO_SOUND,NOTE_F6,NO_SOUND,NOTE_F4,NO_SOUND,NOTE_F6,NO_SOUND,
  NOTE_F4,NO_SOUND,NOTE_F6,NO_SOUND,NOTE_F4,NO_SOUND,NOTE_F6,NO_SOUND,
  NOTE_GS4,NO_SOUND,NOTE_GS6,NO_SOUND,NOTE_GS4,NO_SOUND,NOTE_GS6,NO_SOUND,
  NOTE_GS4,NO_SOUND,NOTE_GS6,NO_SOUND,NOTE_GS4,NO_SOUND,NOTE_GS6,NO_SOUND,
  
  NOTE_C5,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_C5,NO_SOUND,NO_SOUND,NO_SOUND,
  NOTE_C5,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_C5,NO_SOUND,NOTE_C7,NO_SOUND,
  NOTE_CS5,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_CS5,NO_SOUND,NO_SOUND,NO_SOUND,
  NOTE_CS5,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_CS5,NO_SOUND,NO_SOUND,NO_SOUND,
  NOTE_F4,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_F4,NO_SOUND,NO_SOUND,NO_SOUND,
  NOTE_F4,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_F4,NO_SOUND,NO_SOUND,NO_SOUND,
  NOTE_GS4,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_GS4,NO_SOUND,NO_SOUND,NO_SOUND,
  NOTE_GS4,NO_SOUND,NO_SOUND,NO_SOUND,NOTE_GS4,NO_SOUND,NO_SOUND,NO_SOUND,
  
};
  
int noteDuration = 115; // change pace of music


//string for arduinos communication
String commands="nothing";


void setup() {                

  
  pinMode(speaker_pin, OUTPUT);
  
  // Put this line at the beginning of every sketch that uses the GLCD:
  TFTscreen.begin();

  // clear the screen with a black background
  TFTscreen.background(223, 27, 27);
 
  // write the static text to the screen
  // set the font color to white
  TFTscreen.stroke(255,255,255);
  // set the font size
  TFTscreen.setTextSize(4);
  
  play_music=0;
  
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);  //turn on serial communication
}

void loop() {

	correct =0;
	fault =0;
	tension =0;
	outro =0;
	intro =0;
	play_music=0;
 
 	if(!commands.equals("nothing"))
 	{
	 	if(commands=="-1"){
			intro=1;
			play_music=1;
			getPlayerNumber[0]=-1;
			getPlayerNumber[1]=-1;
			getPlayerNumber[2]=-1;
			getPlayerNumber[3]=-1;
			getPlayerNumber[4]=-1;
			clearscreen();
		}
		if(commands=="-2"){
			tension=1;
			play_music=1;
		}
		if(commands=="-3"){
			correct=1;
			play_music=1;
		}
		if(commands=="-4"){
			fault=1;
			play_music=1;
		}
		if(commands=="-5"){
			outro=1;
			play_music=1;
		}
		
		//====players' cases
		
		if(commands=="+1"){
			getPlayerNumber[0] = 1;
		}
		if(commands=="+2"){
			getPlayerNumber[1] = 1;
		}
		if(commands=="+3"){
			getPlayerNumber[2] = 1;
		}
		if(commands=="+4"){
			getPlayerNumber[3] = 1;
		}
		if(commands=="+5"){
			clearscreen();
			getPlayerNumber[4] = 1;
		}
		
		char score_for_lcd[6];
		commands.toCharArray(score_for_lcd, 6);
		
		if( ( !(score_for_lcd[0]=='+') && !(score_for_lcd[0]=='-') ) && getPlayerNumber[4] == -1 ){
			clearscreen();
			TFTscreen.stroke(255,255,255);
			if(getPlayerNumber[0]==1){
				TFTscreen.text("---Score---", 0, 0);
				TFTscreen.text("Blue Player:", 0, 20);
				getPlayerNumber[0]=-1;
			}else if(getPlayerNumber[1]==1){
				TFTscreen.text("---Score---", 0, 0);
				TFTscreen.text("Yellow Player:", 0, 20);
				getPlayerNumber[1]=-1;
			}else if(getPlayerNumber[2]==1){
				TFTscreen.text("---Score---", 0, 0);
				TFTscreen.text("Red Player:", 0, 20);
				getPlayerNumber[2]=-1;
			}else if(getPlayerNumber[3]==1){
				TFTscreen.text("---Score---", 0, 0);
				TFTscreen.text("Green Player:", 0, 20);
				getPlayerNumber[3]=-1;
			}
			TFTscreen.text(score_for_lcd, 0, 40);
		}else if( ( !(score_for_lcd[0]=='+') && !(score_for_lcd[0]=='-') ) && getPlayerNumber[4] == 1 ){
			TFTscreen.stroke(255,255,255);
			if(getPlayerNumber[3]==1){
				TFTscreen.text("Green :", 0, 80);
				TFTscreen.setTextSize(1);
				TFTscreen.text(score_for_lcd, 110, 83);
				TFTscreen.setTextSize(2);
			}else if(getPlayerNumber[2]==1){
				TFTscreen.text("Red :", 0, 60);
				TFTscreen.setTextSize(1);
				TFTscreen.text(score_for_lcd, 110, 63);
				TFTscreen.setTextSize(2);
			}else if(getPlayerNumber[1]==1){
				TFTscreen.text("Yellow :", 0, 40);
				TFTscreen.setTextSize(1);
				TFTscreen.text(score_for_lcd, 110, 43);
				TFTscreen.setTextSize(2);
			}else{
				TFTscreen.text("---Score---", 0, 0);
				TFTscreen.text("Blue :", 0, 20);
				TFTscreen.setTextSize(1);
				TFTscreen.text(score_for_lcd, 110, 23);
				TFTscreen.setTextSize(2);
			}
		}
 	}




  TFTscreen.setTextSize(2);
  if(play_music){ // play starting music 
	//correct
	if(correct==1){
	  TFTscreen.stroke(255,255,255);
  	  TFTscreen.text("Correct", x, y+100);
	  for (int Note = 0; Note <5; Note++) {
	    tone(speaker_pin, melody_correct[Note],noteDuration); //Play note
	
		// to distinguish the notes, set a minimum time between them.
	    delay(noteDuration*1.2);
	  }
	 TFTscreen.stroke(223, 27, 27);
   	 TFTscreen.text("Correct", x, y+100);
	}
	//fault
	if(fault==1){
	  TFTscreen.stroke(255,255,255);
  	  TFTscreen.text("False", x, y+100);
	  for (int Note = 0; Note <5; Note++) {
	    tone(speaker_pin, melody_fault[Note],noteDuration); //Play note
	
		// to distinguish the notes, set a minimum time between them.
	    delay(noteDuration*1.2);
	  }
	  TFTscreen.stroke(223, 27, 27);
   	 TFTscreen.text("False", x, y+100);
	} 
	//tension
	if(tension==1){
	  TFTscreen.stroke(255,255,255);
  	  TFTscreen.text("Next Turn !", x, y+100);
	  for (int Note = 0; Note <8; Note++) {
	    tone(speaker_pin, melody_tension[Note],noteDuration); //Play note
	
		// to distinguish the notes, set a minimum time between them.
	    delay(noteDuration*1.2);
	  }
	  TFTscreen.stroke(223, 27, 27);
  	  TFTscreen.text("Next Turn !", x, y+100);
	}
	//outro
	if(outro==1){
	  TFTscreen.stroke(255,255,255);
  	  TFTscreen.text("Game over", x, y+100);
	  for (int Note = 0; Note <64; Note++) {
	    tone(speaker_pin, melody_outro[Note],noteDuration); //Play note
	
		// to distinguish the notes, set a minimum time between them.
	    delay(noteDuration*1.2);
	  }
	  TFTscreen.stroke(223, 27, 27);
  	  TFTscreen.text("Game over", x, y+100);
	}
	//intro
	if(intro==1){
	  TFTscreen.stroke(255,255,255);
  	  TFTscreen.text("Game starting !", x, y+100);
	  for (int Note = 0; Note <64; Note++) {
	    tone(speaker_pin, melody_intro[Note],noteDuration); //Play note
	
		// to distinguish the notes, set a minimum time between them.
	    delay(noteDuration*1.2);
	  }
	  TFTscreen.stroke(223, 27, 27);
  	  TFTscreen.text("Game starting !", x, y+100);
	}
	
	commands = "nothing" ;
  }
  
}

void receiveEvent(int howMany){
  String readString;
  
  while(0 < Wire.available()){  // loop through all but the last
    char c = Wire.read(); 	 	// receive byte as a character
    readString += c;         	// create the command using the characters
  }
  
  commands = readString;
  

}


void clearscreen(){
	// clear screen, creates and fills rectangle with background color
	// that covers the whole screen.
	TFTscreen.fill(223, 27, 27); 
	TFTscreen.stroke(223, 27, 27); 
  	TFTscreen.rect(0,0,TFTscreen.width(),TFTscreen.height()); 	
	
}