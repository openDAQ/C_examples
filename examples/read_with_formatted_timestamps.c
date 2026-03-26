/*
 * In this example we will explore reading data with its accompanying domain value and
 * converting timestamps from tick to unit of time.
 */
#include <daq_utils.h>
#include <time.h>

int main(void)
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    daqList* availableSignals;
    daqDevice_getSignalsRecursive(simulator, &availableSignals, NULL);

    daqSignal* signal = NULL;
    daqList_getItemAt(availableSignals, 1, (daqBaseObject**)&signal);

    daqStreamReader* streamReader = NULL;
    daqStreamReader_createStreamReader(&streamReader, signal, daqSampleTypeFloat64, daqSampleTypeInt64, daqReadModeRawValue, daqReadTimeoutTypeAny);

    daqSizeT cnt = 0;
    daqFloat smp[1];
    daqReaderStatus* status = NULL;
    // We only care for the event packet here so that we can read get accurate domain and data descriptors
    daqStreamReader_read(streamReader, &smp, &cnt, 1000, &status);

    daqEventPacket* eventPacket = NULL;
    daqReaderStatus_getEventPacket(status, &eventPacket);
    daqString* eventId = NULL;
    daqEventPacket_getEventId(eventPacket, &eventId);

    printDaqFormattedString("\nId of the gathered event packet: %s\n\n", eventId);

    daqBool check = False;
    daqString* checkerStr = NULL;
    daqString_createString(&checkerStr, "DATA_DESCRIPTOR_CHANGED");

    daqBaseObject_equals(eventId, checkerStr, &check);
    
    daqReleaseRef(checkerStr);
    daqDataDescriptor* domainDesc = NULL;

    if (check == True)
    {
        daqDict* parameters = NULL;
        daqEventPacket_getParameters(eventPacket, &parameters);
        daqString* domainDescriptorStr = NULL;
        daqString_createString(&domainDescriptorStr, "DomainDataDescriptor");

        daqDict_get(parameters, domainDescriptorStr, (daqBaseObject**)&domainDesc);
        daqReleaseRef(domainDescriptorStr);
        daqReleaseRef(parameters);
    }

    daqRatio* ratio = NULL;
    daqDataDescriptor_getTickResolution(domainDesc, &ratio);

    daqUnit* unit = NULL;
    daqDataDescriptor_getUnit(domainDesc, &unit);

    daqString* unitSymbol = NULL;
    daqUnit_getSymbol(unit, &unitSymbol);

    daqConstCharPtr unitSymbolCostChar = NULL;
    daqString_getCharPtr(unitSymbol, &unitSymbolCostChar);

    daqReleaseRef(eventId);
    daqReleaseRef(eventPacket);

    daqUInt domainSamples[500];
    daqFloat samples[500];
    const daqSizeT timeoutMs = 1000;

    for (int i = 0; i< 200; i++)
    {
        daqSizeT count = 500;
        // When reading with domain the last argument in the function call again represents a status return
        // from the reading
        daqStreamReader_readWithDomain(streamReader, samples, domainSamples, &count, timeoutMs, NULL);
        if (count > 0)
        {
            // To get a properly scaled timestamp we will multiply it by the tick resolution
            daqInt resolutionNumerator = 0;
            daqInt resolutionDenominator = 0;
            daqRatio_getNumerator(ratio, &resolutionNumerator);
            daqRatio_getDenominator(ratio, &resolutionDenominator);

            daqFloat domainValue = (daqFloat)domainSamples[count - 1] * (daqFloat)resolutionNumerator / (daqFloat)resolutionDenominator;

            daqInt domainValueRounded = (daqInt)domainValue;
            char* dateTimeInString = "";
            dateTimeInString = ctime(&domainValueRounded);

            printf("Value: %f, Domain: %f%s, Date and time: %s\n", samples[count - 1], domainValue, unitSymbolCostChar, dateTimeInString);
        }
    }

    daqReleaseRef(unitSymbol);
    daqReleaseRef(unit);
    daqReleaseRef(ratio);
    daqReleaseRef(domainDesc);
    daqReleaseRef(streamReader);

    daqReleaseRef(signal);
    daqReleaseRef(availableSignals);
    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);

    return 0;
}
 