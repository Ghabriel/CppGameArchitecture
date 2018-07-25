#ifndef LOAD_INPUT_TRACKER_HPP
#define LOAD_INPUT_TRACKER_HPP

#include <fstream>
#include <string>
#include <unordered_map>
#include "engine/input-system/include.hpp"
#include "engine/resource-system/json/include.hpp"
#include "RawInputSFML.hpp"

engine::inputsystem::InputTracker loadInputTracker(
    const std::string& controlsFileName
) {
    using namespace engine::inputsystem;
    std::ifstream controlsFile(controlsFileName);

    auto keyMappingJson = parseJSON(controlsFile);
    std::unordered_map<KeyboardKey, GameKey> keyMapping;

    for (const auto& [keyboardKey, gameKey] : keyMappingJson.asIterableMap()) {
        keyMapping.insert({
            keyFromString(keyboardKey),
            gameKey.get<std::string>()
        });
    }

    return InputTracker(std::make_unique<RawInputSFML>(), keyMapping);
}

#endif
