/*
 * This example shows how to find a signal with a given name and read its last value.
 * The example explores two ways of finding the wanted signal:
 * - Iterate through all signals and check signal for name equality.
 * - Use a built-in filter to find a signal filters with a matching Local ID.
 */

#include <daq_utils.h>

daqSignal* iterativeSearchByName(daqString* wantedSignalName, daqDevice* device)
{
    daqSearchFilter* filter = NULL;
    daqSearchFilter_createAnySearchFilter(&filter);
    daqList* availableSignals = NULL;
    daqDevice_getSignalsRecursive(device, &availableSignals, filter);

    daqIterator* iterator = NULL;
    daqList_createStartIterator(availableSignals, &iterator);

    daqConstCharPtr signalNameConstChar = NULL;
    daqString_getCharPtr(wantedSignalName, &signalNameConstChar);

    // ??
    while (daqIterator_moveNext(iterator) == DAQ_SUCCESS)
    {
        daqSignal* currentSignal = NULL;
        daqIterator_getCurrent(iterator, (daqBaseObject**)&currentSignal);

        daqDataDescriptor* signalDescriptor = NULL;
        daqSignal_getDescriptor(currentSignal, &signalDescriptor);

        daqString* signalName = NULL;
        daqDataDescriptor_getName(signalDescriptor, &signalName);

        daqConstCharPtr nameSignalConstChar = NULL;
        daqString_getCharPtr(signalName, &nameSignalConstChar);

        daqReleaseRef(signalName);
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

daqSignal* filterByLocalId(daqString* localId, daqDevice* device)
{
    daqSearchFilter* filter;
    daqSearchFilter_createLocalIdSearchFilter(&filter, localId);

    daqList* availableSignals = NULL;
    daqDevice_getSignalsRecursive(device, &availableSignals, filter);

    daqSignal* currentSignal = NULL;

    daqSizeT filteredSignalsSize = 0;
    daqList_getCount(availableSignals, &filteredSignalsSize);

    if (filteredSignalsSize > 0)
        daqList_getItemAt(availableSignals, 0, (daqBaseObject**)&currentSignal);

    daqReleaseRef(filter);
    daqReleaseRef(availableSignals);
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

    daqSignal* signalFoundByName = iterativeSearchByName(signalName, simulator);

    daqSignal* signalFoundByLocalId = filterByLocalId(localId, simulator);

    daqFloat* lastValue = NULL;
    daqSignal_getLastValue(signalFoundByName, (daqBaseObject**)&lastValue);
    printf("Lats value from a signal found with matching name: %f\n", *lastValue);

    daqSignal_getLastValue(signalFoundByLocalId, (daqBaseObject**)&lastValue);
    printf("Last value from a signal found with a matching local id: %f\n", *lastValue);

    daqReleaseRef(signalFoundByLocalId);
    daqReleaseRef(signalFoundByName);
    daqReleaseRef(localId);
    daqReleaseRef(signalName);
    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);

    return 0;
}
