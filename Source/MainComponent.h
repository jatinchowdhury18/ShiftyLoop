#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioPlayer.h"

class MainComponent   : public Component
{
public:
    MainComponent();
    ~MainComponent();

    bool keyPressed (const KeyPress& key) override;

    void paint (Graphics&) override;
    void resized() override;

private:
    std::unique_ptr<AudioPlayer> player;

    AudioThumbnailCache cache;
    std::unique_ptr<AudioThumbnail> waveform;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
