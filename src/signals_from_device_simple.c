// An example that 

#include <signals_from_device.h>

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

    daqDataDescriptor* signalDescriptor = NULL;
    daqSignal_getDescriptor(signal, &signalDescriptor);

    daqString* signalName = NULL;
    daqDataDescriptor_getName(signalDescriptor, &signalName);

    daqConstCharPtr signalName_constChar = NULL;
    daqString_getCharPtr(signalName, &signalName_constChar);

    printf("The name of the connected signal is: %s\r\n", signalName_constChar);

    daqReleaseRef(signalName);
    daqReleaseRef(instance);
    daqReleaseRef(availableDevices);
    daqReleaseRef(connectedDevice);
    daqReleaseRef(signal);

    return 0;
}