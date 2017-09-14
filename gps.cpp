#include "hmc5883l.h"
#include <SerialStream.h>
#include <string>
#include <sstream>
#include <math.h>

using namespace std;
using namespace LibSerial;

double dmTOdd(double dm){
    int deg = dm/100;
    double dd = deg+(dm-deg*100.0)/60.0;

    return dd;
}

int main(){
	int portno = 1035;
	char data_packet[256];
	int verbose = 0;
	int ci, cn;
	int cport_nr = 22; 
	int bdrate = 115200;

	unsigned char cbuf[512];

	SerialStream serial;

	HMC5883L hmc5883l;

	int startupAltitude = -1;
	double lat;
	double lon;
	double alt;
	int fixType;
	int satNum;
	double heading;
	int sec = 0, min = 0, hour = 0, day = 0, month = 0, year = 0;

	string gps_input;

	cout.precision(10);

	cout << "Starting GPS...\n";

	//UARL GPS StartUp
	while (!serial.good()) {
		serial.Open("/dev/ttyAMA0");
		serial.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
		serial.SetBaudRate(SerialStreamBuf::BAUD_115200);
		serial.SetNumOfStopBits(1);
		serial.SetFlowControl(SerialStreamBuf::FLOW_CONTROL_NONE);

		if (serial.good()) {
			cout << "GPS has been started (/dev/ttyAMA0).\n";
		}
		else {
			cout << "GPS failed to start, retrying in 3 seconds.\n";
			sleep(3);
		}
	}

	//I2C Compass StartUp
	while(1){
		if(hmc5883l_init(&hmc5883l) != HMC5883L_OKAY){
			cout << "Compass failed to start\n";
		}
		else{
			cout << "Compass has been started (/dev/ttyi2c-1).\n";
			break;
		}
	}

	while(1){

		serial >> gps_input;

		hmc5883l_read(&hmc5883l);

		if (gps_input.length() > 0) {
			if(gps_input.compare(0,6,"$GNGGA") == 0 || gps_input.compare(0,6,"$GNRMC") == 0){

				std::string gps_data[15];
			    int count = 0;
			    
			    std::string word;
			    //std::stringstream stream("$GNGGA,141148.20,40.3815788,N,00839.48463,W,1,06,3.80,30.2,M,49.7,M,,*6E");
			    std::stringstream stream(gps_input);

			    //parse gps data into vector
			    while( getline(stream, word, ',') ){
			        gps_data[count++] = word;
			    }

			    if(gps_input.compare(0,6,"$GNRMC") == 0){
			    	//$GNRMC,152617.00,A,4038.17852,N,00839.50264,W,4.260,312.74,090817,,,A*61
			    	if(gps_data[1].length() > 0 && gps_data[9].length() > 0){

			    		try{
						    hour = std::stoi(gps_data[1].substr(0,2));
						    min = std::stoi(gps_data[1].substr(2,2));
						    sec = std::stoi(gps_data[1].substr(4,2));
						    
						    day = std::stoi(gps_data[9].substr(0,2));
						    month = std::stoi(gps_data[9].substr(2,2));
						    year = std::stoi(gps_data[9].substr(4,2))+2000;
						} 
						catch (const std::invalid_argument& e){
						    cout << "Invalid input" << endl;
						}
						catch (const std::out_of_range& e){
						    cout << "Invalid input" << endl;
						}
			    	}

			    	continue;
			    }

			    if(gps_data[2].length() > 0 && gps_data[4].length() > 0 && gps_data[9].length() > 0){
			    	lat = dmTOdd(atof(gps_data[2].c_str()));
			    	if(gps_data[3] == "S"){
			    		lat *= -1;
			    	}

			    	lon = dmTOdd(atof(gps_data[4].c_str()));
			    	if(gps_data[5] == "W"){
			    		lon *= -1;
			    	}

			    	alt = atof(gps_data[9].c_str());
			    	fixType = atoi(gps_data[6].c_str());
			    	satNum = atoi(gps_data[7].c_str());
			    	heading = hmc5883l._data.orientation_deg;

			    	cout << string(40, '\n');
			    	cout << "GPS DATA:\n";
			    	cout << "Latitude (Deg.M): " << gps_data[2] << "\n";
			    	cout << "Longitude (Deg.M): " << gps_data[4] << "\n";
			    	cout << "-------------------\n";
			    	cout << "Latitude (deg) :" << lat << "\n";
			    	cout << "Longitude (deg):" << lon << "\n";
			    	cout << "Altitude (m)   :" << alt << "\n";
			    	cout << "Time and Date: " << hour << ":" << min << ":" << sec << " " << day << "/" << month << "/" << year << " UTC\n";
			    	cout << "FixType: " << fixType << "\n";
			    	cout << "SatNum: " << satNum << "\n\n";
			    	cout << "Compass DATA:\n";
			    	cout << "Heading (deg): " << heading << "\n";
			    }
			    else{
			    	cout << "No valid GPS signal\n"; 
			    	continue;
			    }
			}
		}
	}

	return 0;
}