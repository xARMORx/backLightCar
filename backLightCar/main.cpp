#include "main.h"

RwRGBA color;

void __cdecl CShadows__StoreShadowForVehicle(void* CVehicle, int vehicleShadowType) {
	uint32_t col_table = *(uint32_t*)(0x4C8390);
	uint8_t m_nPrimaryColor = *(uint8_t*)((uint32_t)CVehicle + 0x434);
	color = colorConverter(*(uint32_t*)(col_table + (m_nPrimaryColor * 4)));

	CShadows__StoreShadowForVehicleHook->call<void*, int>(CVehicle, vehicleShadowType);
}

bool __cdecl CShadows__StoreStaticShadow(int id, char type, RwTexture* texture, CVector* center, float upX, float upY, float rightX, float rightY, short intensity, unsigned char r, unsigned char g, unsigned char b, float zDistance, float scale, float drawDistance, char bTemporary, float upDistance) {
	if (getGameHour() >= 20 || getGameHour() < 6) {
		r = color.blue + 5;
		g = color.green + 5;
		b = color.red + 5;
		intensity = 255;
	}
	else {
		r = 0;
		g = 0;
		b = 0;
	}

	return CShadows__StoreStaticShadowHook->call<int, char, RwTexture*, CVector*, float, float, float, float, short, unsigned char, unsigned char, unsigned char, float, float, float, char, float>(id, type, texture, center, upX, upY, rightX, rightY, intensity, r, g, b, zDistance, scale, drawDistance, bTemporary, upDistance);
}

void CShadows__StoreShadowToBeRendered(unsigned char type, RwTexture* texture, CVector const* posn, float topX, float topY, float rightX, float rightY, short intensity, unsigned char r, unsigned char g, unsigned char b, float zDistance, bool drawOnWater, float scale, void* realTimeShadow, bool drawOnBuildings) {
	if (getGameHour() >= 20 || getGameHour() < 6) {
		r = color.blue + 5;
		g = color.green + 5;
		b = color.red + 5;
	}
	else {
		r = 0;
		g = 0;
		b = 0;
	}

	CShadows__StoreShadowToBeRenderedHook->call<unsigned char, RwTexture*, CVector const*, float, float, float, float, short, unsigned char, unsigned char, unsigned char, float, bool, float, void*, bool>(type, texture, posn, topX, topY, rightX, rightY, intensity, r, g, b, zDistance, drawOnWater, scale, realTimeShadow, drawOnBuildings);
}

void __cdecl CTimer__Update() {
	static bool init{};

	if (!init) {
		CShadows__StoreShadowForVehicleHook = std::make_unique<memwrapper::memhook<CShadows__StoreShadowForVehiclePrototype>>(0x6ABCF5, &CShadows__StoreShadowForVehicle);
		CShadows__StoreShadowForVehicleHook->install();

		CShadows__StoreShadowToBeRenderedHook = std::make_unique<memwrapper::memhook<CShadows__StoreShadowToBeRenderedPrototype>>(0x70C3F8, &CShadows__StoreShadowToBeRendered);
		CShadows__StoreShadowToBeRenderedHook->install();

		CShadows__StoreStaticShadowHook = std::make_unique<memwrapper::memhook<CShadows__StoreStaticShadowPrototype>>(0x70C33F, &CShadows__StoreStaticShadow);
		CShadows__StoreStaticShadowHook->install();
		init = { true };
	}

	CTimer__UpdateHook->call();
}

AsiPlugin::AsiPlugin() {
	CTimer__UpdateHook = std::make_unique<memwrapper::memhook<CTimer__UpdatePrototype>>(0x561B10, &CTimer__Update);
	CTimer__UpdateHook->install();
}

AsiPlugin::~AsiPlugin() {
	CTimer__UpdateHook->remove();
	CShadows__StoreShadowForVehicleHook->remove();
	CShadows__StoreStaticShadowHook->remove();
	CShadows__StoreShadowToBeRenderedHook->remove();
}