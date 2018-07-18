#ifndef ENTITY_SYSTEM_COMPONENT_MANAGER_HPP
#define ENTITY_SYSTEM_COMPONENT_MANAGER_HPP

#include <functional>
#include <unordered_map>
#include "types.hpp"

namespace engine::entitysystem {
    namespace __detail {
        struct Deleted {};

        template<typename Key, typename T>
        T& typeMap() {
            static T value;
            return value;
        }

        template<typename TComponent>
        using EntityDataStorage = std::unordered_map<Entity, TComponent>;

        using DeletedData = EntityDataStorage<Deleted>;

        template<typename TComponent>
        EntityDataStorage<TComponent>& entityData() {
            return typeMap<TComponent, EntityDataStorage<TComponent>>();
        }
    }

    class ComponentManager {
        using Deleted = __detail::Deleted;
        using DeletedData = __detail::DeletedData;
        template<typename TComponent>
        static constexpr auto entityData = __detail::entityData<TComponent>;
      public:
        Entity createEntity();
        void deleteEntity(Entity);
        template<typename T, typename... Ts>
        void cleanup();

        template<typename T>
        void addComponent(Entity);
        template<typename T>
        void removeComponent(Entity);
        template<typename T>
        bool hasComponent(Entity) const;

        template<typename T>
        T& getData(Entity);

        template<typename T, typename... Ts>
        void forEachEntity(
            std::function<void(Entity, T&, std::add_lvalue_reference_t<Ts>...)> fn
        );

    private:
        size_t numDeletedEntities = 0;

        template<typename T, typename... Ts>
        void cleanupHelper(DeletedData&);

        template<typename T, typename... Ts>
        bool hasAllComponents(Entity) const;
    };

    inline Entity ComponentManager::createEntity() {
        static Entity nextEntity = 0;
        return nextEntity++;
    }

    inline void ComponentManager::deleteEntity(Entity entity) {
        addComponent<Deleted>(entity);
        ++numDeletedEntities;
    }

    template<typename T, typename... Ts>
    inline void ComponentManager::cleanup() {
        DeletedData& deletedData = entityData<Deleted>();
        cleanupHelper<T, Ts...>(deletedData);
        deletedData.clear();
        numDeletedEntities = 0;
    }

    template<typename T>
    inline void ComponentManager::addComponent(Entity entity) {
        entityData<T>().insert({entity, T()});
    }

    template<typename T>
    inline void ComponentManager::removeComponent(Entity entity) {
        entityData<T>().erase(entity);
    }

    template<typename T>
    inline bool ComponentManager::hasComponent(Entity entity) const {
        return entityData<T>().count(entity);
    }

    template<typename T>
    inline T& ComponentManager::getData(Entity entity) {
        return entityData<T>().at(entity);
    }

    template<typename T, typename... Ts>
    inline void ComponentManager::forEachEntity(
        std::function<void(Entity, T&, std::add_lvalue_reference_t<Ts>...)> fn
    ) {
        auto& allEntitiesData = entityData<T>();

        if constexpr (sizeof...(Ts) > 0) {
            if (numDeletedEntities > 0) {
                for (auto& [entity, data] : allEntitiesData) {
                    if (!hasComponent<Deleted>(entity) && hasAllComponents<Ts...>(entity)) {
                        fn(entity, data, getData<Ts>(entity)...);
                    }
                }
            } else {
                for (auto& [entity, data] : allEntitiesData) {
                    if (hasAllComponents<Ts...>(entity)) {
                        fn(entity, data, getData<Ts>(entity)...);
                    }
                }
            }
        } else {
            if (numDeletedEntities > 0) {
                for (auto& [entity, data] : allEntitiesData) {
                    if (!hasComponent<Deleted>(entity)) {
                        fn(entity, data);
                    }
                }
            } else {
                for (auto& [entity, data] : allEntitiesData) {
                    fn(entity, data);
                }
            }
        }
    }


    template<typename T, typename... Ts>
    inline void ComponentManager::cleanupHelper(DeletedData& deletedData) {
        for (const auto& [entity, _] : deletedData) {
            entityData<T>().erase(entity);
        }

        if constexpr (sizeof...(Ts) > 0) {
            cleanupHelper<Ts...>(deletedData);
        }
    }

    template<typename T, typename... Ts>
    bool ComponentManager::hasAllComponents(Entity entity) const {
        bool hasT = hasComponent<T>(entity);

        if constexpr (sizeof...(Ts) > 0) {
            return hasT && hasAllComponents<Ts...>(entity);
        } else {
            return hasT;
        }
    }
}

#endif
