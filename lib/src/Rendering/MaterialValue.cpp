#include "Rendering/MaterialValue.h"

namespace fm {


void to_json(nlohmann::json& j, const TextureMat& p) {
        j["texture"] = p.texture;
        j["position"] = p.position;
}

void from_json(const nlohmann::json& j, TextureMat& p) {
        p.texture = j["texture"];
        p.position = j["position"];
}
}
