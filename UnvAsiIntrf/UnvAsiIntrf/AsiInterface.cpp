#include "stdafx.h"
#define DLL_EXPORT __declspec(dllexport)

// IDXGISwapChain::Present callback
// Called right before the actual Present method call, render test calls don't trigger callbacks
// When the game uses DX10 it actually uses DX11 with DX10 feature level
// Remember that you can't call natives inside
// void OnPresent(IDXGISwapChain *swapChain);
typedef void(*PresentCallback)(void *);

//Dummy version enum. We're letting the interface handler deciding its content
enum eGameVersion : int {};

/*Handler functions*/
int(*handlerCreateTexture)(const char*) = nullptr;
void(*handlerDrawTexture)(int, int, int, int, float, float, float, float, float, float, float, float, float, float, float, float) = nullptr;
void(*handlerPresentCallbackRegister)(PresentCallback) = nullptr;
void(*handlerPresentCallbackUnregister)(PresentCallback) = nullptr;
void(*handlerKeyboardHandlerRegister)(KeyboardHandler) = nullptr;
void(*handlerKeyboardHandlerUnregister)(KeyboardHandler) = nullptr;
void(*handlerScriptWait)(DWORD) = nullptr;
void(*handlerScriptRegister)(HMODULE, void*) = nullptr;
void(*handlerScriptRegisterAdditionalThread)(HMODULE, void*) = nullptr;
void(*handlerScriptUnregisterByModule)(HMODULE) = nullptr;
void(*handlerScriptUnregisterByMain)(void*) = nullptr;
void(*handlerNativeInit)(UINT64) = nullptr;
void(*handlerNativePush64)(UINT64) = nullptr;
PUINT64(*handlerNativeCall)() = nullptr;
UINT64*(*handlerGetGlobalPtr)(int) = nullptr;
int(*handlerWorldGetAllVehicles)(int*, int) = nullptr;
int(*handlerWorldGetAllPeds)(int*, int) = nullptr;
int(*handlerWorldGetAllObjects)(int*, int) = nullptr;
int(*handlerWorldGetAllPickups)(int*, int) = nullptr;
BYTE*(*handlerGetScriptHandleBaseAddress)(int) = nullptr;
int(*handlerGetGameVersion)() = nullptr;

/* Handler registration */
extern "C" DLL_EXPORT void RegisterHandler(int(*argCreateTexture)(const char*), void(*argDrawTexture)(int, int, int, int, float, float, float, float, float, float, float, float, float, float, float, float), void(*argPresentCallbackRegister)(PresentCallback), void(*argPresentCallbackUnregister)(PresentCallback), void(*argKeyboardHandlerRegister)(KeyboardHandler), void(*argKeyboardHandlerUnregister)(KeyboardHandler), void(*argScriptWait)(DWORD),void(*argScriptRegister)(HMODULE, void*), void(*argScriptRegisterAdditionalThread)(HMODULE, void*), void(*argScriptUnregisterByModule)(HMODULE), void(*argScriptUnregisterByMain)(void*), void(*argNativeInit)(UINT64), void(*argNativePush64)(UINT64), PUINT64(*argNativeCall)(), UINT64*(*argGetGlobalPtr)(int), int(*argWorldGetAllVehicles)(int*, int), int(*argWorldGetAllPeds)(int*, int), int(*argWorldGetAllObjects)(int*, int),int(*argWorldGetAllPickups)(int*, int), BYTE*(*argGetScriptHandleBaseAddress)(int), int(*argGetGameVersion)())
{
	handlerCreateTexture = argCreateTexture;
	handlerDrawTexture = argDrawTexture;
	handlerPresentCallbackRegister = argPresentCallbackRegister;
	handlerPresentCallbackUnregister = argPresentCallbackUnregister;
	handlerKeyboardHandlerRegister = argKeyboardHandlerRegister;
	handlerKeyboardHandlerUnregister = argKeyboardHandlerUnregister;
	handlerScriptWait = argScriptWait;
	handlerScriptRegister = argScriptRegister;
	handlerScriptRegisterAdditionalThread = argScriptRegisterAdditionalThread;
	handlerScriptUnregisterByModule = argScriptUnregisterByModule;
	handlerScriptUnregisterByMain = argScriptUnregisterByMain;
	handlerNativeInit = argNativeInit;
	handlerNativePush64 = argNativePush64;
	handlerNativeCall = argNativeCall;
	handlerGetGlobalPtr = argGetGlobalPtr;
	handlerWorldGetAllVehicles = argWorldGetAllVehicles;
	handlerWorldGetAllPeds = argWorldGetAllPeds;
	handlerWorldGetAllObjects = argWorldGetAllObjects;
	handlerWorldGetAllPickups = argWorldGetAllPickups;
	handlerGetScriptHandleBaseAddress = argGetScriptHandleBaseAddress;
	handlerGetGameVersion = argGetGameVersion;
}

/* textures */

// Create texture
//	texFileName	- texture file name, it's best to specify full texture path and use PNG textures
//	returns	internal texture id
//	Texture deletion is performed automatically when game reloads scripts
//	Can be called only in the same thread as natives

DLL_EXPORT int createTexture(const char *texFileName)
{
	if(handlerCreateTexture != nullptr)
		return handlerCreateTexture(texFileName);
	else return -1;
}

// Draw texture
//	id		-	texture id recieved from createTexture()
//	index	-	each texture can have up to 64 different instances on screen at one time - does nothing
//	level	-	draw level, being used in global draw order, texture instance with least level draws first - does nothing
//	time	-	how much time (ms) texture instance will stay on screen, the amount of time should be enough
//				for it to stay on screen until the next corresponding drawTexture() call
//	sizeX,Y	-	size in screen space, should be in the range from 0.0 to 1.0, e.g setting this to 0.2 means that
//				texture instance will take 20% of the screen space
//	centerX,Y -	center position in texture space, e.g. 0.5 means real texture center
//	posX,Y	-	position in screen space, [0.0, 0.0] - top left corner, [1.0, 1.0] - bottom right,
//				texture instance is positioned according to it's center
//	rotation -	should be in the range from 0.0 to 1.0
//	screenHeightScaleFactor - screen aspect ratio, used for texture size correction, you can get it using natives
//	r,g,b,a	-	color, should be in the range from 0.0 to 1.0 - does nothing
//
//	Texture instance draw parameters are updated each time script performs corresponding call to drawTexture()
//	You should always check your textures layout for 16:9, 16:10 and 4:3 screen aspects, for ex. in 1280x720,
//	1440x900 and 1024x768 screen resolutions, use windowed mode for this
//	Can be called only in the same thread as natives

DLL_EXPORT void drawTexture(int id, int index, int level, int time, float sizeX, float sizeY, float centerX, float centerY, float posX, float posY, float rotation, float screenHeightScaleFactor, float r, float g, float b, float a)
{
	if(handlerDrawTexture != nullptr)
		handlerDrawTexture(id, index, level, time, sizeX, sizeY, centerX, centerY, posX, posY, rotation, screenHeightScaleFactor, r, g, b, a);
}

// Register IDXGISwapChain::Present callback
// must be called on dll attach
DLL_EXPORT void presentCallbackRegister(PresentCallback cb)
{
	if(handlerPresentCallbackRegister != nullptr)
		handlerPresentCallbackRegister(cb);
}

// Unregister IDXGISwapChain::Present callback
// must be called on dll detach
DLL_EXPORT void presentCallbackUnregister(PresentCallback cb)
{
	if(handlerPresentCallbackUnregister != nullptr)
		handlerPresentCallbackUnregister(cb);
}

/* keyboard */

// Register keyboard handler
// must be called on dll attach
DLL_EXPORT void keyboardHandlerRegister(KeyboardHandler handler)
{
	if(handlerKeyboardHandlerRegister != nullptr)
		handlerKeyboardHandlerRegister(handler);
}

// Unregister keyboard handler
// must be called on dll detach
DLL_EXPORT void keyboardHandlerUnregister(KeyboardHandler handler)
{
	if(handlerKeyboardHandlerUnregister != nullptr)
		handlerKeyboardHandlerUnregister(handler);
}

/* scripts */

DLL_EXPORT void scriptWait(DWORD time)
{
	if(handlerScriptWait != nullptr)
		handlerScriptWait(time);
}

DLL_EXPORT void scriptRegister(HMODULE module, void(*LP_SCRIPT_MAIN)())
{
	if(handlerScriptRegister != nullptr)
		handlerScriptRegister(module, LP_SCRIPT_MAIN);
}

DLL_EXPORT void scriptRegisterAdditionalThread(HMODULE module, void(*LP_SCRIPT_MAIN)())
{
	if(handlerScriptRegisterAdditionalThread != nullptr)
		handlerScriptRegisterAdditionalThread(module, LP_SCRIPT_MAIN);
}

DLL_EXPORT void scriptUnregister(HMODULE module)
{
	if(handlerScriptUnregisterByModule != nullptr)
		handlerScriptUnregisterByModule(module);
}

DLL_EXPORT void scriptUnregister(void(*LP_SCRIPT_MAIN)()) // deprecated
{
	if(handlerScriptUnregisterByMain != nullptr)
		handlerScriptUnregisterByMain(LP_SCRIPT_MAIN);
}

DLL_EXPORT void nativeInit(UINT64 hash)
{
	if(handlerNativeInit != nullptr)
		handlerNativeInit(hash);
}

DLL_EXPORT void nativePush64(UINT64 val)
{
	if(handlerNativePush64 != nullptr)
		handlerNativePush64(val);
}

DLL_EXPORT PUINT64 nativeCall()
{
	if(handlerNativeCall != nullptr)
		return handlerNativeCall();
	else return 0UL;
}

// Returns pointer to global variable
// make sure that you check game version before accessing globals because
// ids may differ between patches
DLL_EXPORT UINT64 *getGlobalPtr(int globalId)
{
	if(handlerGetGlobalPtr != nullptr)
		return handlerGetGlobalPtr(globalId);
	else return 0U;
}

/* world */

// Get entities from internal pools
// return value represents filled array elements count
// can be called only in the same thread as natives
DLL_EXPORT int worldGetAllVehicles(int *arrPointer, int arrSize)
{
	if(handlerWorldGetAllVehicles != nullptr)
		return handlerWorldGetAllVehicles(arrPointer, arrSize);
	else return 0;
}

DLL_EXPORT int worldGetAllPeds(int *arrPointer, int arrSize)
{
	if(handlerWorldGetAllPeds != nullptr)
		return handlerWorldGetAllPeds(arrPointer, arrSize);
	else return 0;
}

DLL_EXPORT int worldGetAllObjects(int *arrPointer, int arrSize)
{
	if(handlerWorldGetAllObjects != nullptr)
		return handlerWorldGetAllObjects(arrPointer, arrSize);
	else return 0;
}

/*Thanks to alexguirre and SHVDN for this !*/
DLL_EXPORT int worldGetAllPickups(int *arrPointer, int arrSize)
{
	if(handlerWorldGetAllPickups != nullptr)
		return handlerWorldGetAllPickups(arrPointer, arrSize);
	else return 0;
}

/* misc */

// Returns base object pointer using it's script handle
// make sure that you check game version before accessing object fields because
// offsets may differ between patches
DLL_EXPORT BYTE *getScriptHandleBaseAddress(int handle)
{
	if(handlerGetScriptHandleBaseAddress != nullptr)
		return handlerGetScriptHandleBaseAddress(handle);
	else return nullptr;
}

DLL_EXPORT eGameVersion getGameVersion()
{
	if(handlerGetGameVersion != nullptr)
		return (eGameVersion)handlerGetGameVersion();
	else return (eGameVersion)-1;
}