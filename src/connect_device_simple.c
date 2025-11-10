#include <copendaq.h>
#include <discover_devices_simple.c>



void connectDevice(daqList* listOfAvailableDevices, daqInstance* instance)
{
    // In the simple example we will connect to 


}

int main()
{
    daqInstance* instance = NULL;
    createInstance(instance);

    daqList* availableDevices = NULL;
    discoverDevices(availableDevices, instance);
}