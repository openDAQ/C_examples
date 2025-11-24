// An example

#include <read_in_loop.h>

int main()
{
    daqInstance* instance = NULL;
    createInstance(&instance);

    daqList* availableDevices = NULL;
    discoverDevices(&availableDevices, instance);

    daqDevice* connectedDevice = NULL;
    connectDevice(availableDevices, instance, &connectedDevice);

    daqSignal* signal = NULL;
    signalsFromDevices(connectedDevice, &signal);

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

    daqConstCharPtr unitSymbolStr = NULL;
    daqString_getCharPtr(unitSymbol, &unitSymbolStr);


    daqUInt domainSamples[100];
    daqFloat samples[100];

    for (int i = 0; i < 40; i++)
    {
        daqSizeT count = 100;
        daqStreamReader_readWithDomain(streamReader, samples, domainSamples, &count, 1000, NULL);
        if (count > 0)
        {
            daqInt resolutionNumerator = 0;
            daqInt resolutionDenominator = 0;
            daqRatio_getNumerator(ratio, &resolutionNumerator);
            daqRatio_getDenominator(ratio, &resolutionDenominator);

            daqFloat domainValue = (daqFloat)domainSamples[count - 1] * resolutionNumerator / resolutionDenominator;
            printf("Value: %f, Domain: %f%s\n", samples[count - 1], domainValue, unitSymbolStr);
        }
    }



    //daqReleaseRef(domainSamples);
    //daqReleaseRef(samples);
    daqReleaseRef(streamReader);
    daqReleaseRef(signal);
    daqReleaseRef(instance);
    daqReleaseRef(availableDevices);
    daqReleaseRef(connectedDevice);
}