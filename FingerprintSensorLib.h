/****************************************************************************

	This lib is under the GPU, General Public Licence, feel free to dowload it and modify
	at your will. I`ll let some intructions grabed from the manual in a more didatic 
	way for beginers as me, since I had to strugle I little bit to start creating this lib.

	If you downloaded this lib from an external souce, please come visit the original link
	of its repository on my GitHUb -->

****************************  HAVE FUN !!! *************************************/

#ifndef _FINGERPRINTSENSORLIB_h
#define _FINGERPRINTSENSORLIB_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#ifdef __AVR__
#include <SoftwareSerial.h>
#endif

// Let it defined is you want to see the Incoming and outging packets, plus more helpfull information to the debug process
// #define debug

class FingerprintSensorLibClass
{
 public:
	
	FingerprintSensorLibClass(SoftwareSerial *);
	FingerprintSensorLibClass(HardwareSerial *);

	bool init(uint32_t baudRate);
	bool start(void);
	bool pegarImagem(void);
	bool img2tz(byte param);
	bool criarModelo(void);
	bool pegarModelo(char*);
	bool excluirDB(void);
	//bool upImagem(void);
	
 protected:

	/*
		These functions are responsible for sending the right intructions to
		the finger print Sensor via Serial Write. They are very simple, but not to
		be messe with, since it last column in the arrays is the aritimetic sum of the
		bytes, in HEX format, inside each column
	*/

	void FINGERPRINT_START(void);
	void FINGERPRINT_GET_IMAGE();
	void FINGERPRINT_2_TZ(byte param);
	void FINGERPRINT_GEN_TEMPLATE(void);
	void FINGERPRINT_GET_TEMPLATE(void);
	void FINGERPRINT_DELETE_ALL(void);

 private:
	 
	 Stream *actSerial; // Actual Serial, a pointer that points to the actual Serial object used by the finger print sensor
#ifdef __AVR__
	 SoftwareSerial *softSerial;
#endif
	 HardwareSerial *hardSerial;

};

#endif

