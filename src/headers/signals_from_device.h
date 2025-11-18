#include <connect_device.h>

void signalsFromDevices(daqInstance* instance, daqDevice* device, daqSignal** signal)
{
    // Connect to the device and connect the signal
    daqList* signals = NULL;
    daqFunctionBlock* channel = NULL;
    daqList* channels = NULL;
    daqDevice_getChannels(device, &channels, NULL);

}

