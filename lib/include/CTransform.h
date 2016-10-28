#pragma once
#include <Component.h>
#include "Vector2.h"
#include "EntityManager.h"

#ifdef GUI
#include <imgui.h>
#endif

class Entity;

namespace fmc {
class CTransform : public Component<CTransform> {
public:
    CTransform(const fm::Vector2f& position, const fm::Vector2f& scale, const float& rotation, const int& layer = 1);
    CTransform();
    ~CTransform() {
    }

    void setFather(size_t id) {
        this->idFather = id;
    }

    void setFather(Entity* e);

    fm::Vector2f getWorldPos();
    fm::Vector2f getWorldPos(EntityManager& manager);

    fm::Vector2f position = { 0, 0 };

    size_t idFather = std::numeric_limits<size_t>::max();

    fm::Vector2f scale = { 100, 100 };
    float rotation = 0;
    int layer = 1;

#ifdef GUI
    void display(bool *value) {
        if(ImGui::CollapsingHeader("Transform")) {
            ImGui::PushItemWidth(100);
            ImGui::DragFloat2("Position", &position.x, 0.02f, -FLT_MAX, FLT_MAX, NULL, 2.0f);
            ImGui::DragFloat2("Size", &scale.x, 0.02f, -FLT_MAX, FLT_MAX, NULL, 2.0f);
            ImGui::DragInt("Layer", &layer, 1, 0, 99);
            ImGui::PopItemWidth();
        }
    }
#endif
};
}