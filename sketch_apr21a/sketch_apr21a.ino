#include "pitches.h"

// output pin that carries the audio square wave
#define AUDIO_PIN P1_6

// define a song to be played automatically
typedef struct _song
{
	uint16_t* notes;
	uint16_t size; // length of the notes array
	uint16_t wholeNoteMillis; // number of milliseconds in one whole note
} song;

// first few notes for the mario soundtrack
uint16_t marioNotes[] = {
	NOTE_E5, 8, // E natural, 5th octave, div. whole note by 8
	NOTE_E5, 8,
	0, 8, // 8th-note rest
	NOTE_E5, 8,
	0, 8,
	NOTE_C5, 8,
	NOTE_E5, 8,
	0, 8,

	NOTE_G5, 4,
	0, 4,
	NOTE_G4, 4
};

// corresponding song
song mario = {
	marioNotes,
	22, // 22-value note array
	1154, // 1154 ms per whole note
};

// play a song as defined above
void playSong(song s) {
	for(int i = 0; i < s.size; i += 2) {
		if(s.notes[i] != 0) { // play the note (if a non-rest)
			analogFrequency(s.notes[i]); // set pitch
			analogWrite(AUDIO_PIN, 127); // 50% duty cycle square wave
		}
		delay(s.wholeNoteMillis / s.notes[i + 1] - 10); // wait most of the note
		analogWrite(AUDIO_PIN, 0); // go silent
		delay(10); // leave a short break between notes
	}
}

// root of the harmonic series and harmonic multiplier
// track previous value to detect a change in input
uint16_t fundamentalTone, prev_fundamentalTone;
uint8_t harmonic, prev_harmonic;

// root notes of each harmonic series depending on which "valves" are pressed
// actual lowest playable note is one octave up
uint16_t fundamentals[] = { NOTE_F3, NOTE_FS3, NOTE_G3, NOTE_GS3,
	NOTE_A3, NOTE_AS3, NOTE_B3, NOTE_C4 };

// runs once
void setup() {
	// harmonic selection buttons
	// input pullup
	P1DIR = 0x00;
	P1REN = 0x07;
	P1OUT = 0x07;

	// fundamental selection buttons
	// input pullup
	P2DIR = 0x00;
	P2REN = 0x07;
	P2OUT = 0x07;
}

// repeats infinitely
void loop() {
	// read port 1 for the harmonic (0x01 == first harmonic, 0x02 == second ...)
	// must invert input logic to account for pull-up resistor
	prev_harmonic = harmonic;
	harmonic = ~P1IN & 0x07;

	// artificial valving - augment by a certain number of half steps
	prev_fundamentalTone = fundamentalTone;
	fundamentalTone = fundamentals[~P2IN & 0x07];

	// play the valve-augmented root of the harmonic series
	// only change the note if button configuration has changed
	if(harmonic != prev_harmonic || fundamentalTone != prev_fundamentalTone) {
		analogFrequency(fundamentalTone * harmonic);
		analogWrite(AUDIO_PIN, (harmonic & 0x02) ? 127 : 0); // "0" and root harmonic kept silent
	}

	// it's-a me!
	if((P1IN & 0x03) == 0 && (P2IN & 0x07) == 0)
		playSong(mario);
}
