# DatabaseByte
A library to store sensor values without using too much RAM


Arduino has a very thin memory, 2 KB. Storing great amount of floats drains quickly the RAM.

If high precision is not needed, keep the records in 8 bits, 0-255, is a good solution.

This library does that, the input and output data can be float type, but the storage is in byte, or uint_8, that has only 1 byte instead of 4 bytes from the float.

In small ranges, it can provide a good resolution, even with the data being internally recorded in 8 bits, it can provide fractional values if a small range is used.

The example sketch store temperature and humidity taken from a BME280 sensor. But this can easelly changed to store a value of a other sensor from analogRead().
