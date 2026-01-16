/*
 * This example demonstrates how to search through properties, 
 * determine their (core)type and display their value.
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

void printCoreTypeObjects(daqProperty* coreTypeObject)
{
    // TODO
}

void displayPropertyTypes(daqPropertyObject* propertyObject)
{
    // List of IProperty
    daqList* properties = NULL;
    daqPropertyObject_getVisibleProperties(propertyObject, &properties);

    daqSizeT count = 0;
    daqList_getCount(properties, &count);

    if (count <= 0)
    {
        daqReleaseRef(properties);
        return;
    }

    for (daqSizeT i = 0; i< count; i++)
    {
        daqBaseObject* currentPropBaseObj = NULL;
        daqList_getItemAt(properties, i, &currentPropBaseObj);

        // Cast into IProperty
        if (DAQ_SUPPORTS_INTERFACE(currentPropBaseObj, DAQ_PROPERTY_INTF_ID))
        {
            daqProperty* prop = NULL;
            daqQueryInterface(currentPropBaseObj, DAQ_PROPERTY_INTF_ID, &prop);

            daqCoreType propCoreType = daqCtUndefined;
            daqProperty_getValueType(prop, &propCoreType);
            if (propCoreType == daqCtObject)
            {
                // Write them up first
                printCoreTypeObjects(prop);
            }
            else if (propCoreType == daqCtUndefined)
            {
                // Maybe needed (don't know yet)
            }
            daqString* propName = NULL;
            daqProperty_getName(prop, &propName);
            printDaqFormattedString("Property name: %s", propName);
            daqBaseObject* propValueObj = NULL;
            daqProperty_getValue(prop, propValueObj);
            daqConstCharPtr propValueObjStr = "";
            daqBaseObject_toString(propValueObj, propValueObjStr);
            printf("\nValue of the property: %s\n", propValueObjStr);

            daqReleaseRef(propValueObj);
            daqReleaseRef(propName);
            daqReleaseRef(prop);
        }

        daqReleaseRef(currentPropBaseObj);
    }
    daqReleaseRef(properties);
}


int main()
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);
    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    displayPropertyTypes((daqPropertyObject*)simulator);

    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);
}
