#ifndef REGISTRY_H
#define REGISTRY_H

#include <unordered_map>
#include <string>
#include <optional>
#include <vector>
#include <iostream>

template<typename T>
class Registry {
public:
    Registry(const T& default_obj) {
        def_obj=default_obj;
    }

    void registerObject(const std::string& name, const T& object) {
        objects[name] = object;
        keys.push_back(name);
    }

    const T& getObject(const std::string& name) const {
        auto it = objects.find(name);
        if (it==objects.end()) {
            return def_obj;
        }
        return it->second;
    }

    const T& getObject(size_t index) const {
        if (index>keys.size()) {
            return def_obj;
        }
        return getObject(keys[index]);
    }

    void clear() {
        objects.clear();
        keys.clear();
    }

    size_t size() const {
        return keys.size();
    }

private:
    std::unordered_map<std::string, T> objects;
    std::vector<std::string> keys;
    T def_obj;
};

#endif
