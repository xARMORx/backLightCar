#ifndef _MAIN_H_
#define _MAIN_H_

#include <Windows.h>
#include <memory>
#include "memwrapper/memwrapper.h"

struct CVector { 
	float x, y, z; 

	CVector(float x, float y, float z) : x{ x }, y{ y }, z{ z } {};
};

struct RwTexture{};
struct RwRGBA {
	unsigned char red, green, blue, alpha;
};

struct RwRGBA colorConverter(int hexValue)
{
	struct RwRGBA rgbColor;
	rgbColor.alpha = ((hexValue >> 24) & 0xFF);
	rgbColor.red = ((hexValue >> 16) & 0xFF);
	rgbColor.green = ((hexValue >> 8) & 0xFF);
	rgbColor.blue = ((hexValue) & 0xFF);

	return rgbColor;
}

using CShadows__StoreStaticShadowPrototype = bool(__cdecl*)(int, char, RwTexture*, CVector*, float, float, float, float, short, unsigned char, unsigned char, unsigned char, float, float, float, char, float);
std::unique_ptr<memwrapper::memhook<CShadows__StoreStaticShadowPrototype>> CShadows__StoreStaticShadowHook;

using CShadows__StoreShadowToBeRenderedPrototype = void(__cdecl*)(unsigned char, RwTexture*, CVector const*, float, float, float, float, short, unsigned char, unsigned char, unsigned char, float, bool, float, void*, bool);
std::unique_ptr<memwrapper::memhook<CShadows__StoreShadowToBeRenderedPrototype>> CShadows__StoreShadowToBeRenderedHook;

using CShadows__StoreShadowForVehiclePrototype = void(__cdecl*)(void*, int);
std::unique_ptr<memwrapper::memhook<CShadows__StoreShadowForVehiclePrototype>> CShadows__StoreShadowForVehicleHook;

using CTimer__UpdatePrototype = void(__cdecl*)();
std::unique_ptr<memwrapper::memhook<CTimer__UpdatePrototype>> CTimer__UpdateHook;

int getGameHour() {
	return *(int8_t*)(0xB70153);
}

uint32_t* CModelInfo__GetModelInfo(const char* name, int* pIndex) {
	return ((uint32_t *(__cdecl*)(const char*, int*))0x4C5940)(name, pIndex);
}

class AsiPlugin{
public:
	explicit AsiPlugin();
	virtual ~AsiPlugin();
} AsiPlugin;

#endif // !_MAIN_H_