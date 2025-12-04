/*
 * In this example we will display everything contained in the data descriptor
 * and how to calculate the sample rate.
 */
#include <daq_utils.h>

int main(void)
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    // Get a Data Descriptor from a named signal and display everything that it contains alongside
    // its domain signal and all domain signals content

    daqString* signalName = NULL;
    daqString_createString(&signalName, "AI 1");

    daqList* availableSignals = NULL;
    daqDevice_getSignalsRecursive(simulator, &availableSignals, NULL);

    daqIterator* iterator = NULL;
    daqList_createStartIterator(availableSignals, &iterator);

    daqConstCharPtr signalNameConstChar = NULL;
    daqString_getCharPtr(signalName, &signalNameConstChar);

    daqSignal* wantedSignal = NULL;

    while (daqIterator_moveNext(iterator) == DAQ_SUCCESS)
    {
        daqSignal* currentSignal = NULL;
        daqIterator_getCurrent(iterator, (daqBaseObject**)&currentSignal);

        daqDataDescriptor* signalDescriptor = NULL;
        daqSignal_getDescriptor(currentSignal, &signalDescriptor);

        daqString* signalDescriptorName = NULL;
        daqDataDescriptor_getName(signalDescriptor, &signalName);

        daqConstCharPtr signalDescriptorNameConstChar = NULL;
        daqString_getCharPtr(signalDescriptorName, &signalDescriptorNameConstChar);

        daqReleaseRef(signalDescriptorName);
        daqReleaseRef(signalDescriptor);

        if (!strcmp(signalNameConstChar, signalDescriptorNameConstChar))
        {
            wantedSignal = currentSignal;
            break;
        }
        daqReleaseRef(currentSignal);
    }

    daqReleaseRef(iterator);
    daqReleaseRef(availableSignals);

    daqDataDescriptor* dataDescriptor = NULL;
    daqSignal_getDescriptor(wantedSignal, &dataDescriptor);

    daqSignal* domainSignal = NULL;
    daqSignal_getDomainSignal(wantedSignal, &domainSignal);

    daqDataDescriptor* domainDescriptor = NULL;
    daqSignal_getDescriptor(domainSignal, &domainDescriptor);

    // Things to display: 
    // TODO:
    // - tickResolution,
    // - dimensions,
    // - metadata,
    // - name,
    // - origin,
    // - postScaling,
    // - rawSampleSize,
    // - referenceDomainInfo,
    // - rule,
    // - sampleSize,
    // - sampleType,
    // - structField,
    // - unit,
    // - valueRange

    // Afterwords get the tick resolution (we already have it from above) and from it calculate the sample rate.

    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);

    return 0;
}
