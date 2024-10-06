#pragma once

#include <cstdint>
#include <utlvector.h>
#include <IGameConfigs.h>

union attribute_data_union_t
{
	float asFloat;
	std::uint32_t asUint32;
	void* asBlobPointer;
};

typedef std::uint16_t attrib_definition_index_t;
typedef std::uint16_t item_definition_index_t;

struct static_attrib_t
{
public:
	attrib_definition_index_t iDefIndex;
	attribute_data_union_t m_value;
	bool bForceGCToGenerate;
	void* m_pKVCustomData;
};

class CEconItemAttributeDefinition;
class CBaseEntity;

class CEconItemAttribute {
public:
	void SetDefIndex(attrib_definition_index_t index) { m_iAttributeDefinitionIndex = index; };
	attrib_definition_index_t GetDefIndex() { return m_iAttributeDefinitionIndex; }

	void SetValue(float value) { m_flValue = value; };
	float GetValue() { return m_flValue; };

	void SetRefundValue(std::int32_t value) { m_nRefundableCurrency = value; };
	std::int32_t GetRefundValue() { return m_nRefundableCurrency; };

	const CEconItemAttributeDefinition* GetStaticData(void) const;
protected:
	void* m_pVTable;
	attrib_definition_index_t m_iAttributeDefinitionIndex;
	float m_flValue;
	std::int32_t m_nRefundableCurrency;
};

class CEconItemDefinition {
public:
	CUtlVector<static_attrib_t>* GetStaticAttributes() { return (CUtlVector<static_attrib_t>*)((std::uint8_t*)this + off_m_vecStaticAttributes); };

	static int off_m_vecStaticAttributes;
};

class CEconItemAttributeDefinition {
public:
	attrib_definition_index_t GetDefinitionIndex() const { return m_nDefIndex; };
	bool IsStoredAsInteger(void) const { return m_bStoredAsInteger; }
	bool IsStoredAsFloat(void) const { return !m_bStoredAsInteger; }
private:
	void* m_pKVAttribute;
	attrib_definition_index_t m_nDefIndex;
	void* m_pAttrType;
	bool m_bHidden;
	bool m_bWebSchemaOutputForced;
	bool m_bStoredAsInteger;
};

class CEconItemSchema {
public:
	CEconItemAttributeDefinition* GetAttributeDefinition(int index) { return (this->*fn_GetAttributeDefinition)(index); }
	CEconItemAttributeDefinition* GetAttributeDefinitionByName(const char* name) { return (this->*fn_GetAttributeDefinitionByName)(name); }
	CEconItemDefinition* GetItemDefinition(int iItemIndex) { return (this->*fn_GetItemDefinition)(iItemIndex); }
protected:
	friend bool Econ_Setup(SourceMod::IGameConfig* conf);
	static CEconItemAttributeDefinition* (CEconItemSchema::*fn_GetAttributeDefinition)(int);
	static CEconItemAttributeDefinition* (CEconItemSchema::*fn_GetAttributeDefinitionByName)(const char*);
	static CEconItemDefinition* (CEconItemSchema::*fn_GetItemDefinition)(int);
};

class CAttributeManager {
public:
	void OnAttributeValuesChanged() {
		union
		{
			void (CAttributeManager::*mfp)();
			struct {
				void* addr;
#ifdef __linux__
				intptr_t adjustor;
#endif	
			} s;
		} u;
		u.s.addr = m_pVTable[off_OnAttributeValuesChanged];
#ifdef __linux__
		u.s.adjustor = 0;
#endif
		(this->*u.mfp)();
	}

	static float AttribHookValue_float(float initial, const char* name, CBaseEntity* ent, CUtlVector<CBaseEntity*>* list = nullptr, bool globalstring = false) {
		return (*fn_AttribHookValue_float)(initial, name, ent, list, globalstring);
	}
	static int AttribHookValue_int(int initial, const char* name, CBaseEntity* ent, CUtlVector<CBaseEntity*>* list = nullptr, bool globalstring = false) {
		return (*fn_AttribHookValue_int)(initial, name, ent, list, globalstring);
	}
protected:
	void** m_pVTable;

	friend bool Econ_Setup(SourceMod::IGameConfig* conf);
	static int off_OnAttributeValuesChanged;
	static float (*fn_AttribHookValue_float)(float, const char*, CBaseEntity*, CUtlVector<CBaseEntity*>*, bool);
	static int (*fn_AttribHookValue_int)(int, const char*, CBaseEntity*, CUtlVector<CBaseEntity*>*, bool);
};

class CAttributeList {
public:
	void SetRuntimeAttributeValue(const CEconItemAttributeDefinition* def, float value) { (this->*fn_SetRuntimeAttributeValue)(def, value); }
	const CEconItemAttribute* GetAttributeByID(int defIndex) const {
		int size = m_Attributes.Count();
		for (int i = 0; i < size; i++) {
			auto data = m_Attributes[i].GetStaticData();
			if (data && (data->GetDefinitionIndex() == defIndex)) {
				return &m_Attributes[i];
			}
		}

		return nullptr;
	}

	void RemoveAttribute(const CEconItemAttributeDefinition* def) { (this->*fn_RemoveAttribute)(def); }
	void DestroyAllAttributes(void) { (this->*fn_DestroyAllAttributes)(); }

	void NotifyManagerOfAttributeValueChanges(void) { 
		if (m_pManager) {
			m_pManager->OnAttributeValuesChanged();
		}
	}

	std::int32_t GetNumAttributes() { return m_Attributes.Count(); };
	CEconItemAttribute* GetAttribute(int index) { return &m_Attributes[index]; };

	CAttributeManager* GetManager() { return m_pManager; };
protected:
	void* m_pVTable;
	CUtlVector<CEconItemAttribute> m_Attributes;
	CAttributeManager* m_pManager;
protected:
	friend bool Econ_Setup(SourceMod::IGameConfig* conf);
	static void (CAttributeList::*fn_SetRuntimeAttributeValue)(const CEconItemAttributeDefinition*, float);
	static void (CAttributeList::*fn_RemoveAttribute)(const CEconItemAttributeDefinition*);
	static void (CAttributeList::*fn_DestroyAllAttributes)();
};

extern CEconItemSchema* (*fn_GetItemSchema)();
inline CEconItemSchema* GetItemSchema() { return fn_GetItemSchema(); };
extern void (*fn_CopyStringAttributeValueToCharPointerOutput)(const void* value, const char** out_value);
inline void CopyStringAttributeValueToCharPointerOutput(const void* value, const char** out_value) { fn_CopyStringAttributeValueToCharPointerOutput(value, out_value); };

inline const CEconItemAttributeDefinition* CEconItemAttribute::GetStaticData(void) const { 
	return GetItemSchema()->GetAttributeDefinition(m_iAttributeDefinitionIndex); 
}

bool Econ_Setup(SourceMod::IGameConfig* conf);