#pragma once

typedef int ImGuiToastDirection;

namespace ImGui {

void PushToast(const char* title, const char* content);
void RenderToasts();

}
