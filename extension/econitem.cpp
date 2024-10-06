#include "econitem.hpp"
#include <smsdk_ext.h>

CEconItemAttributeDefinition* (CEconItemSchema::*CEconItemSchema::fn_GetAttributeDefinition)(int) = nullptr;
CEconItemAttributeDefinition* (CEconItemSchema::*CEconItemSchema::fn_GetAttributeDefinitionByName)(const char*) = nullptr;
CEconItemDefinition* (CEconItemSchema::*CEconItemSchema::fn_GetItemDefinition)(int) = nullptr;

void (CAttributeList::*CAttributeList::fn_SetRuntimeAttributeValue)(const CEconItemAttributeDefinition*, float) = nullptr;
void (CAttributeList::*CAttributeList::fn_RemoveAttribute)(const CEconItemAttributeDefinition*) = nullptr;
void (CAttributeList::*CAttributeList::fn_DestroyAllAttributes)() = nullptr;

int CEconItemDefinition::off_m_vecStaticAttributes = -1;

int CAttributeManager::off_OnAttributeValuesChanged = -1;
float (*CAttributeManager::fn_AttribHookValue_float)(float, const char*, CBaseEntity*, CUtlVector<CBaseEntity*>*, bool);
int (*CAttributeManager::fn_AttribHookValue_int)(int, const char*, CBaseEntity*, CUtlVector<CBaseEntity*>*, bool);

CEconItemSchema* (*fn_GetItemSchema)();
void (*fn_CopyStringAttributeValueToCharPointerOutput)(const void* value, const char** out_value);

bool Econ_Setup(IGameConfig* conf) {
	if (!conf->GetMemSig("GEconItemSchema", reinterpret_cast<void **>(&fn_GetItemSchema))) {
		g_pSM->LogMessage(myself, "Couldn't locate function GEconItemSchema!");
		return false;
	}

	if (!conf->GetMemSig("CAttributeManager::AttribHookValue<float>", reinterpret_cast<void **>(&CAttributeManager::fn_AttribHookValue_float))) {
		g_pSM->LogMessage(myself, "Couldn't locate function CAttributeManager::AttribHookValue<float>!");
		return false;
	}

	if (!conf->GetMemSig("CAttributeManager::AttribHookValue<int>", reinterpret_cast<void **>(&CAttributeManager::fn_AttribHookValue_int))) {
		g_pSM->LogMessage(myself, "Couldn't locate function CAttributeManager::AttribHookValue<int>!");
		return false;
	}

	if (!conf->GetMemSig("CAttributeList::DestroyAllAttributes", reinterpret_cast<void **>(&CAttributeList::fn_DestroyAllAttributes))) {
		g_pSM->LogMessage(myself, "Couldn't locate function CAttributeList::DestroyAllAttributes!");
		return false;
	}

	if (!conf->GetMemSig("CAttributeList::RemoveAttribute", reinterpret_cast<void **>(&CAttributeList::fn_RemoveAttribute))) {
		g_pSM->LogMessage(myself, "Couldn't locate function CAttributeList::RemoveAttribute!");
		return false;
	}

	if (!conf->GetMemSig("CAttributeList::SetRuntimeAttributeValue", reinterpret_cast<void **>(&CAttributeList::fn_SetRuntimeAttributeValue))) {
		g_pSM->LogMessage(myself, "Couldn't locate function CAttributeList::SetRuntimeAttributeValue!");
		return false;
	}

	if (!conf->GetMemSig("CEconItemSchema::GetItemDefinition", reinterpret_cast<void **>(&CEconItemSchema::fn_GetItemDefinition))) {
		g_pSM->LogMessage(myself, "Couldn't locate function CEconItemSchema::GetItemDefinition!");
		return false;
	}

	if (!conf->GetMemSig("CEconItemSchema::GetAttributeDefinitionByName", reinterpret_cast<void **>(&CEconItemSchema::fn_GetAttributeDefinitionByName))) {
		g_pSM->LogMessage(myself, "Couldn't locate function CEconItemSchema::GetAttributeDefinitionByName!");
		return false;
	}

	if (!conf->GetMemSig("CEconItemSchema::GetAttributeDefinition", reinterpret_cast<void **>(&CEconItemSchema::fn_GetAttributeDefinition))) {
		g_pSM->LogMessage(myself, "Couldn't locate function CEconItemSchema::GetAttributeDefinition!");
		return false;
	}

	if (!conf->GetOffset("CAttributeManager::OnAttributeValuesChanged", &CAttributeManager::off_OnAttributeValuesChanged)) {
		g_pSM->LogMessage(myself, "Couldn't get offset for CAttributeManager::OnAttributeValuesChanged!");
		return false;
	}

	if (!conf->GetOffset("CEconItemDefinition::m_vecStaticAttributes", &CEconItemDefinition::off_m_vecStaticAttributes)) {
		g_pSM->LogMessage(myself, "Couldn't get offset for CEconItemDefinition::m_vecStaticAttributes!");
		return false;
	}
	return true;
}