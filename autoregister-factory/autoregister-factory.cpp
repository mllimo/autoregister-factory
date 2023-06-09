#include <iostream>
#include <functional>
#include <unordered_map>
#include <memory>

template <typename Key, typename Object>
class InterfaceFactory {
public:
    template <typename ObjectType>
    void Add(const Key& key) {
        map_.emplace(key, []() { return std::make_unique<ObjectType>(); });
    }

    std::unique_ptr<Object> Get(const Key& key) {
        auto it = map_.find(key);
        if (it == map_.end()) return nullptr;
        return it->second();
    }

private:
    std::unordered_map<Key, std::function<std::unique_ptr<Object>(void)>> map_;
};

template <typename Key, typename Object>
class SingletonIFactory {
public:
    static InterfaceFactory<Key, Object>& Get() {
        static InterfaceFactory<Key, Object> ifactory;
        return ifactory;
    }
private:
    SingletonIFactory() = default;
};

template <typename Key, typename SpecialicedObject, typename BaseObject>
class AutoRegisterFactory {
public:
    AutoRegisterFactory(const Key& key) {
        SingletonIFactory<Key, BaseObject>::Get().Add<SpecialicedObject>(key);
    }
};

class Animal {
public:
    virtual void Say() = 0;
};

class Dog : public Animal {
public:
    void Say() override { std::cout << "Guau"; };
};

namespace DogAutoRegister {
    AutoRegisterFactory<std::string, Dog, Animal> auto_register("dog");
}

class Cat : public Animal {
public:
    void Say() override { std::cout << "Miau"; };
};

namespace CatAutoRegister {
    AutoRegisterFactory<std::string, Cat, Animal> auto_register("cat");
}

int main() {
    auto ptr = SingletonIFactory<std::string, Animal>::Get().Get("dog");
    ptr->Say(); std::cout << std::endl;
    ptr = SingletonIFactory<std::string, Animal>::Get().Get("cat");
    ptr->Say(); std::cout << std::endl;
}
