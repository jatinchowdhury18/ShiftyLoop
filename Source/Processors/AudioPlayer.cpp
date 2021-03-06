#include "AudioPlayer.h"

AudioPlayer::AudioPlayer (File& file) : ProcessorBase (String ("Audio Player"))
{
    formatManager.registerBasicFormats();

    reader = formatManager.createReaderFor (file.createInputStream());

    setPlayConfigDetails (0, 2, getSampleRate(), getBlockSize());

    initLooper();
}

AudioPlayer::AudioPlayer (MemoryInputStream* input) : ProcessorBase (String ("Audio Player"))
{
    formatManager.registerBasicFormats();

    reader = formatManager.createReaderFor (input);

    setPlayConfigDetails (0, 2, getSampleRate(), getBlockSize());

    initLooper();
}

AudioPlayer::~AudioPlayer()
{
}

void AudioPlayer::initLooper()
{
    looper.reset (new Looper (reader->lengthInSamples));
    looper->changeLoop (loopStart, loopEnd);
    updateLoop = false;
}

void AudioPlayer::processBlock (AudioBuffer<float>& buffer, MidiBuffer& /*midiBuffer*/)
{
    if (playState == Stopped)
    {
        buffer.clear();
        return;
    }

    const auto numSamples = buffer.getNumSamples();

    bool loopToBeginning = readerStartSample > loopEnd;
    const auto loopEndSample = loopToBeginning ? reader->lengthInSamples : loopEnd;
    const auto loopStartSample = loopToBeginning ? 0 : loopStart;

    if (readerStartSample + numSamples <= loopEndSample)
    {
        reader->read (&buffer, 0, numSamples, readerStartSample, true, true);
        readerStartSample += numSamples;

        if (updateLoop)
        {
            looper->changeLoop (loopStart, loopEnd);
            updateLoop = false;
        }
    }
    else
    {
        processLoop (buffer, numSamples, loopStartSample, loopEndSample);
        updateLoop = ! loopToBeginning;
    }

    processStartStop (buffer, numSamples);
}

void AudioPlayer::processLoop (AudioBuffer<float>& buffer, int numSamples, const int64 loopStartSample, const int64 loopEndSample)
{
    auto samplesUnder = loopEndSample - readerStartSample;
    reader->read (&buffer, 0, (int) samplesUnder, readerStartSample, true, true);
    reader->read (&buffer, (int) samplesUnder, numSamples - (int) samplesUnder, loopStartSample, true, true);
    readerStartSample = loopStartSample + numSamples - samplesUnder;
}

void AudioPlayer::processStartStop (AudioBuffer<float>& buffer, int numSamples)
{
    //Fade to start play or pause
    if (playState == Starting)
    {
        buffer.applyGainRamp (0, numSamples, 0.0f, 1.0f);
        changePlayState (Playing);
    }
    else if (playState == Stopping)
    {
        buffer.applyGainRamp (0, numSamples, 1.0f, 0.0f);
        changePlayState (Stopped);
    }
}

void AudioPlayer::setLoopMarker (int64 sample, bool isStart)
{
    auto& loopMarker = isStart ? loopStart : loopEnd;

    if (sample < 0)
    {
        loopMarker = isStart ? 0 : reader->lengthInSamples;
        return;
    }

    loopMarker = sample;
}

void AudioPlayer::prepareToPlay (double /*sampleRate*/, int /*samplesPerBlockExpected*/)
{
}

void AudioPlayer::releaseResources()
{
}

void AudioPlayer::togglePlay()
{
    if (playState == Stopped || playState == Stopping)
        changePlayState (Starting);

    else if (playState == Starting || playState == Playing)
        changePlayState (Stopping);
}

void AudioPlayer::changePlayState (PlayState newState)
{
    if (playState == newState)
        return;

    playState = newState;
    
    if (playState == Stopped) //rewind
        readerStartSample = 0;
}
