#define DAC_FREQUENCY 4000000 // 4 MHz DAC clock

#define LED1 P1_0
#define LED2 P1_6

// // levels and timings (time = % of a single cycle)
// uint8_t sinusoid[] = { 0b10, 12, 0b11, 26, 0b10, 12, 0b01, 12, 0b00, 26, 0b01, 12 };
#define SIZE_SIN 12

uint8_t sawtooth[] = { 0b00, 25, 0b01, 25, 0b10, 25, 0b11, 25 };
#define SIZE_SAWTOOTH 8

// void waveConvert(uint8_t* src, uint16_t* dest, uint16_t size, uint16_t f) {
// 	float totalCompareCount = (float)DAC_FREQUENCY / f + 0.5;
// 	uint16_t sampleCompareVal;
// 	for(int i = 1; i < size - 2; i += 2) {
// 		dest[i - 1] = src[i - 1]; // straight copy for levels
// 		totalCompareCount -= (sampleCompareVal = (int)(src[i] * totalCompareCount / 100.0 + 0.5));
// 		dest[i] = sampleCompareVal;
// 	}
// 	dest[size - 1] = int(totalCompareCount); // write the remainder of the cycle to the last sample
// }

// write a voltage level to the DAC
inline void DACwrite(uint8_t level) {
	P2OUT = (P2OUT & 0xfc) | level;
}

// runs at timed intervals
uint16_t waveA[32] = {0b10, 1091, 0b11, 2364, 0b10, 1091, 0b01, 1091, 0b00, 2364, 0b01, 1090};
// uint16_t waveB[32];
uint16_t* waveTable = waveA;
uint16_t size_waveTable = 0, idx_waveTable = 0;
#pragma vector = TIMER0_A0_VECTOR
__interrupt void DAC_ISR(void) {
	DACwrite(waveTable[idx_waveTable]);
	TACCR0 = waveTable[idx_waveTable + 1];
	idx_waveTable += 2; if(idx_waveTable >= size_waveTable) idx_waveTable = 0;
}

// runs once, setup for the program
void setup() {

	pinMode(PUSH2, INPUT_PULLUP);
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);

	// DAC output
	P2DIR = 0x03; // 2-bit DAC
	P2OUT = 0x00;

	// configure the DAC timer
	TA0CTL = TASSEL_2 | ID_3 | MC_0 | TAIE;
	TACCR0 = 10;
	TACCTL0 = CCIE;

	// convert wave tables to dac frequency tables
	// waveConvert(sinusoid, waveA, SIZE_SIN, 440);
	size_waveTable = SIZE_SIN;
	// waveConvert(sawtooth, waveB, SIZE_SAWTOOTH, 120);
	// size_waveTable = SIZE_SAWTOOTH;

	// wait for a button press
	while(digitalRead(PUSH2));

	// start the DAC (timer in UP mode)
	TA0CTL |= MC_1;
}

// repeats forever after setup()
void loop() {
	// if(!digitalRead(PUSH2)) {
	// 	waveTable = waveB;
	// 	size_waveTable = SIZE_SAWTOOTH;
	// }
	// else {
	// 	waveTable = waveA;
	// 	size_waveTable = SIZE_SIN;
	// }
	digitalWrite(LED1, digitalRead(PUSH2));
	digitalWrite(LED2, !digitalRead(PUSH2));
}