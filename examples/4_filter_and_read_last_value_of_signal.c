/*
 * ... :(
 */

#include <daq_utils.h>

daqSignal* findSignalWithName(daqString* signalName, daqDevice* device)
{
    return NULL;
}

int main(void)
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    daqList* signals = NULL;
    daqDevice_getSignalsRecursive(simulator, &signals, NULL);

    daqIterator* iterator = NULL;
    daqList_createStartIterator(signals, &iterator);

    daqBool compare = False;

    while (daqIterator_moveNext(iterator) == DAQ_SUCCESS)
    {
        daqSignal* currentSignal = NULL;
        daqIterator_getCurrent(iterator, (daqBaseObject*)&currentSignal);

        daqDataDescriptor* signalDescriptor = NULL;
        daqSignal_getDescriptor(currentSignal, &signalDescriptor);

        daqString* nameSignal = NULL;
        daqDataDescriptor_getName(signalDescriptor, &nameSignal);

        daqConstCharPtr nameSignalConstChar = NULL;
        daqString_getCharPtr(nameSignal, &nameSignalConstChar);

        compare = !strcmp(nameSignalConstChar, "AI0");

        if (compare)
        {
            daqFloat* value = NULL;
            daqSignal_getLastValue(currentSignal, (daqBaseObject*)&value);
        }

        daqReleaseRef(nameSignal);
        daqReleaseRef(signalDescriptor);
        daqReleaseRef(currentSignal);
    }

    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);

    return 0;
}
