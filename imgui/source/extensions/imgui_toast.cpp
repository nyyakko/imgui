#define IMGUI_DEFINE_MATH_OPERATORS
#include "extensions/imgui_toast.hpp"

#include "imgui.hpp"
#include "imgui_internal.hpp"

#include <list>
#include <string>
#include <chrono>
#include <random>
#include <ranges>

using namespace std::chrono_literals;

template <class T>
T RandomInt(T min, T max)
{
    static std::random_device device;
    static std::mt19937 generator(device());
    std::uniform_int_distribution<T> distribution(min, max);
    return distribution(generator);
}

struct Toast
{
    enum class State { FADE_IN, ALIVE, FADE_OUT, DEAD };

    std::chrono::steady_clock::time_point creationTime;
    std::chrono::steady_clock::time_point deletionTime;

    size_t index;
    size_t tag;
    State state;

    std::string title;
    std::string content;
    ImVec2 position;
    ImVec2 size;
    float opacity = 0;
};

static std::list<Toast>& The_Toasts()
{
    static std::list<Toast> toasts {};
    return toasts;
}

static size_t& The_ToastCounter()
{
    static size_t counter = 0;
    return counter;
}

static inline size_t ToastCounterInc() { return The_ToastCounter()++; }
static inline size_t ToastCounterDec() { return The_ToastCounter()--; }

static std::vector<std::string> Wrap(std::string const& input, size_t width)
{
    std::vector<std::string> result {};

    result.push_back("");

    auto [fontWidth, fontHeight] = ImGui::CalcTextSize("A");

    for (auto const& word : input | std::views::split(' '))
    {
        if ((result.back().size() + word.size()) * fontWidth >= width - fontWidth)
        {
            result.push_back("");
        }

        result.back() += std::string(word.begin(), word.end());
        result.back() += " ";
    }

    return result;
}

static inline std::string JoinLines(std::vector<std::string> const& lines, std::string_view separator = "\n")
{
    std::string result;
    for (auto const& line : lines) result += line + separator.data();
    return result;
}

void ImGui::PushToast(const char* title, const char* content)
{
    auto& toasts = The_Toasts();

    Toast toast {};

    auto [fontWidth, fontHeight] = CalcTextSize("A");
    static constexpr auto TOAST_BASE_WIDTH = 300;
    static constexpr auto TOAST_BASE_HEIGHT = 45;

    toast.creationTime = std::chrono::steady_clock::now();
    toast.tag = RandomInt(0zu, std::numeric_limits<size_t>::max());
    toast.index = ToastCounterInc();
    toast.state = Toast::State::FADE_IN;
    toast.title = title;

    auto lines = Wrap(content, TOAST_BASE_WIDTH);

    toast.size = {
        TOAST_BASE_WIDTH,
        TOAST_BASE_HEIGHT + lines.size() * fontHeight
    };

    toast.content = JoinLines(lines);

    if (toasts.empty())
        toast.position = { (5 + toast.size.x), 5 };
    else
        toast.position = { (5 + toast.size.x), 5 + toasts.front().position.y + toasts.front().size.y };

    toasts.push_front(toast);
}

static void UpdateToastState(Toast& toast)
{
    auto currentTime = std::chrono::steady_clock::now();

    switch (toast.state)
    {
    case Toast::State::FADE_IN: {
        if (currentTime - toast.creationTime < 400ms)
        {
            toast.opacity += 0.1;
            toast.opacity = ImClamp(toast.opacity, 0.f, 1.f);
        }
        else
            toast.state = Toast::State::ALIVE;
        break;
    }
    case Toast::State::ALIVE: {
        if (currentTime - toast.creationTime > 3s)
        {
            toast.deletionTime = currentTime;
            toast.state = Toast::State::FADE_OUT;
        }
        break;
    }
    case Toast::State::FADE_OUT: {
        if (currentTime - toast.deletionTime > 400ms)
            toast.state = Toast::State::DEAD;
        break;
    }
    case Toast::State::DEAD: {
        break;
    }
    }
}

static void UpdateToastPosition(Toast& toast, ImGuiToastDirection direction)
{
    auto [windowWidth, windowHeight] = ImGui::GetWindowSize();

    switch (direction)
    {
    case ImGuiToastDirection_TopLeft: {
        ImGui::SetNextWindowPos({ toast.position.x - toast.size.x, toast.position.y });
        break;
    }
    case ImGuiToastDirection_BottomLeft: {
        ImGui::SetNextWindowPos({ toast.position.x - toast.size.x, windowHeight - toast.size.y - toast.position.y });
        break;
    }
    case ImGuiToastDirection_TopRight: {
        ImGui::SetNextWindowPos({ windowWidth - toast.position.x, toast.position.y });
        break;
    }
    case ImGuiToastDirection_BottomRight: {
        ImGui::SetNextWindowPos({ windowWidth - toast.position.x, windowHeight - toast.size.y - toast.position.y });
        break;
    }
    }
}

void ImGui::RenderToasts(ImGuiToastDirection direction)
{
    auto& toasts = The_Toasts();

    if (!toasts.empty() && toasts.back().state == Toast::State::DEAD)
    {
        auto deadToast = toasts.back();
        toasts.pop_back();
        ToastCounterDec();

        for (auto& toast : toasts)
        {
            toast.position.y -= (5 + deadToast.size.y);
        }
    }

    for (auto& toast : toasts)
    {
        UpdateToastState(toast);
        UpdateToastPosition(toast, direction);
        SetNextWindowBgAlpha(toast.opacity);
        SetNextWindowSize(toast.size);
        char toastLabel[256] = {};
        snprintf(toastLabel, sizeof(toastLabel), "##%s%zu", toast.title.data(), toast.tag);
        Begin(toastLabel, nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
            BringWindowToDisplayFront(GetCurrentWindow());
            Text("%s", toast.title.data());
            Separator();
            Text("%s", toast.content.data());
        End();
    }
}

