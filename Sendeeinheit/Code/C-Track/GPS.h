/*
 * GPS.h
 *
 * Created: 12.03.2024 19:51:46
 *  Author: Simon
 */ 


#ifndef GPS_H_
#define GPS_H_

typedef struct Point
{
	double lat, lng;

	unsigned short day, month, year;
	unsigned short hour, minutes, seconds;
} Point;

int parseRMC(char* str, Point* p);

#endif /* GPS_H_ */