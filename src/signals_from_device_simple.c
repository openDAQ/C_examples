#include <copendaq.h>
#include <stdio.h>
#include <connect_device_simple.c>

void signalsFromDevices(daqInstance* instance, daqDevice* device, daqSignal* signal)
{
    // Connect to the device and connect the signal
    daqList* signals = NULL;
    daqFunctionBlock* channel = NULL;
    daqList* channels = NULL;
    daqDevice_getChannels(device, &channels, NULL);
    
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
    signalsFromDevices(instance, connectedDevice, signal);

    daqDataDescriptor* signalDescriptor = NULL;
    daqSignal_getDescriptor(signal, &signalDescriptor);

    daqString* signalName = NULL;
    daqDataDescriptor_getName(signalDescriptor, &signalName);

    daqConstCharPtr* signalName_constChar = NULL;
    daqString_getCharPtr(signalName, &signalName_constChar);

    printf("The name of the connected signal is: %s\r\n", signalName_constChar);

    daqReleaseRef(signalDescriptor);
    daqReleaseRef(signalName);
    daqReleaseRef(signalName_constChar);
    daqReleaseRef(instance);
    daqReleaseRef(availableDevices);
    daqReleaseRef(connectedDevice);
    daqReleaseRef(signal);

    return 0;
}