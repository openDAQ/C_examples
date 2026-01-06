/*
 * In this example we will demonstrate the structure of openDAQ tree structure 
 * by traversing across it. 
 *
 *
 */

#include <daq_utils.h>

int main()
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    // Example code

    daqReleaseRef(instance);
    daqReleaseRef(simulator);
    daqReleaseRef(simulatorInstance);
    return 0;
}