#ifndef AUDIOPLAYHER_H_INCLUDED
#define AUDIOPLAYHER_H_INCLUDED

#include "ProcessorBase.h"

enum PlayState
{
    Stopped,
    Starting,
    Playing,
    Stopping,
};

class AudioPlayer : public ProcessorBase
{
public:
    AudioPlayer (File& file);
    ~AudioPlayer();

    void processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiBuffer) override;
    void prepareToPlay (double sampleRate, int samplesPerBlockExpected) override;
    void releaseResources() override;

    void changePlayState (PlayState newState);

    int64 getPlayheadPosition() { return readerStartSample; }
    int64 getAudioLength() { return reader->lengthInSamples; }

    int64 getLoopStart() { return loopStart; }
    int64 getLoopEnd() { return loopEnd; }

    /** If sample is less than one set marker to default value */
    void setLoopMarker (int64 sample, bool isStart);

    AudioFormatManager& getFormatManager() { return formatManager; }
    void togglePlay();

private:
    AudioFormatManager formatManager;
    std::unique_ptr<AudioFormatReader> reader;

    PlayState playState = Stopped;

    int64 readerStartSample = 0;
    int64 loopStart = 6000;
    int64 loopEnd = 100000;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlayer)
};

#endif //AUDIOPLAYHER_H_INCLUDED
