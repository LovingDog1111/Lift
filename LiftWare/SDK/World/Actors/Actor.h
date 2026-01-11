#pragma once
#include "ActorFlags.h" //hey we can do this at least
#include "EntityIdTraits.h"
#include "EntityID.h"
#include "EntityContext.h"
#include "../../../Math/IncludeAll.h"
#include "../../../Memory/Memory.h"
#include "../../../Values/Sigs.h"
#include "../../Values/Vtables.h"

class Actor {
public:
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

    void setPos(const Vector3<float>& pos) {
        using func_t = __int64(__thiscall*)(Actor*, const Vector3<float>&);
        static func_t Func = (func_t)(Memory::FindSignature(Sigs::Actor_SetPos, "Actor_SetPosition"));
        Func(this, pos);
    }

    void swing() {
        Memory::CallVFunc<VTables::ActorSwing, void>(this);
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
