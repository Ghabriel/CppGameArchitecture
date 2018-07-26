#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include <SFML/Audio.hpp>
#include "../engine/entity-system/include.hpp"
#include "../engine/resource-system/ResourceStorage.hpp"
#include "../engine/state-system/State.hpp"
#include "../engine/utils/print-fps.hpp"
#include "../engine/utils/xtrace.hpp"
#include "../GameCoreData.hpp"

class MenuState : public engine::statesystem::State {
    using Entity = engine::entitysystem::Entity;
    using ResourceStorage = engine::resourcesystem::ResourceStorage;
 public:
    MenuState(GameCoreData& gameData) : gameData(gameData) { }

 private:
    void executeImpl() override {
        engine::utils::printFPS<1>("Menu Update Rate", 50);
    }

    void onEnterImpl() override {
        ResourceStorage& storage = *gameData.resourceStorage;
        auto bgm = storage.get<std::shared_ptr<sf::Music>>("bgm-littleroot-town");
        bgm->play();
    }

    void onExitImpl() override {
        ResourceStorage &storage = *gameData.resourceStorage;
        auto bgm = storage.get<std::shared_ptr<sf::Music>>("bgm-littleroot-town");
        bgm->stop();
    }

    GameCoreData& gameData;
    Entity environment;
};

#endif
