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

    daqList* availableDevices;
    daqDevice_getSignalsRecursive(simulator, &availableDevices, NULL);

    daqSignal* signal = NULL;
    daqList_getItemAt(availableDevices, 0, (daqBaseObject**)signal);

    // Connect to the first signal,
    // Read with domain,
    // Display the readings with accompanying time

    daqReleaseRef(signal);
    daqReleaseRef(availableDevices);
    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);
    return 0;
}
 