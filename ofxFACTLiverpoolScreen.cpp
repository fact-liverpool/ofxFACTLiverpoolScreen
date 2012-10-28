/*
 *  ofxFACTLiverpoolScreen.h
 *
 *  Created by Mar Canet Sola on 7/15/11.
 *  GPLv3.0 2011.
 *  www.gnu.org/copyleft/gpl.html
 *
 */

#include "ofxFACTLiverpoolScreen.h"

ofxFACTLiverpoolScreen::ofxFACTLiverpoolScreen() 
{
	packageSize = 5;	///< size of a package (address+color)
	nrAddresses = 2500; ///< total number of packages;
	offsetRed	= 2;	///< offset of red color in package
	offsetGreen = 3;	///< offset of green color in package
	offsetBlue	= 4;	///< offset of blue color in package
	totalAddressByPackage = 2500;
	
	printf("%d %d %d\n", sizeof(unsigned short), sizeof(unsigned char), sizeof(Packet));
	// create array
	for (unsigned short address = 0; address < nrAddresses; address++) 
	{
		frameBuffer[address].r = 0;
		frameBuffer[address].g = 0;
		frameBuffer[address].b = 0;
	}
	
	myUDPManagerSender = NULL ;
	myUDPManagerReceiver = NULL ;
	
	myScreen.allocate( ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
	
	lastSendFrame = 0;
	frameRateMillis = 1000/30;
	timeOutReceiveFrame = 1000;
}	
//--------------------------------------------------------------
ofxFACTLiverpoolScreen::~ofxFACTLiverpoolScreen(){
	if(myUDPManagerSender != NULL)		delete myUDPManagerSender;
	if(myUDPManagerReceiver != NULL)	delete myUDPManagerReceiver;
}
//--------------------------------------------------------------
void ofxFACTLiverpoolScreen::setupSend() 
{	
	myUDPManagerSender = new ofxUDPManager();
	myUDPManagerSender->Create();
	myUDPManagerSender->Connect(ipServerFacade.c_str(), 11999);
}

void ofxFACTLiverpoolScreen::setIpServer(string ip){
	ipServerFacade = ip;
}
//--------------------------------------------------------------
void ofxFACTLiverpoolScreen::setupReceive() 
{	
	myUDPManagerReceiver = new ofxUDPManager();
	myUDPManagerReceiver->Create();
	myUDPManagerReceiver->Bind(11999);
	myUDPManagerReceiver->SetNonBlocking(false);
	myUDPManagerReceiver->SetTimeoutReceive(10);
}


//--------------------------------------------------------------
/**
 * set all packages to the same color
 */

void ofxFACTLiverpoolScreen::cleanColor() 
{	
	for (unsigned short address = 0; address < nrAddresses; address++) 
	{
		frameBuffer[address].r = 1;
		frameBuffer[address].g = 1;
		frameBuffer[address].b = 1;
	}
}
//--------------------------------------------------------------
/**
 * set package at address to color
 */

void ofxFACTLiverpoolScreen::setColor(unsigned short address, unsigned char r, unsigned char g, unsigned char b) 
{
	frameBuffer[address].r = r;
	frameBuffer[address].g = g;
	frameBuffer[address].b = b;
}
//--------------------------------------------------------------
/**
 * get color from address
 */

ofColor ofxFACTLiverpoolScreen::getColor(unsigned short address) 
{
	ofColor out;
	out.r = frameBuffer[address].r;
	out.g = frameBuffer[address].g;
	out.b = frameBuffer[address].b;
	return out;
}
//--------------------------------------------------------------
/**
 * send the complete UDP packet to localhost:8080 (might be necessary to change)
 */

void ofxFACTLiverpoolScreen::flush()
{
	if( ofGetElapsedTimeMillis()-lastSendFrame > frameRateMillis ){
		myUDPManagerSender->Send( (char *)&frameBuffer[0],	sizeof(Packet) * 2500 );
		lastSendFrame = ofGetElapsedTimeMillis();
	}
}
//--------------------------------------------------------------
/**
 * receive the complete UDP packet to localhost:8080 (might be necessary to change)
 */

void ofxFACTLiverpoolScreen::receive()
{
	//myUDPManagerReceiver->GetRemoteAddr("192.168.0.221");

	if(myUDPManagerReceiver->GetReceiveBufferSize()>0){
		int total = myUDPManagerReceiver->Receive((char *)&frameBuffer, sizeof(Packet) * 2500 );
		if(total>0){
			lastTimeReceiveFrame = ofGetElapsedTimeMillis();
		}
	}
}
//--------------------------------------------------------------
/**
 * check if in last 10 seconds receive any frame
 */

bool ofxFACTLiverpoolScreen::isReceivingPixels()
{
	return (ofGetElapsedTimeMillis()-lastTimeReceiveFrame) < timeOutReceiveFrame;
}
//--------------------------------------------------------------
/**
 * copy and send the complete UDP packet
 */

void ofxFACTLiverpoolScreen::captureScreen(int x, int y)
{
	if( ofGetElapsedTimeMillis()-lastSendFrame > frameRateMillis ){
		int factorX = 50;
		int factorY = 50;
        myScreen.grabScreen(x, y, factorX, factorY); 
		for(int i=0;i<50;i++)
		{
			for(int j=0;j<50;j++)
			{
				int id = (i*3)+(j*50*3);
				int address = i+(j*50);
				frameBuffer[address].r = myScreen.getPixels()[id];
				frameBuffer[address].g = myScreen.getPixels()[id+1];
				frameBuffer[address].b = myScreen.getPixels()[id+2];
			}
		}
		flush();
        lastSendFrame = ofGetElapsedTimeMillis();
	}
}
//--------------------------------------------------------------
