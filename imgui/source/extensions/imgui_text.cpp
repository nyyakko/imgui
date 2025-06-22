#include "extensions/imgui_text.hpp"

#include "imgui.hpp"

#include <ranges>

std::vector<std::string> SplitToWidth(const char* text, int width)
{
    std::vector<std::string> result {};

    result.push_back("");

    for (auto const& word : std::string(text) | std::views::split(' '))
    {
        auto wordString = std::string(word.begin(), word.end());

        if (ImGui::CalcTextSize((result.back() + " " + wordString).data()).x >= width)
        {
            result.push_back("");
        }

        result.back() += wordString;
        result.back() += " ";
    }

    return result;
}

std::string JoinLines(std::vector<std::string> const& lines, std::string_view separator = "\n")
{
    std::string result;
    for (auto const& line : lines) result += line + separator.data();
    return result;
}
