#include "WaveformViewer.h"

WaveformViewer::WaveformViewer (AudioPlayer* player) : 
    cache (5),
    waveform (512, player->getFormatManager(), cache),
    playhead (player),
    marker (player)
{
    waveform.setReader (player->getReader(), 0x2345);
    setBounds (0, 0, 600, 400);

    addAndMakeVisible (playhead);

    addAndMakeVisible (marker);
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
