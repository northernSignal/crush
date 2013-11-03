#include "Crush!_main.h"
#include <cmath>

#define PI 3.14159
#define BITDEPTH 16
#define MAGICNUMBER 1.25
#define MAGICNUMBER2 0.2

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new crush( audioMaster );
}

crush::crush (audioMasterCallback audioMaster)
	:AudioEffectX (audioMaster, kNumPrograms, kNumParams)
{
	setNumInputs(2);
	setNumOutputs(2);

	setUniqueID('NSCR');
	canProcessReplacing();

	setBitCrush(1);
	setDryWet(1);
}

crush::~crush()
{

}

bool crush::getVendorString(char* text)
{
	vst_strncpy(text, "northernSignal", kVstMaxVendorStrLen);
	return true;
}

VstInt32 crush::getVendorVersion()
{
	return 100;
}

bool crush::getProductString(char* productString)
{
	vst_strncpy(productString, "Crush!", kVstMaxProductStrLen);
	return true;
}

VstPlugCategory crush::getPlugCategory()
{
	return kPlugCategUnknown;
}

bool crush::getEffectName(char* effectName)
{
	vst_strncpy(effectName, "Crush!", kVstMaxEffectNameLen);
	return true;
}

float crush::getParameter(VstInt32 index)
{
	float val = 0;

	switch(index)
	{
		case kBitCrush:	val = fBitCrush;	break;
		case kDryWet:	val = fDryWet;		break;
	}

	return val;
}

void crush::setParameter(VstInt32 index, float value)
{
	switch(index)
	{
		case kBitCrush: setBitCrush(value);		break;
		case kDryWet:	setDryWet(value);		break;
	}
}

void crush::getParameterDisplay(VstInt32 index, char* dispName)
{
	switch(index)
	{
		case kBitCrush:	float2string( (fBitCrush*100), dispName, kVstMaxParamStrLen);	break;
		case kDryWet:	float2string( (fDryWet*100) , dispName, kVstMaxParamStrLen); break;
	}
}
	
void crush::getParameterName(VstInt32 index, char* label)
{
	switch(index)
	{
		case kBitCrush:	vst_strncpy(label, "BitCrush" , kVstMaxParamStrLen);	break;
		case kDryWet:	vst_strncpy(label, "Dry/Wet" , kVstMaxParamStrLen);		break;
	}
}

void crush::getParameterLabel(VstInt32 index, char* unit)
{
	switch(index)
	{
		case kBitCrush: vst_strncpy(unit, "% dist" , kVstMaxParamStrLen);	break;
		case kDryWet:	vst_strncpy(unit, "%" , kVstMaxParamStrLen);	break;
	}
}

void crush::setBitCrush(float value)
{
	fBitCrush = (value/MAGICNUMBER)+MAGICNUMBER2;
}

void crush::setDryWet(float val)
{
	fDryWet = val;
	fDry = 0.7*cos(fDryWet*PI/2);
	fWet = 0.7*sin(fDryWet*PI/2);
}

void crush::processReplacing(float** inputs, float** outputs,
									VstInt32 sampleFrames)
{
	float *in1 = inputs[0], *in2 = inputs[1];
	float *out1 = outputs[0], *out2 = outputs[1];

	// processing loop
	while(--sampleFrames >=0)
	{
		// process
		fQuantized1 = floor( *in1 / fBitCrush );
		fQuantized2 = floor( *in2 / fBitCrush );
		*out1 = (*in1*fDry) + (fWet*(fQuantized1*fBitCrush));
		*out2 = (*in2*fDry) + (fWet*(fQuantized2*fBitCrush));
		
		// increment
		out1++; out2++; in1++; in2++;
	}
}