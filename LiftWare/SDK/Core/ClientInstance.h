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
#include "../Render/LevelRendererPlayer.h"
#include "../World/Level/Level.h"

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

class LevelRenderer {
public:
	BUILD_ACCESS(LevelRendererPlayer*, renderplayer, 0x3F0)
};

class ClientInstance {
public:
	BUILD_ACCESS(MinecraftGame*, mcGame, Offsets::mcGame);
	BUILD_ACCESS(Minecraft*, minecraft, Offsets::minecraft);
	BUILD_ACCESS(GuiData*, guiData, Offsets::guiData);
public:
	ClientHMDState* getHMDState() {
		return Memory::callVirtualFuncSolstice<ClientHMDState*>(407, this, "GetHMDState");
	}

	LocalPlayer* getLocalPlayer() {
		return Memory::callVirtualFuncSolstice<LocalPlayer*>(31, this, "GetLocalPlayer");
	}

	void playUi(const std::string& soundName, float volume,
		float pitch) {
		mcGame->playUI(soundName, volume, pitch);
	}

	GLMatrix* getbadrefdef() {
		return (GLMatrix*)((uintptr_t)(this) + Offsets::BadRefDef);
	}

	LevelRenderer* getLevelRenderer() {
		return Memory::callVirtualFuncSolstice<LevelRenderer*>(216, this, "GetLevelRenderer");
	}

	Vector2<float> getFov() {
		float x = getLevelRenderer()->getrenderplayer()->getFovX();
		float y = getLevelRenderer()->getrenderplayer()->getFovY();
		return Vector2<float>(x, y);
	}

	void grabMouse() {
		Memory::callVirtualFuncSolstice<void>(VTables::ClientInstanceGrabMouse, this, "GetMouse");
	}

	void releaseMouse() {
		Memory::callVirtualFuncSolstice<void>(VTables::ClientInstanceReleaseMouse, this, "ReleaseMouse");
	}

	void focusMouse() {
		Memory::callVirtualFuncSolstice<void>(VTables::ClientInstanceFocusMouse, this, "FocusMouse");
	}

	Level* getLevel() {
		return Memory::callVirtualFuncSolstice<Level*>(191, this, "GetLevel");
	}
};
