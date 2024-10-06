#pragma once

#include <smsdk_ext.h>

class TF2AttribExt : public SDKExtension
{
public:
	virtual bool SDK_OnLoad(char *error, size_t maxlength, bool late);
};