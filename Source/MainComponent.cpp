#include "MainComponent.h"

MainComponent::MainComponent() : 
    cache (5)
{
    setSize (600, 400);

    const File rootDir = File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory();
    const File loopsDir = File (rootDir.getChildFile ("Loops"));
    File loop = File (loopsDir.getChildFile ("drums.wav"));

    player.reset (new AudioPlayer (loop));

    setWantsKeyboardFocus (true);

    waveform.reset (new AudioThumbnail (512, player->getFormatManager(), cache));
    waveform->setSource (new FileInputSource (loop));
}

MainComponent::~MainComponent()
{
    waveform->clear();
    cache.clear();
    waveform->setReader (nullptr, 0);
}

bool MainComponent::keyPressed (const KeyPress& key)
{
    if (key == KeyPress::spaceKey)
        player->togglePlay();

    return true;
}

void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::transparentWhite);

    //g.setFont (Font (16.0f));
    //g.setColour (Colours::white);
    //g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);

    waveform->drawChannels (g, Rectangle<int> (getWidth(), getHeight()), 0, waveform->getTotalLength(), 1.0f);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
