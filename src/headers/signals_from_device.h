#include <connect_device.h>

void signalsFromDevices(daqInstance* instance, daqDevice* device, daqSignal** signal)
{
    // Connect to the device and connect the signal
    daqList* signals = NULL;
    daqFunctionBlock* channel = NULL;
    daqList* channels = NULL;
    daqDevice_getChannels(device, &channels, NULL);

    daqIterator* iterator = NULL;
    
    daqList_createStartIterator(channels, &iterator);

    while (daqIterator_moveNext(iterator) == DAQ_SUCCESS)
    {
        daqSignal* currentSignal = NULL;
        daqIterator_getCurrent(iterator, &currentSignal);

        daqDataDescriptor* dataDescriptor = NULL;
        daqSignal_getDescriptor(currentSignal, &dataDescriptor);

        daqString* name = NULL;
        daqDataDescriptor_getName(currentSignal, &name);

        daqConstCharPtr name_constChar = NULL;
        daqString_getCharPtr(name, &name_constChar);

        printf("Name of the signal: %s\n", name_constChar);

        signal = &currentSignal;

        daqReleaseRef(currentSignal);
        daqReleaseRef(name);
    }
}

