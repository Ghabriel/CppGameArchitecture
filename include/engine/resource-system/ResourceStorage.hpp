#ifndef RESOURCE_STORAGE_HPP
#define RESOURCE_STORAGE_HPP

#include <string>

namespace engine::resourcesystem {
    namespace __detail {
        template<typename Key, typename T>
        T& typeMap() {
            static T value;
            return value;
        }

        template<typename TResource>
        using ResourceDataStorage = std::unordered_map<std::string, TResource>;

        template<typename TResource>
        ResourceDataStorage<TResource>& resourceData() {
            return typeMap<TResource, ResourceDataStorage<TResource>>();
        }
    }

    class ResourceStorage {
    public:
        template<typename T>
        void store(const std::string& identifier, const T& data);

        template<typename T>
        T& get(const std::string& identifier) const;
    };

    template<typename T>
    void ResourceStorage::store(const std::string& identifier, const T& data) {
        __detail::resourceData<T>().insert({identifier, data});
    }

    template<typename T>
    T& ResourceStorage::get(const std::string& identifier) const {
        return __detail::resourceData<T>().at(identifier);
    }
}

#endif
