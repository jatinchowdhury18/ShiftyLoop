#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize (600, 400);

    formatManager.registerBasicFormats();

    const File rootDir = File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory();
    const File loopsDir = File (rootDir.getChildFile ("Loops"));
    const File loop = File (loopsDir.getChildFile ("drums.wav"));

    auto* reader = formatManager.createReaderFor (loop);
    readerSource.reset (new AudioFormatReaderSource (reader, true));
    transportSource.setSource (readerSource.get(), 0, nullptr, reader->sampleRate);
    transportSource.setLooping (true);
    transportSource.setPosition (0.0);

    setWantsKeyboardFocus (true);

    setAudioChannels (0, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
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

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void MainComponent::releaseResources()
{
    transportSource.releaseResources();
}

bool MainComponent::keyPressed (const KeyPress& key)
{
    if (key == KeyPress::spaceKey)
    {
        if (playState == Stopped || playState == Stopping)
            changePlayState (Starting);

        else if (playState == Starting || playState == Playing)
            changePlayState (Stopping);
    }

    return true;
}

void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setFont (Font (16.0f));
    g.setColour (Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

void MainComponent::changePlayState (PlayState newState)
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
