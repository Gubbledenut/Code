/*  OpenPipe Breakout MIDI example
 *   MIDIUSB_ Chanter. wiring included in code. 
 *   Requires Arduino libraries, WIRE, OPENPIPE, MIDI to compile. 
 *  It will work fine with either a Teensy LC ,3.2 or 4 board and 
 *   one MPR121 cap touch 12 input board.
 *
 *  Send MIDI commands to external synths.
 *
 *  Connect the OpenPipe Breakout wires to Arduino as follows:
 *  YELLOW-> A5 (SCL)
 *  GREEN-> A4 (SDA)
 *  BLACK -> A3 (GND) (Grey in old openpipes)
 *  RED -> A2 (VCC) (White in old openpipes)
 *  
 *  MPR121 to Tennsy Pin ( LC, 3.2 or 4)
 *  SDA - 18
 *  SCL - 19
 *  IRQ -  6 
 *  GND - GND
 *  3.3V -3.3V
 *  
 *  MRP121 Sensor Pins
FINGER HOLE sensor from top 
T=THUMB HOLE mounted on rear
MPR121 PIN NO

LH T-E9
    LH 1 -10
    LH 2 -6
    LH 3 -7

    RH 1 0-4
RH T-5
    RH 2 - 2
    RH 3 - 1
    RH 4 - 0
 *
 *  MIDI output directly through the teensy USB port
 *
 *  This example code is in the public domain.
 */

#include <Wire.h> // Wire library for comunicating with OpenPipe
#include <OpenPipe.h> // OpenPipe Library
//#include <MIDI.h> // The MIDI Library

// Select here which fingering to use
#define FINGERING FINGERING_GAITA_GALEGA
//#define FINGERING FINGERING_GAITA_ASTURIANA
//#define FINGERING FINGERING_GREAT_HIGHLAND_BAGPIPE
//#define FINGERING FINGERING_UILLEANN_PIPE
//#define FINGERING FINGERING_SACKPIPA

// Select here which MIDI instrument to use
// http://en.wikipedia.org/wiki/General_MIDI
#define MIDI_INSTRUMENT 66 // Tenor Sax

// Uncomment the following line if you want to use Hairless MIDI<->Serial Bridge
//#define USE_HAIRLESS_BRIDGE

// Global variables
unsigned long fingers, previous_fingers;
unsigned char previous_note;
char playing;

//MIDI_CREATE_DEFAULT_INSTANCE();

void setup(){

  // OpenPipe setup
  OpenPipe.power(A2, A3); // VCC PIN in A2 and GND PIN in A3
  OpenPipe.config();
  OpenPipe.setFingering(FINGERING);

  // MIDI setup
 // MIDI.begin(0);

#ifdef USE_HAIRLESS_BRIDGE
  // http://projectgus.github.io/hairless-midiserial/#how_do_i_use_the_arduino_midi_library
  Serial.begin(115200); //Use this line for Hairless MIDI<->Serial Bridge
#endif

  //MIDI.sendProgramChange(MIDI_INSTRUMENT,1); // Select MIDI instrument
usbMIDI.sendProgramChange(MIDI_INSTRUMENT,1); // Select MIDI instrument

  // Variables initialization
  fingers=0;
  previous_fingers=0xFF;
  playing=false;

}

void loop(){

  // Read OpenPipe fingers
  fingers=OpenPipe.readFingers();

  // If fingers have changed...
  if (fingers!=previous_fingers){
    previous_fingers=fingers;

    // Check the low right thumb sensor
    if (OpenPipe.isON()){
      playing=true;

      // If note changed...
      if (OpenPipe.note!=previous_note && OpenPipe.note!=0xFF){
         usbMIDI.sendNoteOff(previous_note,127,1);   // Stop the previous note
         usbMIDI.sendNoteOn(OpenPipe.note,127,1);   // Start the current note
         previous_note=OpenPipe.note;
      }
    }else{
      if (playing){
         usbMIDI.sendNoteOff(OpenPipe.note,0,1);   // Stop the note
         playing = false;
       }
    }
  }
}
