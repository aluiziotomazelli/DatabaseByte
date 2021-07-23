/*
 *		MinMaxByte class
 *
 *		A solution to record sensor data economically in RAM.
 *
 *		Converts values from a range and records internally in 8 bits,
 *		the larger the range, the worse the resolution of the recorded database.
 *		In small ranges, it can provide a good resolution, even with the data being
 *		internally recorded in 8 bits, it can provide fractional values if a small range
 *		is used.
 *
 *		Can be used for temperature, humidity, or things like that.
 *
 *		@author Aluizio Tomazelli Junior
 *	    https://github.com/aluiziotomazelli
 */

#ifndef DATABASEBYTE_H_
#define DATABASEBYTE_H_
#include "Arduino.h"
/**
 * 	The number of samples will be the memory RAM, in bytes, used for each class
 * 		objetc created.
 * 	The interval is the time, in milliseconds, between each record.
 *
 * 	If a 24h of sampling is desired, the interval must be:
 * 		24h * 60m * 60 s * 1000ms =  86'400'000ms
 *		86'400'000ms / 288 sample =
 *
 *		300'000ms, or 5m of interval, or 12 sample per hour
 *
 *	Barometric pressure, otherwise, may be more useful to have database from
 *		few hours. Interval can be changed latter, in Arduino setup(),
 *		.setinterval(50000). With a 50'000 ms of interval:
 *		50'000m * 288 samples = 14'400'000 ms
 *		14'400'000ms / 60m / 60s / 1000ms = 4h of sampling
 */

class DatabaseByte {

	enum MinOrMax : uint8_t {			// used to indicate more easily if is MAX or MIN
		MIN, MAX
	};

	float *actual;						// pointer to external variable being monitored
	uint8_t min;						// 8 bits MIN value
	uint8_t max;						// 8 bits MAX value

	int rangeMin;						// MIN value expected from external variable
	int rangeMax;						// MAX value expected from external variable

	uint8_t *database;					// pointer to external array to store the data
	int numberSamples;					// number of samples

	unsigned long timeNow;				// used to mark time in no delay function
	unsigned long interval;				// interval between each record

	bool isFirst;						// indicates if is the first loop, and full the
										// the array with actual values, or else the MIN
										// value will be zero until the whole array is full

	bool timeWasElapsed();
	void rightShift();
	uint8_t getData(MinOrMax min_max);
	uint8_t floatToByte(float data);
	uint8_t mapByte(float x, int in_min, int in_max, uint8_t out_min, uint8_t out_max);
	float byteToFloat(uint8_t data);

public:
	DatabaseByte(float *ptrActual, int rangeMin, int rangeMax, uint8_t *database);
	void setIntervalSamples(unsigned long interval, int numberSamples);
	void update();
	float getMax();
	float getMin();
	float getPastValueByX(uint8_t x);
};

#endif /* DATABASEBYTE_H_ */
