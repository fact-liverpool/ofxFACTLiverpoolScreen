/*
 *  ofxFACTLiverpoolScreen.h
 *
 *  Created by Mar Canet Sola on 7/15/11.
 *  GPLv3.0 2011.
 *  www.gnu.org/copyleft/gpl.html
 *
 */

#ifndef _FRAME_H
#define _FRAME_H

#include "ofMain.h"
#include "ofxUDPManager.h"

#pragma pack(push,1)
struct Packet
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class ofxFACTLiverpoolScreen {
private:	
	int packageSize;	///< size of a package (address+color)
	int nrAddresses;    ///< total number of packages;
	int offsetRed;		///< offset of red color in package
	int offsetGreen;	///< offset of green color in package
	int offsetBlue;		///< offset of blue color in package
	ofImage myScreen;
	
	long lastSendFrame;
	int frameRateMillis;

	
#pragma pack(pop)
	struct Packet frameBuffer [ 2500 ];
	
	ofxUDPManager* myUDPManagerSender;
	ofxUDPManager* myUDPManagerReceiver;
	int totalAddressByPackage;
	long lastTimeReceiveFrame;
	int timeOutReceiveFrame;
	string ipServerFacade;
public:	
	ofxFACTLiverpoolScreen();
	//--------------------------------------------------------------
	~ofxFACTLiverpoolScreen();	
	//--------------------------------------------------------------
	void setupSend();	
	//--------------------------------------------------------------
	void setupReceive();
	//--------------------------------------------------------------
	/**
	 * set all packages to the same color
	 */
	
	void cleanColor();	
	//--------------------------------------------------------------
	/**
	 * set package at address to color
	 */
	
	void setColor(unsigned short address, unsigned char r, unsigned char g, unsigned char b);	
	//--------------------------------------------------------------
	/**
	 * get color from address
	 */
	
	ofColor getColor(unsigned short address);	
	//--------------------------------------------------------------
	/**
	 * send the complete UDP packet to localhost:8080 (might be necessary to change)
	 */
	
	void flush();
	//--------------------------------------------------------------
	/**
	 * receive the complete UDP packet to localhost:8080 (might be necessary to change)
	 */
	
	void receive();	
	//--------------------------------------------------------------
	/**
	 * copy and send the complete UDP packet
	 */
	
	void captureScreen(int x=0, int y=0);
	//--------------------------------------------------------------
	/**
	 * setIp from server of FACADE
	 */
	bool isReceivingPixels();

	void setIpServer(string ip);
};

#endif
