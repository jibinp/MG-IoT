/*
 This is the main program used in micro-grid integrated with IOT project

*/

/*************************Declaring the Header Files*****************************/

#include <sys/ioctl.h> //System functions, Input Output Control
#include <linux/spi/spidev.h> //SPI Communications
#include <fcntl.h> //Function control
#include <iostream> //output input control
#include <cstring>    //String Functions
#include <stdlib.h> //Standard Library
#include <wiringPi.h>//GPIO Driving
#include <ctime> //system Time
#include <fstream> //File management


using namespace std;

/*************************End of Declaring the Header Files**********************/


/*************************Declaring the variables********************************/

unsigned int speed = 1000000;  //SPI Transfer Speed

//System variables
int results;
int fd;
int sys_Status = 1;
float batteryVolt = 0;
float renCurrent = 0 ;
float mainCurrent = 0 ;

float renVolt = 110 ;
float mainVolt = 110 ;


unsigned char recvDataByte; //SPI receive buffer

//Device code array
//unsigned char dvcCdArry[3] = {'b','r','m'};
unsigned char dvcCdArry[3] = {'0','1','2'};

//Handshake variables
unsigned char handshake = 'r';
unsigned char ackReciv  = 'a';

//Input min and max values.
const int minIn = 0;
const int maxIn = 1024;

int errVal = 9999; //Error value
float rc;
float mc;

//Current and voltage max and min values in Arduino analog read range
//From 0 to 1024.
int batteryVoltageMin = 0;
int batteryVoltageMax = 1024;

int renCurrentMin = 0;
int renCurrentMax = 1024;

int mainCurrentMin = 0;
int mainCurrentMax = 1024;

//Current and voltage max and min values in Current and Voltage range
float batteryActVoltageMin = 0;     // 0V
float batteryActVoltageMax = 12.00; //12V

float renActCurrentMin = 0;   //0A
float renActCurrentMax = 10.00;  //10A

float mainActCurrentMin = 0;       //0A
float mainActCurrentMax = 10.00;  //10A


//Current and voltage threshold values in Arduino analog read range
//From 0 to 1024.
int batteyThresholdV = 500;
int renThresholdA = 500;
int mainThresholdA = 500;

//Current and voltage threshold values in Current and Voltage range
float batteyActThresholdV = 10.00;  //11V
float renActThresholdA = 2.0;  //2A
float mainActThresholdA = 10.00; //2A


//Defining the GPIO Pins

int ledPwr = 2;   // BCM_GPIO0 pin 17 (Pi - 11)
int ledIndc = 3;  // BCM_GPIO1 pin 18 (Pi - 12)

int batteyIndc = 4; // BCM_GPIO0 pin 27 (Pi - 13)
int renwRelay = 0;  // BCM_GPIO1 pin 17 (PI - 11)
int mainRelay = 1;  // BCM_GPIO2 pin 18 (Pi - 12)

int renRelaySts = 0;
int batSts = 0;

/**Unions allow variables to occupy the same memory space a convenient way to move
back and forth between 8-bit and 16-bit values etc.

Here 4 unions are declared :
1. battery level
2. Renewable source current level.
3. Main line current.
4. For receiveing the data.
**/

union bBuffer_T
  {
  int bInt;
  unsigned char  bChar [2];
  } bBuffer;

union rBuffer_T
  {
  int rInt;
  unsigned char  rChar [2];
  } rBuffer;

union mBuffer_T
  {
  int mInt;
  unsigned char  mChar [2];
  } mBuffer;

union recvDataBuffer_T
  {
  int recvDataInt;
  unsigned char  recvDataChar [2];
  } recvDataBuffer;

/************************End of Declaring the variables**************************/

/*************************Declaring the Functions********************************/

void spiInt (void);
void mainLoop (void);
void myDelay(long);
int spiTxRx(unsigned char);
void spiTx(unsigned char);
int spiRx(void);
int pingArduino(char i);
void arduinoPingSPItest(void);
void ledBlink (int,long);
void printSystemStatus (void);
void end (void);

void batteryVoltCheck (void);
void renCurrentCheck (void);
void mainCurrentCheck (void);
void relayAct (void);
void sysFileUpdate(void);


/*************************End of Declaring the Functions*************************/


/*************************Main Function******************************************/

int main (void)
  {
  //Setting wiring Pi
  if (wiringPiSetup () == -1)
    cout << "Wiring Pi failed to load. " << endl;
  else cout << "Wiring Pi loaded sucessfully." << endl;

  pinMode(ledPwr,OUTPUT);
  pinMode(ledIndc,OUTPUT);
  digitalWrite(ledPwr,HIGH);
  digitalWrite(ledIndc,LOW);

  system ("clear"); //Clear screen
  spiInt(); //Intialize the SPI

  //LED connected for battery configured as output (Testing Purpose).
  pinMode(batteyIndc, OUTPUT);

  //Relay connected to renwable source configured as output.
  pinMode (renwRelay, OUTPUT) ;

  //Relay connected to main source configured as output.
  pinMode (mainRelay, OUTPUT) ;

  digitalWrite(renwRelay,LOW);
  digitalWrite(mainRelay,LOW);
  //arduinoPingSPItest();

  ledBlink(10,50);

  mainLoop();//The main continous loop function

  end();
  }


/*************************End of Main Function***********************************/


/*************************Continous loop Function********************************/
//The main continous loop function
void mainLoop (void)
  {
   char ch;
   while (1)
    {
      digitalWrite(mainRelay,LOW);
     /*cout << "\n\n(Press ESC and then hit Enter to exit testing)" << endl;
     cout << "Press any key to continue : " << endl;
    cin >> ch;
     if(int(ch) == 27)
      break;*/
    batteryVoltCheck();
    renCurrentCheck ();
    mainCurrentCheck ();
    relayAct();
    sysFileUpdate();

    //htmlRefresh();

    if(sys_Status)
      {
      cout << sys_Status << endl;
      printSystemStatus ();
      }
    }
  }

/*************************End of Continous loop Function*************************/

/*************************SPI Intialization**************************************/

void spiInt (void)
  {
  //Setup SPI Open file spidev0.0 (chip enable 0)
  //for read/write access
  //with the file descriptor "fd"
  cout << "SPI Initializing........" << endl;
  fd = open("/dev/spidev0.0", O_RDWR);
  cout << "Opened \"/dev/spidev0.0\" sucessfully in Read and Wrte mode. " << endl;

  //Configure transfer speed (1MkHz)
  ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
  cout << "Configured the SPI Clock speed as " << speed/1000000 << " MHz" << endl;
  cout << "SPI Initialized sucessfully..." << endl << endl << endl;
  }

/*************************End of SPI Intialization*******************************/


/**************************SPI Tx Rx Function************************************/

/**Transmits one byte via the SPI device, and returns one byte as the result.
Establishes a data structure, spi_ioc_transfer as defined by spidev.h
and loads the various members to pass the data and configuration parameters
to the SPI device via IOCTL

Local variables txDat and rxDat are defined and passed by reference. **/

int spiTxRx(unsigned char txDat)
  {
  unsigned char rxDat;

  struct spi_ioc_transfer spi;

  memset (&spi, 0, sizeof (spi));

  spi.tx_buf  = (unsigned long)&txDat;
  spi.rx_buf  = (unsigned long)&rxDat;
  spi.len     = 1;

  ioctl (fd, SPI_IOC_MESSAGE(1), &spi);

  return rxDat;
  }


/*************************End of SPI Tx Rx Function*****************************/


/*************************Arduino Ping Function*********************************/

/**Send a formatted command sequence/packet to the Arduino one byte at and
capture the results**/


int pingArduino(char command)
  {
  unsigned char recvAck = '\0';
  unsigned char recvData_0 = '\0';
  unsigned char recvData_1 = '\0';
  unsigned char recvData[2] ;
  int count = 0;
  int recvCount[2] = {0,0};
  bool ack;
  //ledBlink(5,50);
  /**An initial handshake sequence sends a one byte start code ('r') and
  loops endlessly until it receives the one byte acknowledgment code ('a')
  and sets the ack flag to true.
  The loop also sends the command byte while still in handshake sequence
  to avoid wasting a transmit cycle**/
pingStart :
  do
    {
    ack = false;

    spiTxRx(handshake);
    myDelay (50);

    recvAck = spiTxRx(command);
    if (recvAck == ackReciv)
      {
      ack = true;
      }

    }while (ack == false);




  recvData_0 = spiTxRx(0);
  recvDataBuffer.recvDataChar[0] = recvData_0;
  myDelay (50);


  recvData_1 = spiTxRx(0);
  recvDataBuffer.recvDataChar[1] = recvData_1;
  myDelay (50);

  return recvDataBuffer.recvDataInt;

  }




/*************************myDelay Function**************************************/
//Delay function used to get one millisecond delay.
void myDelay(long millSec)
  {
  unsigned long oneMillVal = 100;
  for(int mS = 0; mS < millSec; mS++)
    {
    for(unsigned long delayLoop = 0; delayLoop < oneMillVal ; delayLoop++)
      {}
    }
  }

/************************ End of myDelay Function******************************/



/*************************Arduino ping SPI test********************************/


void arduinoPingSPItest(void)
  {
  //An endless loop that repeatedly sends the demonstration
  //commands to the Arduino and displays the results

  while (1)
    {
    char dcodTst;
    cout << "Arduino ping SPI testing......" << endl;
    cout << "(Press ESC and then hit Enter to exit testing)" << endl;
    cout << "\tEnter the device code : " ;
    cin >>dcodTst;
    if(int(dcodTst) == 27)
      break;
    results = pingArduino(dcodTst);
    cout << "\tResults : " <<  (int)(results) << endl << endl;
    //sleep(1);
    }
  }

/*************************End of Arduino ping SPI test*************************/


/*************************batteryVoltCheck Function****************************/
//Checks and responds for battery voltage

void batteryVoltCheck (void)
  {
  batteryVolt = 0;
  results = 0;
  results = pingArduino(dvcCdArry[0]);
  batteryVolt = (float)results/50 ;
  cout << "Battery Voltage : " <<  batteryVolt << "V" << endl;
  if(batteryVolt <= batteyActThresholdV)
    batSts = 0;
  else batSts = 1;


  }

/*************************End of batteryVoltCheck Function********************/


/*************************renCurrentCheck Function****************************/
//Checks and responds for load through renewable source

void renCurrentCheck (void)
  {

  renCurrent = 0 ;
  results = 0;
  results = pingArduino(dvcCdArry[1]);
  renCurrent = (float)results/100 ;

  if(renCurrent >= renActThresholdA)
    renRelaySts = 0;
  else renRelaySts = 1;



  }

/*************************End of renCurrentCheck Function********************/


/*************************mainCurrentCheck Function**************************/
//Checks and responds for load through main source

void mainCurrentCheck (void)
  {
  mainCurrent = 0 ;
  results = 0;
  results = pingArduino(dvcCdArry[2]);
  mainCurrent = (float)results/100 ;

  //if(mainCurrent >= mainActThresholdA)
    //digitalWrite (mainRelay, 0);
  //else digitalWrite (mainRelay, 1);

  }

/*************************End of mainCurrentCheck Function*******************/

/*************************Relay Activating**********************************/

void relayAct()
  {
      //Relay acting function
  int r = 0, m = 0, b;
  if(!batSts) //battery voltage < threshold`
    {
    digitalWrite(renwRelay,1);

    }

  else if(!renRelaySts)
      {
    digitalWrite(renwRelay,1);

    }
  else {
    digitalWrite(renwRelay,0);


    }

     }


/*************************End of Relay Activating*******************/


/*************************sysFileIpdate*****************************/

//This function update the system file and this system file acting as global file.
void sysFileUpdate ()
  {
   ofstream outputFile;
   //Opening the file.
   outputFile.open("/home/pi/mgIOT/System_Files/sysValues.jam");
   if(!outputFile)
      cout << "Unable to open the file sysValue.jam" << endl;

    //Updating the file
   outputFile << renActCurrentMax;
   outputFile << ",";
   outputFile << (rc*100/renActCurrentMax);
   outputFile << ",";
   outputFile << rc;
   outputFile << ",";
   outputFile << renVolt;
   outputFile << ",";
   outputFile << renVolt*rc;
   outputFile << ",\n";

   outputFile << mainActCurrentMax;
   outputFile << ",";
   outputFile << (mc*100/mainActCurrentMax);
   outputFile << ",";
   outputFile << mc;
   outputFile << ",";
   outputFile << mainVolt;
   outputFile << ",";
   outputFile << mainVolt*mc;
   outputFile << ",\n";

   outputFile << batteryActVoltageMax;
   outputFile << ",";
   outputFile << (batteryVolt*100/batteryActVoltageMax);
   outputFile << ",";
   outputFile << batteryVolt;
   outputFile << ",";
   outputFile << "1";
   outputFile << ",";
   outputFile << "0";
   outputFile << ";";


   outputFile.close();
  }


/*************************End of sysFileIpdate**********************/

/*************************ledBlink*******************************************/

void ledBlink (int count_B,long speed_B)
  {
    //Led Blink for indication
  for (int blink_I = 0; blink_I < count_B; blink_I++)
    {
    digitalWrite (ledIndc, 1) ;       // On
    delay (speed_B) ;               // mS
    digitalWrite (ledIndc, 0) ;       // Off
    delay (speed_B) ;
    }
  }

/************************ End of ledBlink************************************/

/*************************printSystemStatus Function*************************/

void printSystemStatus(void)
  {
      //Printing the system status
  cout << "Battery Voltage : " <<  batteryVolt << "V" << endl;
  cout << "Load through renewable source : " <<  rc << "A" << endl;
  cout << "Load through main source : " <<  mc << "A" << endl;
  }

/************************ End of printSystemStatus Function******************/

/*************************End Function***************************************/

void end(void)
  {
      //Shutting down the system
  system ("clear");
  cout << "\n\n\tThe system gonna shutdown...." << endl ;
  for(long iEnd = 0; iEnd <92213247; iEnd++)
    {}
  system ("clear");
  digitalWrite(ledPwr,LOW);
  digitalWrite(ledIndc,LOW);
  }

/************************ End of End Function********************************/

