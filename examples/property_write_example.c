/*
 * This example demonstrates how to set values of properties based on their value type.
 * When setting more complex properties that contain structures as their value types (enumeration, struct),
 * conversion between (in this case) C and openDAQ is required (an example on how to do/create these conversions can be found 
 * in this example suite in advanced_conversions_example.c).
 */

#include <daq_property_utils.h>

// Change the values of all basic types of properties (int, float, bool, string, ...)
// Change GlobalSampleRate (example of float)
// Change AcquisitionLoopTime (example of int)
// Change EnableCANChannel (example of bool)
// Change Waveform (example of selection)
// Change userName (example of string)
// Change CustomRange (example of struct)
// -- For now ok --

void assignFloat(daqPropertyObject* propObj, daqBaseObject* value)
{
    // Assigning FloatProperty 
    daqProperty* prop = NULL;
    daqBaseObject* propValue = NULL;
    daqString* propName = NULL;
    daqString_createString(&propName, "GlobalSampleRate");

    // get the GlobalSampleRate
    daqPropertyObject_getProperty(propObj, propName, &prop);
    daqProperty_getValue(prop, &propValue);
    exdaq_printSimpleCoreTypeValue(propValue, daqCtFloat);
    daqReleaseRef(propValue);

    // set the GlobalSampleRate
    validateDaqFloat(value, prop);
    daqProperty_setValue(prop, value);

    // get the GlobalSampleRate and display before and after
    daqProperty_getValue(prop, &propValue);
    exdaq_printSimpleCoreTypeValue(propValue, daqCtFloat);
    daqReleaseRef(propValue);

    daqReleaseRef(propName);
    daqReleaseRef(prop);
}

void assignInt(daqPropertyObject* propObj, daqBaseObject* value)
{
    // Assigning IntProperty
    daqProperty* prop = NULL;
    daqBaseObject* propValue = NULL;
    daqString* propName = NULL;
    daqString_createString(&propName, "AcquisitionLoopTime");

    // get
    daqPropertyObject_getProperty(propObj, propName, &prop);
    daqProperty_getValue(prop, &propValue);
    exdaq_printSimpleCoreTypeValue(propValue, daqCtInt);
    daqReleaseRef(propValue);

    // set
    validateDaqInt(value, prop);
    daqProperty_setValue(prop, value);

    // get and display both gets
    daqProperty_getValue(prop, &propValue);
    exdaq_printSimpleCoreTypeValue(propValue, daqCtInt);
    daqReleaseRef(propValue);

    daqReleaseRef(propName);
    daqReleaseRef(prop);
}

void assignBool(daqPropertyObject* propObj, daqBaseObject* value)
{
    // Assigning BoolProperty
    daqProperty* prop = NULL;
    daqBaseObject* propValue = NULL;
    daqString* propName = NULL;
    daqString_createString(&propName, "EnableCANChannel");

    // get
    daqPropertyObject_getProperty(propObj, propName, &prop);
    daqProperty_getValue(prop, &propValue);
    exdaq_printSimpleCoreTypeValue(propValue, daqCtBool);
    daqReleaseRef(propValue);

    // set
    validateDaqBool(value, prop);
    daqProperty_setValue(prop, value);

    // get and display both gets
    daqProperty_getValue(prop, &propValue);
    exdaq_printSimpleCoreTypeValue(propValue, daqCtBool);
    daqReleaseRef(propValue);

    daqReleaseRef(propName);
    daqReleaseRef(prop);
}

void assignSelection(daqPropertyObject* propObj, daqBaseObject* value)
{
    // Assigning SelectionProperty
    daqProperty* prop = NULL;
    daqBaseObject* propValue = NULL;
    daqString* propName = NULL;
    daqString_createString(&propName, "Waveform");

    // get
    daqPropertyObject_getProperty(propObj, propName, &prop);
    daqProperty_getValue(prop, &propValue);
    exdaq_printSimpleCoreTypeValue(propValue, daqCtInt);
    daqReleaseRef(propValue);

    // set
    validateDaqList(value, prop);
    daqProperty_setValue(prop, value);

    // get and display both gets
    daqProperty_getValue(prop, &propValue);
    exdaq_printSimpleCoreTypeValue(propValue, daqCtInt);
    daqReleaseRef(propValue);

    daqReleaseRef(propName);
    daqReleaseRef(prop);
}

void assignString(daqProperty* prop, daqBaseObject* value)
{
    // Assigning StringProperty

    // get

    // set

    // get and display
}

void assignStruct(daqProperty* prop, daqBaseObject* value)
{
    // Assigning StructProperty

    // get

    // set

    // get and display
}

void setPropValue(daqProperty* property, void* value)
{
    // Check that the void* can be converted to the corresponding openDAQ object than run the subselected assignment (functions beginning with assign)
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