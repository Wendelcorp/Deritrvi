//-------------------------------------------------------------------------------------------------------
// 
// Deritrvi 
//
// Category     : VST 2.x Plugin
// Filename     : deritrvi.h
// Created by   : Y4684134
// Description  : Delay/Ringmod/Tremolo/deritrvi plugin
//
//-------------------------------------------------------------------------------------------------------

#ifndef __deritrvi__
#define __deritrvi__

#include "public.sdk/source/vst2.x/audioeffectx.h"

enum
{
	//enumerate all parameters that are accessable to the user
	kNumPrograms = 10,
	kGainMod,		//output control 
	kWet,			//left delay wet control
	kWet2,			//right delay wet control
	kDelay,			//delay time control
	kFreqMod,		//ring modulation frequency control
	kDepthTrem,		//tremolo depth control
	kFreqTrem,		//tremolo speed control
	kDepthVib,		//vibrato depth control
	kFreqVib,		//vibrato speed control

	kNumParams
};

class deritrvi;

//------------------------------------------------------------------------
class deritrviProgram
{
friend class deritrvi;
public:
	deritrviProgram ();
	~deritrviProgram () {}

//container for each set of parameters to receive the snap shots of user settings, these are generic representations 
//of the parameters and not the parameters themselves but places where they will be copied from when the user selects another program
private:	
	float fGainMod;		//output control container
	float fWet;			//left delay wet control container
	float fWet2;		//right delay wet control container
	float fDelay;		//delay time control container
	double fFreqMod;	//ring modulation frequency control container
	float fDepthTrem;	//tremolo depth control container
	float fFreqTrem;	//tremolo speed control container
	float fDepthVib;	//vibrato depth control container
	float fFreqVib;		//vibrato speed control container
	char name[24];		//variable declaration 
};

//------------------------------------------------------------------------
class deritrvi : public AudioEffectX
{
public:
	deritrvi (audioMasterCallback audioMaster);
	~deritrvi ();

	//---from AudioEffect-----------------------
	//Declartion of functions within deretrvi.cpp
	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
	virtual void setProgram (VstInt32 program);
	virtual void setProgramName (char* name);
	virtual void getProgramName (char* name);
	virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);
	virtual void setParameter (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterLabel (VstInt32 index, char* label);
	virtual void getParameterDisplay (VstInt32 index, char* text);
	virtual void getParameterName (VstInt32 index, char* text);
	virtual void resume ();
	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion () { return 1000; }
	virtual VstPlugCategory getPlugategory () { return kPlugCategEffect; }
	
protected:
	deritrviProgram* programs;
	
	//declartion of protected variables within deritrvi.cpp
	double fFreqMod;
	float fGainMod;
	float fDepthTrem;
	float fFreqTrem;
	float fDepthVib;
	float fWet;
	float fWet2;
	float fDelay;
	float fFreqVib;
	long counterTrem;
	long counter;
	char programName[32];



private:
	//declartion of private variables within deritrvi.cpp
	float *Buffer1;
	int	iBufferSize;
	int iWrite1;
	int iRead1;
	float *Buffer2;
	int iWrite2;
	int iRead2;
	float counterRing;
	float *BufferDelay1;
	float *BufferDelay2;
	int	iBufferSizeDelay;
	int iWriteDelay1;
	int iReadDelay1;
	int iWriteDelay2;
	int iReadDelay2;
	
};

#endif
