#include "entity.hpp"
#include <iservernetworkable.h>
#include <server_class.h>
#include <smsdk_ext.h>

void (CTFPlayer::*CTFPlayer::fn_AddCustomAttribute)(const char* attrib, float value, float duration) = nullptr;
void (CTFPlayer::*CTFPlayer::fn_RemoveCustomAttribute)(const char* attrib) = nullptr;

CAttributeList* CBaseEntity::GetAttributeList() {
	auto net = GetNetworkable();
	if (!net) {
		return nullptr;
	}

	auto srv_class = net->GetServerClass();
	if (!srv_class) {
		return nullptr;
	}

	sm_sendprop_info_t info;
	if (!gamehelpers->FindSendPropInfo(srv_class->GetName(), "m_AttributeList", &info)) {
		return nullptr;
	}

	return (CAttributeList*)((std::uint8_t*)this + info.actual_offset);
}

CTFPlayer* CBaseEntity::ToTFPlayer() {
	auto net = GetNetworkable();
	if (!net) {
		return nullptr;
	}

	auto edict = net->GetEdict();
	if (!edict) {
		return nullptr;
	}

	if (!playerhelpers->GetGamePlayer(edict)) {
		return nullptr;
	}
	return static_cast<CTFPlayer*>(this);
}

bool Entity_Setup(IGameConfig* conf) {
	if (!conf->GetMemSig("CTFPlayer::AddCustomAttribute", reinterpret_cast<void **>(&CTFPlayer::fn_AddCustomAttribute))) {
		g_pSM->LogMessage(myself, "Couldn't locate function CTFPlayer::AddCustomAttribute!");
		return false;
	}

	if (!conf->GetMemSig("CTFPlayer::RemoveCustomAttribute", reinterpret_cast<void **>(&CTFPlayer::fn_RemoveCustomAttribute))) {
		g_pSM->LogMessage(myself, "Couldn't locate function CAttributeList::DestroyAllAttributes!");
		return false;
	}
	return true;
}