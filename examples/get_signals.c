/*
 * This example demonstrates how to list both visible and invisible signals of a device.
 */

#include <daq_utils.h>

void printSignalNames(daqList* signals)
{
    daqIterator* iterator = NULL;
    daqList_createStartIterator(signals, &iterator);

    while (daqIterator_moveNext(iterator) == DAQ_SUCCESS)
    {
        daqSignal* currentSignal = NULL;
        daqIterator_getCurrent(iterator, (daqBaseObject**)&currentSignal);

        daqString* nameSignal = NULL;
        daqComponent_getName((daqComponent*)currentSignal, &nameSignal);

        printDaqFormattedString("The name of the signal is: %s\n", nameSignal);

        daqReleaseRef(nameSignal);
        daqReleaseRef(currentSignal);
    }

    daqReleaseRef(iterator);
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

    printf("Visible signals:\n");
    printSignalNames(signals);

    daqSearchFilter* filter = NULL;
    daqSearchFilter_createAnySearchFilter(&filter);
    daqDevice_getSignalsRecursive(simulator, &signals, filter);
    
    printf("\nVisible and invisible signals:\n");
    printSignalNames(signals);

    daqReleaseRef(filter);
    daqReleaseRef(signals);
    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);

    return 0;
}