/*
 * Example that show how to properly display all relevant attributes and 
 * metadata in properties.
 */
#include <daq_property_utils.h>

void recursive(daqPropertyObject* property)
{
    daqList* visibleProperties = NULL;
    daqPropertyObject_getVisibleProperties(property, &visibleProperties);

    daqSizeT count = 0;
    daqList_getCount(visibleProperties, &count);

    daqBaseObject* temp = NULL;
    daqProperty* value = NULL;
    daqCoreType check = daqCtUndefined;

    for(daqSizeT i = 0; i< count; i++)
    {
        daqList_getItemAt(visibleProperties, i, &temp);
        daqQueryInterface(temp, DAQ_PROPERTY_INTF_ID, &value);
        daqReleaseRef(temp);
        daqProperty_getValueType(value, &check);
        daqProperty_getValue(value, &temp);
        if (check == daqCtObject)
        {
            daqString* name = NULL;
            daqProperty_getName(value, &name);
            printDaqFormattedString("- Property name: %s -\n", name);
            daqReleaseRef(name);
            daqPropertyObject* propObj = NULL;
            daqQueryInterface(temp, DAQ_PROPERTY_OBJECT_INTF_ID, &propObj);
            daqReleaseRef(temp);
            recursive(propObj);
            daqReleaseRef(propObj);
            printf("---\n\n");
        }
        else
        {
            printPropertyMetadata(value);
            printf("- Value: ");
            printPropertyValue(temp, value);
            daqReleaseRef(temp);
            printf("\n");
        }
        daqReleaseRef(value);
    }
    daqReleaseRef(visibleProperties);
}

int main()
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    recursive((daqPropertyObject*) simulator);

    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);
    return 0;
}