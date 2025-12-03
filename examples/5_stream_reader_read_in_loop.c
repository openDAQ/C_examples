/*
 * Example show how to create, connect and read data with an OpenDAQ streamReader 
 */
#include <daq_utils.h>

void connectToFirstAvailableSignal(daqSignal** signal, daqDevice* device)
{
    
}

int main(void)
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);
    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    daqSignal* connectedSignal = NULL;
    connectToFirstAvailableSignal(&connectedSignal, simulator);
    // Use the skeleton of the application_example.c, however use read instead of read_with_domain

    daqReleaseRef(instance);
    daqReleaseRef(simulator);
    daqReleaseRef(simulatorInstance);
    return 0;
}