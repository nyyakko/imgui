#pragma once

typedef int ImGuiToastDirection;

enum ImGuiToastDirection_
{
    ImGuiToastDirection_TopLeft,
    ImGuiToastDirection_BottomLeft,
    ImGuiToastDirection_TopRight,
    ImGuiToastDirection_BottomRight,
};

namespace ImGui {

void PushToast(const char* title, const char* content);
void RenderToasts(ImGuiToastDirection direction = ImGuiToastDirection_TopRight);

}
