#pragma once
#include "../../Memory/Memory.h"
#include "../../Math/Vector2.h"
#include "../../Math/Vector3.h"
#include "../../Math/Vector4.h"
#include "ClientHMDState.h"
#include "Minecraft.h"
#include "MinecraftGame.h"
#include "GuiData.h"
#include "../World/Actors/Actor.h"
#include "../World/Actors/Mob.h"
#include "../World/Actors/Player.h"
#include "../World/Actors/LocalPlayer.h"
#include "../../Values/Vtables.h"

struct GLMatrix {
public:
	float matrix[16];
	float matrix_nest[4][4];

	GLMatrix* correct() {
		GLMatrix* newMatPtr = new GLMatrix;

		for (int i = 0; i < 4; i++) {
			newMatPtr->matrix[i * 4 + 0] = matrix[0 + i];
			newMatPtr->matrix[i * 4 + 1] = matrix[4 + i];
			newMatPtr->matrix[i * 4 + 2] = matrix[8 + i];
			newMatPtr->matrix[i * 4 + 3] = matrix[12 + i];
		}
		return newMatPtr;
	};

	bool WorldToScreen(Vector3<float> origin, Vector3<float> pos, Vector2<float>& screen, Vector2<float>& fov, Vector2<float>& displaySize) const
	{
		pos = pos - origin;

		float x = transformx(pos);
		float y = transformy(pos);
		float z = transformz(pos);

		if (z > 0)
			return false;

		float mX = (float)displaySize.x / 2.0F;
		float mY = (float)displaySize.y / 2.0F;

		screen.x = mX + (mX * x / -z * fov.x);
		screen.y = mY - (mY * y / -z * fov.y);

		return true;
	}

	float transformDot(int index, const Vector3<float>& p) const {
		return p.x * matrix[index * 4 + 0] +
			p.y * matrix[index * 4 + 1] +
			p.z * matrix[index * 4 + 2] +
			matrix[index * 4 + 3];
	}

	__forceinline float transformx(const Vector3<float>& p) const {
		return p.x * matrix[0] + p.y * matrix[4] + p.z * matrix[8] + matrix[12];
	}

	__forceinline float transformy(const Vector3<float>& p) const {
		return p.x * matrix[1] + p.y * matrix[5] + p.z * matrix[9] + matrix[13];
	}

	__forceinline float transformz(const Vector3<float>& p) const {
		return p.x * matrix[2] + p.y * matrix[6] + p.z * matrix[10] + matrix[14];
	}
};

class ClientInstance {
public:
	BUILD_ACCESS(MinecraftGame*, mcGame, Offsets::mcGame);
	BUILD_ACCESS(Minecraft*, minecraft, Offsets::minecraft);
	BUILD_ACCESS(GuiData*, guiData, Offsets::guiData);
public:
	ClientHMDState* getHMDState() {

		return reinterpret_cast<ClientHMDState*>((uintptr_t)this + Offsets::HMDState);
	}

	LocalPlayer* getLocalPlayer() {

		return Memory::CallVFunc<VTables::ClientInstanceLocalPlayerOffset, LocalPlayer*>(this);
	}

	void playUI(std::string str, float a1, float a2)
	{
		if (this == nullptr)
			return;

		Memory::CallVFunc<VTables::ClientInstancePlayUI, void>(this, str, a1, a2);
	}

	GLMatrix* getbadrefdef() {
		return (GLMatrix*)((uintptr_t)(this) + Offsets::BadRefDef);
	}

	Vector2<float> getFov() {
		Vector2<float> fov;
		fov.x = *(float*)(reinterpret_cast<uintptr_t>(this) + Offsets::FovX);
		fov.y = *(float*)(reinterpret_cast<uintptr_t>(this) + Offsets::FovY);
		return fov;
	}

	void grabMouse() {
		Memory::CallVFunc<VTables::ClientInstanceGrabMouse, void>(this);
	}

	void releaseMouse() {
		Memory::CallVFunc<VTables::ClientInstanceReleaseMouse, void>(this);
	}

	void focusMouse() {
		Memory::CallVFunc<VTables::ClientInstanceFocusMouse, void>(this);
	}
};
