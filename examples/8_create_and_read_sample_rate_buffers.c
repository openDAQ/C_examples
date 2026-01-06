/*
 * This example will demonstrate how to create and read into buffers
 * that are created based on the calculated sample rate
 */
#include <daq_utils.h>

int main(void)
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    // Connect to the first available signal on the simulator
    daqList* availableSignals = NULL;
    if (daqDevice_getSignalsRecursive(simulator, &availableSignals, NULL) != DAQ_SUCCESS)
    {
        daqReleaseRef(simulator);
        daqReleaseRef(instance);
        daqReleaseRef(simulatorInstance);
        return 1;
    }

    daqSignal* connectedSignal = NULL;
    daqList_getItemAt(availableSignals, 0, (daqBaseObject**)&connectedSignal);

    daqStreamReader* streamReader = NULL;
    daqStreamReader_createStreamReader(&streamReader, connectedSignal, daqSampleTypeFloat64, daqSampleTypeInt64, daqReadModeRawValue, daqReadTimeoutTypeAny);

    daqDataDescriptor* domainDataDescriptor = NULL;
    domainDescriptorFromEventPacket(streamReader, &domainDataDescriptor);

    daqRatio* ratio = NULL;
    daqDataDescriptor_getTickResolution(domainDataDescriptor, &ratio);

    daqInt numerator = 1;
    daqRatio_getNumerator(ratio, &numerator);
    
    daqInt denominator = 1;
    daqRatio_getDenominator(ratio, &denominator);

    daqSizeT sampleRate = (daqSizeT) ((daqFloat) denominator / (daqFloat) numerator);

    daqFloat samples[1000 * 2];

    daqSizeT sampleAmount = sampleRate * 2;

    printf("Sample amount: %llu\n", sampleAmount);

    daqStreamReader_read(streamReader, samples, &sampleAmount, 10000, NULL);

    for (daqSizeT i = 0; i < sampleAmount; i++)
        printf("Entry: %llu, Sample value: %f\n", i, samples[i]);

    daqReleaseRef(streamReader);
    daqReleaseRef(domainDataDescriptor);
    daqReleaseRef(instance);
    daqReleaseRef(simulator);
    daqReleaseRef(simulatorInstance);

    return 0;
}
