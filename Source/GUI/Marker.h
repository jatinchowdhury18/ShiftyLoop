#ifndef MARKER_H_INCLUDED
#define MARKER_H_INCLUDED

#include "../Processors/AudioPlayer.h"

enum MarkerType
{
    LoopStart,
    LoopEnd,
};

struct Mark
{
    int64 markPos;
    MarkerType type;

    bool isOver = false;
    bool isDragging = false;

    void paint (Graphics& g, AudioPlayer* player, Rectangle<float> bounds)
    {
        float pos = 0.0f;
        if (! isDragging)
        {
            auto playerPosition = type == LoopStart ? player->getLoopStart() : player->getLoopEnd();
            auto posFraction = (float) playerPosition / (float) player->getAudioLength();
            pos = posFraction * bounds.getWidth() + bounds.getX();
            markPos = roundToInt (pos);
        }
        else
            pos = (float) markPos;

        g.drawLine (pos, bounds.getY(), pos, bounds.getBottom(), 2.0f);
    }

    bool mouseOver (const MouseEvent& event)
    {
        const int r = 10;
        if (Range<int> ((int) markPos - r, (int) markPos + r).contains (event.x))
            isOver = true;
        else
            isOver = false;

        return isOver;
    }

    void dragging (const MouseEvent& event, AudioPlayer* player)
    {
        isDragging = true;
        markPos = event.x;

        player->setLoopMarker (-1, type == LoopStart);
    }

    void endDrag (const MouseEvent& event, AudioPlayer* player, Rectangle<float> bounds)
    {
        isDragging = false;

        markPos = event.x;
        auto posFraction = (float) markPos / bounds.getWidth();
        auto playerPosition = (int64) (posFraction * player->getAudioLength());

        player->setLoopMarker (playerPosition, type == LoopStart);
    }
};

class Marker : public Component
{
public:
    Marker (AudioPlayer* player) :
        player (player)
    {
        setBounds (0, 0, 600, 400);

        startMark.type = LoopStart;
        endMark.type = LoopEnd;
    }

    void paint (Graphics& g) override
    {
        g.setColour (Colours::dodgerblue);

        startMark.paint (g, player, getBounds().toFloat());
        endMark.paint (g, player, getBounds().toFloat());
    }

    void mouseMove (const MouseEvent& event) override
    {
        auto overStart = startMark.mouseOver (event);
        auto overEnd = endMark.mouseOver (event);

        if (overStart || overEnd)
            setMouseCursor (MouseCursor::LeftRightResizeCursor);
        else
            setMouseCursor (MouseCursor::NormalCursor);
    }

    void mouseDrag (const MouseEvent& event) override
    {
        if (startMark.isOver)
            startMark.dragging (event, player);
        else if (endMark.isOver)
            endMark.dragging (event, player);
    }

    void mouseUp (const MouseEvent& event) override
    {
        if (startMark.isDragging)
            startMark.endDrag (event, player, getBounds().toFloat());
        else if (endMark.isDragging)
            endMark.endDrag (event, player, getBounds().toFloat());

        repaint();
    }

private:
    AudioPlayer* player;
    
    Mark startMark;
    Mark endMark;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Marker)
};

#endif //MARKER_H_INCLUDED
