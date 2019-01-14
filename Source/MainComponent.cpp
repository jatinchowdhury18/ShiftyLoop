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
    transportSource.start();

    setAudioChannels (0, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    transportSource.getNextAudioBlock (bufferToFill);
}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void MainComponent::releaseResources()
{
    transportSource.releaseResources();
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
