#pragma once
#include "../../Memory/Memory.h"
#include "../../Math/IncludeAll.h"
#include <functional>
#include "../../Values/Sigs.h"

class Actor;
class Player;
class InputMode;
class ItemStack;
class Block;

class GameMode {
public:
    BUILD_ACCESS(Player*, player, 0x8);
    BUILD_ACCESS(BlockPos, lastBreakPos, 0x10);
    BUILD_ACCESS(int32_t, lastBreakFace, 0x1C);
    BUILD_ACCESS(float, lastDestroyProgress, 0x20);
    BUILD_ACCESS(float, destroyProgress, 0x24);
private:
    virtual void Destructor();
public:
    virtual __int64 startDestroyBlock(Vector3<int> const& pos, unsigned char blockSide, bool& isDestroyedOut);
    virtual __int64 destroyBlock(const Vector3<int>&, unsigned char blockSide);
    virtual __int64 continueDestroyBlock(Vector3<int> const& a1, unsigned char a2, Vector3<float> const& a3, bool& a4);
    virtual __int64 stopDestroyBlock(Vector3<int> const&);
    virtual __int64 startBuildBlock(Vector3<int> const& a1, unsigned char a2, bool auth);
    virtual __int64 buildBlock(Vector3<int> const& a1, unsigned char a2, bool auth);
    virtual __int64 continueBuildBlock(Vector3<int> const& a1, unsigned char a2);
    virtual __int64 stopBuildBlock();
    virtual __int64 tick();
    virtual __int64 getPickRange(InputMode const& a1, bool a2);
    virtual __int64 useItem(class ItemStack& a1);
    virtual __int64 useItemOn(ItemStack& a1, Vector3<int> const& a2, unsigned char a3, Vector3<float> const& a4, class Block const* a5);
    virtual __int64 interact(Actor* a1, Vector3<float> const& a2);
    virtual __int64 attack(Actor*);
    virtual __int64 releaseUsingItem();
    virtual void setTrialMode(bool a1);
    virtual void isInTrialMode();
    virtual __int64 registerUpsellScreenCallback(std::function<void> a3);
public:
    bool baseUseItem(ItemStack* item) {
        using func_t = bool(__thiscall*)(GameMode*, ItemStack*);
        static func_t Func = reinterpret_cast<func_t>(Memory::FindSignature(Sigs::GameMode_BaseUseItem, "GameMode_BaseUseItem"));
        return Func(this, item);
    }

    float getDestroyRate(Block* block) {
        using func_t = float(__thiscall*)(GameMode*, Block*);
        static func_t Func = reinterpret_cast<func_t>(Memory::FindSignature(Sigs::GameMode_GetDestroyRate, "GameMode_GetDestroyRate"));
        return Func(this, block);
    }
};
