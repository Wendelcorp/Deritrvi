//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// 
//
// Category     : VST 2.x Plugin
// Created by   : Y4684134
// Description  : Delay/Ringmod/Tremolo/deritrvi plugin
//
// © 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#ifndef __deritrvi__
#include "deritrvi.h"
#endif

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new deritrvi (audioMaster);
}

