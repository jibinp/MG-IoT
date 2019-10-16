//This file update the html page with the current values.
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <ctype.h>
using namespace std;

//Defing the variables.
char sysValues[100][100];
char htmlData[10000][10000];
string chkStr[100];
int chkStr_Len[100];
int chkStr_Cnt = 0;
string endStr[100];
int endStr_Len[100];

int matX = 0;
int matY = 0;
int XMax = 0;
int YMax = 5;

int hX = 0;
int hY = 0;
int hX_Max = 240;
int hY_Max;

//The file pointers.
int rMax_Ptr = 0, rPer_Ptr = 1, rCur_Ptr = 2, rVolt_Ptr = 3, rPwr_Ptr = 4;
int mMax_Ptr = 5, mPer_Ptr = 6, mCur_Ptr = 7, mVolt_Ptr = 8, mPwr_Ptr = 9;
int bMax_Ptr = 10, bPer_Ptr = 11, bCur_Ptr = 12, sysStus = 13;


int _read__SysValues (void);
void importHtml (void);
void checkString (void);
void altValues(int,int);
void exportHtml(void);
void displayHtml(void);
void loadHtml(void);


int main ()
    {
//Continus loop that manipulates the program.
    while (1)
        {
        loadHtml();
        _read__SysValues ();
        //importHtml ();
        //displayHtml();
        checkString();
        exportHtml();
        //displayHtml();
        }
    }


    //This function pass a string to another function and check it.
void checkString(void)
    {

    chkStr_Cnt = 0;
    int sysSt = sysValues[sysStus][0] ;



    //Renewable Maximum Value
    chkStr[chkStr_Cnt] = "<p>Load through renewable source : (Max : ";
    chkStr_Len[chkStr_Cnt] = strlen("<p>Load through renewable source : (Max : ");
    endStr[chkStr_Cnt] = "A) </p>";
    endStr_Len[chkStr_Cnt] = strlen("A) </p>");
    altValues(chkStr_Cnt,rMax_Ptr);
    chkStr_Cnt++;

    //Renewable Percentage Value
    chkStr[chkStr_Cnt] = "w3-padding w3-green\" style=\"width: ";
    chkStr_Len[chkStr_Cnt] = strlen("w3-padding w3-green\" style=\"width: ");
    endStr[chkStr_Cnt] = "%\">";
    endStr_Len[chkStr_Cnt] = strlen("%\">");
    altValues(chkStr_Cnt,rPer_Ptr);
    chkStr_Cnt++;

    //Renewable Current  Value
    chkStr[chkStr_Cnt] = "<ld_thr_rn_src>";
    chkStr_Len[chkStr_Cnt] = strlen("<ld_thr_rn_src>");
    endStr[chkStr_Cnt] = "A<end1>";
    endStr_Len[chkStr_Cnt] = strlen("A<end1>");
    altValues(chkStr_Cnt,rCur_Ptr);
    chkStr_Cnt++;

     //Renewable Percentage  Value Display
    chkStr[chkStr_Cnt] = "<end1>(";
    chkStr_Len[chkStr_Cnt] = strlen("<end1>(");
    endStr[chkStr_Cnt] = "%) </div";
    endStr_Len[chkStr_Cnt] = strlen("%) </div");
    altValues(chkStr_Cnt,rPer_Ptr);
    chkStr_Cnt++;

    //Main load Maximum Value
    chkStr[chkStr_Cnt] = "<p>Main Line Load : (Max : ";
    chkStr_Len[chkStr_Cnt] = strlen("<p>Main Line Load : (Max : ");
    endStr[chkStr_Cnt] = "A) </p>";
    endStr_Len[chkStr_Cnt] = strlen("A) </p>");
    altValues(chkStr_Cnt,mMax_Ptr);
    chkStr_Cnt++;

    //Main load Percentage Value
    chkStr[chkStr_Cnt] = "w3-padding w3-orange\" style=\"width: ";
    chkStr_Len[chkStr_Cnt] = strlen("w3-padding w3-orange\" style=\"width: ");
    endStr[chkStr_Cnt] = "%\">";
    endStr_Len[chkStr_Cnt] = strlen("%\">");
    altValues(chkStr_Cnt,mPer_Ptr);
    chkStr_Cnt++;

    //Main load Current  Value
    chkStr[chkStr_Cnt] = "<ld_thr_mn_src>";
    chkStr_Len[chkStr_Cnt] = strlen("<ld_thr_mn_src>");
    endStr[chkStr_Cnt] = "A<end2>";
    endStr_Len[chkStr_Cnt] = strlen("A<end2>");
    altValues(chkStr_Cnt,mCur_Ptr);
    chkStr_Cnt++;

     //Main load Percentage  Value Display
    chkStr[chkStr_Cnt] = "<end2>(";
    chkStr_Len[chkStr_Cnt] = strlen("<end2>(");
    endStr[chkStr_Cnt] = "%) </div";
    endStr_Len[chkStr_Cnt] = strlen("%) </div");
    altValues(chkStr_Cnt,mPer_Ptr);
    chkStr_Cnt++;

     //Battery Volt Maximum Value
    chkStr[chkStr_Cnt] = "<p>Battery Voltage : (Max : ";
    chkStr_Len[chkStr_Cnt] = strlen("<p>Battery Voltage : (Max : ");
    endStr[chkStr_Cnt] = "V) </p>";
    endStr_Len[chkStr_Cnt] = strlen("V) </p>");
    altValues(chkStr_Cnt,bMax_Ptr);
    chkStr_Cnt++;

    //Battery Volt Percentage Value
    chkStr[chkStr_Cnt] = "w3-padding w3-red\" style=\"width: ";
    chkStr_Len[chkStr_Cnt] = strlen("w3-padding w3-red\" style=\"width: ");
    endStr[chkStr_Cnt] = "%\">";
    endStr_Len[chkStr_Cnt] = strlen("%\">");
    altValues(chkStr_Cnt,bPer_Ptr);
    chkStr_Cnt++;

    //Battery Volt Current  Value
    chkStr[chkStr_Cnt] = "<btry_Volt>";
    chkStr_Len[chkStr_Cnt] = strlen("<btry_Volt>");
    endStr[chkStr_Cnt] = "V<end3>";
    endStr_Len[chkStr_Cnt] = strlen("V<end3>");
    altValues(chkStr_Cnt,bCur_Ptr);
    chkStr_Cnt++;

     //Battery Volt Percentage  Value Display
    chkStr[chkStr_Cnt] = "<end3>(";
    chkStr_Len[chkStr_Cnt] = strlen("<end3>(");
    endStr[chkStr_Cnt] = "%) </div";
    endStr_Len[chkStr_Cnt] = strlen("%) </div");
    altValues(chkStr_Cnt,bPer_Ptr);
    chkStr_Cnt++;


    //Table//////

    //Renewable line Voltage - Table
    chkStr[chkStr_Cnt] = "<td>Renewable line Voltage : </td><td><i>";
    chkStr_Len[chkStr_Cnt] = strlen("<td>Renewable line Voltage : </td><td><i>");
    endStr[chkStr_Cnt] = "V</i>";
    endStr_Len[chkStr_Cnt] = strlen("V</i>");
    altValues(chkStr_Cnt,rVolt_Ptr);
    chkStr_Cnt++;

    //Renewable line Current : - Table
    chkStr[chkStr_Cnt] = "<td>Renewable line Current : </td><td><i>";
    chkStr_Len[chkStr_Cnt] = strlen("<td>Renewable line Current : </td><td><i>");
    endStr[chkStr_Cnt] = "A</i>";
    endStr_Len[chkStr_Cnt] = strlen("A</i>");
    altValues(chkStr_Cnt,rCur_Ptr);
    chkStr_Cnt++;

    //Renewable line Power : - Table
    chkStr[chkStr_Cnt] = "<td>Renewable line Power : </td><td><i>";
    chkStr_Len[chkStr_Cnt] = strlen("<td>Renewable line Power : </td><td><i>");
    endStr[chkStr_Cnt] = "W</i>";
    endStr_Len[chkStr_Cnt] = strlen("W</i>");
    altValues(chkStr_Cnt,rPwr_Ptr);
    chkStr_Cnt++;

    //Main line Voltage - Table
    chkStr[chkStr_Cnt] = "<td>Main line Voltage : </td><td><i>";
    chkStr_Len[chkStr_Cnt] = strlen("<td>Main line Voltage : </td><td><i>");
    endStr[chkStr_Cnt] = "V</i>";
    endStr_Len[chkStr_Cnt] = strlen("V</i>");
    altValues(chkStr_Cnt,mVolt_Ptr);
    chkStr_Cnt++;

    //Main line Current : - Table
    chkStr[chkStr_Cnt] = "<td>Main line current : </td><td><i>";
    chkStr_Len[chkStr_Cnt] = strlen("<td>Main line current : </td><td><i>");
    endStr[chkStr_Cnt] = "A</i></td>";
    endStr_Len[chkStr_Cnt] = strlen("A</i></td>");
    altValues(chkStr_Cnt,mCur_Ptr);
    chkStr_Cnt++;

    //Main line Power : - Table
    chkStr[chkStr_Cnt] = "<td>Main line Power : </td><td><i>";
    chkStr_Len[chkStr_Cnt] = strlen("<td>Main line Power : </td><td><i>");
    endStr[chkStr_Cnt] = "W</i>";
    endStr_Len[chkStr_Cnt] = strlen("W</i>");
    altValues(chkStr_Cnt,mPwr_Ptr);
    chkStr_Cnt++;

    //Battery Voltage : - Table
    chkStr[chkStr_Cnt] = "<td>Battery voltage : </td><td><i>";
    chkStr_Len[chkStr_Cnt] = strlen("<td>Battery voltage : </td><td><i>");
    endStr[chkStr_Cnt] = "V</i>";
    endStr_Len[chkStr_Cnt] = strlen("V</i>");
    altValues(chkStr_Cnt,bCur_Ptr);
    chkStr_Cnt++;

    }


void importHtml (void)
    {
        //Import the html file
    hX = 0;
    hY = 0;
    ifstream instream("/home/pi/mgIOT/webSite/acountJibin.html");
    if(!instream)
        cout << "Cannot open file \"/home/pi/mgIOT/webSite/acountJibin.html\" " << endl;
    while (instream >> htmlData[hX][hY])
        {

        if (instream.peek() == '\n')
            {
            hY++;
            htmlData[hX][hY] = '\0';
            hY=-1;
            htmlData[hX][hY] = '\0';
            hX++;
            }
        if (instream.peek() == ' ')
            {
            hY++;
            htmlData[hX][hY] = ' ';
            }

 if (htmlData[hX][hY] == '>')
            {
            hY++;
            htmlData[hX][hY] = '\0';
            hY=-1;
            htmlData[hX][hY] = '\0';
            hX++;
            }
 hY++;

        }

    hX_Max = hX;
    instream.close();
    }
//This function update the html page with new values.
void exportHtml(void)
    {
    ofstream outputFile;
    outputFile.open("/home/pi/mgIOT/webSite/acountJibin.html");

    for(int exprtCnt_X=0; exprtCnt_X<hX_Max ;exprtCnt_X++)
        {
         for(int exprtCnt_Y=0; htmlData[exprtCnt_X][exprtCnt_Y] != '\0' ;exprtCnt_Y++)
            outputFile << htmlData[exprtCnt_X][exprtCnt_Y];
        outputFile <<  '\n';
        }

    outputFile.close();
    system("sudo cp /home/pi/mgIOT/webSite/acountJibin.html /var/www/html");
    }

    //This function display the html page
void displayHtml(void)
    {
        cout << "Display";
    int displyCnt = 0;
    for(int displyCnt_X=0; displyCnt_X<hX_Max ;displyCnt_X++)
        {
         for(int displyCnt_Y=0; htmlData[displyCnt_X][displyCnt_Y] != '\0' ;displyCnt_Y++)
            cout << htmlData[displyCnt_X][displyCnt_Y];
        cout <<  '\n';
        }
    }

    //Reding the system values from system values file.
int _read__SysValues ()
    {
    matX = 0;
    matY = 0;
    char read_sysValues;
    ifstream inFile;
    inFile.open("/home/pi/mgIOT/System_Files/sysValues.jam");
    if (!inFile)
        {
        cout << "Unable to open file";
        return 1; // terminate with error
        }
    while (inFile >> read_sysValues)
        {
        //cout << endl << read_sysValues
        if (read_sysValues != ';')
            {
            if (read_sysValues == ',')
                {
                //cout << "\nMatx = " << matX;
                 sysValues[matX][matY] = '\0';
                 matX++;
                 matY = 0;
                }
            else
                {
                sysValues[matX][matY] = read_sysValues;
                matY++;
                }
            }
        else break;
        }
        //cout << endl;
       /* for(int i = 0; i <=matX; i++)
            {
             for(int j = 0; sysValues[i][j] != '\0'; j++)
                cout << sysValues[i][j];
             cout << endl;
            }*/
    XMax = matX;
    inFile.close();
    }



void altValues(int pntr, int dataPntr)
    {
    int html_X_Pntr = 0;
    int html_Y_Pntr = 0;
    string sTag = chkStr[pntr];
    string endTag = endStr[pntr];
    char tempStr[1000];
    int sTag_Len = chkStr_Len[pntr];
    sTag_Len--;
    int sTag_Var = 0;
    int chkP;

    //Finding where the code string is located
    for(html_X_Pntr=0; html_X_Pntr <= hX_Max; html_X_Pntr++)
        {
        for(html_Y_Pntr=0; htmlData[html_X_Pntr][html_Y_Pntr] != '\0'; html_Y_Pntr++)
            {
            if((htmlData[html_X_Pntr][html_Y_Pntr]) == (sTag [sTag_Var]))
                {
                //cout << htmlData[html_X_Pntr][html_Y_Pntr];
                sTag_Var++;
               if(sTag_Var == sTag_Len)
                    goto point_1;
                }
            else
                {
                sTag_Var = 0;
                //cout << endl;
                }
            }
        }
point_1 :


    //Skipping the <end> string
    int endTag_Var = 0;
    int temp;
    for(temp = html_Y_Pntr; htmlData[html_X_Pntr][temp] != '\0'; temp++)
        {
        if(htmlData[html_X_Pntr][temp] == endTag [endTag_Var])
            {
            endTag_Var++;
            if(endTag_Var == endStr_Len[pntr])
                break;
            }
        else endTag_Var = 0;
        }

        html_Y_Pntr+=2;

   //Copying the string after the end string to a temp array
     int tp = 0;
    for(temp; htmlData[html_X_Pntr][temp] != '\0' ; temp++)
        {
        tempStr[tp] = htmlData[html_X_Pntr][temp];
        tp++;
        }
    tempStr[tp] = '\0';


    //new system values are copying to the current html Pointer
    for(int i = 0; sysValues[dataPntr][i] !='\0';  i++)
        {
        htmlData[html_X_Pntr][html_Y_Pntr] = sysValues[dataPntr][i];
        html_Y_Pntr ++;
        }
 //Copying end string to new location
    for(int i=0; endStr[pntr][i] != '\0'; i++)
        {
         htmlData[html_X_Pntr][html_Y_Pntr] = endStr[pntr][i] ;
         html_Y_Pntr ++;
        }

    //Copyin temp string to new location
    for(int i = 1; tempStr[i] !='\0';  i++)
        {
        htmlData[html_X_Pntr][html_Y_Pntr]= tempStr[i];
        html_Y_Pntr ++;
        }

    htmlData[html_X_Pntr][html_Y_Pntr] = '\0';

    }


