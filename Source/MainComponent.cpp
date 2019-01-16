#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize (600, 400);

    /*
    const File rootDir = File::getCurrentWorkingDirectory().getParentDirectory().getParentDirectory();
    const File loopsDir = File (rootDir.getChildFile ("Loops"));
    File loop = File (loopsDir.getChildFile ("Phil_Selway.wav"));
    */
    MemoryInputStream* loop = new MemoryInputStream (BinaryData::Phil_Selway_wav, BinaryData::Phil_Selway_wavSize, false);

    player = new AudioPlayer (loop);
    output.reset (new OutputGraph (player));

    setWantsKeyboardFocus (true);

    waveformView.reset (new WaveformViewer (player));
    addAndMakeVisible (waveformView.get());
}

MainComponent::~MainComponent() {}

bool MainComponent::keyPressed (const KeyPress& key)
{
    if (key == KeyPress::spaceKey)
        player->togglePlay();

    return true;
}

void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::antiquewhite);

    //g.setFont (Font (16.0f));
    //g.setColour (Colours::white);
    //g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);

    waveformView->repaint();
}

void MainComponent::resized()
{
    //waveformView->setBounds (0, 0, 600, 400);
}
