#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include "Util/AssetStore.hpp"
#include "Util/Image.hpp"
#include <memory>

class ResourceManager {
public:
    // This function returns our global, shared Image Store
    static Util::AssetStore<std::shared_ptr<Util::Image>>& GetImageStore() {
        
        // We define the 'loader' function right here! 
        // When the store needs a missing image, it runs this code.
        static Util::AssetStore<std::shared_ptr<Util::Image>> store(
            [](const std::string& path) { 
                return std::make_shared<Util::Image>(path); 
            }
        );
        
        return store;
    }
};

#endif