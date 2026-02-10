#include <daq_c_conversions.h>

enum DAQ_PropType
{
    selection,
    sparseSelection,
    integer,
    floatObject,
    string,
    boolean,
    ratio,
    enumeration,
    structObject,
    function,
    list,
    dictionary,
    objectObject,
    complexNo,
    oh_no
};

void printSimpleCoreTypeValue(daqBaseObject* selectedValueObj, daqCoreType suggestedValuesItemType);
char* daqCoreTypeToString(daqCoreType type);
enum DAQ_PropType daq_getPropType(daqProperty* property);
void printPropertyValue(daqBaseObject* value, daqProperty* property);
void printPropertyMetadata(daqProperty* property);
void printUnit(daqUnit* unit);
void printCallableInfo(daqCallableInfo* callableInfo);
void printName(daqProperty* property);
void printVisible(daqProperty* property);
void printReadOnly(daqProperty* property);
void printNumber(daqNumber* number, enum DAQ_PropType type);
void printDaqDict(daqBaseObject* value, daqCoreType keyType, daqCoreType itemType);
void printDaqList(daqBaseObject* value, daqCoreType itemType);
void printRatio(struct Ratio native);

void printSimpleCoreTypeValue(daqBaseObject* selectedValueObj, daqCoreType suggestedValuesItemType)
{
    switch (suggestedValuesItemType)
    {
    case daqCtInt:
    {
        daqInteger* value = NULL;
        daqQueryInterface(selectedValueObj, DAQ_INTEGER_INTF_ID, &value);
        printf("%lld\n", daq_fromDaqInteger(value));
        daqReleaseRef(value);
        return;
    }
    case daqCtFloat:
    {
        daqFloatObject* value = NULL;
        daqQueryInterface(selectedValueObj, DAQ_FLOAT_OBJECT_INTF_ID, &value);
        printf("%f\n", daq_fromDaqFloat(value));
        daqReleaseRef(value);
    }
    case daqCtBool:
    {
        daqBoolean* value = NULL;
        daqQueryInterface(selectedValueObj, DAQ_BOOLEAN_INTF_ID, &value);
        printf("%u\n", daq_fromDaqBoolean(value));
        daqReleaseRef(value);
    }
    case daqCtString:
    {
        daqString* value = NULL;
        daqQueryInterface(selectedValueObj, DAQ_STRING_INTF_ID, &value);
        printDaqFormattedString("%s\n", value);
        daqReleaseRef(value);
    }
    case daqCtRatio:
    {
        daqRatio* value = NULL;
        daqQueryInterface(selectedValueObj, DAQ_RATIO_INTF_ID, &value);
        printRatio(daq_fromDaqRatio(value));
        daqReleaseRef(value);
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

enum DAQ_PropType daq_getPropType(daqProperty* property)
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
                return sparseSelection;
            }
            else if (DAQ_SUPPORTS_INTERFACE(temp, DAQ_LIST_INTF_ID))
            {
                daqReleaseRef(temp);
                return selection;
            }
            daqReleaseRef(temp);
        }
        return integer;
    }

    case daqCtString:
        return string;

    case daqCtBool:
        return boolean;

    case daqCtFloat:
        return floatObject;

    case daqCtProc:
    case daqCtFunc:
        return function;

    case daqCtObject:
        return objectObject;

    case daqCtStruct:
        return structObject;

    case daqCtRatio:
        return ratio;

    case daqCtComplexNumber:
        return complexNo;

    case daqCtList:
        return list;

    case daqCtDict:
        return dictionary;

    default:
        return oh_no;
    }
}

void printPropertyValue(daqBaseObject* value, daqProperty* property)
{
    enum DAQ_PropType propType = daq_getPropType(property);

    switch (propType)
    {
    case integer:
    case string:
    case floatObject:
    case ratio:
    {
        daqCoreType valueType;
        daqProperty_getValueType(property, &valueType);
        printSimpleCoreTypeValue(value, valueType);
        break;
    }
    case function:
    {
        printf("Function\n");
        break;
    }
    case selection:
    {
        daqInteger* val = NULL;
        daqQueryInterface(value, DAQ_INTEGER_INTF_ID, &val);

        daqBaseObject* temp = NULL;
        daqProperty_getSelectionValues(property, &temp);
        daqList* selectionValues = NULL;
        daqQueryInterface(temp, DAQ_LIST_INTF_ID, &selectionValues);
        daqReleaseRef(temp);

        daqList_getItemAt(selectionValues, daq_fromDaqInteger(val), &temp);
        daqReleaseRef(val);
        daqReleaseRef(selectionValues);

        daqCoreType itemType = daqCtUndefined;
        daqProperty_getItemType(property, &itemType);
        printSimpleCoreTypeValue(temp, itemType);
        daqReleaseRef(temp);
        break;
    }
    case sparseSelection:
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
    case dictionary:
    {
        daqCoreType itemType = daqCtUndefined;
        daqCoreType keyType = daqCtUndefined;
        daqProperty_getItemType(property, &itemType);
        daqProperty_getKeyType(property, &keyType);
        printDaqDict(value, keyType, itemType);
        break;
    }
    case list:
    {
        daqCoreType itemType = daqCtUndefined;
        daqProperty_getItemType(property, &itemType);
        printDaqList(value, itemType);
        break;
    }
    case objectObject:
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
    enum DAQ_PropType type = daq_getPropType(property);

    // Strings
    printName(property);

    daqString* description = NULL;
    daqProperty_getDescription(property, &description);
    if (description != NULL)
    {
        printDaqFormattedString("- Description: %s\n", description);
        daqReleaseRef(description);
    }

    // Booleans
    printVisible(property);
    printReadOnly(property);

    // Value Type
    daqCoreType coreType = daqCtUndefined;
    daqProperty_getValueType(property, &coreType);
    printf("Value type: %s\n", daqCoreTypeToString(coreType));

    // Default Value
    daqBaseObject* defaultValue = NULL;
    daqProperty_getDefaultValue(property, &defaultValue);
    printf("Default value: ");
    printPropertyValue(defaultValue, property);
    daqReleaseRef(defaultValue);

    if (type == dictionary)
    {
        daqProperty_getKeyType(property, &coreType);
        printf("- Key type: %s\n", daqCoreTypeToString(coreType));
    }

    if (type == dictionary || type == list)
    {
        daqProperty_getItemType(property, &coreType);
        printf("- Item type: %s\n", daqCoreTypeToString(coreType));
    }

    if (type == integer || type == floatObject)
    {
        daqNumber* min = NULL;
        daqNumber* max = NULL;
        daqProperty_getMinValue(property, &min);
        daqProperty_getMaxValue(property, &max);
        if (min != NULL)
        {
            printf("- Min value:");
            printNumber(min, type);
        }
        if (max != NULL)
        {
            printf("- Max value:");
            printNumber(max, type);
        }
    }

    if (type == selection)
    {
        daqBaseObject* temp = NULL;
        daqProperty_getSelectionValues(property, &temp);
        daqProperty_getItemType(property, &coreType);
        printf("- Selection values:\n");
        printDaqList(temp, coreType);
        daqReleaseRef(temp);
    }

    if (type == sparseSelection)
    {
        daqBaseObject* temp = NULL;
        daqProperty_getSelectionValues(property, &temp);
        daqProperty_getItemType(property, &coreType);
        printf("- Selection values: \n");
        printDaqDict(temp, daqCtInt, coreType);
        daqReleaseRef(temp);
    }

    if (type == integer || type == string || type == floatObject)
    {
        daqBaseObject* temp = NULL;
        daqProperty_getSuggestedValues(property, &temp);
        if (temp != NULL)
        {
            daqProperty_getValueType(property, &coreType);
            printf("- Suggested values:\n");
            printDaqList(temp, coreType);
            daqReleaseRef(temp);
        }
    }

    if (type == integer || type == floatObject)
    {
        daqUnit* unit = NULL;
        daqProperty_getUnit(property, &unit);
        if (unit != NULL)
        {
            printUnit(unit);
            daqReleaseRef(unit);
        }
    }

    if (type == function)
    {
        daqCallableInfo* callableInfo = NULL;
        daqProperty_getCallableInfo(property, &callableInfo);
        printCallableInfo(callableInfo);
        daqReleaseRef(callableInfo);
    }
}

void printUnit(daqUnit* unit)
{
    printf("- Unit:\n");
    int64_t id = 0;
    daqUnit_getId(unit, &id);
    if (id >-1)
    {
        printf("  -- Id: %lld\n", id);
    }

    daqString* value = NULL;
    daqUnit_getName(unit, &value);
    if (value != NULL)
    {
        printDaqFormattedString("  -- Unit name: %s\n", value);
        daqReleaseRef(value);
    }

    daqUnit_getQuantity(unit, &value);
    if (value != NULL)
    {
        printDaqFormattedString("  -- Quantity: %s\n", value);
        daqReleaseRef(value);
    }

    daqUnit_getSymbol(unit, &value);
    if (value != NULL)
    {
        printDaqFormattedString("  -- Symbol: %s\n", value);
        daqReleaseRef(value);
    }
}

void printCallableInfo(daqCallableInfo* callableInfo)
{
    // Argument Info
}

void printName(daqProperty* property)
{
    daqString* name = NULL;
    daqProperty_getName(property, &name);
    printDaqFormattedString("Name: %s\n", name);
    daqReleaseRef(name);
}

void printVisible(daqProperty* property)
{
    uint8_t visible = False;
    daqProperty_getVisible(property, &visible);
    printf("- Visible: %s\n", visible == True ? "True" : "False");
}

void printReadOnly(daqProperty* property)
{
    uint8_t readOnly = False;
    daqProperty_getReadOnly(property, &readOnly);
    printf("- Read only: %s\n", readOnly == True ? "True" : "False");
}

void printNumber(daqNumber* number, enum DAQ_PropType type)
{
    if (type == integer)
    {
        int64_t value = 0;
        daqNumber_getIntValue(number, &value);
        printf("%lld\n", value);
    }
    else if(type == floatObject)
    {
        double value = 0.0;
        daqNumber_getFloatValue(number, &value);
        printf("%f\n", value);
    }
}

void printDaqDict(daqBaseObject* value, daqCoreType keyType, daqCoreType itemType)
{

}

void printDaqList(daqBaseObject* value, daqCoreType itemType)
{

}

void printRatio(struct Ratio native)
{
    printf("Ratio:\n");
    printf("  - Denominator: %lld\n", native.denominator);
    printf("  - Numerator: %lld\n", native.numerator);
}
