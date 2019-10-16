//This function update the amazon server.

#include <sys/ioctl.h> //System functions, Input Output Control
#include <linux/spi/spidev.h> //SPI Communications
#include <fcntl.h>
#include <iostream>
#include <cstring>    //String Functions
#include <stdlib.h>
#include <wiringPi.h>//GPIO Driving
#include <ctime>
#include <fstream>
using namespace std;

int main ()
	{
	while(1)
		{
        //Setting the path to aws folder
        system("PATH=/usr/bin:/usr/local/bin");
        //Uploading the system value file to server.
		system("aws s3 cp /home/pi/mgIOT/System_Files/sysValues.jam s3://mgiot");
		delay(3000);
		//Updating the webpage to server.
		system("aws s3 cp //home/pi/mgIOT/webSite/acountJibin.html s3://mgiot");
		}
	return 0;
	}

