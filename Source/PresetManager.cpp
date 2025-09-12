#include "PresetManager.h"

PresetManager::PresetManager(juce::AudioProcessorValueTreeState& apvts) : valueTreeState(apvts) {}

void PresetManager::savePreset(const juce::String& presetName)
{
    auto presetDirectory = getPresetDirectory();
    if (!presetDirectory.exists()) {
        presetDirectory.createDirectory();
    }
    
    auto presetFile = presetDirectory.getChildFile(presetName + ".xml");
    auto state = valueTreeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    xml->writeTo(presetFile);
    
    currentPresetName = presetName;
}

void PresetManager::loadPreset(const juce::String& presetName)
{
    auto presetFile = getPresetDirectory().getChildFile(presetName + ".xml");
    if (presetFile.existsAsFile()) {
        std::unique_ptr<juce::XmlElement> xml(juce::XmlDocument::parse(presetFile));
        if (xml != nullptr) {
            valueTreeState.replaceState(juce::ValueTree::fromXml(*xml));
            currentPresetName = presetName;
        }
    }
}

juce::StringArray PresetManager::getAllPresets() const
{
    juce::StringArray presets;
    auto presetDirectory = getPresetDirectory();
    
    if (presetDirectory.exists()) {
        auto files = presetDirectory.findChildFiles(juce::File::findFiles, false, "*.xml");
        for (const auto& file : files) {
            presets.add(file.getFileNameWithoutExtension());
        }
    }
    
    presets.sort();
    return presets;
}

const juce::File PresetManager::getPresetDirectory()
{
    return juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
        .getChildFile("Successor37")
        .getChildFile("Presets");
}