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

    read_from_stream_reader(signal, 100);

    daqReleaseRef(signal);
    daqReleaseRef(instance);
    daqReleaseRef(availableDevices);
    daqReleaseRef(connectedDevice);
}