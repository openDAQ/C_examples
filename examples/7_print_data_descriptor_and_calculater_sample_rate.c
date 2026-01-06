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

    daqDataRule* dataRule = NULL;
    daqDataDescriptor_getRule(domainDescriptor, &dataRule);

    daqDict* parametersDataRule = NULL;
    daqDataRule_getParameters(dataRule, &parametersDataRule);

    daqString* deltaString = NULL;
    daqString_createString(&deltaString, "delta");

    daqBaseObject* deltaObj = NULL;

    daqDict_get(parametersDataRule, deltaString, &deltaObj);

    daqNumber* delta = NULL;
    daqQueryInterface(deltaObj, DAQ_NUMBER_INTF_ID, &delta);

    daqInt deltaInt = 1;
    daqNumber_getIntValue(delta, &deltaInt);

    daqInt numerator = 1;
    daqRatio_getNumerator(ratio, &numerator);

    daqInt denominator = 1;
    daqRatio_getDenominator(ratio, &denominator);

    daqFloat sampleRate = 1;
    sampleRate = (daqFloat) denominator / (daqFloat) numerator / (daqFloat)deltaInt;

    printf("Calculated sample rate is: %f Hz\n", sampleRate);

    daqReleaseRef(delta);
    daqReleaseRef(deltaString);
    daqReleaseRef(parametersDataRule);
    daqReleaseRef(dataRule);
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
