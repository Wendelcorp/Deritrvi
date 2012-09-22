//-------------------------------------------------------------------------------------------------------
// 
// Category     : VST 2.x Plugin
// Created by   : Y4684134
// Description  : Delay/Ringmod/Tremolo/deritrvi plugin
//
//-------------------------------------------------------------------------------------------------------
//CHANGED INCLUDE FILES
#ifndef __deritrvieditor__
#include "deritrvieditor.h"
#endif

#ifndef __deritrvi__
#include "deritrvi.h"
#endif

#ifndef __resource__
#include "resource.h"
#endif

#include <stdio.h>

//-----------------------------------------------------------------------------
// resource id's 
enum {
	// bitmaps
	kBackgroundId = 101, //IDB_BITMAP1, //CHANGED
	kFaderBodyId,
	kFaderHandleId,
	kLinkButtonId, //ADDED
	
	// positions
	kFaderX = 35, //CHANGED
	kFaderY = 50, //CHANGED

	kFaderInc = 18, 
	

	kDisplayX = 27,//percentage box position x
	kDisplayY = 230,//percentage box position y
	kDisplayXWidth = 49,//percentage box width
	kDisplayHeight = 14//percentage box height
};

//-----------------------------------------------------------------------------
// prototype string converts float -> percent & float -> Hz
//ADDED!!
void percentStringConvert (float value, char* string);
void HzStringConvert (float value, char* string);
void dBStringConvert (float value, char* string);

void percentStringConvert (float value, char* string)
{
	 sprintf (string, "%d%%", (int)(100 * value + 0.5f));
}

// String convert, converts float -> xx.xHz value
void HzStringConvert (float value, char* string)
{
	sprintf (string, "%1.1f%Hz", (float)(1000 * value));
}
void dBStringConvert (float value, char* string)
{
	sprintf (string, "%d%%", (float)(value - 100));
}

//-----------------------------------------------------------------------------
// deritrviEditor class implementation
//-----------------------------------------------------------------------------
deritrviEditor::deritrviEditor (AudioEffect *effect)
 : AEffGUIEditor (effect) 
{
	//CHANGED. ADD YOUR CONTROLS HERE
GainModFader = 0;
WetFader = 0;
Wet2Fader = 0;
DelayFader = 0;
FreqModFader = 0;
DepthTremFader = 0;
FreqTremFader = 0;
DepthVibFader = 0;
FreqVibFader = 0;



	// load the background bitmap
	// we don't need to load all bitmaps, this could be done when open is called
	hBackground = new CBitmap (kBackgroundId);

	// init the size of the plugin
	rect.left   = 0;
	rect.top    = 0;
	rect.right  = (short)hBackground->getWidth ();
	rect.bottom = (short)hBackground->getHeight ();
}

//-----------------------------------------------------------------------------
deritrviEditor::~deritrviEditor ()
{
	// free the background bitmap
	if (hBackground)
		hBackground->forget ();
	hBackground = 0;
}

//-----------------------------------------------------------------------------
bool deritrviEditor::open (void *ptr)
{
	// !!! always call this !!!
	AEffGUIEditor::open (ptr);
	
	//--load some bitmaps
	CBitmap* hFaderBody   = new CBitmap (kFaderBodyId);
	CBitmap* hFaderHandle = new CBitmap (kFaderHandleId);

	CBitmap* hLinkButton = new CBitmap (kLinkButtonId);//ADDED

	//--init background frame-----------------------------------------------
	// We use a local CFrame object so that calls to setParameter won't call into objects which may not exist yet. 
	// If all GUI objects are created we assign our class member to this one. See bottom of this method.
	CRect size (0, 0, hBackground->getWidth (), hBackground->getHeight ());
	CFrame* lFrame = new CFrame (size, ptr, this);
	lFrame->setBackground (hBackground);

//ADDED



	//--init the faders------------------------------------------------
	int minPos = kFaderY;
	int maxPos = kFaderY + hFaderBody->getHeight () - hFaderHandle->getHeight () - 1;
	CPoint point (0, 0);
	CPoint offset (4, 0); //CHANGED

//ADD YOUR OWN BUTTONS HERE!!
	// Gain
	size (kFaderX, kFaderY,
          kFaderX + hFaderBody->getWidth (), kFaderY + hFaderBody->getHeight ());
	GainModFader = new CVerticalSlider (size, this, kGainMod, minPos, maxPos, hFaderHandle, hFaderBody, point);
	GainModFader->setOffsetHandle (offset);
	GainModFader->setValue (effect->getParameter (kGainMod));
	lFrame->addView (GainModFader);

	// Wet
	size.offset (kFaderInc + hFaderBody->getWidth ()+40, 0);
	WetFader = new CVerticalSlider (size, this, kWet, minPos, maxPos, hFaderHandle, hFaderBody, point);
	WetFader->setOffsetHandle (offset);
	WetFader->setValue (effect->getParameter (kWet));
	lFrame->addView (WetFader);

	// Wet2
	size.offset (kFaderInc + hFaderBody->getWidth ()+5, 0);
	Wet2Fader = new CVerticalSlider (size, this, kWet2, minPos, maxPos, hFaderHandle, hFaderBody, point);
	Wet2Fader->setOffsetHandle (offset);
	Wet2Fader->setValue (effect->getParameter (kWet2));
	Wet2Fader->setDefaultValue (0.75f);
	lFrame->addView (Wet2Fader);

	// Delay Time
	size.offset (kFaderInc + hFaderBody->getWidth ()+5, 0);
	DelayFader = new CVerticalSlider (size, this, kDelay, minPos, maxPos, hFaderHandle, hFaderBody, point);
	DelayFader->setOffsetHandle (offset);
	DelayFader->setValue (effect->getParameter (kDelay));
	DelayFader->setDefaultValue (0.75f);
	lFrame->addView (DelayFader);

		// Frequency mod
	size.offset (kFaderInc + hFaderBody->getWidth ()+40, 0);
	FreqModFader = new CVerticalSlider (size, this, kFreqMod, minPos, maxPos, hFaderHandle, hFaderBody, point);
	FreqModFader->setOffsetHandle (offset);
	FreqModFader->setValue (effect->getParameter (kFreqMod));
	FreqModFader->setDefaultValue (0.75f);
	lFrame->addView (FreqModFader);

		// Tremolo Depth
	size.offset (kFaderInc + hFaderBody->getWidth ()+40, 0);
	DepthTremFader = new CVerticalSlider (size, this, kDepthTrem, minPos, maxPos, hFaderHandle, hFaderBody, point);
	DepthTremFader->setOffsetHandle (offset);
	DepthTremFader->setValue (effect->getParameter (kDepthTrem));
	DepthTremFader->setDefaultValue (0.75f);
	lFrame->addView (DepthTremFader);

		// Tremolo Frequency
	size.offset (kFaderInc + hFaderBody->getWidth ()+5, 0);
	FreqTremFader = new CVerticalSlider (size, this, kFreqTrem, minPos, maxPos, hFaderHandle, hFaderBody, point);
	FreqTremFader->setOffsetHandle (offset);
	FreqTremFader->setValue (effect->getParameter (kFreqTrem));
	FreqTremFader->setDefaultValue (0.75f);
	lFrame->addView (FreqTremFader);

		// deritrvi Depth
	size.offset (kFaderInc + hFaderBody->getWidth ()+40, 0);
	DepthVibFader = new CVerticalSlider (size, this, kDepthVib, minPos, maxPos, hFaderHandle, hFaderBody, point);
	DepthVibFader->setOffsetHandle (offset);
	DepthVibFader->setValue (effect->getParameter (kDepthVib));
	DepthVibFader->setDefaultValue (0.75f);
	lFrame->addView (DepthVibFader);

		// deritrvi Freq
	size.offset (kFaderInc + hFaderBody->getWidth ()+5, 0);
	FreqVibFader = new CVerticalSlider (size, this, kFreqVib, minPos, maxPos, hFaderHandle, hFaderBody, point);
	FreqVibFader->setOffsetHandle (offset);
	FreqVibFader->setValue (effect->getParameter (kFreqVib));
	FreqVibFader->setDefaultValue (0.75f);
	lFrame->addView (FreqVibFader);



	//--init the display------------------------------------------------
	// Gain
	size (kDisplayX, kDisplayY,
          kDisplayX + kDisplayXWidth, kDisplayY + kDisplayHeight);
	GainModDisplay = new CParamDisplay (size, 0, kCenterText);
	GainModDisplay->setFont (kNormalFontSmall);
	GainModDisplay->setFontColor (kWhiteCColor);
	GainModDisplay->setBackColor (kBlackCColor);
	GainModDisplay->setFrameColor (kWhiteCColor);
	GainModDisplay->setStringConvert (percentStringConvert);
	GainModDisplay->setValue (effect->getParameter (kGainMod));
	lFrame->addView (GainModDisplay);

	// Wet
	size.offset (kFaderInc + hFaderBody->getWidth ()+40, 0);
	WetDisplay = new CParamDisplay (size, 0, kCenterText);
	WetDisplay->setFont (kNormalFontSmall);
	WetDisplay->setFontColor (kWhiteCColor);
	WetDisplay->setBackColor (kBlackCColor);
	WetDisplay->setFrameColor (kWhiteCColor);
	WetDisplay->setValue (effect->getParameter (kWet));
	WetDisplay->setStringConvert (percentStringConvert);
	lFrame->addView (WetDisplay);


	// Wet2
	size.offset (kFaderInc + hFaderBody->getWidth ()+5, 0);
	Wet2Display = new CParamDisplay (size, 0, kCenterText);
	Wet2Display->setFont (kNormalFontSmall);
	Wet2Display->setFontColor (kWhiteCColor);
	Wet2Display->setBackColor (kBlackCColor);
	Wet2Display->setFrameColor (kWhiteCColor);
	Wet2Display->setStringConvert (percentStringConvert);
	Wet2Display->setValue (effect->getParameter (kWet2));
	lFrame->addView (Wet2Display);

	// Delay time
	size.offset (kFaderInc + hFaderBody->getWidth ()+5, 0);
	DelayDisplay = new CParamDisplay (size, 0, kCenterText);
	DelayDisplay->setFont (kNormalFontSmall);
	DelayDisplay->setFontColor (kWhiteCColor);
	DelayDisplay->setBackColor (kBlackCColor);
	DelayDisplay->setFrameColor (kWhiteCColor);
	DelayDisplay->setValue (effect->getParameter (kDelay));
	DelayDisplay->setStringConvert (percentStringConvert);
	lFrame->addView (DelayDisplay);

	// Frequency mod
	size.offset (kFaderInc + hFaderBody->getWidth ()+40, 0);
	FreqModDisplay = new CParamDisplay (size, 0, kCenterText);
	FreqModDisplay->setFont (kNormalFontSmall);
	FreqModDisplay->setFontColor (kWhiteCColor);
	FreqModDisplay->setBackColor (kBlackCColor);
	FreqModDisplay->setFrameColor (kWhiteCColor);
	FreqModDisplay->setValue (effect->getParameter (kFreqMod));
	FreqModDisplay->setStringConvert (HzStringConvert);
	lFrame->addView (FreqModDisplay);

	// Tremolo depth
	size.offset (kFaderInc + hFaderBody->getWidth ()+40, 0);
	DepthTremDisplay = new CParamDisplay (size, 0, kCenterText);
	DepthTremDisplay->setFont (kNormalFontSmall);
	DepthTremDisplay->setFontColor (kWhiteCColor);
	DepthTremDisplay->setBackColor (kBlackCColor);
	DepthTremDisplay->setFrameColor (kWhiteCColor);
	DepthTremDisplay->setValue (effect->getParameter (kDepthTrem));
	DepthTremDisplay->setStringConvert (percentStringConvert);
	lFrame->addView (DepthTremDisplay);

	// Tremolo freq
	size.offset (kFaderInc + hFaderBody->getWidth ()+5, 0);
	FreqTremDisplay = new CParamDisplay (size, 0, kCenterText);
	FreqTremDisplay->setFont (kNormalFontSmall);
	FreqTremDisplay->setFontColor (kWhiteCColor);
	FreqTremDisplay->setBackColor (kBlackCColor);
	FreqTremDisplay->setFrameColor (kWhiteCColor);
	FreqTremDisplay->setValue (effect->getParameter (kFreqTrem));
	FreqTremDisplay->setStringConvert (percentStringConvert);
	lFrame->addView (FreqTremDisplay);

	// deritrvi depth
	size.offset (kFaderInc + hFaderBody->getWidth ()+40, 0);
	DepthVibDisplay = new CParamDisplay (size, 0, kCenterText);
	DepthVibDisplay->setFont (kNormalFontSmall);
	DepthVibDisplay->setFontColor (kWhiteCColor);
	DepthVibDisplay->setBackColor (kBlackCColor);
	DepthVibDisplay->setFrameColor (kWhiteCColor);
	DepthVibDisplay->setValue (effect->getParameter (kDepthVib));
	DepthVibDisplay->setStringConvert (percentStringConvert);
	lFrame->addView (DepthVibDisplay);

	// deritrvi freq
	size.offset (kFaderInc + hFaderBody->getWidth ()+5, 0);
	FreqVibDisplay = new CParamDisplay (size, 0, kCenterText);
	FreqVibDisplay->setFont (kNormalFontSmall);
	FreqVibDisplay->setFontColor (kWhiteCColor);
	FreqVibDisplay->setBackColor (kBlackCColor);
	FreqVibDisplay->setFrameColor (kWhiteCColor);
	FreqVibDisplay->setValue (effect->getParameter (kFreqVib));
	FreqVibDisplay->setStringConvert (percentStringConvert);
	lFrame->addView (FreqVibDisplay);


	// Note : in the constructor of a CBitmap, the number of references is set to 1.
	// Then, each time the bitmap is used (for hinstance in a vertical slider), this
	// number is incremented.
	// As a consequence, everything happens as if the constructor by itself was adding
	// a reference. That's why we need til here a call to forget ().
	// You mustn't call delete here directly, because the bitmap is used by some CControls...
	// These "rules" apply to the other VSTGUI objects too.
	hFaderBody->forget ();
	hFaderHandle->forget ();

	frame = lFrame;
	return true;
}

//-----------------------------------------------------------------------------
void deritrviEditor::close ()
{
	delete frame;
	frame = 0;
}

//-----------------------------------------------------------------------------
void deritrviEditor::setParameter (VstInt32 index, float value)
//CHANGE THESE TO YOUR OWN BUTTONS!!
{
	if (frame == 0)
		return;

	// called from deritrvi.cpp (Set Parameter)
	switch (index)
	{
		case kGainMod:
			if (GainModFader)
				GainModFader->setValue (effect->getParameter (index));
			if (GainModDisplay)
				GainModDisplay->setValue (effect->getParameter (index));
			break;

		case kWet:
			if (WetFader)
				WetFader->setValue (effect->getParameter (index));
			if (WetDisplay)
				WetDisplay->setValue (effect->getParameter (index));
			break;

		case kWet2:
			if (Wet2Fader)
				Wet2Fader->setValue (effect->getParameter (index));
			if (Wet2Display)
				Wet2Display->setValue (effect->getParameter (index));
			break;

		case kDelay:
			if (DelayFader)
				DelayFader->setValue (effect->getParameter (index));
			if (DelayDisplay)
				DelayDisplay->setValue (effect->getParameter (index));
			break;

		case kFreqMod:
			if (FreqModFader)
				FreqModFader->setValue (effect->getParameter (index));
			if (FreqModDisplay)
				FreqModDisplay->setValue (effect->getParameter (index));
			break;

		case kDepthTrem:
			if (DepthTremFader)
				DepthTremFader->setValue (effect->getParameter (index));
			if (DepthTremDisplay)
				DepthTremDisplay->setValue (effect->getParameter (index));
			break;

		case kFreqTrem:
			if (FreqTremFader)
				FreqTremFader->setValue (effect->getParameter (index));
			if (FreqTremDisplay)
				FreqTremDisplay->setValue (effect->getParameter (index));
			break;

		case kDepthVib:
			if (DepthVibFader)
				DepthVibFader->setValue (effect->getParameter (index));
			if (DepthVibDisplay)
				DepthVibDisplay->setValue (effect->getParameter (index));
			break;

		case kFreqVib:
			if (FreqVibFader)
				FreqVibFader->setValue (effect->getParameter (index));
			if (FreqVibDisplay)
				FreqVibDisplay->setValue (effect->getParameter (index));
			break;
	}
}

//-----------------------------------------------------------------------------
void deritrviEditor::valueChanged (CDrawContext* context, CControl* control)
{
	long tag = control->getTag ();
	switch (tag)
	{//CHANGE THESE TO YOUR OWN BUTTONS!!
		case kGainMod:
		case kWet:
		case kWet2:
		case kDelay:
		case kFreqMod:
		case kDepthTrem:
		case kFreqTrem:
		case kDepthVib:
		case kFreqVib:
			effect->setParameterAutomated (tag, control->getValue ());
			control->setDirty ();
		break;
	}
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
