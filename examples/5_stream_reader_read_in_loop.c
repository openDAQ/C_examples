/*
 * Example show how to create, connect and read data with an OpenDAQ streamReader 
 */
#include <daq_utils.h>

void connectToFirstAvailableSignal(daqSignal** signal, daqDevice* device)
{
    daqList* availableDevices;
    daqDevice_getSignalsRecursive(device, &availableDevices, NULL);

    daqList_getItemAt(availableDevices, 0, (daqBaseObject**)signal);

    daqReleaseRef(availableDevices);
}

int main(void)
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);
    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    daqSignal* connectedSignal = NULL;
    connectToFirstAvailableSignal(&connectedSignal, simulator);

    daqStreamReader* reader = NULL;
    daqStreamReader_createStreamReader(&reader, connectedSignal, 
        daqSampleTypeFloat64, daqSampleTypeInt64,
                    daqReadModeRawValue, daqReadTimeoutTypeAny);
    daqFloat samples[100];
    daqSizeT placeholder = 100;
    daqSizeT* count = &placeholder;
    const daqSizeT timeoutMs = 500;

    for (uint8_t i = 0; i < 100; i++)
    {
        *count = 100;
        // The last parameter is NULL because we will ignore the returned status
        // of the reader (in this case)
        daqStreamReader_read(reader, samples, count, timeoutMs, NULL);
        if (*count > 0)
            printf(" %u no. of times read, %llu sample, %f value\n",
                    i, *count, samples[*count - 1]);
    }

    daqReleaseRef(reader);
    daqReleaseRef(instance);
    daqReleaseRef(simulator);
    daqReleaseRef(simulatorInstance);
    return 0;
}