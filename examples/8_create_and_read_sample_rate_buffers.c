/*
 * This example will demonstrate how to create and read into buffers
 * that are created based on the calculated sample rate
 */
#include <daq_utils.h>

int main(void)
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    // Calculate sample rate

    // Create sample rate buffer

    // Create reader

    // Read into the created buffer

    daqReleaseRef(instance);
    daqReleaseRef(simulator);
    daqReleaseRef(simulatorInstance);

    return 0;
}
