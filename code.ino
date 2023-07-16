// Include the library
#include <TM1637.h>
#include <MIDI.h>
#include <RotaryEncoder.h>

// Define the connections pins
#define CLK 4
#define DIO 5
#define PIN_IN1 A2
#define PIN_IN2 A3


// Create a display object of type TM1637Display
TM1637 tm(4, 5);
RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::FOUR3);
int currentProgram = 0;

MIDI_CREATE_DEFAULT_INSTANCE();



void controlChange(byte channel, byte number, byte value) {

  switch (currentProgram) {
    case 0:
      //+2 OCT
      if (number == 127) {
        MIDI.sendProgramChange(0, 1);
      } else if (number == 0) {
        MIDI.sendProgramChange(21, 1);
      } else {
        MIDI.sendControlChange(value, number, channel);
      }
      break;
    case 1:
      //+1 OCT
      if (number == 127) {
        MIDI.sendProgramChange(1, 1);
      } else if (number == 0) {
        MIDI.sendProgramChange(22, 1);
      } else {
        MIDI.sendControlChange(value, number, channel);
      }
      break;
    case 2:
      //-1 OCT
      if (number == 127) {
        MIDI.sendProgramChange(7, 1);
      } else if (number == 0) {
        MIDI.sendProgramChange(28, 1);
      } else {
        MIDI.sendControlChange(value, number, channel);
      }
      break;
    case 3:
      //-2 OCT

      if (number == 127) {
        MIDI.sendProgramChange(8, 1);
      } else if (number == 0) {
        MIDI.sendProgramChange(29, 1);
      } else {
        MIDI.sendControlChange(value, number, channel);
      }
      break;
    case 4:
      //DIVE
      if (number == 127) {
        MIDI.sendProgramChange(9, 1);
      } else if (number == 0) {
        MIDI.sendProgramChange(30, 1);
      } else {
        MIDI.sendControlChange(value, number, channel);
      }
      break;
    case 5:
      //+1/-1 HAR
      if (number == 127) {
        MIDI.sendProgramChange(20, 1);
      } else if (number == 0) {
        MIDI.sendProgramChange(41, 1);
      } else {
        MIDI.sendControlChange(value, number, channel);
      }
      break;
  }
}

void setup() {
  tm.begin();

  MIDI.begin(1);
  MIDI.turnThruOff();
  MIDI.setHandleControlChange(controlChange);

  tm.changeBrightness(10);
  tm.display("P2OC");
}


void loop() {
  MIDI.read();

  static int pos = 0;
  encoder.tick();

  int newPos = encoder.getPosition();
  if (pos != newPos) {

    int dir = (int)encoder.getDirection();

    if (dir == 1) {
      //CLOCKWISE
      currentProgram++;
      if (currentProgram >= 6) {
        currentProgram = 0;  // loop back to the start
      }
    } else if (dir == -1) {
      //CCW
      currentProgram--;
      if (currentProgram < 0) {
        currentProgram = 6 - 1;  // loop back to the end
      }
    }

    tm.clearScreen();


    switch (currentProgram) {
      case 0:
        tm.display("P2OC");
        break;
      case 1:
        tm.display("P1OC");
        break;

      case 2:
        tm.display("N1OC");
        break;

      case 3:
        tm.display("N2OC");
        break;

      case 4:
        tm.display("DI");
        break;

      case 5:
        tm.display("HARM");
        break;
    }

    pos = newPos;
  }
}
