/*
 * Example shows how to create, connect and read data with an openDAQ stream reader. 
 */
#include <daq_utils.h>

// This function retrieves a list of 
// all available signals on the device and returns the first one in the list
void retrieveFirstAvailableSignal(daqSignal** signal, daqDevice* device)
{
    daqList* availableSignals;
    daqDevice_getSignalsRecursive(device, &availableSignals, NULL);

    daqSizeT signalAmountAvailable = 0;
    daqList_getCount(availableSignals, &signalAmountAvailable);

    if (signalAmountAvailable > 0)
        daqList_getItemAt(availableSignals, 0, (daqBaseObject**)signal);

    daqReleaseRef(availableSignals);
}

int main(void)
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);
    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    daqSignal* connectedSignal = NULL;
    retrieveFirstAvailableSignal(&connectedSignal, simulator);

    daqStreamReader* reader = NULL;
    daqStreamReader_createStreamReader(&reader, connectedSignal, daqSampleTypeFloat64, daqSampleTypeInt64, daqReadModeRawValue, daqReadTimeoutTypeAny);
    daqFloat samples[500];
    daqSizeT count = 500;
    const daqSizeT timeoutMs = 1000;

    for (uint8_t i = 0; i < 200; i++)
    {
        count = 500;
        // The last parameter is NULL because we will ignore the returned status
        // of the reader (in this case)
        daqStreamReader_read(reader, samples, &count, timeoutMs, NULL);
        if (count > 0)
            printf(" %u no. of times read, %llu sample, %f value\n",
                    i, count, samples[count - 1]);
    }

    daqReleaseRef(reader);
    daqReleaseRef(instance);
    daqReleaseRef(simulator);
    daqReleaseRef(simulatorInstance);
    return 0;
}