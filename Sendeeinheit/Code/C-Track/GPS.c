/*
 * GPS.c
 *
 * Created: 12.03.2024 19:51:56
 *  Author: Simon
 */ 

#include "GPS.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline char convertTwo(const char* str)
{
    return 10 * (str[0] - '0') + (str[1] - '0');
}

/*
    function for parsing NMEA Sentences with RMC-format
    
    returns 0 if sentence is invalid; 1 if everything is ok
*/
int parseRMC(char *str, Point *p) {
    /*
        function for parsing nmea strings with data in RMC-format

        example for rmc string:
        $GNRMC,155401.000,A,4809.0000,N,01135.0002,E,0.00,0.00,251219„,A*79
    */
	
	double degrees, minutes, decimal;
    int i = 0;

    char* ptr = strtok(str, ",");

    while(ptr != NULL)
    {
        switch(i) 
        {
            case 0:
                // check the beginning of nmea sentece
                // should be $GNRMC...
                if (ptr[0] != '$') return 0;
                if (strncmp(ptr+3, "RMC",3) != 0) return 0;
                i++;
                break;

            case 1:
                // first field with value is time 
                // ...RMC,155401.000,A...
                
                // if field is empty the next field is read
                if (*ptr == 'A' || *ptr == 'V') return 0;

                sscanf(ptr, "%2hu%2hu%2hu", &p->hour, &p->minutes, &p->seconds);
                i++;
                break;

            case 2:
                // field says if values are valid
                // ...,A,...

                // A -> valid; V -> invalid
                if (*ptr == 'V') return 0;
                i++;
                break;

            case 3:
            case 5:
                // field 3 and 5 are lat and lng
                // ..,4809.0000,..

                // information is in ddmm.mmmm format

                // read the value
                degrees = atof(ptr);

                // seperate minutes and degrees
                minutes = degrees - ((int)((int)degrees / 100) * 100);
                degrees -= minutes;

                // convert into decimal degrees
                decimal = (degrees / 100) + (minutes / 60);

                if (i == 3)
                    p->lat = decimal;
                else
                    p->lng = decimal;

                i++;
                break;
            
            case 4:
                if (*ptr == 'W') p->lat *= -1;
                i++;
                break;

            case 6:
                if (*ptr == 'S') p->lng *= -1;
                i++;
                break;
            
            case 9:
                sscanf(ptr, "%02hu%02hu%02hu", &p->day, &p->month, &p->year);
                i++;
                break;

            default:
                printf("::%s\n", ptr);
                i++;
                break;
        }

        ptr = strtok(NULL, ",");
    }
    return 1;
}