#ifndef WAVEFORMVIEWER_H_INCLUDED
#define WAVEFORMVIEWER_H_INCLUDED

#include "Playhead.h"
#include "AudioPlayer.h"

class WaveformViewer : public Component
{
public:
    WaveformViewer (AudioPlayer* player, File& file);
    ~WaveformViewer();

    void paint (Graphics&) override;
    void resized() override;

private:
    AudioThumbnailCache cache;
    AudioThumbnail waveform;

    Playhead playhead;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformViewer)
};

#endif //WAVEFORMVIEWER_H_INCLUDED