#pragma once

#include <IMemoryPointer.h>
#include <smsdk_ext.h>
#include <vector>

class ForeignMemoryPointer : public SourceMod::IMemoryPointer {
public:
	ForeignMemoryPointer(const void* ptr) : m_ptr(ptr) {}

	virtual void Delete() override
	{
		delete this;
	}

	virtual cell_t GetSize() override
	{
		return 0;
	}

	virtual void* Get() override
	{
		return (void*)m_ptr;
	}
protected:
	const void* m_ptr;
};

extern HandleType_t g_MemoryPtr;

inline void ReleasePawnAddress(Handle_t hndl, IPluginContext* context)
{
	HandleSecurity security;
	security.pIdentity = myself->GetIdentity();
	if (context) {
		security.pOwner = context->GetIdentity();
	} else {
		security.pOwner = nullptr;
	}
	security.pOwner = nullptr;

	handlesys->FreeHandle(hndl, &security);
}

inline Handle_t PtrToPawnAddress(IPluginContext* context, const void* ptr) {
	if (ptr == nullptr) {
		return BAD_HANDLE;
	}

	auto foreignPtr = new ForeignMemoryPointer(ptr);

	IdentityToken_t* identity = nullptr;
	if (context) {
		identity = context->GetIdentity();
	}

	Handle_t handle = handlesys->CreateHandle(g_MemoryPtr, foreignPtr, identity, myself->GetIdentity(), nullptr);
	if (handle == BAD_HANDLE)
	{
		delete foreignPtr;
		return BAD_HANDLE;
	}

	return handle;
}

inline void* PawnAddressToPtr(IPluginContext* context, cell_t cellHndl) {
	Handle_t hndl = (Handle_t)cellHndl;
	if (hndl == BAD_HANDLE) {
		return nullptr;
	}

	HandleError err = HandleError_None;
	IMemoryPointer* ptr = nullptr;

	HandleSecurity security;
	security.pIdentity = myself->GetIdentity();
	if (context) {
		security.pOwner = context->GetIdentity();
	} else {
		security.pOwner = nullptr;
	}

	if ((err=handlesys->ReadHandle(hndl, g_MemoryPtr, &security, (void **)&ptr)) != HandleError_None || ptr == nullptr) {
		if (context) {
			context->ThrowNativeError("Could not read Handle %x (error %d)", hndl, err);
		}
		return nullptr;
	}

	return ptr->Get();
}

void natives_setup(std::vector<sp_nativeinfo_t>& natives);