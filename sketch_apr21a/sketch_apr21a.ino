#include "pitches.h"

#define AUDIO_PIN P1_2

uint16_t fundamentalTone = NOTE_F4;

void setup() {
	P2DIR = 0x00;
	P2REN = 0x07;
	P2OUT = 0x07;

	pinMode(PUSH2, INPUT_PULLUP);
}

int i = 1;
void loop() {
	// switch(P2IN & 0x07) {
	// 	case 0x07:
	// 		fundamentalTone = NOTE_F3;
	// 		break;
	// 	case 0x06:
	// 		fundamentalTone = NOTE_FS3;
	// 		break;
	// 	case 0x05:
	// 		fundamentalTone = NOTE_G3;
	// 		break;
	// 	case 0x04:
	// 		fundamentalTone = NOTE_GS3;
	// 		break;
	// 	case 0x03:
	// 		fundamentalTone = NOTE_A3;
	// 		break;
	// 	case 0x02:
	// 		fundamentalTone = NOTE_AS3;
	// 		break;
	// 	case 0x01:
	// 		fundamentalTone = NOTE_B3;
	// 		break;
	// 	case 0x00:
	// 		fundamentalTone = NOTE_C4;
	// 		break;
	// 	default:
	// 		fundamentalTone = NOTE_F3;
	// 		break;
	// }

	// analogFrequency(fundamentalTone);
	analogFrequency(fundamentalTone * i++);
	if(i > 12) i = 1;
	analogWrite(AUDIO_PIN, 127);
	// analogWrite(AUDIO_PIN, digitalRead(PUSH2) ? 0 : 127);
	delay(500);
}

