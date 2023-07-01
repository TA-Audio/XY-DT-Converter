// Include the library
#include <TM1637Display.h>
#include <MIDI.h>
#include <RotaryEncoder.h>

// Define the connections pins
#define CLK 4
#define DIO 5
#define PIN_IN1 A2
#define PIN_IN2 A3


// Create a display object of type TM1637Display
TM1637Display display = TM1637Display(CLK, DIO);
RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);
int currentProgram = 0;

MIDI_CREATE_DEFAULT_INSTANCE();

struct Program {
  uint8_t Name[21];
};

const Program programs[] = {
  { { SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,
      SEG_F | SEG_E | SEG_D | SEG_C | SEG_G,
      SEG_F | SEG_E | SEG_G | SEG_C | SEG_B,
      SEG_B | SEG_C } },
  { { SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,
      SEG_F | SEG_E | SEG_D | SEG_C | SEG_G,
      SEG_F | SEG_E | SEG_G | SEG_C | SEG_B,
      SEG_A | SEG_B | SEG_G | SEG_E | SEG_D } },
  { { SEG_A | SEG_F | SEG_G | SEG_C | SEG_E,
      SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,
      SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,
      SEG_A | SEG_F | SEG_G | SEG_C | SEG_D } },
  { { SEG_E | SEG_F | SEG_A | SEG_B | SEG_C,
      SEG_A | SEG_F | SEG_E | SEG_D | SEG_G,
      SEG_F | SEG_E | SEG_D | SEG_C | SEG_G } },
  { { SEG_E | SEG_F | SEG_A | SEG_B | SEG_C,
      SEG_A | SEG_F | SEG_E | SEG_D | SEG_G,
      SEG_F | SEG_E | SEG_D | SEG_C | SEG_G,
      SEG_A | SEG_B | SEG_G | SEG_E | SEG_D } },
  { { SEG_F | SEG_B | SEG_D,
      SEG_A | SEG_F | SEG_G | SEG_C | SEG_E,
      SEG_F | SEG_B | SEG_D,
      0x00 } },
  { { SEG_B | SEG_C,
      SEG_E | SEG_F | SEG_A | SEG_B | SEG_C,
      0x00,
      0x00 } },
  { { SEG_A | SEG_F | SEG_E | SEG_D | SEG_C,
      SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,
      SEG_F | SEG_E | SEG_D,
      SEG_F | SEG_E | SEG_D } }
};


const int numPrograms = sizeof(programs) / sizeof(programs[0]);

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
  display.setBrightness(5);
  //   display.setSegments(programs[currentProgram].Name);

  MIDI.begin(1);
  MIDI.turnThruOff();
  MIDI.setHandleControlChange(controlChange);
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
      if (currentProgram >= numPrograms) {
        currentProgram = 0;  // loop back to the start
      }
    } else if (dir == -1) {
      //CCW
      currentProgram--;
      if (currentProgram < 0) {
        currentProgram = numPrograms - 1;  // loop back to the end
      }
    }
  }
}
