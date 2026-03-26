/*
 * This example demonstrates how to set values of properties based on their value type.
 * When setting more complex properties that contain structures as their value types (enumeration, struct),
 * conversion between (in this case) C and openDAQ is required (an example on how to do/create these conversions can be found 
 * in this example suite in advanced_conversions_example.c).
 */

#include <daq_property_utils.h>

// Change the values of all basic types of properties (int, float, bool, string, ...)
// 


void setPropValue(daqProperty* property, void* value)
{
    
}


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
}