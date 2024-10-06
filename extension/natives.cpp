#include "natives.hpp"
#include "econitem.hpp"
#include "entity.hpp"

CBaseEntity* Get(IPluginContext* context, cell_t param) {
	auto entity = gamehelpers->ReferenceToEntity(param);
	if (!entity) {
		context->ThrowNativeError("Invalid entity reference %d", param);
		return nullptr;
	}
	return entity;
}

CAttributeList* GetAttributeList(IPluginContext* context, CBaseEntity* entity) {
	auto list = entity->GetAttributeList();
	if (!list) {
		context->ThrowNativeError("Entity does not have an attribute list!");
		return nullptr;
	}
	return list;
}

cell_t SetByName(IPluginContext* context, const cell_t* params) {
	auto entity = Get(context, params[1]);
	if (!entity) {
		return 0;
	}

	auto list = GetAttributeList(context, entity);
	if (!list) {
		return 0;
	}

	char* name = nullptr;
	context->LocalToString(params[2], &name);

	auto def = GetItemSchema()->GetAttributeDefinitionByName(name);
	if (!def) {
		context->ThrowNativeError("Attribute \"%s\" does not exist!", name);
		return 0;
	}

	float value = sp_ctof(params[3]);

	list->SetRuntimeAttributeValue(def, value);
	return 1;
}

cell_t SetByDefIndex(IPluginContext* context, const cell_t* params) {
	auto entity = Get(context, params[1]);
	if (!entity) {
		return 0;
	}

	auto list = GetAttributeList(context, entity);
	if (!list) {
		return 0;
	}

	auto def = GetItemSchema()->GetAttributeDefinition(params[2]);
	if (!def) {
		context->ThrowNativeError("Attribute index %d does not exist!", params[2]);
		return 0;
	}

	float value = sp_ctof(params[3]);

	list->SetRuntimeAttributeValue(def, value);
	return 1;
}

cell_t GetByName(IPluginContext* context, const cell_t* params) {
	auto entity = Get(context, params[1]);
	if (!entity) {
		return 0;
	}

	auto list = GetAttributeList(context, entity);
	if (!list) {
		return 0;
	}

	char* name = nullptr;
	context->LocalToString(params[2], &name);

	auto def = GetItemSchema()->GetAttributeDefinitionByName(name);
	if (!def) {
		context->ThrowNativeError("Attribute \"%s\" does not exist!", name);
		return 0;
	}

	auto index = def->GetDefinitionIndex();
	return PtrToPawnAddress(context, list->GetAttributeByID(index));
}

cell_t GetByDefIndex(IPluginContext* context, const cell_t* params) {
	auto entity = Get(context, params[1]);
	if (!entity) {
		return 0;
	}

	auto list = GetAttributeList(context, entity);
	if (!list) {
		return 0;
	}

	return PtrToPawnAddress(context, list->GetAttributeByID(params[2]));
}

cell_t RemoveByName(IPluginContext* context, const cell_t* params) {
	auto entity = Get(context, params[1]);
	if (!entity) {
		return 0;
	}

	auto list = GetAttributeList(context, entity);
	if (!list) {
		return 0;
	}

	char* name = nullptr;
	context->LocalToString(params[2], &name);

	auto def = GetItemSchema()->GetAttributeDefinitionByName(name);
	if (!def) {
		context->ThrowNativeError("Attribute \"%s\" does not exist!", name);
		return 0;
	}

	list->RemoveAttribute(def);
	return 1;
}

cell_t RemoveByDefIndex(IPluginContext* context, const cell_t* params) {
	auto entity = Get(context, params[1]);
	if (!entity) {
		return 0;
	}

	auto list = GetAttributeList(context, entity);
	if (!list) {
		return 0;
	}

	auto def = GetItemSchema()->GetAttributeDefinition(params[2]);
	if (!def) {
		context->ThrowNativeError("Attribute index %d does not exist!", params[2]);
		return 0;
	}

	list->RemoveAttribute(def);
	return 1;
}

cell_t RemoveAll(IPluginContext* context, const cell_t* params) {
	auto entity = Get(context, params[1]);
	if (!entity) {
		return 0;
	}

	auto list = GetAttributeList(context, entity);
	if (!list) {
		return 0;
	}

	list->DestroyAllAttributes();
	return 1;
}

cell_t ClearCache(IPluginContext* context, const cell_t* params) {
	auto entity = Get(context, params[1]);
	if (!entity) {
		return 0;
	}

	auto list = entity->GetAttributeList();
	if (!list) {
		return 0;
	}

	list->NotifyManagerOfAttributeValueChanges();
	return 1;
}

cell_t SetDefIndex(IPluginContext* context, const cell_t* params) {
	auto attrib = static_cast<CEconItemAttribute*>(PawnAddressToPtr(context, params[1]));
	if (attrib == nullptr) {
		return 0;
	}

	attrib->SetDefIndex(params[2]);
	return 0;
}

cell_t GetDefIndex(IPluginContext* context, const cell_t* params) {
	auto attrib = static_cast<CEconItemAttribute*>(PawnAddressToPtr(context, params[1]));
	if (attrib == nullptr) {
		return 0;
	}

	return attrib->GetDefIndex();
}

cell_t SetValue(IPluginContext* context, const cell_t* params) {
	auto attrib = static_cast<CEconItemAttribute*>(PawnAddressToPtr(context, params[1]));
	if (attrib == nullptr) {
		return 0;
	}

	attrib->SetValue(sp_ctof(params[2]));
	return 0;
}

cell_t GetValue(IPluginContext* context, const cell_t* params) {
	auto attrib = static_cast<CEconItemAttribute*>(PawnAddressToPtr(context, params[1]));
	if (attrib == nullptr) {
		return 0;
	}

	return sp_ftoc(attrib->GetValue());
}

cell_t UnsafeGetStringValue(IPluginContext* context, const cell_t* params) {
	auto attrib = PawnAddressToPtr(context, params[1]);
	if (attrib == nullptr) {
		return 0;
	}	

	const char* name = nullptr;
	CopyStringAttributeValueToCharPointerOutput(attrib, &name);

	if (name) {
		size_t len;
		context->StringToLocalUTF8(params[2], params[3], name, &len);
		return (cell_t)len;
	}
	return 0;
}

cell_t SetRefundableCurrency(IPluginContext* context, const cell_t* params) {
	auto attrib = static_cast<CEconItemAttribute*>(PawnAddressToPtr(context, params[1]));
	if (attrib == nullptr) {
		return 0;
	}

	attrib->SetRefundValue(params[2]);
	return 0;
}

cell_t GetRefundableCurrency(IPluginContext* context, const cell_t* params) {
	auto attrib = static_cast<CEconItemAttribute*>(PawnAddressToPtr(context, params[1]));
	if (attrib == nullptr) {
		return 0;
	}

	return attrib->GetRefundValue();
}

cell_t ListDefIndices(IPluginContext* context, const cell_t* params) {
	auto entity = Get(context, params[1]);
	if (!entity) {
		return 0;
	}

	auto list = entity->GetAttributeList();
	if (!list) {
		return 0;
	}

	cell_t* arr;
	context->LocalToPhysAddr(params[2], &arr);

	cell_t len = params[3];
	cell_t size = list->GetNumAttributes();

	int i = 0;
	for (; i < size && i < len; i++) {
		arr[i] = list->GetAttribute(i)->GetDefIndex();
	}
	return i;
}

cell_t GetStaticAttribs(IPluginContext* context, const cell_t* params) {
	auto def = GetItemSchema()->GetItemDefinition(params[1]);
	if (!def) {
		return 0;
	}

	cell_t* indices;
	context->LocalToPhysAddr(params[2], &indices);
	cell_t* values;
	context->LocalToPhysAddr(params[3], &values);

	cell_t len = params[4];
	auto staticAttrib = def->GetStaticAttributes();
	cell_t size = staticAttrib->Count();

	int i = 0;
	for (; i < len && i < size; i++) {
		auto& attrib = (*staticAttrib)[i];
		int index = attrib.iDefIndex;
		indices[i] = index;
		values[i] = sp_ftoc(attrib.m_value.asFloat);
	}
	return i;
}

cell_t IsIntegerValue(IPluginContext* context, const cell_t* params) {
	auto def = GetItemSchema()->GetAttributeDefinition(params[1]);
	if (!def) {
		context->ThrowNativeError("Attribute index %d does not exist!", params[2]);
		return 0;
	}

	return (def->IsStoredAsInteger()) ? 1 : 0;
}

cell_t IsValidAttributeName(IPluginContext* context, const cell_t* params) {
	char* name = nullptr;
	context->LocalToString(params[1], &name);

	return (GetItemSchema()->GetAttributeDefinitionByName(name)) ? 1 : 0;
}

cell_t AddCustomPlayerAttribute(IPluginContext* context, const cell_t* params) {
	auto entity = Get(context, params[1]);
	if (!entity) {
		return 0;
	}

	CTFPlayer* player = entity->ToTFPlayer();
	if (!player)  {
		context->ThrowNativeError("Entity %d is not a player!", params[1]);
		return 0;
	}

	char* name = nullptr;
	context->LocalToString(params[2], &name);

	player->AddCustomAttribute(name, sp_ctof(params[3]), sp_ctof(params[4]));
	return 0;
}

cell_t RemoveCustomPlayerAttribute(IPluginContext* context, const cell_t* params) {
	auto entity = Get(context, params[1]);
	if (!entity) {
		return 0;
	}

	CTFPlayer* player = entity->ToTFPlayer();
	if (!player)  {
		context->ThrowNativeError("Entity %d is not a player!", params[1]);
		return 0;
	}

	char* name = nullptr;
	context->LocalToString(params[2], &name);

	player->RemoveCustomAttribute(name);
	return 0;
}

cell_t HookValueFloat(IPluginContext* context, const cell_t* params) {
	auto entity = Get(context, params[3]);
	if (!entity) {
		return 0;
	}

	char* name = nullptr;
	context->LocalToString(params[2], &name);

	return sp_ftoc(CAttributeManager::AttribHookValue_float(sp_ctof(params[1]), name, entity));
}

cell_t HookValueInt(IPluginContext* context, const cell_t* params) {
	auto entity = Get(context, params[3]);
	if (!entity) {
		return 0;
	}

	char* name = nullptr;
	context->LocalToString(params[2], &name);

	return CAttributeManager::AttribHookValue_int(params[1], name, entity);
}

cell_t IsReady(IPluginContext* context, const cell_t* params) {
	return 1;
}

void natives_setup(std::vector<sp_nativeinfo_t>& natives) {
	sp_nativeinfo_t list[] = {
		{"TF2Attrib_SetByName", SetByName},
		{"TF2Attrib_SetByDefIndex", SetByDefIndex},
		{"TF2Attrib_GetByName", GetByName},
		{"TF2Attrib_GetByDefIndex", GetByDefIndex},
		{"TF2Attrib_RemoveByName", RemoveByName},
		{"TF2Attrib_RemoveByDefIndex", RemoveByDefIndex},
		{"TF2Attrib_RemoveAll", RemoveAll},
		{"TF2Attrib_ClearCache", ClearCache},
		{"TF2Attrib_SetDefIndex", SetDefIndex},
		{"TF2Attrib_GetDefIndex", GetDefIndex},
		{"TF2Attrib_SetValue", SetValue},
		{"TF2Attrib_GetValue", GetValue},
		{"TF2Attrib_UnsafeGetStringValue", UnsafeGetStringValue},
		{"TF2Attrib_SetRefundableCurrency", SetRefundableCurrency},
		{"TF2Attrib_GetRefundableCurrency", GetRefundableCurrency},
		{"TF2Attrib_ListDefIndices", ListDefIndices},
		{"TF2Attrib_GetStaticAttribs", GetStaticAttribs},
		{"TF2Attrib_IsIntegerValue", IsIntegerValue},
		{"TF2Attrib_IsValidAttributeName", IsValidAttributeName},
		{"TF2Attrib_AddCustomPlayerAttribute", AddCustomPlayerAttribute},
		{"TF2Attrib_RemoveCustomPlayerAttribute", RemoveCustomPlayerAttribute},
		{"TF2Attrib_HookValueFloat", HookValueFloat},
		{"TF2Attrib_HookValueInt", HookValueInt},
		{"TF2Attrib_IsReady", IsReady},
	};
	natives.insert(natives.end(), std::begin(list), std::end(list));
}