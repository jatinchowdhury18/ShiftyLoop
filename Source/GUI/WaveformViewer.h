#ifndef WAVEFORMVIEWER_H_INCLUDED
#define WAVEFORMVIEWER_H_INCLUDED

#include "Playhead.h"
#include "MarkerView.h"
#include "../Processors/AudioPlayer.h"

class WaveformViewer : public Component
{
public:
    WaveformViewer (AudioPlayer* player);
    ~WaveformViewer();

    void paint (Graphics&) override;
    void resized() override;

private:
    AudioThumbnailCache cache;
    AudioThumbnail waveform;

    Playhead playhead;
    MarkerView marker;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformViewer)
};

#endif //WAVEFORMVIEWER_H_INCLUDED
