#include <signals_from_device.h>

void read_in_loop(daqSignal* signal, daqDevice* device)
{
    daqDeviceInfo* devInfo = NULL;
    daqDevice_getInfo(device, &devInfo);

    // Input parameters: Connected device, signal from whitch the data will be read



    // Output data: The Data gets displayed in via printf (optional)

}

void display_read_data()
{
    // 
}
