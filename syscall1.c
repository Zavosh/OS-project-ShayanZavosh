#include <sys/time.h>
#include <stdio.h>
#include <time.h>

int main(){
	struct timeval tv;// A struct having seconds and microseconds
	struct tm* ptm;// A struct having year since 1900, month, day of month, hour, minute, second, day of week and day of year
	char time_string[40];
	gettimeofday(&tv, NULL);// this function put the current time in seconds in value of first argumet's pointer and reminder nanoseconds in second argumet's pointer
	ptm = localtime(&tv.tv_sec);// this function reform time in seconds to form of tm struct
	strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", ptm);// this function reform the tm struct format to string form
	printf("%s\n", time_string);// finally prints time in stdout
	return 0;
}

