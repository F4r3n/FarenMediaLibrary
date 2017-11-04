#include "Components/CTransform.h"
#include "Entity.h"
#include <EntityManager.h>
//#include "TransformInspector.h"
using namespace fmc;
const std::string CTransform::name = "Transform";

CTransform::CTransform() {
    _name = "Transform";
    //inspector = new gui::TransformInspector();
}

CTransform::CTransform(const fm::math::Vector2f& position,
                       const fm::math::Vector2f& scale,
                       const float& rotation,
                       const int& layer)
    :  position(position), scale(scale), rotation(rotation), layer(layer) {
        _name = "Transform";
    //inspector = new gui::TransformInspector();

}

fm::math::Vector2f CTransform::getWorldPos() {
    Entity* father = EntityManager::get().getEntity(idFather);
    if(!father)
        return position;

    CTransform* fatherTransform = father->get<CTransform>();
    if(!fatherTransform)
        return position;
    return position + fatherTransform->getWorldPos();
}

fm::math::Vector2f CTransform::getWorldPos(EntityManager& manager) {
    Entity* father = manager.getEntity(idFather);
    if(!father)
        return position;

    CTransform* fatherTransform = father->get<CTransform>();
    if(!fatherTransform)
        return position;
    return position + fatherTransform->getWorldPos();
}

void CTransform::setFather(Entity* e) {
    setFather(e->ID);
}

void CTransform::serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) {
    writer.Key(name.c_str());
    writer.StartArray();
    writer.Double(position.x);
    writer.Double(position.y);
    writer.Double(scale.x);
    writer.Double(scale.y);
    writer.Int(layer);
    writer.EndArray();
}

void CTransform::parse(rapidjson::Value& value) {
    position.x = value[name.c_str()][0].GetDouble();
    position.y = value[name.c_str()][1].GetDouble();
    scale.x = value[name.c_str()][2].GetDouble();
    scale.y = value[name.c_str()][3].GetDouble();
    layer = value[name.c_str()][4].GetInt();

    std::cout << position << " " << scale << " " << layer << std::endl;
}