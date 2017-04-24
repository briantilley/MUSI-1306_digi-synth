#include "pitches.h"

#define AUDIO_PIN P1_6

uint16_t fundamentalTone, prev_fundamentalTone;
uint8_t harmonic, prev_harmonic;

uint16_t fundamentals[] = { NOTE_F2, NOTE_FS2, NOTE_G2, NOTE_GS2, NOTE_A2,
	NOTE_AS2, NOTE_B2, NOTE_C3, NOTE_CS3, NOTE_D3, NOTE_DS3, NOTE_E3,
	NOTE_F3, NOTE_FS3, NOTE_G3, NOTE_GS3, NOTE_A3, NOTE_AS3, NOTE_B3,
	NOTE_C4, NOTE_CS4, NOTE_D4 };
#define SIZE_FUNDAMENTALS 22
uint8_t baseFundamentalIdx = 0;

// for fun
long f_ts;

void setup() {
	// harmonic selection
	P1DIR = 0x00;
	P1REN = 0x03;
	P1OUT = 0x03;

	// fundamental selection
	P2DIR = 0x00;
	P2REN = 0x07;
	P2OUT = 0x07;

	f_ts = millis();
}

void loop() {

	// read port 1 for the harmonic (0x01 == first harmonic, 0x02 == second ...)
	// must invert input logic to account for pull-up resistor
	prev_harmonic = harmonic;
	harmonic = ~P1IN & 0x07;

	// artificial valving - augment by a certain number of half steps
	prev_fundamentalTone = fundamentalTone;
	fundamentalTone = fundamentals[baseFundamentalIdx + (~P2IN & 0x07)];

	// play the valve-augmented root of the harmonic series
	if(harmonic != prev_harmonic || fundamentalTone != prev_fundamentalTone) {
		analogFrequency(fundamentalTone * harmonic);
		analogWrite(AUDIO_PIN, (harmonic & 0x02) ? 127 : 0); // "0" harmonic is silent (no buttons pressed)
	}

	if(millis() - 250 > f_ts) {
		f_ts = millis();
		baseFundamentalIdx++;
		if(baseFundamentalIdx > SIZE_FUNDAMENTALS - 8) {
			baseFundamentalIdx = 0;
		}
	}
}
