/*
 * Example that show how to properly display all relevant attributes and 
 * metadata in properties.
 */
#include <daq_c_conversions.h>

void determinePropertyTypeAndPrint(daqProperty* property);
void displayPropertyTypes(daqPropertyObject* propertyObject);
void displayCommonMetadata(daqProperty* property);
void intProp(daqProperty* property);
void floatProp(daqProperty* property);
void stringProp(daqProperty* property);
void boolProp(daqProperty* property);
void sparseSelectionProp(daqProperty* property);
void selectionProp(daqProperty*  property);

void determinePropertyTypeAndPrint(daqProperty* property)
{
    displayCommonMetadata(property);
    daqCoreType propCoreType = daqCtUndefined;
    daqProperty_getValueType(property, &propCoreType);
    switch(propCoreType)
    {
    case daqCtBool:
        boolProp(property);
        break;
    case daqCtFloat:
        floatProp(property);
        break;
    case daqCtString:
        stringProp(property);
        break;
    case daqCtInt:
    {
        daqBaseObject* selectionValues = NULL;
        daqProperty_getSelectionValues(property, &selectionValues);
        if (selectionValues == NULL)
            intProp(property);
        else if (DAQ_SUPPORTS_INTERFACE(selectionValues, DAQ_DICT_INTF_ID))
            sparseSelectionProp(property);
        else
            selectionProp(property);
        break;
    }
    default:
        break;
    }
}

void displayPropertyTypes(daqPropertyObject* propertyObject)
{
    daqList* properties = NULL;
    daqPropertyObject_getVisibleProperties(propertyObject, &properties);

    daqSizeT count = 0;
    daqList_getCount(properties, &count);

    daqBaseObject* temp = NULL;
    daqProperty* prop = NULL;
    daqString* name = NULL;
    daqCoreType propCoreType = daqCtUndefined;

    for(daqSizeT i = 0; i < count; i++)
    {
        daqList_getItemAt(properties, i, &temp);
        if (DAQ_SUPPORTS_INTERFACE(temp, DAQ_PROPERTY_INTF_ID))
        {
            daqQueryInterface(temp, DAQ_PROPERTY_INTF_ID, &prop);

            daqProperty_getValueType(prop, &propCoreType);
            daqProperty_getName(prop, &name);
            printDaqFormattedString("\nProperty Name: %s\n", name);
            daqReleaseRef(name);
            determinePropertyTypeAndPrint(prop);
            daqReleaseRef(prop);
        }
        daqReleaseRef(temp);
    }

    daqReleaseRef(properties);
}

void displayCommonMetadata(daqProperty* property)
{
    daqBool visible = False;
    daqBool readOnly = False;

    daqProperty_getVisible(property, &visible);
    daqProperty_getReadOnly(property, &readOnly);

    printf("\n- Visible: %s\n", visible == True ? "True" : "False");
    printf("- Read only: %s\n", readOnly == True ? "True" : "False");
}

void intProp(daqProperty* property)
{
    daqBaseObject* temp = NULL;
    daqProperty_getValue(property, &temp);
    daqInteger* value = NULL;
    daqQueryInterface(temp, DAQ_INTEGER_INTF_ID, &value);
    daqInt valueNative = openDAQIntConversion(value);
    daqReleaseRef(value);
    daqReleaseRef(temp);

    printf("- Value %lld\n", valueNative);

    daqProperty_getDefaultValue(property, &temp);
    if (temp != NULL)
    {
        daqQueryInterface(temp, DAQ_INTEGER_INTF_ID, &value);
        valueNative = openDAQIntConversion(value);
        daqReleaseRef(value);
        daqReleaseRef(temp);
        printf("- Default Value: %lld\n", valueNative);
    }

    daqNumber* edgeCase = NULL;
    daqProperty_getMinValue(property, &edgeCase);
    if (edgeCase != NULL)
    {
        daqNumber_getIntValue(edgeCase, &valueNative);
        printf("- Minimum Value: %lld\n", valueNative);
        daqReleaseRef(edgeCase);
        daqProperty_getMaxValue(property, &edgeCase);
        if (edgeCase != NULL)
        {
            daqNumber_getIntValue(edgeCase, &valueNative);
            printf("- Maximum Value: %lld\n", valueNative);
            daqReleaseRef(edgeCase);
        }
    }

    daqList* suggestedValues = NULL;
    daqProperty_getSuggestedValues(property, &suggestedValues);

    daqSizeT count = 0;
    if (suggestedValues != NULL)
    {
        daqList_getCount(suggestedValues, &count);
        printf("- Suggested Values: \n");

        for (daqSizeT i = 0; i < count; i++)
        {
            daqList_getItemAt(suggestedValues, i, &temp);
            value = daqQueryInterfacePtr(temp, DAQ_INTEGER_INTF_ID);
            valueNative = openDAQIntConversion(value);
            daqReleaseRef(value);
            daqReleaseRef(temp);
            printf("  -- %lld\n", valueNative);
        }
        daqReleaseRef(suggestedValues);
    }
}

void floatProp(daqProperty* property)
{
    daqBaseObject* temp = NULL;
    daqProperty_getValue(property, &temp);

    daqFloatObject* value = NULL;
    daqQueryInterface(temp, DAQ_FLOAT_OBJECT_INTF_ID, &value);

    daqFloat valueNative = 0;
    daqFloatObject_getValue(value, &valueNative);
    daqReleaseRef(value);
    daqReleaseRef(temp);

    printf("- Value: %f\n", valueNative);

    daqProperty_getDefaultValue(property, &temp);
    if (temp != NULL)
    {
        daqQueryInterface(temp, DAQ_FLOAT_OBJECT_INTF_ID, &value);
        daqFloatObject_getValue(value, &valueNative);
        daqReleaseRef(value);
        daqReleaseRef(temp);

        printf("- Default Value: %f\n", valueNative);
    }

    daqNumber* edgeCase = NULL;
    daqProperty_getMinValue(property, &edgeCase);
    if (edgeCase != NULL)
    {
        daqNumber_getFloatValue(edgeCase, &valueNative);
        printf("- Minimum Value: %f\n", valueNative);
        daqReleaseRef(edgeCase);
        daqProperty_getMaxValue(property, &edgeCase);
        if (edgeCase != NULL)
        {
            daqNumber_getFloatValue(edgeCase, &valueNative);
            printf("- Maximum Value: %f\n", valueNative);
            daqReleaseRef(edgeCase);
        }
    }
    
    daqList* suggestedValues = NULL;
    daqProperty_getSuggestedValues(property, &suggestedValues);
    if (suggestedValues != NULL)
    {
        printf("- Suggested Values:\n");

        daqSizeT count = 0;
        daqList_getCount(suggestedValues, &count);
        for(daqSizeT i = 0; i < count; i++)
        {
            daqList_getItemAt(suggestedValues, i, &temp);
            value = daqQueryInterfacePtr(temp, DAQ_FLOAT_OBJECT_INTF_ID);
            valueNative = openDAQFloatConversion(value);
            daqReleaseRef(value);
            daqReleaseRef(temp);

            printf("  -- %f\n", valueNative);
        }
        daqReleaseRef(suggestedValues);
    }
}

void stringProp(daqProperty* property)
{
    daqBaseObject* temp = NULL;
    daqProperty_getValue(property, &temp);

    daqString* value = NULL;
    daqQueryInterface(temp, DAQ_STRING_INTF_ID, &value);
    printDaqFormattedString("- Value: %s\n", value);
    daqReleaseRef(value);
    daqReleaseRef(temp);

    daqProperty_getDefaultValue(property, &temp);
    daqQueryInterface(temp, DAQ_STRING_INTF_ID, &value);
    printDaqFormattedString("- Default Value: %s\n", value);
    daqReleaseRef(value);
    daqReleaseRef(temp);

    daqList* suggestedValues = NULL;
    daqProperty_getSuggestedValues(property, &suggestedValues);

    daqSizeT count = 0;
    if (suggestedValues)
        daqList_getCount(suggestedValues, &count);
    for(daqSizeT i = 0; i< count; i++)
    {
        daqList_getItemAt(suggestedValues, i, &temp);
        value = daqQueryInterfacePtr(temp, DAQ_STRING_INTF_ID);

        printDaqFormattedString("\n%s\n", value);
        daqReleaseRef(value);
        daqReleaseRef(temp);
    }
}

void boolProp(daqProperty* property)
{
    daqBaseObject* temp = NULL;
    daqBoolean* value = NULL;

    daqProperty_getValue(property, &temp);
    daqQueryInterface(temp, DAQ_BOOLEAN_INTF_ID, &value);
    daqBool valueNative = False;
    daqBoolean_getValue(value, &valueNative);
    daqReleaseRef(value);
    daqReleaseRef(temp);

    printf("- Value: %s\n", valueNative == True ? "True" : "False");

    daqProperty_getDefaultValue(property, &temp);
    daqQueryInterface(temp, DAQ_BOOLEAN_INTF_ID, &value);
    valueNative = False;
    daqBoolean_getValue(value, &valueNative);
    daqReleaseRef(value);
    daqReleaseRef(temp);

    printf("- Default Value: %s\n", valueNative == True ? "True" : "False");
}

void sparseSelectionProp(daqProperty* property)
{

}

void selectionProp(daqProperty* property)
{

}

int main()
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    displayPropertyTypes((daqPropertyObject*) simulator);

    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);
    return 0;
}