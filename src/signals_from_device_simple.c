#include <copendaq.h>
#include <stdio.h>
#include <connect_device_simple.c>

void signalsFromDevices(daqDevice* device, daqSignal* signal)
{
    // Connect to the device and connect the signal

}

int main()
{
    daqInstance* instance = NULL;
    createaInstance(instance);

    daqList* availableDevices = NULL;
    discoverDevices(availableDevices, instance);

    daqDevice* connectedDevice = NULL;
    connectDevice(availableDevices, instance, connectedDevice);

    daqSignal* signal = NULL;
    signalsFromDevices(connectedDevice, signal);



    daqReleaseRef(instance);
    daqReleaseRef(availableDevices);
    daqReleaseRef(connectedDevice);
    daqReleaseRef(signal);

    return 0;
}