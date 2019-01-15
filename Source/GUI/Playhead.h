#ifndef PLAYHEAD_H_INCLUDED
#define PLAYHEAD_H_INCLUDED

#include "../Processors/AudioPlayer.h"

class Playhead : public Component,
                 private Timer
{
public:
    Playhead (AudioPlayer* player) : player (player)
    {
        startTimer (40);
        setBounds (0, 0, 600, 400);
    }

    void timerCallback() override
    {
        repaint();
    }

    void paint (Graphics& g) override
    {
        g.setColour (Colours::forestgreen);

        auto posFraction = (float) player->getPlayheadPosition() / (float) player->getAudioLength();
        auto pos = (float) (posFraction * getWidth() + getX());
        g.drawLine (pos, (float) getY(), pos, (float) getBottom(), 2.0f);
    }

private:
    AudioPlayer* player;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Playhead)
};

#endif //PLAYHEAD_H_INCLUDED
