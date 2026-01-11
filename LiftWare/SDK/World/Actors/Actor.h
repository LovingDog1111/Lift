#pragma once
#include "ActorFlags.h" //hey we can do this at least
#include "EntityIdTraits.h"
#include "EntityID.h"
#include "EntityContext.h"
#include "../../../Math/IncludeAll.h"
#include "../../../Memory/Memory.h"
#include "../../../Values/Sigs.h"
#include "../../../Values/Vtables.h"
#include "../Level/Level.h"
#include "../Components/AABBShapeComponent.h"
#include "../Components/ActorRotationComponent.h"
#include "../Components/StateVectorComponent.h"
#include "../Components/ActorHeadRotationComponent.h"

class Actor {
public:
    BUILD_ACCESS(Level*, level, Offsets::Level);
    BUILD_ACCESS(StateVectorComponent*, stateVector, Offsets::stateVector);
    BUILD_ACCESS(AABBShapeComponent*, aabbShape, Offsets::aabbShape);
    BUILD_ACCESS(ActorRotationComponent*, rotation, Offsets::rotation);
public:
    template <typename T>
    T* getComponent() {
        return const_cast<T*>(getEntityContext()->getRegistry().try_get<T>(getEntityContext()->mEntity));
    }

    template <typename T>
    bool hasComponent() {
        return getEntityContext()->getRegistry().all_of<T>(getEntityContext()->mEntity);
    }

    template <typename T>
    void getOrEmplaceComponent() {
        return getEntityContext()->getRegistry().get_or_emplace<T>(getEntityContext()->mEntity);
    }

    template <typename T>
    void removeComponent() {
        getEntityContext()->getRegistry().remove<T>(getEntityContext()->mEntity);
    }

    Vector3<float> getPosition() {
        return stateVector ? stateVector->pos : Vector3<float>(0.f, 0.f, 0.f);
    }

    void setPosition(const Vector3<float>& pos) {
        using func_t = __int64(__thiscall*)(Actor*, const Vector3<float>&);
        static func_t Func = (func_t)(Memory::FindSignature(Sigs::Actor_SetPos, "Actor_SetPosition"));
        Func(this, pos);
    }

    void swing() {
        Memory::CallVFunc<VTables::ActorSwing, void>(this);
    }

    ActorHeadRotationComponent* getActorHeadRotationComponent() {
        using func_t = ActorHeadRotationComponent * (__cdecl*)(__int64, __int64);
        static func_t Func = (func_t)(Memory::getFuncFromCall(Memory::FindSignature(Sigs::TryGetActorHeadRotationComponent)));
        __int64 a1 = *(__int64*)((__int64)this + 0x10);
        __int64 a2 = (__int64)&(*(__int64*)((__int64)this + 0x18));
        return Func(a1, a2);
    }

    EntityContext* getEntityContext()
    {
        uintptr_t address = reinterpret_cast<uintptr_t>(this);
        return reinterpret_cast<EntityContext*>((uintptr_t)this + Offsets::Player);
    }
public:
    virtual bool getStatusFlag(ActorFlags flag);
    virtual void setStatusFlag(ActorFlags flag, bool value);
};
