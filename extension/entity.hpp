#pragma once

#include <stddef.h>
#include <IGameConfigs.h>
#include <iserverentity.h>

class CAttributeList;
class CTFPlayer;

class CBaseEntity : public IServerEntity {
public:
	// Returns nullptr if the entity lacks a CAttributeList
	CAttributeList* GetAttributeList();
	// Returns nullptr if not a player
	CTFPlayer* ToTFPlayer();
};

class CTFPlayer : public CBaseEntity {
public:
	void AddCustomAttribute(const char* attrib, float value, float duration) { (this->*fn_AddCustomAttribute)(attrib, value, duration); };
	void RemoveCustomAttribute(const char* attrib) { (this->*fn_RemoveCustomAttribute)(attrib); };
protected:
	friend bool Entity_Setup(SourceMod::IGameConfig* conf);
	static void (CTFPlayer::*fn_AddCustomAttribute)(const char* attrib, float value, float duration);
	static void (CTFPlayer::*fn_RemoveCustomAttribute)(const char* attrib);
};

bool Entity_Setup(SourceMod::IGameConfig* conf);