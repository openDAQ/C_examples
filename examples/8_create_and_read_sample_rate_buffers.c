/*
 * This example will demonstrate how to create and read into buffers
 * that are created based on the calculated sample rate.
 */
#include <daq_utils.h>

int main(void)
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    // Connect to the first available signal on the simulator.
    daqList* availableSignals = NULL;
    daqDevice_getSignalsRecursive(simulator, &availableSignals, NULL);

    daqSignal* connectedSignal = NULL;
    daqList_getItemAt(availableSignals, 0, (daqBaseObject**)&connectedSignal);

    daqStreamReader* streamReader = NULL;
    daqStreamReader_createStreamReader(&streamReader, connectedSignal, daqSampleTypeFloat64, daqSampleTypeInt64, daqReadModeRawValue, daqReadTimeoutTypeAny);

    daqDataDescriptor* domainDataDescriptor = NULL;
    daqReadStatus* status = NULL;

    zeroCountReadStatus(streamReader, &status);
    domainDescriptorFromReadStatus(status, &domainDataDescriptor);

    daqSizeT sampleRate;
    retrieveSampleRate(&sampleRate, domainDataDescriptor);

    // Picking 1000 as a constant here is arbitrary as it servers a simplification derived from
    // the fact that arrays in C require constant expression to be compiled.
    daqFloat samples[1000 * 2];

    daqSizeT sampleAmount = sampleRate * 2;

    printf("Sample amount: %llu\n", sampleAmount);

    for (daqSizeT i = 0; i < 200; i++)
    {
        daqStreamReader_read(streamReader, samples, &sampleAmount, 10000, NULL);

        if (sampleAmount > 0)
            printf("Entry: %llu, Sample value: %f\n", i, samples[sampleAmount-1]);

        sampleAmount = sampleRate * 2;
    }

    daqReleaseRef(status);
    daqReleaseRef(domainDataDescriptor);
    daqReleaseRef(streamReader);
    daqReleaseRef(connectedSignal);
    daqReleaseRef(availableSignals);
    daqReleaseRef(instance);
    daqReleaseRef(simulator);
    daqReleaseRef(simulatorInstance);

    return 0;
}
