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
#include "../Components/ActorTypeComponent.h"
#include "../Components/Flags.h"

class Actor {
public:
    BUILD_ACCESS(Level*, level, 0x1d8)
    //BUILD_ACCESS(StateVectorComponent*, stateVector, Offsets::stateVector);
public:

    AABBShapeComponent* getAABBShapeComponent() {
        return getEntityContext()->tryGetComponent<AABBShapeComponent>();
    }

    StateVectorComponent* getStateVectorComponent() {
        return getEntityContext()->tryGetComponent<StateVectorComponent>();
    }

    ActorTypeComponent* getActorTypeComponent() {
        return getEntityContext()->tryGetComponent<ActorTypeComponent>();
    }

    ActorRotationComponent* getActorRotationComponent() {
        return getEntityContext()->tryGetComponent<ActorRotationComponent>();
    }

    Vector3<float> getPosition() {
        return getStateVectorComponent() ? getStateVectorComponent()->pos : Vector3<float>(0.f, 0.f, 0.f);
    }

    bool isOnGround() {
        return getEntityContext()->hasComponent<OnGroundFlagComponent>();
    }

    void Jump() {
        return getEntityContext()->getOrAddComponent<JumpFromGroundRequestComponent>();
    }

    void setOnGround(bool state) {
        if (state) {
            return getEntityContext()->getOrAddComponent<OnGroundFlagComponent>();
        }
        else {
            getEntityContext()->removeComponent<OnGroundFlagComponent>();
        }
    };

    void swing() {
        Memory::callVirtualFuncSolstice<void>(VTables::ActorSwing, this, "ActorSwing");
    }

    ActorHeadRotationComponent* getActorHeadRotationComponent() {
        return getEntityContext()->tryGetComponent<ActorHeadRotationComponent>();
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
