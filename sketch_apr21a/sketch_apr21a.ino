#include "pitches.h"

#define AUDIO_PIN P1_6

typedef struct _song
{
	uint16_t* notes;
	uint16_t size;
	uint16_t wholeNoteMillis;
} song;

uint16_t marioNotes[] = {
	NOTE_E5, 8,
	NOTE_E5, 8,
	0, 8,
	NOTE_E5, 8,
	0, 8,
	NOTE_C5, 8,
	NOTE_E5, 8,
	0, 8,

	NOTE_G5, 4,
	0, 4,
	NOTE_G4, 4
};

song mario = {
	marioNotes,
	22,
	1154
};

void playSong(song s) {
	for(int i = 0; i < s.size; i += 2) {
		if(s.notes[i] != 0) {
			analogFrequency(s.notes[i]);
			analogWrite(AUDIO_PIN, 127);
		}
		else
			analogWrite(AUDIO_PIN, 0);
		delay(s.wholeNoteMillis / s.notes[i + 1] - 10);
		analogWrite(AUDIO_PIN, 0);
		delay(10);
	}
	analogWrite(AUDIO_PIN, 0);
}

uint16_t fundamentalTone, prev_fundamentalTone;
uint8_t harmonic, prev_harmonic;

uint16_t fundamentals[] = { NOTE_F2, NOTE_FS2, NOTE_G2, NOTE_GS2, NOTE_A2,
	NOTE_AS2, NOTE_B2, NOTE_C3, NOTE_CS3, NOTE_D3, NOTE_DS3, NOTE_E3,
	NOTE_F3, NOTE_FS3, NOTE_G3, NOTE_GS3, NOTE_A3, NOTE_AS3, NOTE_B3,
	NOTE_C4, NOTE_CS4, NOTE_D4 };
#define SIZE_FUNDAMENTALS 22
uint8_t baseFundamentalIdx = 7;

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
	fundamentalTone = fundamentals[baseFundamentalIdx + (~P2IN & 0x07)];

	// play the valve-augmented root of the harmonic series
	if(harmonic != prev_harmonic || fundamentalTone != prev_fundamentalTone) {
		analogFrequency(fundamentalTone * harmonic);
		analogWrite(AUDIO_PIN, (harmonic & 0x02) ? 127 : 0); // "0" and root harmonic kept silent
	}

	baseFundamentalIdx = analogRead(P1_7) * (SIZE_FUNDAMENTALS - 8) / (1 << 10);

	if((P1IN & 0x03) == 0 && (P2IN & 0x07) == 0)
		playSong(mario);
}
