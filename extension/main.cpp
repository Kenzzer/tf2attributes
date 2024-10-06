#include "main.hpp"
#include "econitem.hpp"
#include "entity.hpp"
#include "natives.hpp"

TF2AttribExt gExt;
SMEXT_LINK(&gExt);

std::vector<sp_nativeinfo_t> gNatives;
HandleType_t g_MemoryPtr;

bool TF2AttribExt::SDK_OnLoad(char* error, size_t maxlength, bool late) {
	IGameConfig* conf;
	if (!gameconfs->LoadGameConfigFile("tf2.attributes", &conf, error, maxlength)) {
		return false;
	}

	g_MemoryPtr = handlesys->FindHandleType("MemoryPointer", &g_MemoryPtr);

	gNatives.reserve(100);
	natives_setup(gNatives);
	gNatives.push_back({nullptr, nullptr});
	sharesys->AddNatives(myself, gNatives.data());

	return Econ_Setup(conf) && Entity_Setup(conf);
}