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
    // its domain signal and all domain signals attributes.

    daqList* availableSignals = NULL;
    daqDevice_getSignalsRecursive(simulator, &availableSignals, NULL);

    daqConstCharPtr signalNameConstChar = "AI1";

    daqSignal* wantedSignal = NULL;

    daqSizeT listSize = 0;
    daqList_getCount(availableSignals, &listSize);

    for (daqSizeT i = 0; i < listSize; i++)
    {
        daqSignal* currentSignal = NULL;
        daqList_getItemAt(availableSignals, i, (daqBaseObject**) &currentSignal);

        daqString* nameCurrentSignal = NULL;
        daqComponent_getName((daqComponent*) currentSignal, &nameCurrentSignal);

        daqConstCharPtr signalDescriptorNameConstChar = NULL;
        daqString_getCharPtr(nameCurrentSignal, &signalDescriptorNameConstChar);

        daqReleaseRef(nameCurrentSignal);

        if (!strcmp(signalNameConstChar, signalDescriptorNameConstChar))
        {
            wantedSignal = currentSignal;
            break;
        }
        daqReleaseRef(currentSignal);
    }

    daqReleaseRef(availableSignals);

    daqDataDescriptor* dataDescriptor = NULL;
    daqSignal_getDescriptor(wantedSignal, &dataDescriptor);

    daqSignal* domainSignal = NULL;
    daqSignal_getDomainSignal(wantedSignal, &domainSignal);

    daqDataDescriptor* domainDescriptor = NULL;
    daqSignal_getDescriptor(domainSignal, &domainDescriptor);

    daqCharPtr dataDescriptorChar = NULL;
    daqBaseObject_toString(dataDescriptor, &dataDescriptorChar);

    daqCharPtr domainDescriptorChar = NULL;
    daqBaseObject_toString(domainDescriptor, &domainDescriptorChar);

    printf("Data descriptor:\n%s\nDomain descriptor:\n%s\n",dataDescriptorChar, domainDescriptorChar);

    daqDataRule* dataRule = NULL;
    daqDataDescriptor_getRule(domainDescriptor, &dataRule);

    if (checkLinearRule(dataRule))
    {
        daqRatio* ratio = NULL;
        daqDataDescriptor_getTickResolution(domainDescriptor, &ratio);

        daqDict* parametersDataRule = NULL;
        daqDataRule_getParameters(dataRule, &parametersDataRule);

        daqString* deltaString = NULL;
        daqString_createString(&deltaString, "delta");

        daqBaseObject* deltaObj = NULL;
        daqDict_get(parametersDataRule, deltaString, &deltaObj);

        daqNumber* delta = NULL;
        daqQueryInterface(deltaObj, DAQ_NUMBER_INTF_ID, &delta);

        daqSizeT sampleRate = 1;
        calculateSampleRate(&sampleRate, ratio, delta);

        printf("Calculated sample rate is: %llu Hz\n", sampleRate);

        daqReleaseRef(delta);
        daqReleaseRef(deltaObj);
        daqReleaseRef(deltaString);

        daqReleaseRef(parametersDataRule);
        daqReleaseRef(ratio);
    }
    else
    {
        printf("Data rule of the signal is not linear, therefore we cannot calculate the sample rate.");
    }

    daqReleaseRef(dataRule);
    daqReleaseRef(dataDescriptor);
    daqReleaseRef(domainDescriptor);
    daqReleaseRef(domainSignal);
    daqReleaseRef(wantedSignal);

    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);

    return 0;
}
