#include <signals_from_device.h>

void read_from_stream_reader(daqSignal* signal, daqUInt duration)
{
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

    daqConstCharPtr unitSybolStr = NULL;
    daqString_getCharPtr(unitSymbol, &unitSybolStr);

    daqUInt domainSample[100];
    daqFloat samples[100];

    for (int i = 0; i < duration; i++)
    {
        daqSizeT count = 100;
        daqStreamReader_readWithDomain(streamReader, samples, domainSample, &count, 1000, NULL);
        if (count > 0)
        {
            daqInt resolutionNumerator = 0;
            daqInt resolutionDenominator = 0;
            daqRatio_getNumerator(ratio, &resolutionNumerator);
            daqRatio_getDenominator(ratio, &resolutionDenominator);

            daqFloat domainValue = (daqFloat)domainSample[count - 1] * resolutionNumerator / resolutionDenominator;
            printf("Value: %f, Domain: %f%s\n", samples[count - 1], domainValue, unitSybolStr);
        }
    }

    daqReleaseRef(unit);
    daqReleaseRef(ratio);
    daqReleaseRef(dataDescriptor);
    daqReleaseRef(domainSignal);
    daqReleaseRef(streamReader);
}
