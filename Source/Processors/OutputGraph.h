#ifndef OUTPUTGRAPH_H_INCLUED
#define OUTPUTGRAPH_H_INCLUED

#include "AudioPlayer.h"

class OutputGraph : public AudioProcessorGraph
{
public:
    using AudioGraphIOProcessor = AudioProcessorGraph::AudioGraphIOProcessor;
    using Node = AudioProcessorGraph::Node;

    OutputGraph (AudioPlayer* player)
    {
        deviceManager.initialiseWithDefaultDevices (0, 2);
        deviceManager.addAudioCallback (&processPlayer);

        audioOutputNode = addNode (new AudioGraphIOProcessor (AudioGraphIOProcessor::audioOutputNode));
        audioOutputNode->getProcessor()->setPlayConfigDetails (2, 2, getSampleRate(), getBlockSize());

        playerNode = addNode (player);
        for (int channel = 0; channel < 2; ++channel)
        {
            addConnection ({ { playerNode->nodeID,      channel },
                             { audioOutputNode->nodeID, channel } });
        }

        processPlayer.setProcessor (this);
    }

    ~OutputGraph()
    {
        deviceManager.removeAudioCallback (&processPlayer);
    }



private:
    AudioDeviceManager deviceManager;;
    AudioProcessorPlayer processPlayer;

    Node::Ptr audioOutputNode;
    Node::Ptr playerNode;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OutputGraph)
};

#endif //OUTPUTGRAPH_H_INCLUED
