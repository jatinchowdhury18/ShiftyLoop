#include "Looper.h"

Looper::Looper (const int64 lenSamples)
{
    loopPoints.reset (new std::unique_ptr<int64[]>[timeSig]);
    for (int i = 0; i < numBars; i++)
        loopPoints[i].reset (new int64[numBars]);

    const int64 beatLen = lenSamples / (timeSig * numBars);

    int64 sample = 0;
    for (int bar = 0; bar < numBars; bar++)
    {
        for (int beat = 0; beat < timeSig; beat++)
        {
            loopPoints[bar][beat] = sample;
            sample += beatLen;
        }
    }
}

void Looper::changeLoop (int64& start, int64& end)
{
    const int beat = abs (random.nextInt()) % timeSig;
    const int startBar = abs (random.nextInt()) % (numBars - 1);
    const int endBar = startBar + 1 + (abs (random.nextInt()) % (numBars - startBar - 1));

    start = loopPoints[startBar][beat];
    end = loopPoints[endBar][beat];
}
