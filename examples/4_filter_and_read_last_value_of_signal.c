/*
 * This example shows how to find a specific signal and read its last know value.
 * The example explores two ways of finding the wanted signal,
 * the first being searching through all signals and comparing the names
 * and the second one find the signal via inbuilt signal filters looking for a matching localId.
 */

#include <daq_utils.h>

daqSignal* filterSignalWithName(daqString* signalName, daqDevice* device)
{
    daqSearchFilter* filter = NULL;
    daqSearchFilter_createAnySearchFilter(&filter);
    daqList* availableSignals = NULL;
    daqDevice_getSignalsRecursive(device, &availableSignals, filter);


    daqIterator* iterator = NULL;
    daqList_createStartIterator(availableSignals, &iterator);

    daqConstCharPtr signalNameConstChar = NULL;
    daqString_getCharPtr(signalName, &signalNameConstChar);

    while (daqIterator_moveNext(iterator) == DAQ_SUCCESS)
    {
        daqSignal* currentSignal = NULL;
        daqIterator_getCurrent(iterator, (daqBaseObject**)&currentSignal);

        daqDataDescriptor* signalDescriptor = NULL;
        daqSignal_getDescriptor(currentSignal, &signalDescriptor);

        daqString* nameSignal = NULL;
        daqDataDescriptor_getName(signalDescriptor, &nameSignal);

        daqConstCharPtr nameSignalConstChar = NULL;
        daqString_getCharPtr(nameSignal, &nameSignalConstChar);

        daqReleaseRef(nameSignal);
        daqReleaseRef(signalDescriptor);
        
        if (!strcmp(nameSignalConstChar, signalNameConstChar))
        {
            daqReleaseRef(iterator);
            daqReleaseRef(availableSignals);
            daqReleaseRef(filter);
            return currentSignal;
        }

        daqReleaseRef(currentSignal);
    }

    daqReleaseRef(iterator);
    daqReleaseRef(availableSignals);
    daqReleaseRef(filter);

    return NULL;
}

daqSignal* filterWithInbuiltFilter(daqString* localId, daqDevice* device)
{
    daqSearchFilter* filter;
    daqSearchFilter_createLocalIdSearchFilter(&filter, localId);

    daqList* availableSignals = NULL;
    daqDevice_getSignalsRecursive(device, &availableSignals, filter);

    daqIterator* iterator = NULL;

    daqList_createStartIterator(availableSignals, &iterator);

    daqSignal* currentSignal = NULL;

    if (daqIterator_moveNext(iterator) == DAQ_SUCCESS)
        daqIterator_getCurrent(iterator, (daqBaseObject**)&currentSignal);

    daqReleaseRef(filter);
    daqReleaseRef(availableSignals);
    daqReleaseRef(iterator);
    return currentSignal;

}

int main(void)
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    daqString* signalName = NULL;
    daqString_createString(&signalName, "AI 1");
    daqString* localId = NULL;
    daqString_createString(&localId, "AI0");

    daqSignal* signalFoundByName = filterSignalWithName(signalName, simulator);

    daqSignal* signalFoundByLocalId = filterWithInbuiltFilter(localId, simulator);

    daqFloat* lastValueName = NULL;
    daqSignal_getLastValue(signalFoundByName, (daqBaseObject**)&lastValueName);
    printf("Lats value from a signal found with matching name: %f\n", *lastValueName);

    daqFloat* lastValueLocalId = NULL;
    daqSignal_getLastValue(signalFoundByLocalId, (daqBaseObject**)&lastValueLocalId);
    printf("Last value from a signal found with a matching local id: %f\n", *lastValueLocalId);

    daqReleaseRef(signalFoundByLocalId);
    daqReleaseRef(signalFoundByName);
    daqReleaseRef(localId);
    daqReleaseRef(signalName);
    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);

    return 0;
}
