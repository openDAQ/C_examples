#include <daq_c_conversions.h>

enum daqExample_propertyType
{
    daqExample_propertyType_selection,
    daqExample_propertyType_sparseSelection,
    daqExample_propertyType_integer,
    daqExample_propertyType_floatObject,
    daqExample_propertyType_string,
    daqExample_propertyType_boolean,
    daqExample_propertyType_ratio,
    daqExample_propertyType_enumeration,
    daqExample_propertyType_structObject,
    daqExample_propertyType_function,
    daqExample_propertyType_list,
    daqExample_propertyType_dictionary,
    daqExample_propertyType_object,
    daqExample_porpertyType_complexNumber,
    daqExample_propertyType_oh_no
};

void printSimpleCoreTypeValue(daqBaseObject* selectedValueObj, daqCoreType suggestedValuesItemType);
char* daqCoreTypeToString(daqCoreType type);
enum daqExample_propertyType daq_getPropType(daqProperty* property);
void printPropertyValue(daqBaseObject* value, daqProperty* property);
void printPropertyMetadata(daqProperty* property);
void printDaqUnit(daqUnit* unit);
void printDaqPropertyCallableInfo(daqCallableInfo* callableInfo);
void printDapPropertyName(daqProperty* property);
void printDaqPropertyVisible(daqProperty* property);
void printDaqPropertyReadOnly(daqProperty* property);
void printNumber(daqNumber* number, enum daqExample_propertyType type);
void printDaqDict(daqBaseObject* value, daqCoreType keyType, daqCoreType itemType);
void printDaqList(daqBaseObject* value, daqCoreType itemType);
void printDaqRatio(struct daqExample_Ratio native);

void printSimpleCoreTypeValue(daqBaseObject* selectedValueObj, daqCoreType suggestedValuesItemType)
{
    switch (suggestedValuesItemType)
    {
    case daqCtInt:
    {
        daqInteger* value = NULL;
        daqQueryInterface(selectedValueObj, DAQ_INTEGER_INTF_ID, &value);
        printf("%lld\n", daqExample_fromDaqInteger(value));
        daqReleaseRef(value);
        return;
    }
    case daqCtFloat:
    {
        daqFloatObject* value = NULL;
        daqQueryInterface(selectedValueObj, DAQ_FLOAT_OBJECT_INTF_ID, &value);
        printf("%f\n", daqExample_fromDaqFloat(value));
        daqReleaseRef(value);
        return;
    }
    case daqCtBool:
    {
        daqBoolean* value = NULL;
        daqQueryInterface(selectedValueObj, DAQ_BOOLEAN_INTF_ID, &value);
        printf("%s\n", daqExample_fromDaqBoolean(value) == True ? "True" : "False");
        daqReleaseRef(value);
        return;
    }
    case daqCtString:
    {
        daqString* value = NULL;
        daqQueryInterface(selectedValueObj, DAQ_STRING_INTF_ID, &value);
        printDaqFormattedString("%s\n", value);
        daqReleaseRef(value);
        return;
    }
    case daqCtRatio:
    {
        daqRatio* value = NULL;
        daqQueryInterface(selectedValueObj, DAQ_RATIO_INTF_ID, &value);
        printDaqRatio(daqExample_fromDaqRatio(value));
        daqReleaseRef(value);
        return;
    }
    case daqCtEnumeration:
        printf("Enumeration\n");
        break;
    case daqCtStruct:
        printf("Struct\n");
        break;
    case daqCtFunc:
        printf("Function\n");
        break;
    case daqCtProc:
        printf("Procedure\n");
        break;
    case daqCtObject:
    case daqCtList:
    case daqCtDict:
    default:
    {
        daqConstCharPtr* value = NULL;
        daqBaseObject_toString(selectedValueObj, value);
        printf("%s\n", *value);
        return;
    }
    }
}

char* daqCoreTypeToString(daqCoreType type)
{
    switch(type)
    {
    case daqCtBool:
        return "Bool";
    case daqCtInt:
        return "Integer";
    case daqCtFloat:
        return "Float";
    case daqCtString:
        return "String";
    case daqCtList:
        return "List";
    case daqCtDict:
        return "Dictionary";
    case daqCtRatio:
        return "Ratio";
    case daqCtProc:
        return "Procedure";
    case daqCtObject:
        return "Object";
    case daqCtBinaryData:
        return "Binary data";
    case daqCtFunc:
        return "Function";
    case daqCtComplexNumber:
        return "Complex number";
    case daqCtStruct:
        return "Struct";
    case daqCtEnumeration:
        return "Enumeration";
    case daqCtUndefined:
        return "Undefined";
    }
}

enum daqExample_propertyType daq_getPropType(daqProperty* property)
{
    daqCoreType type = daqCtUndefined;
    daqProperty_getValueType(property, &type);

    daqBaseObject* temp = NULL;

    switch (type)
    {
    case daqCtInt:
    {
        daqProperty_getSelectionValues(property, &temp);
        if (temp != NULL)
        {
            if (DAQ_SUPPORTS_INTERFACE(temp, DAQ_DICT_INTF_ID))
            {
                daqReleaseRef(temp);
                return daqExample_propertyType_sparseSelection;
            }
            else if (DAQ_SUPPORTS_INTERFACE(temp, DAQ_LIST_INTF_ID))
            {
                daqReleaseRef(temp);
                return daqExample_propertyType_selection;
            }
            daqReleaseRef(temp);
        }
        return daqExample_propertyType_integer;
    }

    case daqCtString:
        return daqExample_propertyType_string;

    case daqCtBool:
        return daqExample_propertyType_boolean;

    case daqCtFloat:
        return daqExample_propertyType_floatObject;

    case daqCtProc:
    case daqCtFunc:
        return daqExample_propertyType_function;

    case daqCtObject:
        return daqExample_propertyType_object;

    case daqCtStruct:
        return daqExample_propertyType_structObject;

    case daqCtRatio:
        return daqExample_propertyType_ratio;

    case daqCtComplexNumber:
        return daqExample_porpertyType_complexNumber;

    case daqCtList:
        return daqExample_propertyType_list;

    case daqCtDict:
        return daqExample_propertyType_dictionary;

    default:
        return daqExample_propertyType_oh_no;
    }
}

void printPropertyValue(daqBaseObject* value, daqProperty* property)
{
    enum daqExample_propertyType propType = daq_getPropType(property);

    switch (propType)
    {
    case daqExample_propertyType_integer:
    case daqExample_propertyType_string:
    case daqExample_propertyType_boolean:
    case daqExample_propertyType_floatObject:
    case daqExample_propertyType_ratio:
    {
        daqCoreType valueType;
        daqProperty_getValueType(property, &valueType);
        printSimpleCoreTypeValue(value, valueType);
        break;
    }
    case daqExample_propertyType_function:
    {
        printf("Function\n");
        break;
    }
    case daqExample_propertyType_selection:
    {
        daqInteger* val = NULL;
        daqQueryInterface(value, DAQ_INTEGER_INTF_ID, &val);

        daqBaseObject* temp = NULL;
        daqProperty_getSelectionValues(property, &temp);
        daqList* selectionValues = NULL;
        daqQueryInterface(temp, DAQ_LIST_INTF_ID, &selectionValues);
        daqReleaseRef(temp);

        daqList_getItemAt(selectionValues, daqExample_fromDaqInteger(val), &temp);
        daqReleaseRef(val);
        daqReleaseRef(selectionValues);

        daqCoreType itemType = daqCtUndefined;
        daqProperty_getItemType(property, &itemType);
        printSimpleCoreTypeValue(temp, itemType);
        daqReleaseRef(temp);
        break;
    }
    case daqExample_propertyType_sparseSelection:
    {
        daqBaseObject* temp = NULL;
        daqDict* selectionValues = NULL;

        daqProperty_getSelectionValues(property, &temp);
        daqQueryInterface(temp, DAQ_DICT_INTF_ID, &selectionValues);
        daqReleaseRef(temp);

        daqDict_get(selectionValues, value, &temp);
        daqReleaseRef(selectionValues);

        daqCoreType type = daqCtUndefined;
        daqProperty_getItemType(property, &type);
        printSimpleCoreTypeValue(temp, type);
        daqReleaseRef(temp);
        break;
    }
    case daqExample_propertyType_dictionary:
    {
        daqCoreType itemType = daqCtUndefined;
        daqCoreType keyType = daqCtUndefined;
        daqProperty_getItemType(property, &itemType);
        daqProperty_getKeyType(property, &keyType);
        printDaqDict(value, keyType, itemType);
        break;
    }
    case daqExample_propertyType_list:
    {
        daqCoreType itemType = daqCtUndefined;
        daqProperty_getItemType(property, &itemType);
        printDaqList(value, itemType);
        break;
    }
    case daqExample_propertyType_object:
        printf("Object\n");
        break;
    default:
    {
        printf("Property Value\n");
        break;
    }
    }
}

void printPropertyMetadata(daqProperty* property)
{
    enum daqExample_propertyType propertyType = daq_getPropType(property);

    // Strings
    printDapPropertyName(property);

    daqString* description = NULL;
    daqProperty_getDescription(property, &description);
    if (description != NULL)
    {
        printDaqFormattedString("- Description: %s\n", description);
        daqReleaseRef(description);
    }

    // Booleans
    printDaqPropertyVisible(property);
    printDaqPropertyReadOnly(property);

    // Value Type
    daqCoreType coreType = daqCtUndefined;
    daqProperty_getValueType(property, &coreType);
    printf("- Value type: %s\n", daqCoreTypeToString(coreType));

    // Default Value
    daqBaseObject* defaultValue = NULL;
    daqProperty_getDefaultValue(property, &defaultValue);
    printf("- Default value: ");
    printPropertyValue(defaultValue, property);
    if (defaultValue != NULL)
        daqReleaseRef(defaultValue);

    if (propertyType == daqExample_propertyType_dictionary)
    {
        daqProperty_getKeyType(property, &coreType);
        printf("- Key type: %s\n", daqCoreTypeToString(coreType));
    }

    if (propertyType == daqExample_propertyType_dictionary || propertyType == daqExample_propertyType_list)
    {
        daqProperty_getItemType(property, &coreType);
        printf("- Item type: %s\n", daqCoreTypeToString(coreType));
    }

    if (propertyType == daqExample_propertyType_integer || propertyType == daqExample_propertyType_floatObject)
    {
        daqNumber* min = NULL;
        daqNumber* max = NULL;
        daqProperty_getMinValue(property, &min);
        daqProperty_getMaxValue(property, &max);
        if (min != NULL)
        {
            printf("- Min value:");
            printNumber(min, propertyType);
        }
        if (max != NULL)
        {
            printf("- Max value:");
            printNumber(max, propertyType);
        }
    }

    if (propertyType == daqExample_propertyType_selection)
    {
        daqBaseObject* temp = NULL;
        daqProperty_getSelectionValues(property, &temp);
        daqProperty_getItemType(property, &coreType);
        printf("- Selection values:\n");
        printDaqList(temp, coreType);
        daqReleaseRef(temp);
    }

    if (propertyType == daqExample_propertyType_sparseSelection)
    {
        daqBaseObject* temp = NULL;
        daqProperty_getSelectionValues(property, &temp);
        daqProperty_getItemType(property, &coreType);
        printf("- Selection values: \n");
        printDaqDict(temp, daqCtInt, coreType);
        daqReleaseRef(temp);
    }

    if (propertyType == daqExample_propertyType_integer || propertyType == daqExample_propertyType_string || propertyType == daqExample_propertyType_floatObject)
    {
        daqList* temp = NULL;
        daqProperty_getSuggestedValues(property, &temp);
        if (temp != NULL)
        {
            daqProperty_getValueType(property, &coreType);
            printf("- Suggested values:\n");
            printDaqList((daqBaseObject*)temp, coreType);
            daqReleaseRef(temp);
        }
    }

    if (propertyType == daqExample_propertyType_integer || propertyType == daqExample_propertyType_floatObject)
    {
        daqUnit* unit = NULL;
        daqProperty_getUnit(property, &unit);
        if (unit != NULL)
        {
            printDaqUnit(unit);
            daqReleaseRef(unit);
        }
    }

    if (propertyType == daqExample_propertyType_function)
    {
        daqCallableInfo* callableInfo = NULL;
        daqProperty_getCallableInfo(property, &callableInfo);
        printDaqPropertyCallableInfo(callableInfo);
        daqReleaseRef(callableInfo);
    }
}

void printDaqUnit(daqUnit* unit)
{
    printf("- Unit:\n");
    int64_t id = 0;
    daqUnit_getId(unit, &id);
    if (id > -1)
    {
        printf("  -- Id: %lld\n", id);
    }

    daqString* value = NULL;
    daqUnit_getName(unit, &value);
    if (value != NULL && !isDaqStringEmpty(value))
    {
        printDaqFormattedString("  -- Unit name: %s\n", value);
        daqReleaseRef(value);
    }

    daqUnit_getQuantity(unit, &value);
    if (value != NULL && !isDaqStringEmpty(value))
    {
        printDaqFormattedString("  -- Quantity: %s\n", value);
        daqReleaseRef(value);
    }

    daqUnit_getSymbol(unit, &value);
    if (value != NULL && !isDaqStringEmpty(value))
    {
        printDaqFormattedString("  -- Symbol: %s\n", value);
        daqReleaseRef(value);
    }
}

void printDaqPropertyCallableInfo(daqCallableInfo* callableInfo)
{
    daqList* arguments = NULL;
    daqCallableInfo_getArguments(callableInfo, &arguments);

    printf("- Callable info:\n");

    daqSizeT count = 0;
    daqList_getCount(arguments, &count);

    daqBaseObject* temp = NULL;
    daqArgumentInfo* value = NULL;
    daqString* name = NULL;
    for (daqSizeT i = 0; i< count; i++)
    {
        // Arguments are a list of Argument Info Objects
        daqList_getItemAt(arguments, i, &temp);
        daqQueryInterface(temp, DAQ_ARGUMENT_INFO_INTF_ID, &value);
        daqReleaseRef(temp);

        daqArgumentInfo_getName(value, &name);
        printDaqFormattedString("  -- Argument: %s\n", name);
        daqReleaseRef(name);

        daqCoreType argumentType = daqCtUndefined;
        daqArgumentInfo_getType(value, &argumentType);
        printf("  -- Type: %s\n", daqCoreTypeToString(argumentType));
        
        daqReleaseRef(value);
    }

    daqReleaseRef(arguments);

    daqCoreType returnType = daqCtUndefined;
    daqCallableInfo_getReturnType(callableInfo, &returnType);
    if (returnType != daqCtUndefined)
        printf("  -- Return type: %s\n", daqCoreTypeToString(returnType));
}

void printDapPropertyName(daqProperty* property)
{
    daqString* name = NULL;
    daqProperty_getName(property, &name);
    printDaqFormattedString("Name: %s\n", name);
    daqReleaseRef(name);
}

void printDaqPropertyVisible(daqProperty* property)
{
    uint8_t visible = False;
    daqProperty_getVisible(property, &visible);
    printf("- Visible: %s\n", visible == True ? "True" : "False");
}

void printDaqPropertyReadOnly(daqProperty* property)
{
    uint8_t readOnly = False;
    daqProperty_getReadOnly(property, &readOnly);
    printf("- Read only: %s\n", readOnly == True ? "True" : "False");
}

void printNumber(daqNumber* number, enum daqExample_propertyType type)
{
    if (type == daqExample_propertyType_integer)
    {
        int64_t value = 0;
        daqNumber_getIntValue(number, &value);
        printf("%lld\n", value);
    }
    else if(type == daqExample_propertyType_floatObject)
    {
        double value = 0.0;
        daqNumber_getFloatValue(number, &value);
        printf("%f\n", value);
    }
}

void printDaqDict(daqBaseObject* value, daqCoreType keyType, daqCoreType itemType)
{
    daqDict* valueDict = NULL;
    daqQueryInterface(value, DAQ_DICT_INTF_ID, &valueDict);

    daqBaseObject* key = NULL;
    daqBaseObject* val = NULL;

    daqSizeT count = 0;
    daqDict_getCount(valueDict, &count);
    
    daqList* keys = NULL;
    daqDict_getKeyList(valueDict, &keys);

    printf("- Dictionary:\n");

    for(daqSizeT i = 0; i<count; i++)
    {
        daqList_getItemAt(keys, i, &key);
        daqDict_get(valueDict, key, &val);

        printf("  -- Key: ");
        printSimpleCoreTypeValue(key, keyType);
        printf("  -- Value: ");
        printSimpleCoreTypeValue(val, itemType);
        daqReleaseRef(key);
        daqReleaseRef(val);
    }
    daqReleaseRef(keys);
    daqReleaseRef(valueDict);
}

void printDaqList(daqBaseObject* value, daqCoreType itemType)
{
    daqList* valueList = NULL;
    daqQueryInterface(value, DAQ_LIST_INTF_ID, &valueList);

    daqBaseObject* val = NULL;

    daqSizeT count = 0;
    daqList_getCount(valueList, &count);

    printf("- List:\n");
    
    for(daqSizeT i = 0; i< count; i++)
    {
        daqList_getItemAt(valueList, i, &val);

        printf("  -- Value: ");
        printSimpleCoreTypeValue(val, itemType);
        daqReleaseRef(val);
    }
    daqReleaseRef(valueList);
}

void printDaqRatio(struct daqExample_Ratio native)
{
    printf("Ratio:\n");
    printf("  - Denominator: %lld\n", native.denominator);
    printf("  - Numerator: %lld\n", native.numerator);
}
