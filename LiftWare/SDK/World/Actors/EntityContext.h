#pragma once
#include "../../Libs/entt/entt.hpp"

struct EntityRegistry : std::enable_shared_from_this<EntityRegistry> {
    std::string name;
    entt::basic_registry<EntityId> registry;
    uint32_t id;
};

struct EntityContext {
    EntityRegistry& registry;
    entt::basic_registry<EntityId>& enttRegistry;
    EntityId entity;

    template <typename T>
    [[nodiscard]] T* tryGetComponent() {
        return this->enttRegistry.try_get<T>(this->entity);
    }

    template <typename T>
    [[nodiscard]] const T* tryGetComponent() const {
        return this->enttRegistry.try_get<T>(this->entity);
    }

    template <typename T>
    [[nodiscard]] bool hasComponent() const {
        return this->enttRegistry.all_of<T>(this->entity);
    }

    template <typename T>
    void getOrAddComponent() {
        return this->enttRegistry.get_or_emplace<T>(this->entity);
    }

    template <typename T>
    void removeComponent() {
        this->enttRegistry.remove<T>(this->entity);
    }
};