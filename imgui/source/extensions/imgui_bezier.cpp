#define IMGUI_DEFINE_MATH_OPERATORS
#include "extensions/imgui_bezier.hpp"

#include "imgui.hpp"
#include "imgui_internal.hpp"

static auto constexpr SMOOTHNESS = 64;
static auto constexpr CURVE_WIDTH = 3;
static auto constexpr GRABBER_LINE_WIDTH = 1;
static auto constexpr GRABBER_RADIUS = 6;

static void RenderGrid(ImDrawList* const drawList, ImVec2 size, ImRect frame)
{
    static const ImColor COLOR = ImGui::GetColorU32(ImGuiCol_FrameBg);
    for (size_t i = 0; i <= size.x; i += size.x / 4) drawList->AddLine({ frame.Min.x + i, frame.Min.y }, { frame.Min.x + i, frame.Max.y }, COLOR);
    for (size_t i = 0; i <= size.y; i += size.y / 4) drawList->AddLine({ frame.Min.x, frame.Min.y + i }, { frame.Max.x, frame.Min.y + i }, COLOR);
}

static void RenderBezier(ImDrawList* const drawList, ImRect frame, float anchors[4])
{
    static const ImColor COLOR = ImGui::GetStyle().Colors[ImGuiCol_PlotLines];

    for (size_t i = 0; i < SMOOTHNESS; i += 1)
    {
        float tA = (i+0)/(float)SMOOTHNESS;
        float tB = (i+1)/(float)SMOOTHNESS;
        ImVec2 p (
                (0 + (3 * (1-tA)*(1-tA) * tA)*anchors[0] + (3 * (1-tA) * tA*tA)*anchors[2] + (tA*tA*tA)),
            1 - (0 + (3 * (1-tA)*(1-tA) * tA)*anchors[1] + (3 * (1-tA) * tA*tA)*anchors[3] + (tA*tA*tA))
        );
        ImVec2 q (
                (0 + (3 * (1-tB)*(1-tB) * tB)*anchors[0] + (3 * (1-tB) * tB*tB)*anchors[2] + (tB*tB*tB)),
            1 - (0 + (3 * (1-tB)*(1-tB) * tB)*anchors[1] + (3 * (1-tB) * tB*tB)*anchors[3] + (tB*tB*tB))
        );
        ImVec2 r(p.x * (frame.Max.x - frame.Min.x) + frame.Min.x, p.y * (frame.Max.y - frame.Min.y) + frame.Min.y);
        ImVec2 s(q.x * (frame.Max.x - frame.Min.x) + frame.Min.x, q.y * (frame.Max.y - frame.Min.y) + frame.Min.y);
        drawList->AddLine(r, s, COLOR, CURVE_WIDTH);
    }
}

static bool RenderGrabbers(ImDrawList* const drawList, ImRect frame, ImVec2 size, const char* label, float anchors[4])
{
    static const ImColor GRABBER_LINE_COLOR = ImGui::GetStyle().Colors[ImGuiCol_Border];
    static const ImColor GRABBER_COLOR = ImGui::GetStyle().Colors[ImGuiCol_SliderGrab];
    static const ImColor GRABBER_ACTIVE_COLOR = ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive];

    bool changed = false;

    for (size_t i = 0; i < 2; i += 1)
    {
        ImVec2 p1 = i == 0 ? ImVec2(frame.Min.x, frame.Max.y) : ImVec2(frame.Max.x, frame.Min.y);
        ImVec2 p2 = ImVec2(anchors[i * 2 + 0], 1 - anchors[i * 2 + 1]) * (frame.Max - frame.Min) + frame.Min;
        ImGui::SetCursorScreenPos(p2 - ImVec2(GRABBER_RADIUS, GRABBER_RADIUS));
        char anchorTooltip[256] {};
        snprintf(anchorTooltip, sizeof(anchorTooltip), "%lu##%s", i, label);
        ImGui::InvisibleButton(anchorTooltip, { 2 * GRABBER_RADIUS, 2 * GRABBER_RADIUS });

        bool active = ImGui::IsItemActive() || ImGui::IsItemHovered();

        if (active)
        {
            ImGui::SetTooltip("(%4.3f, %4.3f)", anchors[i * 2 + 0], anchors[i * 2 + 1]);
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }

        if (active && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            anchors[i * 2 + 0] += ImGui::GetIO().MouseDelta.x / size.x;
            anchors[i * 2 + 0] = ImClamp(anchors[i * 2 + 0], 0.f, 1.f);
            anchors[i * 2 + 1] -= ImGui::GetIO().MouseDelta.y / size.y;
            anchors[i * 2 + 1] = ImClamp(anchors[i * 2 + 1], 0.f, 1.f);
            changed = true;
        }

        drawList->AddLine(p1, p2, GRABBER_LINE_COLOR, GRABBER_LINE_WIDTH);
        drawList->AddCircleFilled(p2, GRABBER_RADIUS, active ? GRABBER_ACTIVE_COLOR : GRABBER_COLOR);
    }

    ImGui::SetCursorScreenPos({ frame.Min.x, frame.Max.y + GRABBER_RADIUS });

    return changed;
}

bool ImGui::BezierEditor(const char* label, float* points, ImVec2 size)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems) return false;

    bool changed = false;

    BeginGroup();

    Text("%s", label);
    ImRect frame(window->DC.CursorPos, window->DC.CursorPos + size);
    ItemSize(frame);

    if (ItemAdd(frame, 0))
    {
        ImGuiStyle& style = GetStyle();
        ImDrawList* drawList = GetWindowDrawList();

        RenderFrame(frame.Min, frame.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

        RenderGrid(drawList, size, frame);
        RenderBezier(drawList, frame, points);
        changed = RenderGrabbers(drawList, frame, size, label, points);

        PushItemWidth(size.x / 2 - 3);
        for (size_t i = 0; i <= 2; i += 2)
        {
            BeginGroup();
            for (size_t j = i; j < 2 + i; j += 1)
            {
                char sliderLabel[256] {};
                snprintf(sliderLabel, sizeof(sliderLabel), "##%lu%s", j, label);
                changed |= SliderFloat(sliderLabel, &points[j], 0, 1);
            }
            EndGroup();
            SameLine();
        }
        PopItemWidth();
    }

    EndGroup();

    return changed;
}
