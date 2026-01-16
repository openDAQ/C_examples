/*
 * This example 
 */

#include <daq_utils.h>

enum PropertyType
{
    Property = 0,
    BoolProprerty,
    IntProperty,
    FloatProperty,
    StringProperty,
    ListProperty,
    DictProperty,
    RatioProperty,
    ObjectProperty,
    ReferenceProperty,
    FunctionProperty,
    SelectionProperty,
    SparseSelectionProperty,
    StructSelectionProperty,
    EnumerationProperty
};


int main()
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);
    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    // Get the globalId of a componet

    // 

    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);
}
