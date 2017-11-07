#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	long double arr1[10], arr2[10], fraction;
	FILE* fp;
	fp = fopen("/proc/stat","r");
	fscanf(fp, "%*s %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf", &arr1[0], &arr1[1], &arr1[2], &arr1[3], &arr1[4], &arr1[5], &arr1[6], &arr1[7], &arr1[8], &arr1[9]);
	fclose(fp);
	sleep(1);
	fp = fopen("/proc/stat","r");
	fscanf(fp, "%*s %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf %Lf", &arr2[0], &arr2[1], &arr2[2], &arr2[3], &arr2[4], &arr2[5], &arr2[6], &arr2[7], &arr2[8], &arr2[9]);
	fclose(fp);
	fraction = ((arr2[0]+arr2[1]+arr2[2]+arr2[5]+arr2[6]+arr2[7]+arr2[8]+arr2[9])-(arr1[0]+arr1[1]+arr1[2]+arr1[5]+arr1[6]+arr1[7]+arr1[8]+arr1[9])) / ((arr2[0]+arr2[1]+arr2[2]+arr2[3]+arr2[4]+arr2[5]+arr2[6]+arr2[7]+arr2[8]+arr2[9])-(arr1[0]+arr1[1]+arr1[2]+arr1[3]+arr1[4]+arr1[5]+arr1[6]+arr1[7]+arr1[8]+arr1[9]));
	printf("The current CPU utilization is: %Lf percent.\n", fraction*100.0);
	return 0;
}
