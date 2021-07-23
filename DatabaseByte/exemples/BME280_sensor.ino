/*
 * 		Arduino has a very thin memory, 2 KB. Storing great amount of floats drains
 * 		quickly the RAM.
 *
 * 		If high precision is not needed, keep the records in 8 bits, 0-255, is
 * 		a good solution.
 *
 * 		This library does that, the input and output data can be float type,
 * 		but the storage is in byte, or uint_8, that has only 1 byte instead
 * 		of 4 bytes from the float.
 *
 * 		In small ranges, it can provide a good resolution, even with the data being
 *		internally recorded in 8 bits, it can provide fractional values if a small range
 *		is used.
 *
 *		This sketch store temperature and humidity taken from a BME280 sensor.
 *
 * 		@author Aluizio Tomazelli Junior
 *	    https://github.com/aluiziotomazelli
 *	   	2021
 */


#include "Arduino.h"
#include "DatabaseByte.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

void freeRam(); //prototype of freeRam function


const int tempSamples = 100;    //Number of samples, must be const
uint8_t dataTemp[tempSamples];	//Array to store records of data
float temperature;				//Variable that will be recorded
/**
 * Constructor
 * DatabaseByte objectName(&variable, MIN value expected, MAX value expected, arrayName)
 *
 * @param &temperature - a float variable that is representing of the parameter being
 * 		monitored
 * @param -10 - the minimal value expected from this parameter
 * @param 50 - the maximum value expected from parameter
 * @param dataTemp - name of array created to store the values
 */
DatabaseByte maxMinTemp(&temperature, -10, 50, dataTemp);

/*
 * The number of samples has direct impact on RAM. A number of
 * 		288 sample will take 288 bytes from RAM. The same number
 * 		need be passed in setup, commented bellow.
 */
const int humiSamples = 100;
uint8_t dataHumi[humiSamples];
float humidity;
DatabaseByte maxMinHumi(&humidity, 0, 100, dataHumi);

// BME Contructor
Adafruit_BME280 bme;

void setup() {

	Serial.begin(115200);

	//BME setup
	bme.begin();
	Serial.println(F("BME280 beginning"));

	/** setIntervalSamples - here the interval between each sample and the number of
	 * 	sample are given.
	 *
	 * 	The number of samples need to be the same of the array length.
	 *
	 * 	The interval and the number of sample will determine how long the data will
	 * 	survive in memory:
	 *
	 * 	In this simple example, the number of sample are 100, and the interval is 600:
	 * 	600 ms * 100 samples = 60'000 ms or 60 seconds.
	 *
	 * 	A little interval is more useful for testing the library, the library will
	 * 	keep the max and min for 60 seconds and new data will be recorded in database.
	 *
	 * 	For a long term usage, like keep the last 24 h in database:
	 * 	24 h * 60m * 60s * 1000 ms = 86'400'000 ms
	 *
	 * 	If one recording every 10 min is desired:
	 * 	24 h * 6 samples = 144 total samples
	 *
	 * 	86'400'000 ms / 144 = 600'000 ms of interval
	 *
	 * 	If one recorging every 5 minutes:
	 * 	24 * 12 = 288
	 * 	86'400'000 / 288 = 300'000 od interval
	 */
	maxMinTemp.setIntervalSamples(600, tempSamples);
	maxMinHumi.setIntervalSamples(600, humiSamples);
}

void loop() {
	// Reading the values from BME sensor
	temperature = bme.readTemperature();
	humidity = bme.readHumidity();

	// This .update() needs to be called every loop, this function
	// will analize and store the value since the interval time was elapsed
	maxMinTemp.update();
	maxMinHumi.update();

	// The .getMax() returns the maximum value stored in the database
	float maxTemp = maxMinTemp.getMax();

	// The .getMin() returns the minimum value stored in the database
	float minTemp = maxMinTemp.getMin();

	float maxHumi = maxMinHumi.getMax();
	float minHumi = maxMinHumi.getMin();

	// Print data
	Serial.print(F("Temperature: "));
	Serial.println(temperature, 2);
	Serial.print(F("TempMax:     "));
	Serial.println(maxTemp, 2);
	Serial.print(F("TempMin:     "));
	Serial.println(minTemp, 2);
	Serial.println();

	Serial.print(F("Humidity:    "));
	Serial.println(humidity, 2);
	Serial.print(F("HumiMax:     "));
	Serial.println(maxHumi, 2);
	Serial.print(F("TempMin:     "));
	Serial.println(minHumi, 2);
	Serial.println();

	// This show the free RAM in the arduino, is good to keep an eye on this
	// if a big number of samples are being stored.
	freeRam();
	Serial.println(" ");
	delay(500);
}

void freeRam() {
	extern int __heap_start, *__brkval;
	int v;
	int fr = (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
	Serial.print(F("Free ram: "));
	Serial.println(fr);
}
