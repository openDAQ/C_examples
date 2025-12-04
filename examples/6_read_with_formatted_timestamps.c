/*
 * In this example we will explore reading data with its accompanying domain value.
 */
#include <daq_utils.h>

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

    daqUInt domainSamples[100];
    daqFloat samples[100];
    const daqSizeT timeoutMs = 1000;

    for (int i = 0; i< 40; i++)
    {
        daqSizeT count = 100;
        daqStreamReader_readWithDomain(streamReader, samples, domainSamples, &count, timeoutMs, NULL);
        if (count > 0)
        {
            daqInt resolutionNumerator = 0;
            daqInt resolutionDenominator = 0;
            daqRatio_getNumerator(ratio, &resolutionNumerator);
            daqRatio_getDenominator(ratio, &resolutionDenominator);

            daqFloat domainValue = (daqFloat)domainSamples[count - 1] * resolutionNumerator / resolutionDenominator;
            printf("Value: %f, Domain: %f%s\n", samples[count - 1], domainValue, unitSymbolCostChar);
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
 