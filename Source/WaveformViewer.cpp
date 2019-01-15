#include "WaveformViewer.h"

WaveformViewer::WaveformViewer (AudioPlayer* player, File& file) : 
    cache (5),
    waveform (512, player->getFormatManager(), cache),
    playhead (player)
{
    waveform.setSource (new FileInputSource (file));
    setBounds (0, 0, 600, 400);

    addAndMakeVisible (playhead);
}

WaveformViewer::~WaveformViewer()
{
    waveform.clear();
    cache.clear();
    waveform.setReader (nullptr, 0);
}

void WaveformViewer::paint (Graphics& g)
{
    g.setColour (Colours::indianred);
    waveform.drawChannels (g, getBounds(), 0, waveform.getTotalLength(), 1.0f);
}

void WaveformViewer::resized()
{
}
