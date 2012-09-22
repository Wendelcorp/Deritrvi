//-------------------------------------------------------------------------------------------------------
//
// Deritrvi
//
// Category     : VST 2.x Plugin
// Filename     : deritrvi.cpp
// Created by   : Y4684134
// Description  : Delay/Ringmod/Tremolo/deritrvi plugin
//
//-------------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef __deritrvi__
#include "deritrvi.h"
#endif

#ifndef __deritrvieditor__
#include "deritrvieditor.h"
#endif
//------------------------------------------------------------------------------------------------------- 
deritrviProgram::deritrviProgram ()
{
	//starting values of plug-in controls
	fGainMod = 0.5;
	fWet = 0;
	fWet2 = 0;
	fDelay = 0;
	fFreqMod = 0;
	fDepthTrem = 0;
	fFreqTrem = 0;
	fDepthVib = 0;
	fFreqVib = 0;

	strcpy (name, "Preset");
}

//--------------------------------------------------------------------------------------------------------
deritrvi::deritrvi (audioMasterCallback audioMaster)
	: AudioEffectX (audioMaster, kNumPrograms, kNumParams)
{
	iBufferSize = 1764;													//configures Buffer1 and Buffer 2 to this size
	iRead1 = (iBufferSize/2);											//set initial location from which Buffer1 will be read
	iWrite1 = 0;														//set initial location from which Buffer2 will be written
	Buffer1 = new float[iBufferSize];									//allocate memory for Buffer1 
	iRead2 = (iBufferSize/2);											//set initial location from which Buffer2 will be read
	iWrite2 = 0;														//set initial location from which Bugger2 will be written
	Buffer2 = new float[iBufferSize];									//allocate memory for Buffer2
	iBufferSizeDelay = (int)getSampleRate();							//make sure plug will work correctly for all sample rates
	iReadDelay1 = 0;													//set initial location from which BufferDelay1 will be read
	iWriteDelay1 = iReadDelay1 + (int)(0.5 * getSampleRate());			//set initial location from which BufferDelay1 will be written, delay is 0.5 seconds
	BufferDelay1 = new float[iBufferSizeDelay];							//allocate memory for bufferDelay1
	iReadDelay2 = 0;													//set initial location from which BufferDelay2 will be read
	iWriteDelay2 = iReadDelay2 + (int)(0.5 * getSampleRate());			//set initial location from which BufferDelay2 will be written, delay to 0.5 seconds
	BufferDelay2 = new float[iBufferSizeDelay];							//allocate memory for BufferDelay2
	if(iReadDelay1 >= iBufferSizeDelay) iReadDelay1 -= iBufferSizeDelay;	//make sure buffer 'wraps' around
	if(iReadDelay2 >= iBufferSizeDelay) iReadDelay2 -= iBufferSizeDelay;	//make sure buffer 'wraps' around

	setNumInputs(2);		//stereo in
	setNumOutputs(2);		//stereo out
	setUniqueID('Dela');	//identify
	canMono();				//makes sense to feed both inputs with the same signal
	canProcessReplacing();	//supports both accumulating and replacing output

	editor = new deritrviEditor (this); //build new editor for creation of GUI
	programs = new deritrviProgram[numPrograms];
	if (programs)
		setProgram (0);

	resume ();	//call resume function to flush buffers
}

//--------------------------------------------------------------------------------------------------------
//destructor cleans buffers
deritrvi::~deritrvi ()
{
	if (Buffer1) delete[] Buffer1;
	if (Buffer2) delete[] Buffer2;
	if (BufferDelay1) delete[] BufferDelay1;
	if (BufferDelay2) delete[] BufferDelay2;

}

//--------------------------------------------------------------------------------------------------------
//called when the host wants to change the current active program in the plug-in
//(re-) initializes both generic, internal parameters, and variables
void deritrvi::setProgram (VstInt32 program)
{
	deritrviProgram* ap = &programs[program];

	curProgram = program;
	setParameter (kGainMod, ap->fGainMod);
	setParameter (kWet, ap->fWet);
	setParameter (kWet2, ap->fWet2);
	setParameter (kDelay, ap->fDelay);
	setParameter (kDepthTrem, ap->fFreqMod);
	setParameter (kFreqTrem, ap->fDepthTrem);
	setParameter (kDepthVib, ap->fFreqTrem);
	setParameter (kFreqVib, ap->fDepthVib);
	setParameter (kFreqVib, ap->fFreqVib);

}

//--------------------------------------------------------------------------------------------------------
void deritrvi::setProgramName (char *name)
{
	strcpy (programs[curProgram].name, name);
}

//--------------------------------------------------------------------------------------------------------
void deritrvi::getProgramName (char *name)
{
	if (!strcmp (programs[curProgram].name, "Preset"))
		sprintf (name, "%s %d", programs[curProgram].name, curProgram + 1);
	else
		strcpy (name, programs[curProgram].name);
}

//--------------------------------------------------------------------------------------------------------
bool deritrvi::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
	if (index < kNumPrograms)
	{
		strcpy (text, programs[index].name);
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------------------------------------
//when plug-in is resumed flush buffers
void deritrvi::resume ()
{
	memset (Buffer1, 0, iBufferSize * sizeof (float));			//Flush Buffer1 by adding 0 to each index
	memset (Buffer2, 0, iBufferSize * sizeof (float));			//Flush Buffer2 by adding 0 to each index
	memset (BufferDelay1, 0, iBufferSizeDelay * sizeof (float));//Flush BufferDelay1 by adding 0 to each index
	memset (BufferDelay2, 0, iBufferSizeDelay * sizeof (float));//Flush BufferDelay2 by adding 0 to each index
	AudioEffectX::resume ();
}

//--------------------------------------------------------------------------------------------------------
//called from the Host, parameters stored back into the current program to maintain the currently edited state of any particular program
void deritrvi::setParameter (VstInt32 index, float value)
{
	deritrviProgram* ap = &programs[curProgram];

	switch (index)
	{
		case kGainMod	:	 fGainMod = ap->fGainMod = value;			break;
		case kWet		:    fWet = ap->fWet = value;					break;
		case kWet2		:	 fWet2 = ap->fWet2 = value;					break;
		case kDelay		:	 fDelay = ap->fDelay = value;				break;
		case kFreqMod	:	 fFreqMod = ap->fFreqMod = value;			break;
		case kDepthTrem	:	 fDepthTrem = ap->fDepthTrem = value;		break;
		case kFreqTrem	:	 fFreqTrem = ap->fFreqTrem = value;			break;
		case kDepthVib	:	 fDepthVib = ap->fDepthVib = value;			break;
		case kFreqVib	:	 fFreqVib = ap->fFreqVib = value;			break;

	}
	
	if (editor)
		((AEffGUIEditor*)editor)->setParameter (index, value); //GUI values updated
}

//--------------------------------------------------------------------------------------------------------
//called from host, parameters of current variables given back to the host
float deritrvi::getParameter (VstInt32 index)
{
	float v = 0;

	switch (index)
	{
		case kWet		: v = fWet;			break;
		case kWet2		: v= fWet2;			break;
		case kDelay		: v = fDelay;		break;
		case kGainMod	: v = fGainMod;		break;
		case kFreqMod	: v = fFreqMod;		break;
		case kDepthTrem : v = fDepthTrem;	break;
		case kFreqTrem	: v = fFreqTrem;	break;
		case kDepthVib	: v = fDepthVib;	break;
		case kFreqVib	: v = fFreqVib;		break;
	}
	return v;
}

//--------------------------------------------------------------------------------------------------------
//set parameter names for user interface, not needed due to custom GUI but good practise to include anyway
void deritrvi::getParameterName (VstInt32 index, char *label)
{
	switch (index)
	{
		case kWet		: strcpy(label, "  WetLeft  ");		 break;
		case kWet2		: strcpy(label, "  WetRight  ");	 break;
		case kDelay		: strcpy(label, "  DelayTime  ");	 break;
		case kGainMod	: strcpy(label, "  Mix Output  ");	 break;
		case kFreqMod	: strcpy(label, "  ModFrequency  "); break;
		case kDepthTrem	: strcpy(label, "  TremDepth  ");	 break;
		case kFreqTrem	: strcpy(label, "  TremSpeed  ");	 break;
		case kDepthVib	: strcpy(label, "  VibDepth  ");	 break;
		case kFreqVib	: strcpy(label, "  VibSpeed  ");	 break;

	}
}

//--------------------------------------------------------------------------------------------------------
//set conversion functions for user interface, not needed due to custom GUI but good practise to include anyway
void deritrvi::getParameterDisplay (VstInt32 index, char *text)
{
	switch (index)
	{
		case kGainMod	:	    dB2string (fGainMod, text, kVstMaxParamStrLen);			break;
		case kWet		:       float2string (fWet, text, kVstMaxParamStrLen);			break;
		case kWet2		:		float2string (fWet2, text, kVstMaxParamStrLen);			break;
		case kDelay		:		float2string (fDelay, text, kVstMaxParamStrLen);		break;
		case kFreqMod	:		float2string (fFreqMod, text, kVstMaxParamStrLen);		break;
		case kDepthTrem :		float2string (fDepthTrem, text, kVstMaxParamStrLen);	break;
		case kFreqTrem	:		float2string (fFreqTrem, text, kVstMaxParamStrLen);		break;
		case kDepthVib	:		float2string (fDepthVib, text, kVstMaxParamStrLen);		break;
		case kFreqVib	:		float2string (fFreqVib, text, kVstMaxParamStrLen);		break;

	}
}

//--------------------------------------------------------------------------------------------------------
//set parameter units for user interface, not needed due to custom GUI but good practise to include anyway
void deritrvi::getParameterLabel (VstInt32 index, char *label)
{
	switch (index)
	{
		case kWet		: strcpy(label, "   %   ");		break;
		case kWet2		: strcpy(label, "   %   ");		break;
		case kDelay		: strcpy(label, "   s   ");		break;
		case kGainMod	: strcpy(label, "   dB   ");	break;
		case kFreqMod	: strcpy(label, "   Hz   ");	break;
		case kDepthTrem	: strcpy(label, "   %   ");		break;
		case kFreqTrem	: strcpy(label, "   %   ");		break;
		case kDepthVib	: strcpy(label, "   %   ");		break;
		case kFreqVib	: strcpy(label, "   %   ");		break;

	}
}

//--------------------------------------------------------------------------------------------------------
//set the name of the effect
bool deritrvi::getEffectName (char* name)
{
	strcpy (name, "Deritrvi");
	return true;
}

//--------------------------------------------------------------------------------------------------------
//set the name of the product
bool deritrvi::getProductString (char* text)
{
	strcpy (text, "Y4684134");
	return true;
}

//--------------------------------------------------------------------------------------------------------
//set the name of the vendor
bool deritrvi::getVendorString (char* text)
{
	strcpy (text, "Y4684134");
	return true;
}

//--------------------------------------------------------------------------------------------------------
//function called when plug-in is placed as an insert
void deritrvi::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames) //parameter inputs from host
{
    float *in1  =  inputs[0]; //left channel input from host
    float *in2  =  inputs[1]; //right channel input from host
    float *out1 = outputs[0]; //left channel output to host
    float *out2 = outputs[1]; //right channel output to host

	double sampleRate = updateSampleRate(); //update sample rate
	double VibSine1;						//variable declaration for vibrato effect
	double VibSine2;						//variable declaration for vibrato effect
	double VibModifier1;					//variable declaration for vibrato effect
	double VibModifier2;					//variable declaration for vibrato effect
	double Pi = 3.14159265;					//variable declaration of Pi
	float modifier;							//variable declaration for tremolo effect
	double sinMod;							//variable declaration for ring modulation effect



	while (--sampleFrames >= 0) //continue loop while samples are inputted from host
	{

//--------------------------------------------------------------------------------------------------------
//Vibrato Effect	

	Buffer1[iWrite1++] = (*in1++);		//input left channel samples from host into index iWrite1, increment on each loop
	Buffer2[iWrite2++] = (*in2++);		//input right channel samples from host into index iWrite2, increment on each loop

	float vibOut1 = Buffer1[iRead1++];  //output left channel samples to delay effect from index iRead1, increment on each loop
	float vibOut2 = Buffer2[iRead2++];	//output right channel samples to delay effect from index iRead2, increment on each loop

	VibSine1 = (sin((2 * Pi *(fFreqVib * 10)) * (counter/sampleRate))); //create sine wave at user define frequency fFreqVib and multiply by the counter position divided by the sample rate to produce the correct length of sine wave
	VibSine2 = fDepthVib * VibSine1; //multiply by user definied fDepthVib to modulate the depth the sine wave produces

	if (counter < ((1/(fFreqVib * 10)) * sampleRate)) //check if counter is less than the samples in one cycle of the user defined frequency
			{
				counter++;							  //if yes increment counter by one
			}
	else
			{
				counter = 0;						  //else reset counter to 0 and begin cycle again
			}

	VibModifier1 = iWrite1 + ((VibSine2 * 882)+882);  //set VibModifier1 to position of iWrite1 + the value output by the sinewave multiplied by half the sample rate and with half the sample rate added
	VibModifier2 = iWrite2 + ((VibSine2 * 882)+882);  //set VibModifier2 to position of iWrite2 + the value output by the sinewave multiplied by half the sample rate and with half the sample rate added

	iRead1 = VibModifier1; //set iRead1 to VibModifier1 so Buffer1 may be read at the current iWrite1 position + the value of the sine wave to achieve the pitch shift effect 
	iRead2 = VibModifier2; //set iRead2 to VibModifier2 so Buffer2 may be read at the current iWrite2 position + the value of the sine wave to achieve the pitch shift effect 
		
	//to make buffers circular when end of buffer is reach set iWrite/iRead position back to the start by subtracting the total length of the buffer
 	if (iWrite1 >= iBufferSize) iWrite1 -= iBufferSize;
	if (iRead1 >= iBufferSize) iRead1 -= iBufferSize;
	if (iWrite2 >= iBufferSize) iWrite2 -= iBufferSize;
	if (iRead2 >= iBufferSize) iRead2 -= iBufferSize;

//--------------------------------------------------------------------------------------------------------
//Ring Modulation

	if (counterRing < ((1/fFreqMod * 5000) * sampleRate)) //check if counter is less than the samples in one cycle of the user defined frequency
			{
				counterRing++;							  //if yes increment counter by one
			}
	else
			{
				counterRing = 0;						  //else reset counter to 0 and begin cycle again
			}

	sinMod = (sin((2 * Pi *(fFreqMod * 5000)) * (counterRing/sampleRate))); //create sine wave at user definied frequency, multiply the user definied value by 5000 to achieve a workable frequency, multiply by counterRing divded by samplerate to produce the correct length of sine wave

//--------------------------------------------------------------------------------------------------------
//Tremolo

	if (counterTrem < ((1/fFreqTrem * 10) * sampleRate)) //check if counter is less than the samples in one cycle of the user defined frequency
			{
				counterTrem++;							 //if yes increment counter by one
			}
	else
			{
				counterTrem = 0;						 //else reset counter to 0 and begin cycle again
			}

	modifier = (1.0 - fDepthTrem) + fDepthTrem * (sin((2 * 3.14 * (fFreqTrem * 10)) * (counterTrem/sampleRate)))* (sin((2 * 3.14 * (fFreqTrem * 10)) * (counterTrem/sampleRate)));  //Tremolo equation
		
//--------------------------------------------------------------------------------------------------------
//Delay

	BufferDelay1[iWriteDelay1] = vibOut1; //take left output from vibrato effect and input into BufferDelay1 at index specified by iWriteDelay1
	float delayOut1 = (fWet * BufferDelay1[iReadDelay1]) + ((1.0 - fWet) * (vibOut1)); //produce left channel output by reading the delay at the index specified by iReadDelay1, combine this with the input from the vibrato effect to achieve a delay sound
	iWriteDelay1 = iReadDelay1 + (int)(fDelay * getSampleRate()); //update delay time by spacing the gap between the iWrite point and the iRead point, this is a user definied value specified by fDelay
		
	BufferDelay2[iWriteDelay2] = vibOut2; //take right output from vibrato effect and input into BufferDelay2 at index specified by iWriteDelay2
	float delayOut2 = (fWet2 * BufferDelay2[iReadDelay2]) + ((1.0 - fWet2) * (vibOut2)); //produce right channel output by reading the delay at the index specified by iReadDelay2, combine this with the input from the vibrato effect to achieve a delay sound
	iWriteDelay2 = iReadDelay2 + (int)(fDelay * getSampleRate()); //update delay time by spacing the gap between the iWrite point and the iRead point, this is a user definied value specified by fDelay

	//Increment variables by one on each iteration of the while loop
	vibOut1++;
	vibOut2++;
	iWriteDelay1++;
	iReadDelay1++;
	iWriteDelay2++;
	iReadDelay2++;
		
	//to make buffers circular when end of buffer is reach set iWrite/iRead position back to the start by subtracting the total length of the buffer
 	if (iWriteDelay1 >= iBufferSizeDelay) iWriteDelay1 -= iBufferSizeDelay;
	if (iReadDelay1 >= iBufferSizeDelay) iReadDelay1 -= iBufferSizeDelay;
 	if (iWriteDelay2 >= iBufferSizeDelay) iWriteDelay2 -= iBufferSizeDelay;
	if (iReadDelay2 >= iBufferSizeDelay) iReadDelay2 -= iBufferSizeDelay;

//--------------------------------------------------------------------------------------------------------
//Output	

	if (fFreqMod > 0) //if ring modulation frequency value is greater than 0 output all effects varied by the gain parameter 
		{
			(*out1++) = ((delayOut1 * (sinMod/2)) * modifier) * fGainMod;
			(*out2++) = ((delayOut2 * (sinMod/2)) * modifier) * fGainMod;
		}
					 //else output only the vibrato, tremolo and delay effects to start stop ring modulator from multiplying the equation by 0 resulting in no output
	else 
		{
			(*out1++) = (delayOut1 * modifier) * fGainMod;
			(*out2++) = (delayOut2 * modifier) * fGainMod;
		}


	}
}
