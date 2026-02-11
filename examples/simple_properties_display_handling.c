/*
 * Example that show how to properly display all relevant attributes and 
 * metadata in properties.
 */
#include <daq_property_utils.h>

void recursive(daqPropertyObject* propertyObject)
{
    daqList* visibleProperties = NULL;
    daqPropertyObject_getVisibleProperties(propertyObject, &visibleProperties);

    daqSizeT count = 0;
    daqList_getCount(visibleProperties, &count);

    for (daqSizeT i = 0; i < count; i++)
    {
        daqBaseObject* listItem = NULL;
        daqProperty* property = NULL;

        daqList_getItemAt(visibleProperties, i, &listItem);
        daqQueryInterface(listItem, DAQ_PROPERTY_INTF_ID, &property);
        daqReleaseRef(listItem);

        daqCoreType valueType = daqCtUndefined;
        daqProperty_getValueType(property, &valueType);

        daqBaseObject* propertyValue;
        daqProperty_getValue(property, &propertyValue);

        if (valueType == daqCtObject)
        {
            daqString* name = NULL;
            daqProperty_getName(property, &name);
            printDaqFormattedString("- Property name: %s -\n", name);
            daqReleaseRef(name);

            daqPropertyObject* childPropertyObject = NULL;
            daqQueryInterface(propertyValue, DAQ_PROPERTY_OBJECT_INTF_ID, &childPropertyObject);
            recursive(childPropertyObject);
            printf("---\n\n");
            daqReleaseRef(childPropertyObject);
        }
        else
        {
            printPropertyMetadata(property);
            printf("- Value: ");
            printPropertyValue(propertyValue, property);
            printf("\n");
        }

        daqReleaseRef(propertyValue);
        daqReleaseRef(property);
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