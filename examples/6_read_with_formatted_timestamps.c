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

    daqSignal* domainSignal = NULL;
    daqSignal_getDomainSignal(signal, &domainSignal);

    daqDataDescriptor* dataDescriptor = NULL;
    daqSignal_getDescriptor(domainSignal, &dataDescriptor);

    daqRatio* ratio = NULL;
    daqDataDescriptor_getTickResolution(dataDescriptor, &ratio);

    daqUnit* unit = NULL;
    daqDataDescriptor_getUnit(dataDescriptor, &unit);

    daqString* unitSymbol = NULL;
    daqUnit_getSymbol(unit, &unitSymbol);

    daqConstCharPtr unitSymbolCostChar = NULL;
    daqString_getCharPtr(unitSymbol, &unitSymbolCostChar);

    daqUInt domainSamples[500];
    daqFloat samples[500];
    const daqSizeT timeoutMs = 1000;

    for (int i = 0; i< 20; i++)
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

            daqInt domainValueRounded = (daqIntegerObject)domainValue;
            char* dateTimeInString = "";
            (void)ctime_s(dateTimeInString, 64, &domainValueRounded);

            printf("Value: %f, Domain: %f%s, Date and time: %s\n", samples[count - 1], domainValue, unitSymbolCostChar, dateTimeInString);
        }
    }

    daqReleaseRef(unitSymbol);
    daqReleaseRef(unit);
    daqReleaseRef(ratio);
    daqReleaseRef(dataDescriptor);
    daqReleaseRef(domainSignal);
    daqReleaseRef(streamReader);

    daqReleaseRef(signal);
    daqReleaseRef(availableSignals);
    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);

    return 0;
}
 