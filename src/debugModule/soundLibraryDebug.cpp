#ifdef DEBUG

#include "soundLibraryDebug.h"
#include "cocos2d.h"
#include "ImGuiEXT/imgui/imgui.h"
#include "ImGuiEXT/imgui/imgui_internal.h"
#include "common/audioModule/audioEngine.h"
#include "common/utilityModule/stringUtility.h"

using namespace mb::debugProfile;


soundLibraryDebug::soundLibraryDebug() {}
soundLibraryDebug::~soundLibraryDebug() {}

soundLibraryDebug* currentSoundDebugger = nullptr;

soundLibraryDebug& soundLibraryDebug::getInstance() {
    if (currentSoundDebugger == nullptr) {
        currentSoundDebugger = new soundLibraryDebug();
    }
    return *currentSoundDebugger;
}

void soundLibraryDebug::update() {
    if (ImGui::Button("Sound library")) {
        isOpened = !isOpened;
    }
    if (isOpened) {
        soundWindow(&isOpened);
    }
}

void soundLibraryDebug::soundWindow(bool* windowOpened) {
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(400), static_cast<float>(350)),
                             ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Sound library", reinterpret_cast<bool*>(windowOpened))) {
        ImGui::End();
        return;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

    ImGui::BeginChild("sounds");
    ImGui::PushID("sounds");

    auto effects = GET_AUDIO_ENGINE().getAllSounds();
    static int currentEffect = 0;
    int countEffects = 0;
    std::vector<std::string> list;
    std::string text;
    std::for_each(effects.begin(), effects.end(), [&text, &list, &countEffects](const auto& effect) {
        text += text.empty() ? effect.first.c_str() : STRING_FORMAT("\0%s", effect.first.c_str());
        list.push_back(effect.first);
        countEffects++;
    });
    const char* items = text.c_str();

    ImGui::Combo("Effects", &currentEffect, items);
    if (ImGui::Button("Play effect") && currentEffect >= 0 && currentEffect < static_cast<int>(effects.size())) {
        GET_AUDIO_ENGINE().play(list[currentEffect]);
    }


    ImGui::PopID();
    ImGui::EndChild();

    ImGui::PopStyleVar();
    ImGui::End();
}

#endif