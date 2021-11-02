#include "Parser.h"
#include "Game.h"
#include "../Components/RenderModelComponent.h"
#include "../pch.h"

namespace Clumsy {
    std::string path = "../Clumsy/src/models/";
    void Parser::SceneParser(std::string name)
    {
        YAML::Node config = YAML::LoadFile(name);
        const YAML::Node& prefabs = config["PrefabInstance"];
        for (YAML::const_iterator it = prefabs.begin(); it != prefabs.end(); ++it) {
            const YAML::Node& prefab = *it;
            Transform transform;
            if (prefab["propertyPath"].as<std::string>() == "m_LocalPosition.x") {
                transform.SetPosX(prefab["value"].as<float>());
            }
            if (prefab["propertyPath"].as<std::string>() == "m_LocalPosition.y") {
                transform.SetPosY(prefab["value"].as<float>());
            }
            if (prefab["propertyPath"].as<std::string>() == "m_LocalPosition.z") {
                transform.SetPosZ(prefab["value"].as<float>());
            }
            if (prefab["propertyPath"].as<std::string>() == "m_LocalRotation.x") {
                transform.SetRotX(prefab["value"].as<float>());
            }
            if (prefab["propertyPath"].as<std::string>() == "m_LocalRotation.y") {
                transform.SetRotY(prefab["value"].as<float>());
            }
            if (prefab["propertyPath"].as<std::string>() == "m_LocalRotation.z") {
                transform.SetRotZ(prefab["value"].as<float>());
            }
            if (prefab["propertyPath"].as<std::string>() == "m_LocalRotation.w") {
                transform.SetRotW(prefab["value"].as<float>());
            }
            if (prefab["propertyPath"].as<std::string>() == "m_Name") {
                Model m1(path + prefab["value"].as<std::string>());

                //Game::AddToScene((new Clumsy::GameObject(transform))->AddComponent(new Clumsy::RenderModelComponent(m1, transform)));
            }
            

        }
    }

}