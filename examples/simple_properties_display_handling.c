/*
 * Example that show how to properly display all relevant attributes and 
 * metadata in properties.
 */
#include <daq_c_conversions.h>

void determinePropertyTypeAndPrint(daqProperty* property);
void printPropertyObject(daqPropertyObject* propertyObject);
void printMetadata(daqProperty* property);
void intProp(daqProperty* property);
void floatProp(daqProperty* property);
void stringProp(daqProperty* property);
void boolProp(daqProperty* property);
void sparseSelectionProp(daqProperty* property);
void selectionProp(daqProperty*  property);
void ratioProp(daqProperty* property);
void functionProp(daqProperty* property);
void objectProp(daqProperty* property);
void prop(daqProperty* property);
void printSelectedValue(daqBaseObject* container, daqCoreType itemType, daqInteger* keyValue);


void determinePropertyTypeAndPrint(daqProperty* property)
{
    printMetadata(property);
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
    case daqCtFunc:
    {
        functionProp(property);
        break;
    }
    case daqCtProc:
    {
        functionProp(property);
        break;
    }
    case daqCtRatio:
    {
        ratioProp(property);
        break;
    }
    case daqCtObject:
    {
        objectProp(property);
        break;
    }
    default:
        prop(property);
        break;
    }
}

void printPropertyObject(daqPropertyObject* propertyObject)
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
        prop = daqBorrowInterfacePtr(temp, DAQ_PROPERTY_INTF_ID);
        if (prop != NULL)
        {
            daqQueryInterface(temp, DAQ_PROPERTY_INTF_ID, &prop);

            daqProperty_getValueType(prop, &propCoreType);
            daqProperty_getName(prop, &name);
            printDaqFormattedString("\n%s\n", name);
            daqReleaseRef(name);
            determinePropertyTypeAndPrint(prop);
            daqReleaseRef(prop);
        }
        daqReleaseRef(temp);
    }

    daqReleaseRef(properties);
}

void printMetadata(daqProperty* property)
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
    daqInt valueNative = daq_fromDaqInteger(value);
    daqReleaseRef(value);
    daqReleaseRef(temp);

    printf("- Value %lld\n", valueNative);

    daqProperty_getDefaultValue(property, &temp);
    if (temp != NULL)
    {
        daqQueryInterface(temp, DAQ_INTEGER_INTF_ID, &value);
        valueNative = daq_fromDaqInteger(value);
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
            valueNative = daq_fromDaqInteger(value);
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
            valueNative = daq_fromDaqFloat(value);
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
    // Value
    daqBaseObject* tempObj = NULL;
    daqProperty_getValue(property, &tempObj);
    daqInteger* value = NULL;
    daqQueryInterface(tempObj, DAQ_INTEGER_INTF_ID, &value);
    daqReleaseRef(tempObj);

    // Selection values
    daqDict* selectionValues = NULL;
    daqProperty_getSelectionValues(property, &tempObj);
    daqQueryInterface(tempObj, DAQ_DICT_INTF_ID, &selectionValues);
    daqReleaseRef(tempObj);

    daqCoreType tempEnum = daqCtUndefined;
    daqProperty_getItemType(property, &tempEnum);

    printSelectedValue(selectionValues, tempEnum, value);
    daqReleaseRef(value);

    // Default Value
    daqProperty_getDefaultValue(property, &tempObj);

    if (tempObj != NULL)
        return;
    
    daqQueryInterface(tempObj, DAQ_INTEGER_INTF_ID, &value);
    daqReleaseRef(tempObj);

    printSelectedValue(selectionValues, tempEnum, value);
    daqReleaseRef(value);
}

void selectionProp(daqProperty* property)
{
    // Value
    daqBaseObject* tempObj = NULL;
    daqProperty_getValue(property, &tempObj);
    daqInteger* value = NULL;
    daqQueryInterface(tempObj, DAQ_INTEGER_INTF_ID, &value);
    daqReleaseRef(tempObj);

    // Selection values
    daqList* selectionValues = NULL;
    daqProperty_getSelectionValues(property, &tempObj);
    daqQueryInterface(tempObj, DAQ_LIST_INTF_ID, &selectionValues);
    daqReleaseRef(tempObj);

    daqCoreType tempEnum = daqCtUndefined;
    daqProperty_getItemType(property, &tempEnum);

    printSelectedValue(selectionValues, tempEnum, value);
    daqReleaseRef(value);

    // Default Value
    daqProperty_getDefaultValue(property, &tempObj);

    if (tempObj != NULL)
        return;

    daqQueryInterface(tempObj, DAQ_INTEGER_INTF_ID, &value);
    daqReleaseRef(tempObj);

    printSelectedValue(selectionValues, tempEnum, value);
    daqReleaseRef(value);
}

void ratioProp(daqProperty* property)
{
    // Value
    daqBaseObject* tempObj = NULL;
    daqProperty_getValue(property, &tempObj);
    daqRatio* temp = NULL;
    daqQueryInterface(tempObj, DAQ_RATIO_INTF_ID, &temp);
    daqReleaseRef(tempObj);

    daqInt denominator = 0;
    daqInt numerator = 0;
    daqRatio_getDenominator(temp, &denominator);
    daqRatio_getNumerator(temp, &numerator);
    daqReleaseRef(temp);

    // Print both
    printf("- Value:\n");
    printf(" -- denominator: %lld\n", denominator);
    printf(" -- numerator: %lld\n", numerator);

    // Default value
    daqProperty_getDefaultValue(property, &tempObj);
    
    if (tempObj == NULL)
        return;

    daqQueryInterface(tempObj, DAQ_RATIO_INTF_ID, &temp);
    daqReleaseRef(tempObj);
    denominator = 0;
    numerator = 0;
    daqRatio_getDenominator(temp, &denominator);
    daqRatio_getNumerator(temp, &numerator);
    daqReleaseRef(temp);

    // Print both
    printf("- Default Value:\n");
    printf(" -- denominator: %lld\n", denominator);
    printf(" -- numerator: %lld\n", numerator);
}

void functionProp(daqProperty* property)
{
    printf("\nMethod\n");
}

void objectProp(daqProperty* property)
{
    daqBaseObject* tempObj = NULL;
    daqProperty_getValue(property, &tempObj);
    daqPropertyObject* temp = NULL;
    daqQueryInterface(tempObj, DAQ_PROPERTY_OBJECT_INTF_ID, &temp);
    daqReleaseRef(tempObj);
    printf("-- Object property Value --\n");
    printPropertyObject(temp);
    daqReleaseRef(temp);

    printf("\n");
    daqProperty_getDefaultValue(property, &tempObj);

    if (tempObj == NULL)
    {
        printf("---\n");
        return;
    }

    daqQueryInterface(tempObj, DAQ_PROPERTY_OBJECT_INTF_ID, &temp);
    daqReleaseRef(tempObj);
    printf("-- Object property Default Value --\n");
    printPropertyObject(temp);
    daqReleaseRef(temp);
    printf("---\n");
}

void prop(daqProperty* property)
{
    // Check for others and display what they are as in functionProp
    daqCoreType tempCt = daqCtUndefined;
    daqProperty_getValueType(property, &tempCt);
    switch(tempCt)
    {
        case daqCtStruct:
        {
            printf("\nOpenDAQ Struct\n");
        }
        case daqCtEnumeration:
        {
            printf("\nOpenDAQ Enumeration\n");
        }
        case daqCtList:
        {
            printf("\nOpenDAQ List\n");
        }
        case daqCtDict:
        {
            printf("\nOpenDAQ Dictionary\n");
        }
        default:
        {
            daqProperty* tempProp = NULL;
            daqProperty_getReferencedProperty(property, &tempProp);
            if (tempProp != NULL)
            {
                daqString* name = NULL;
                daqProperty_getName(tempProp, &name);
                printDaqFormattedString("\nReferenced property: %s\n", name);
                daqReleaseRef(name);
                determinePropertyTypeAndPrint(tempProp);
                daqReleaseRef(tempProp);
            }
            else
            {
                break;
            }
        }
    }
}

void printSelectedValue(daqBaseObject* container, daqCoreType itemType, daqInteger* keyValue)
{
    daqInt key = 0;
    daqInteger_getValue(keyValue, &key);

    daqBaseObject* temp = NULL;

    if(DAQ_SUPPORTS_INTERFACE(container, DAQ_LIST_INTF_ID))
    {
        daqList* tempContainer = NULL;
        daqQueryInterface(container, DAQ_LIST_INTF_ID, &tempContainer);
        daqList_getItemAt(tempContainer, key, &temp);
        daqReleaseRef(tempContainer);
    }
    else
    {
        daqDict* tempContainer = NULL;
        daqQueryInterface(container, DAQ_DICT_INTF_ID, &tempContainer);
        daqDict_get(tempContainer, keyValue, &temp);
        daqReleaseRef(tempContainer);
    }
    
}

int main()
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    printPropertyObject((daqPropertyObject*) simulator);

    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);
    return 0;
}