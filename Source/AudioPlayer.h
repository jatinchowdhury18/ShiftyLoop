#ifndef AUDIOPLAYHER_H_INCLUDED
#define AUDIOPLAYHER_H_INCLUDED

#include "JuceHeader.h"

enum PlayState
{
    Stopped,
    Starting,
    Playing,
    Stopping,
};

class AudioPlayer : public AudioSource
{
public:
    AudioPlayer (File& file);
    ~AudioPlayer();

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void releaseResources() override;

    void changePlayState (PlayState newState);

    AudioFormatManager& getFormatManager() { return formatManager; }
    void togglePlay();

private:
    AudioFormatManager formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;
    PlayState playState = Stopped;

    AudioDeviceManager deviceManager;
    AudioSourcePlayer audioSourcePlayer;

    void setAudioChannels (int numInputChannels, int numOutputChannels);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlayer)
};

#endif //AUDIOPLAYHER_H_INCLUDED
