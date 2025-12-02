#include <daq_utils.h>

int main(void)
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    // Due to native streaming module not being loaded, we cannot currently detect the simulator...

    daqList* signals = NULL;
    daqDevice_getSignalsRecursive(simulator, &signals, NULL);

    daqIterator* iterator = NULL;
    daqList_createStartIterator(signals, &iterator);

    while (daqIterator_moveNext(iterator) == DAQ_SUCCESS)
    {
        daqSignal* currentSignal = NULL;
        daqIterator_getCurrent(iterator, (daqBaseObject*)&currentSignal);

        daqDataDescriptor* signalDescriptor = NULL;
        daqSignal_getDescriptor(currentSignal, &signalDescriptor);

        daqString* nameSignal = NULL;
        daqDataDescriptor_getName(signalDescriptor, &nameSignal);

        printDaqFormattedString("The name of the signal is:", nameSignal);

        daqReleaseRef(nameSignal);
        daqReleaseRef(signalDescriptor);
        daqReleaseRef(currentSignal);
    }

    daqReleaseRef(iterator);
    daqReleaseRef(signals);
    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);

    return 0;
}