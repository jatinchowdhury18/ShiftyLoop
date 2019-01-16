#ifndef AUDIOPLAYHER_H_INCLUDED
#define AUDIOPLAYHER_H_INCLUDED

#include "ProcessorBase.h"
#include "Looper.h"

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
    AudioPlayer (MemoryInputStream* input);
    ~AudioPlayer();
    void initLooper();

    void processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiBuffer) override;
    void prepareToPlay (double sampleRate, int samplesPerBlockExpected) override;
    void releaseResources() override;

    void processLoop (AudioBuffer<float>& buffer, int numSamples, const int64 loopStartSample, const int64 loopEndSample);
    void processStartStop (AudioBuffer<float>& buffer, int numSamples);

    void changePlayState (PlayState newState);

    int64 getPlayheadPosition() { return readerStartSample; }
    int64 getAudioLength() { return reader->lengthInSamples; }

    int64 getLoopStart() { return loopStart; }
    int64 getLoopEnd() { return loopEnd; }

    /** If sample is less than one set marker to default value */
    void setLoopMarker (int64 sample, bool isStart);

    AudioFormatManager& getFormatManager() { return formatManager; }
    void togglePlay();

    AudioFormatReader* getReader() { return reader; }

private:
    AudioFormatManager formatManager;
    AudioFormatReader* reader;

    PlayState playState = Stopped;

    int64 readerStartSample = 0;
    int64 loopStart;
    int64 loopEnd;
    bool updateLoop;

    std::unique_ptr<Looper> looper;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlayer)
};

#endif //AUDIOPLAYHER_H_INCLUDED
