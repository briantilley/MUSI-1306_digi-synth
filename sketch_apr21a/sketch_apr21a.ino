#include "pitches.h"

#define AUDIO_PIN P1_6

uint16_t fundamentalTone, prev_fundamentalTone;
uint8_t harmonic, prev_harmonic;

uint16_t fundamentals[] = { NOTE_F2, NOTE_FS2, NOTE_G2, NOTE_GS2, NOTE_A2,
	NOTE_AS2, NOTE_B2, NOTE_C3, NOTE_CS3, NOTE_D3, NOTE_DS3, NOTE_E3,
	NOTE_F3, NOTE_FS3, NOTE_G3, NOTE_GS3, NOTE_A3, NOTE_AS3, NOTE_B3,
	NOTE_C4, NOTE_CS4, NOTE_D4 };
uint8_t baseFundamentalIdx = 0;

void setup() {
	// harmonic selection
	P1DIR = 0x00;
	P1REN = 0x03;
	P1OUT = 0x03;

	// fundamental selection
	P2DIR = 0x00;
	P2REN = 0x07;
	P2OUT = 0x07;
}

void loop() {

	// read port 1 for the harmonic (0x01 == first harmonic, 0x02 == second ...)
	// must invert input logic to account for pull-up resistor
	prev_harmonic = harmonic;
	harmonic = ~P1IN & 0x07;

	// artificial valving - augment by a certain number of half steps
	prev_fundamentalTone = fundamentalTone;
	switch(~P2IN & 0x07) {
		case 0x00:
			fundamentalTone = NOTE_C3;
			break;
		case 0x01:
			fundamentalTone = NOTE_CS3;
			break;
		case 0x02:
			fundamentalTone = NOTE_D3;
			break;
		case 0x03:
			fundamentalTone = NOTE_DS3;
			break;
		case 0x04:
			fundamentalTone = NOTE_E3;
			break;
		case 0x05:
			fundamentalTone = NOTE_F3;
			break;
		case 0x06:
			fundamentalTone = NOTE_FS3;
			break;
		case 0x07:
			fundamentalTone = NOTE_G3;
			break;
		default:
			fundamentalTone = NOTE_C3;
			break;
	}

	// play the valve-augmented root of the harmonic series
	if(harmonic != prev_harmonic || fundamentalTone != prev_fundamentalTone) {
		analogFrequency(fundamentalTone * harmonic);
		analogWrite(AUDIO_PIN, (harmonic & 0x02) ? 127 : 0); // "0" harmonic is silent (no buttons pressed)
	}
}
