#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class MainComponent   : public AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent();

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;

    void paint (Graphics&) override;
    void resized() override;

private:
    AudioFormatManager formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
