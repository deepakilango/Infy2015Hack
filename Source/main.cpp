// Includes
#include "CBebopInterface.h"
#include "Utility.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <iostream>
#include "redis3m/redis3m.hpp"

// Namespaces
using namespace rebop;
using namespace redis3m;
using namespace std;

#define TAKE_OFF "TAKEOFF-"
#define WAIT "WAIT----"
#define FORWARD "FORWARD-"
#define BACKWARD "BACKWARD"
#define TAKEPIC "TAKEPIC-"
#define TURNRIGHT "TURNRIGH"
#define LAND "LAND----"
#define TURNLEFT "TURNLEFT"


void runOnKeyStrokes(CBebopInterface bebop){
	int indexCtr = 0;
	bool flag = true;
	char key;
	initscr();
	keypad(stdscr, TRUE);
	cbreak();
	echo();
	TPilotCommand commandIn;  
	bebop.setAltitudeListener(commandIn);
        while (flag)
        {
            key = getch();
            
            if ((key == 27) || (key =='q'))
            {
		std::cout << "DroneIT:: terminating...";
		std::cout << key;
		//LOG( INFO ) << "Enter anything to land.";
		//std::cin >> temp;

		// Land
		bebop.Land();

		flag = false;
            }
            else if(key == 'a')
            {
		std::cout << "DroneIT:: LEFT";
		
			commandIn.flag = 0;
			commandIn.roll = 0;
			commandIn.pitch = 0;
			commandIn.yaw = 15;
			commandIn.gaz = 0;
			commandIn.psi = 0;
		bebop.SendPilotCommand(commandIn);
            }
            else if(key == 'x')
            {
		std::cout << "DroneIT:: DOWN";
		// Land
		bebop.Land();
            }
            else if(key == 'w')
            {
		std::cout << "DroneIT:: UP";
		// Take off
		bebop.Takeoff();
            }
            else if(key == 'd')
            {
		std::cout << "DroneIT:: RIGHT";
			commandIn.flag = 0;
			commandIn.roll = 0;
			commandIn.pitch = 0;
			commandIn.yaw = -15;
			commandIn.gaz = 0;
			commandIn.psi = 0;
		bebop.SendPilotCommand(commandIn);
            }
            else if(key == 's')
            {
		std::cout << "DroneIT:: CENTER";
			commandIn.flag = 0;
			commandIn.roll = 0;
			commandIn.pitch = 0;
			commandIn.yaw = 0;
			commandIn.gaz = 0;
			commandIn.psi = 0;
		bebop.SendPilotCommand(commandIn);
            }
            else if(key == 'u')
            {
		std::cout << "DroneIT:: PITCH UP";
			commandIn.flag = 1;
			commandIn.roll = 0;
			commandIn.pitch = 15;
			commandIn.yaw = 0;
			commandIn.gaz = 0;
			commandIn.psi = 0;
		bebop.SendPilotCommand(commandIn);
            }
            else if(key == 'n')
            {
		std::cout << "DroneIT:: PITCH DOWN";
			commandIn.flag = 1;
			commandIn.roll = 0;
			commandIn.pitch = -15;
			commandIn.yaw = 0;
			commandIn.gaz = 0;
			commandIn.psi = 0;
		bebop.SendPilotCommand(commandIn);
            }

            else if(key == 't')
            {
		std::cout << "DroneIT:: HEIGHT UP";
			commandIn.flag = 1;
			commandIn.roll = 0;
			commandIn.pitch = 15;
			commandIn.yaw = 0;
			commandIn.gaz = 15;
			commandIn.psi = 0;
		bebop.SendPilotCommand(commandIn);
            }
            else if(key == 'g')
            {
		std::cout << "DroneIT:: HEIGHT DOWN";
			commandIn.flag = 1;
			commandIn.roll = 0;
			commandIn.pitch = -15;
			commandIn.yaw = 0;
			commandIn.gaz = -15;
			commandIn.psi = 0;
		bebop.SendPilotCommand(commandIn);
            }

            else if(key == 'h')
            {
		std::cout << "DroneIT:: ROLL LEFT";
			commandIn.flag = 1;
			commandIn.roll = 15;
			commandIn.pitch = 0;
			commandIn.yaw = 0;
			commandIn.gaz = 0;
			commandIn.psi = 0;
		bebop.SendPilotCommand(commandIn);
            }
            else if(key == 'j')
            {
		std::cout << "DroneIT:: ROLL RIGHT";
			commandIn.flag = 1;
			commandIn.roll = -15;
			commandIn.pitch = 0;
			commandIn.yaw = 0;
			commandIn.gaz = 0;
			commandIn.psi = 0;
		bebop.SendPilotCommand(commandIn);
            }
            else if(key == 'p')
            {		
		bebop.takePicture(0);
            } 
            else if(key == 'o')
            {		
		bebop.setCameraAngle(0, 90);
            } 
            else if(key == 'i')
            {		
		bebop.setCameraAngle(0, -90);
            } 
            else if(key == 'm')
            {		
		bebop.initializePictureSettings();
            }
	    else if(key == 'r')
            {		
		bebop.readNavData();
            } 
            else
            {
		std::cout << key;
            }
	std::cout << "\n";
	}

	// Cleanup - Kill the network and clean up memory
	bebop.Cleanup();
	endwin();
}

void sleepPreDetermined() {
	usleep(60000);
	usleep(60000);
	usleep(60000);
	usleep(60000);
	usleep(60000);
}

void runAutoMode(CBebopInterface bebop) {
	TPilotCommand commandIn;  

	std::cout << "DroneIT:: Drone taking off...";
	// Take off
	bebop.Takeoff();
	sleep(3);
	sleepPreDetermined();

	//Set camera angle
	bebop.setCameraAngle(0,-90);
	sleepPreDetermined();

	// Move forward 
	for(int i=0;i<11;i++) {
		if(i%3) {
		   //Take picture
		   bebop.takePicture(0);
		   sleepPreDetermined();
		}

		std::cout << "DroneIT:: Move forward";
		commandIn.flag = 1;
		commandIn.roll = 0;
		commandIn.pitch = 15;
		commandIn.yaw = 0;
		commandIn.gaz = 0;
		commandIn.psi = 0;
		bebop.SendPilotCommand(commandIn);
		sleepPreDetermined();
	}
	//Take picture
	bebop.takePicture(0);
	sleepPreDetermined();

	//Land
	std::cout << "DroneIT:: Drone Landing...";
	bebop.Land();
	sleepPreDetermined();
}

int executeCommand(CBebopInterface bebop, std::string command) {
    TPilotCommand commandIn;  
    int commandLength = command.length();
    std::string trimmedCommand;
    std::string additionalParam;
    int measures=0;
    if(commandLength>=10) {
        trimmedCommand = command.substr(2,8);
    	cout <<"DroneIT:: Trimmed Command: "<<trimmedCommand<<endl;
	if(commandLength>=14) {
            additionalParam = command.substr(11,3);
       	    cout <<"DroneIT:: Additional param: "<<additionalParam<<endl;
	    measures = atoi (additionalParam.c_str());
       	    cout <<"DroneIT:: Measures: "<<measures<<endl;
	}
    }
    if(trimmedCommand == TAKE_OFF) {
	// Take off
	bebop.Takeoff();
	sleepPreDetermined();
	cout << "DroneIT:: Success - Take OFF" << endl;
    }
    else if(trimmedCommand == WAIT) {
	cout << "DroneIT:: Wait (sleep) for "<< measures << " seconds..."  << endl;
	sleep(measures);
    }
    else if(trimmedCommand == FORWARD) {
	cout << "DroneIT:: Moving forward " << measures << " steps... " << endl;
	for(int i=0;i<measures;i++) {
		commandIn.flag = 1;
		commandIn.roll = 0;
		commandIn.pitch = 15;
		commandIn.yaw = 0;
		commandIn.gaz = 0;
		commandIn.psi = 0;
		bebop.SendPilotCommand(commandIn);
		sleepPreDetermined();
	}
    }
    else if(trimmedCommand == BACKWARD) {
	cout << "DroneIT:: Moving backward " << measures << " steps... " << endl;
	for(int i=0;i<measures;i++) {
		commandIn.flag = 1;
		commandIn.roll = 0;
		commandIn.pitch = -15;
		commandIn.yaw = 0;
		commandIn.gaz = 0;
		commandIn.psi = 0;
		bebop.SendPilotCommand(commandIn);
		sleepPreDetermined();
	}
    }
    else if(trimmedCommand == TAKEPIC) {
	cout << "DroneIT:: Taking picture" << endl;
	bebop.takePicture(0);
	sleepPreDetermined();
    }
    else if(trimmedCommand == TURNRIGHT) {
	cout << "DroneIT:: Success - Moving right " << measures << " steps..." << endl;
	for(int i=0;i<measures;i++) {
		commandIn.flag = 0;
		commandIn.roll = -15;
		commandIn.pitch = 0;
		commandIn.yaw = 0;
		commandIn.gaz = 0;
		commandIn.psi = 0;
		bebop.SendPilotCommand(commandIn);
		sleepPreDetermined();
	}
    }
    else if(trimmedCommand == TURNLEFT) {
	cout << "DroneIT:: Success - Moving left " << measures << " steps..." << endl;
	for(int i=0;i<measures;i++) {
		commandIn.flag = 0;
		commandIn.roll = 15;
		commandIn.pitch = 0;
		commandIn.yaw = 0;
		commandIn.gaz = 0;
		commandIn.psi = 0;
		bebop.SendPilotCommand(commandIn);
		sleepPreDetermined();
	}
    }
    else if(trimmedCommand == LAND) {
	bebop.Land();
	sleepPreDetermined();
	cout << "DroneIT:: Success - LAND" << endl;
    }
    else {
	cout << "DroneIT:: Command not recognized... :( " << endl;
	return -1;
    }
    return 1;
}


void runUIMode(CBebopInterface bebop) {
	std::cout << "DroneIT:: Running from UI mode...";
	connection::ptr_t conn = connection::create();
	reply r = conn->run(command("RPOP") << "NavCommands" );
	std::string tempString;
	while(true) {
            if(r.str().size() > 0) {
		std::cout << "DroneIT:: Command received " << r.str() << endl;
		}
	    if(r.str().size() <= 0) {
                //std::cout << "DroneIT:: Invaid Command " << r.str() << endl;
	    }
	    else if(r.str().at(0) == 'Q') {
                std::cout << "DroneIT:: Terminating... " << r.str() << endl;
		// Land
		bebop.Land();
		break;
	    }
	    else if(r.str().at(0) == 'O') {
                std::cout << "DroneIT:: Override received... " << r.str() << endl;
		executeCommand(bebop, r.str());
	    }
	    else if(r.str().at(0) == 'R') {
                std::cout << "DroneIT:: Resume from regular queue... " << r.str() << endl;
		executeCommand(bebop, r.str());
	    }
	    else {
	    	executeCommand(bebop, r.str());
	    }
	    r = conn->run(command("RPOP") << "NavCommands" );
	}
}

int main()
{
	CBebopInterface bebop;
	int rc;
	// Initialize
	bebop.Initialize();

	// Connected and ready to do stuff, theoretically
	if( bebop.IsConnected() == false )
	{
		LOG( ERROR ) << "DroneIT:: No connection! Can't do anything!";
	}
	else
	{
		LOG( ERROR ) << "DroneIT:: Ready to do stuff!";
		//bebop.FlatTrim();
		//runAutoMode(bebop);		
		  runUIMode(bebop);
		//runOnKeyStrokes(bebop);
	}
	// Cleanup - Kill the network and clean up memory

	bebop.Cleanup();
	endwin();
/*
	std::cout << "Started...";
	sleep(10);
	std::cout << "Stop...";
*/
	return 0;
}
