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


#include "DatabaseByte.h"

#include "Arduino.h"


/**	Constructor
 *
 * The range are useful and important because the class register the data internally
 * 		in 8 bits (0-255). If small changes in the parameter are expected, the resolution
 *		will cover smal changes in parameter.
 *
 * @param ptrActual - a reference (&var) to a variable that represent the parameter
 * 		to be monitored
 * @param rangeMin - min expected value of this variable
 * @param rangeMax - max expected value of this variable
 */
DatabaseByte::DatabaseByte(float *ptrActual, int rangeMin, int rangeMax, uint8_t *database) :
		actual(ptrActual), rangeMin(rangeMin), rangeMax(rangeMax){
	this->database = database,
	min = floatToByte(*actual);
	max = floatToByte(*actual);
	numberSamples = 100;
	timeNow = 0;
	interval = 1000;
	isFirst = true;
}

/**
 * Must be called on setup, otherwise the interval will be default parameters
 * 		in constructor: interval = 600, numberSamples = 100
 *
 * @param interval - interval in milliseconds between each record
 * @param numberSamples - number of records, impacts on RAM
 */
void DatabaseByte::setIntervalSamples(unsigned long interval, int numberSamples){
	this->interval = interval;
	this->numberSamples = numberSamples;
}

/**
 * 	This function must be called in the loop, when the interval passes, the class
 * 			register the values internally.
 */
void DatabaseByte::update() {
	/**
	 *	The MIN value will be 0 in the first run along the period time, until
	 *	the database dont store all position of the array. To prevent this,
	 *	in the first run, the actual value is stored in all positions.
	 */
	if (isFirst) {
		for (int i = 0; i < numberSamples; ++i) {
			database[i] = floatToByte(*actual);
			isFirst = false;
		}
	}
	uint8_t now = floatToByte(*actual);		// take the actual reading and transform in a byte
	max = getData(MAX);						// read the MAX value from the database
	if (now > max) {						// if actual is bigger
		max = now;							// MAX is equal to actual
		database[0] = max;					// store the data
	}

	min = getData(MIN);						// the same for MIN value
	if (now < min) {
		min = now;
		database[0] = min;
	}

	if (timeWasElapsed()) {					// if time was elapsed
		rightShift();						// shift the array one position to the right
		database[0] = now;					// store the actual value on position [0]
	}
}

/**
 *  Retrieves the min or max value stored in a 8 bits raw format,
 *  	from 0-255
 *
 * @param min_max - MIN or MAX, what kind of data to be returned
 * @return - a float with the max or min value
 */
uint8_t DatabaseByte::getData(MinOrMax min_max) {
	uint8_t result = database[0];					// take the last stored data
	if (min_max == MIN) {
		for (int i = 1; i < numberSamples; ++i) {	// run the entire array
			if (database[i] < result)				// if a value is less than the first
				result = database[i];				// save the value
		}
	} else if (min_max == MAX) {					//the same for MAX value
		for (int i = 1; i < numberSamples; ++i) {
			if (database[i] > result)				// if a value is bigger the actual
				result = database[i];				// save the value
		}
	}
	return result;									// return the result
}

/**
 * Count the milliseconds, millis(), and compare with the interval
 *
 * @return - true when the time was elapsed
 */
bool DatabaseByte::timeWasElapsed() {
	if (millis() - timeNow > interval) {
		timeNow = millis();
		return true;
	} else
		return false;
}

/**
 *  Do a right shift on the array, so that the newer data recorded will
 *  	stay always in the array[0]
 */
void DatabaseByte::rightShift() {
	for (int i = numberSamples - 1; i > 0; --i) {
		database[i] = database[i - 1];
	}
}

/**
 *
 * @return - a float with the MAX value stored in database
 */
float DatabaseByte::getMax() {
	return byteToFloat(max);
}

/**
 *
 * @return - a float with the MIN value stored in database
 */
float DatabaseByte::getMin() {
	return byteToFloat(min);
}

/**
 * Take a float value and map it in 8 bits, using the range to fit, or expand,
 * 		the original resolution in 0-255
 *
 * @param data - the float value to be stored in 8 bits array
 * @return - the value in 8 bits, 0-255
 */
uint8_t DatabaseByte::floatToByte(float data) {
	return (data - rangeMin) * 255 / (rangeMax - rangeMin);
}

/**
 * Take a value in 8 bits and retrieves a float, using the parameter
 * 		ranges to give a return
 *
 * @param data - 8 bits from database
 * @return - a float interpolation value to parameter ranges
 */
float DatabaseByte::byteToFloat(uint8_t data) {
	return data * (float(rangeMax) - float(rangeMin)) / 255 + float(rangeMin);
}

/**
 * Retrieves a past record from database
 *
 * @param x - the number of past records since the current
 * @return - the value from the selected position
 */
float DatabaseByte::getPastValueByX(uint8_t x) {
	return byteToFloat(database[x]);
}

