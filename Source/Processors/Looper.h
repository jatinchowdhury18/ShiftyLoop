#ifndef LOOPER_H_INCLUDED
#define LOOPER_H_INCLUDED

//#include "../GUI/Marker.h"
#include "JuceHeader.h"

class Looper
{
public:
    Looper (const int64 lenSamples);//, Marker startMark, Marker endMark);

    void changeLoop (int64& start, int64& end);

private:
    const int timeSig = 4;
    const int numBars = 4;
    std::unique_ptr<std::unique_ptr<int64[]>[]> loopPoints;

    Random random = Random();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Looper)
};

#endif //LOOPER_H_INCLUDED
