#pragma once
#include <string>
#include <vector>
#include "imgui.h"
bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f);
bool DrawCombo(const std::string& inNameCombo, const std::vector<std::string>& values, std::string& currentItem, size_t* index);
bool DrawCombo(const std::string& inNameCombo, const std::vector<std::string>& values, size_t* index);
bool TreeNodeWithIcon(const char* label, ImTextureID inImage);
