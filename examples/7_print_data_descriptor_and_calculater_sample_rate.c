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

    daqConstCharPtr signalNameConstChar = NULL;
    daqString_getCharPtr(signalName, &signalNameConstChar);

    daqSignal* wantedSignal = NULL;

    daqSizeT listSize = 0;
    daqList_getCount(availableSignals, &listSize);

    for (daqSizeT i = 0; i < listSize; i++)
    {
        daqSignal* currentSignal = NULL;
        daqList_getItemAt(availableSignals, i, (daqBaseObject**) &currentSignal);

        daqDataDescriptor* signalDescriptor = NULL;
        daqSignal_getDescriptor(currentSignal, &signalDescriptor);

        daqString* signalDescriptorName = NULL;
        daqDataDescriptor_getName(signalDescriptor, &signalDescriptorName);

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

    daqReleaseRef(availableSignals);

    daqDataDescriptor* dataDescriptor = NULL;
    daqSignal_getDescriptor(wantedSignal, &dataDescriptor);

    daqStreamReader* streamReader = NULL;
    daqStreamReader_createStreamReader(&streamReader, wantedSignal, daqSampleTypeFloat64, daqSampleTypeInt64, daqReadModeRawValue, daqReadTimeoutTypeAny);

    daqDataDescriptor* domainDescriptor = NULL;
    domainDescriptorFromEventPacket(streamReader, &domainDescriptor);

    daqCharPtr dataDescriptorChar = NULL;
    daqBaseObject_toString(dataDescriptor, &dataDescriptorChar);

    daqCharPtr domainDescriptorChar = NULL;
    daqBaseObject_toString(domainDescriptor, &domainDescriptorChar);

    printf("Data descriptor:\n%s\nDomain descriptor:\n%s\n",dataDescriptorChar, domainDescriptorChar);

    daqRatio* ratio = NULL;
    daqDataDescriptor_getTickResolution(domainDescriptor, &ratio);

    daqInt resolutionNumerator = 0;
    daqRatio_getNumerator(ratio, &resolutionNumerator);

    daqInt resolutionDenominator = 0;
    daqRatio_getDenominator(ratio, &resolutionDenominator);

    daqFloat sampleRate = 0;
    sampleRate = (daqFloat)resolutionDenominator / (daqFloat)resolutionNumerator;

    printf("Calculated sample rate is: %f Hz\n", sampleRate);
    
    daqReleaseRef(ratio);
    daqReleaseRef(dataDescriptor);
    daqReleaseRef(domainDescriptor);
    daqReleaseRef(streamReader);
    daqReleaseRef(wantedSignal);

    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);

    return 0;
}
