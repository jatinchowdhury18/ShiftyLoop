#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioPlayer.h"
#include "WaveformViewer.h"

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

    std::unique_ptr<WaveformViewer> waveformView;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
