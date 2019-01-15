#include "AudioPlayer.h"

AudioPlayer::AudioPlayer (File& file) : ProcessorBase (String ("Audio Player"))
{
    formatManager.registerBasicFormats();

    reader.reset (formatManager.createReaderFor (file.createInputStream()));

    setPlayConfigDetails (0, 2, getSampleRate(), getBlockSize());
}

AudioPlayer::~AudioPlayer()
{
}

void AudioPlayer::processBlock (AudioBuffer<float>& buffer, MidiBuffer& /*midiBuffer*/)
{
    if (playState == Stopped)
    {
        buffer.clear();
        return;
    }

    const auto numSamples = buffer.getNumSamples();

    if (readerStartSample + numSamples <= reader->lengthInSamples)
    {
        reader->read (&buffer, 0, numSamples, readerStartSample, true, true);
        readerStartSample += numSamples;
    }
    else //loop
    {
        auto samplesUnder = reader->lengthInSamples - readerStartSample;
        reader->read (&buffer, 0, (int) samplesUnder, readerStartSample, true, true);
        reader->read (&buffer, (int) samplesUnder, numSamples - (int) samplesUnder, 0, true, true);
        readerStartSample = numSamples - samplesUnder;
    }

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
