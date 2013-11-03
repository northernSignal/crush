#ifndef __crush__
#define __crush__

// Our base class declaration
#include "audioeffectx.h"

enum
{
	kNumPrograms = 0,
	kBitCrush = 0,
	kDryWet,
	kNumParams
};

class crush : public AudioEffectX{

public:
// Constructor
	crush(audioMasterCallback audioMaster);
// Destructor
	~crush();

// Setup
	bool crush::getVendorString(char* text);

	VstInt32 crush::getVendorVersion();

	bool crush::getProductString(char* productString);

	VstPlugCategory crush::getPlugCategory();

	bool crush::getEffectName(char* effectName);

// Processing
	void crush::processReplacing (float** inputs, float** outputs,
							VstInt32 sampleFrames);

	void crush::setBitCrush (float value);

	void crush::setDryWet (float value);

// Parameters
	float crush::getParameter(VstInt32 index);
	void crush::setParameter(VstInt32 index, float value);
	void crush::getParameterDisplay(VstInt32 index, char* dispName);
	void crush::getParameterName(VstInt32 index, char* label);
	void crush::getParameterLabel(VstInt32 index, char* unit);

protected:

	float fBitDepth;

	float fBitCrush, fQuantized1, fQuantized2;
	float fDry, fWet, fDryWet;

};

#endif