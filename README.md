# DatabaseByte
A library to store sensor values without using too much RAM

Arduino has a very thin memory, 2 KB. Storing great amount of floats drains quickly the RAM.

If high precision is not needed, keep the records in 8 bits, 0-255, is a good solution.

This library does that, the input and output data can be float type, but the storage is in byte, or uint_8, that has only 1 byte instead of 4 bytes from the float.

In small ranges, it can provide a good resolution, even with the data being internally recorded in 8 bits, it can provide fractional values if a small range is used.

The example BME280 sketch store temperature and humidity taken from a BME280 sensor. But this can easelly changed to store a value of a other sensor from analogRead().


------------------------------------------------------------

**Using the library**

------------------------------------------------------------


**DatabaseByte(float *ptrActual, int rangeMin, int rangeMax, uint8_t *database)**


Constructor

@param &temperature - a float variable that is representing of the parameter being	monitored
@param -10 - the minimal value expected from this parameter
@param 50 - the maximum value expected from parameter
@param dataTemp - name of array created to store the values


	DatabaseByte objectName(&variable, MIN value expected, MAX value expected, arrayName)
 
Example:

	DatabaseByte maxMinTemp(&temperature, -10, 50, dataTemp);



------------------------------------------------------------

**setIntervalSamples(unsigned long interval, int numberSamples)**




Here the interval between each sample and the number of sample are given. 
The number of samples need to be the same of the array length. 
The interval and the number of sample will determine how long the data will survive in memory:

In this simple example, the number of sample are 100, and the interval is 600:
600 ms * 100 samples = 60'000 ms or 60 seconds.

A little interval is more useful for testing the library, the library will keep the max and min for 60 seconds and new data will be recorded in database.

For a long periods, like keep the last 24 h in database:

24 h * 60m * 60s * 1000 ms = 86'400'000 ms
If one recording every 10 min is desired:
24 h * 6 samples = 144 total samples
86'400'000 ms / 144 = 600'000 ms of interval

If one recorging every 5 minutes:
24 * 12 = 288
86'400'000 / 288 = 300'000 od interval

Example:


	maxMinTemp.setIntervalSamples(600, 100);
	

------------------------------------------------------------

**update()**
This .update() needs to be called every loop, this function will analize and store the value since the interval time was elapsed

	maxMinTemp.update();


------------------------------------------------------------


**getMax()** 
Called when you want to read the maximum value stored in the database:

	float maxTemp = maxMinTemp.getMax();

------------------------------------------------------------


**getMix()** 
Called when you want to read the minimum value stored in the database:

	float minTemp = maxMinTemp.getMin();
