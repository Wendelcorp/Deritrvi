//-------------------------------------------------------------------------------------------------------
// 
// Category     : VST 2.x Plugin
//
// Created by   : Y4684134
// Description  : Delay/Ringmod/Tremolo/deritrvi plugin
//
//-------------------------------------------------------------------------------------------------------
//CHANGE TO YOUR OWN EDITOR
#ifndef __deritrvieditor__
#define __deritrvieditor__
//FIND VSTGUI AND ADD IT TO YOUR FILES
// include VSTGUI
#ifndef __vstgui__
#include "vstgui.sf/vstgui/vstgui.h"
#endif


//CHANGE THISvv-----------------------------------------------------------------------------
class deritrviEditor : public AEffGUIEditor, public CControlListener
{
//SAME UNTIL PRIVATE
public:
	deritrviEditor (AudioEffect* effect);
	virtual ~deritrviEditor ();

public:
	virtual bool open (void* ptr);
	virtual void close ();

	virtual void setParameter (VstInt32 index, float value);
	virtual void valueChanged (CDrawContext* context, CControl* control);

private:
	// Controls ADD YOUR OWN HERE!!
	CVerticalSlider* GainModFader;
	CVerticalSlider* WetFader;
	CVerticalSlider* Wet2Fader;
	CVerticalSlider* DelayFader;
	CVerticalSlider* FreqModFader;
	CVerticalSlider* DepthTremFader;
	CVerticalSlider* FreqTremFader;
	CVerticalSlider* DepthVibFader;
	CVerticalSlider* FreqVibFader;

	CParamDisplay* GainModDisplay;
	CParamDisplay* WetDisplay;
	CParamDisplay* Wet2Display;
	CParamDisplay* DelayDisplay;
	CParamDisplay* FreqModDisplay;
	CParamDisplay* DepthTremDisplay;
	CParamDisplay* FreqTremDisplay;
	CParamDisplay* DepthVibDisplay;
	CParamDisplay* FreqVibDisplay;


	// Bitmap MAKE OWN BACKGROUND BITMAP??
	CBitmap* hBackground;
};

#endif
