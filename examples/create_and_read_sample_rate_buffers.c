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

    daqSizeT count = 0;
    daqList_getCount(availableSignals, &count);

    if (count <= 0)
    {
        printf("No available signals were found.\n");
        daqReleaseRef(availableSignals);
        daqReleaseRef(simulator);
        daqReleaseRef(instance);
        daqReleaseRef(simulatorInstance);
        return 0;
    }

    daqSignal* connectedSignal = NULL;
    daqList_getItemAt(availableSignals, 0, (daqBaseObject**)&connectedSignal);

    daqStreamReaderBuilder* streamReaderBuilder = NULL;
    daqStreamReaderBuilder_createStreamReaderBuilder(&streamReaderBuilder);

    daqStreamReaderBuilder_setReadMode(streamReaderBuilder, daqReadModeScaled);
    daqStreamReaderBuilder_setDomainReadType(streamReaderBuilder, daqSampleTypeInt64);
    daqStreamReaderBuilder_setValueReadType(streamReaderBuilder, daqSampleTypeFloat64);
    daqStreamReaderBuilder_setReadTimeoutType(streamReaderBuilder, daqReadTimeoutTypeAny);
    daqStreamReaderBuilder_setSignal(streamReaderBuilder, connectedSignal);

    daqStreamReader* streamReader = NULL;
    daqStreamReaderBuilder_build(streamReaderBuilder, &streamReader);

    daqReleaseRef(streamReaderBuilder);

    daqDataDescriptor* domainDataDescriptor = NULL;
    daqReaderStatus* status = NULL;

    zeroCountRead(streamReader, &status);
    domainDescriptorFromReaderStatus(status, &domainDataDescriptor);

    daqSizeT sampleRate;
    getSampleRate(&sampleRate, domainDataDescriptor);

    daqFloat* samples = NULL;
    const daqSizeT bufSize = sampleRate * 2;
    samples = malloc(bufSize * sizeof(daqFloat));
    daqSizeT sampleAmount = sampleRate;
    const daqSizeT timeoutMs = 1000;

    printf("Sample amount: %llu\n", sampleAmount);

    for (daqSizeT i = 0; i < 200; i++)
    {
        daqReader_getAvailableCount((daqReader*)streamReader, &sampleAmount);

        if (sampleAmount > bufSize)
            sampleAmount = bufSize;
        
        daqStreamReader_read(streamReader, samples, &sampleAmount, timeoutMs, NULL);

        if (sampleAmount > 0)
            printf("Entry: %llu, Sample value: %f\n", (i+1), samples[sampleAmount-1]);
    }

    free(samples);

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
