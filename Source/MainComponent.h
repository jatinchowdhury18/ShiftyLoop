#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

enum PlayState
{
    Stopped,
    Starting,
    Playing,
    Stopping,
};

class MainComponent   : public AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent();

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;

    bool keyPressed (const KeyPress& key) override;

    void paint (Graphics&) override;
    void resized() override;

private:
    AudioFormatManager formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    PlayState playState = Stopped;

    void changePlayState (PlayState newState);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
