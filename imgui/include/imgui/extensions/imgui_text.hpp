#pragma once

#include <vector>
#include <string>
#include <string_view>

namespace ImGui {

std::vector<std::string> SplitToWidth(const char* text, int width);
std::string JoinLines(std::vector<std::string> const& lines, std::string_view separator = "\n");

}
