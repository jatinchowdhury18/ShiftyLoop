#include "AudioPlayer.h"

AudioPlayer::AudioPlayer (File& file)
{
    formatManager.registerBasicFormats();

    auto* reader = formatManager.createReaderFor (file);
    readerSource.reset (new AudioFormatReaderSource (reader, true));

    transportSource.setSource (readerSource.get(), 0, nullptr, reader->sampleRate);
    transportSource.setLooping (true);
    transportSource.setPosition (0.0);

    setAudioChannels (0, 2);
}

AudioPlayer::~AudioPlayer()
{
    audioSourcePlayer.setSource (nullptr);
    deviceManager.removeAudioCallback (&audioSourcePlayer);
    deviceManager.closeAudioDevice();
}

void AudioPlayer::setAudioChannels (int numInputChannels, int numOutputChannels)
{
    String audioError;
    audioError = deviceManager.initialise (numInputChannels, numOutputChannels, nullptr, true);

    jassert (audioError.isEmpty());

    deviceManager.addAudioCallback (&audioSourcePlayer);
    audioSourcePlayer.setSource (this);
}

void AudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    if (playState == Stopped)
        return;

    transportSource.getNextAudioBlock (bufferToFill);

    if (playState == Starting)
    {
        bufferToFill.buffer->applyGainRamp (0, bufferToFill.numSamples, 0.0f, 1.0f);
        changePlayState (Playing);
    }
    else if (playState == Stopping)
    {
        bufferToFill.buffer->applyGainRamp (0, bufferToFill.numSamples, 1.0f, 0.0f);
        changePlayState (Stopped);
    }

}

void AudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void AudioPlayer::releaseResources()
{
    transportSource.releaseResources();
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
    switch (playState)
    {
    case Stopped:
        transportSource.stop();
        transportSource.setPosition (0.0);
        return;

    case Starting:
        transportSource.start();
        return;

    case Playing:
        return;

    case Stopping:
        return;
    }
}
