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

    /**
     * \brief Manages the game entities and their components.
     *
     * In the Component-Entity-System architecture, entities are simple GUIDs.
     * Components by themselves have no logic and can be bound to entities to
     * add data to them. All the game logic is implemented by systems, which
     * operate on the data bound to the entities.
     */
    class ComponentManager {
        using Deleted = __detail::Deleted;
        using DeletedData = __detail::DeletedData;
        template<typename TComponent>
        static constexpr auto entityData = __detail::entityData<TComponent>;
     public:
        /** \brief Creates a new entity without any components. */
        Entity createEntity();
        /**
         * \brief Deletes an entity, making it invisible to forEachEntity().
         * Note that the bindings between the entity and its components are NOT
         * deleted by this method, i.e the memory consumption is not affected.
         * The "actual cleanup" is performed by cleanup().
         */
        void deleteEntity(Entity);
        /**
         * \brief Removes the bindings between all deleted entities and their
         * components, effectively removing them from the system. Note that
         * deleted entities can be "brought back to life" by having components
         * added to them as usual.
         */
        template<typename T, typename... Ts>
        void cleanup();

        /**
         * \brief Removes the bindings between every entity and the input components.
         */
        template<typename T, typename... Ts>
        void clearAll();

        /**
         * \brief Adds a default-initialized T component to an entity.
         */
        template<typename T>
        void addComponent(Entity);
        /**
         * \brief Removes the T component from an entity.
         */
        template<typename T>
        void removeComponent(Entity);
        /**
         * \brief Checks if an entity has a T component.
         */
        template<typename T>
        bool hasComponent(Entity) const;
        /**
         * \brief Checks if an entity has all the input components.
         */
        template<typename T, typename... Ts>
        bool hasAllComponents(Entity) const;

        /**
         * \brief Returns the T component data of an entity. Throws if
         * the entity doesn't have the T component.
         */
        template<typename T>
        T& getData(Entity);

        /**
         * \brief Iterates over all entities that have all the input components,
         * executing a callback for each of them. The entity itself and its
         * data for all the input components is passed to the callback.
         * Note: the performance of this method increases if the least common
         * components come first in the list, especially the first.
         */
        template<typename T, typename... Ts>
        void forEachEntity(
            std::function<void(Entity, T&, std::add_lvalue_reference_t<Ts>...)> fn
        );

     private:
        size_t numDeletedEntities = 0;

        template<typename T, typename... Ts>
        void cleanupHelper(DeletedData&);
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
        DeletedData& deletedData = __detail::entityData<Deleted>();
        cleanupHelper<T, Ts...>(deletedData);
        deletedData.clear();
        numDeletedEntities = 0;
    }

    template <typename T, typename... Ts>
    void ComponentManager::clearAll() {
        entityData<T>().clear();

        if constexpr (sizeof...(Ts) > 0) {
            clearAll<Ts...>();
        }
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

    template<typename T, typename... Ts>
    bool ComponentManager::hasAllComponents(Entity entity) const {
        bool hasT = hasComponent<T>(entity);

        if constexpr (sizeof...(Ts) > 0) {
            return hasT && hasAllComponents<Ts...>(entity);
        } else {
            return hasT;
        }
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
}

#endif
