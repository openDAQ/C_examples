/*
 *
 */
#include <daq_c_conversions.h>

//

int main()
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);



    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);
    return 0;
}