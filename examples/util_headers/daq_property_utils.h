#include <daq_c_conversions.h>

enum daqExample_PropertyType
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
    daqExample_propertyType_unknown
};

void printSimpleCoreTypeValue(daqBaseObject* selectedValueObj, daqCoreType suggestedValuesItemType);
char* daqCoreTypeToString(daqCoreType type);
enum daqExample_PropertyType daq_getPropType(daqProperty* property);
void printPropertyValueOrDefault(daqProperty* property, daqBool defaultValue);
void printPropertyMetadata(daqProperty* property);
void printDaqNumber(daqNumber* number, enum daqExample_PropertyType type);
void printDaqDict(daqBaseObject* value, daqCoreType keyType, daqCoreType itemType);
void printDaqList(daqBaseObject* value, daqCoreType itemType);
void printDaqRatio(struct daqExample_Ratio native);
void printSelectedListValue(daqProperty* property, daqBool defaultValue);
void printSelectedDictValue(daqProperty* property, daqBool defaultValue);
void printDaqPropertyName(daqProperty* property);
void printDaqPropertyDescription(daqProperty* property);
void printDaqPropertyVisible(daqProperty* property);
void printDaqPropertyReadOnly(daqProperty* property);
void printDaqPropertyValueType(daqProperty* property);
void printDaqPropertyDefaultValue(daqProperty* property);
void printDaqPropertyKeyType(daqProperty* property);
void printDaqPropertyItemType(daqProperty* property);
void printDaqPropertyMinMaxValue(daqProperty* property, enum daqExample_PropertyType propertyType);
void printDaqPropertySelectionValues(daqProperty* property);
void printDaqPropertySparseSelectionValues(daqProperty* property);
void printDaqPropertySuggestedValues(daqProperty* property);
void printDaqPropertyUnit(daqProperty* property);
void printDaqPropertyCallableInfo(daqProperty* property);

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
        daqConstCharPtr value = NULL;
        daqBaseObject_toString(selectedValueObj, &value);
        printf("%s\n", value);
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

enum daqExample_PropertyType daq_getPropType(daqProperty* property)
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
        return daqExample_propertyType_unknown;
    }
}

void printPropertyValueOrDefault(daqProperty* property, daqBool defaultValue)
{
    daqBaseObject* value = NULL;

    if (defaultValue)
        daqProperty_getDefaultValue(property, &value);
    else
        daqProperty_getValue(property, &value);

    enum daqExample_PropertyType propType = daq_getPropType(property);

    switch (propType)
    {
    case daqExample_propertyType_integer:
    {
        printSimpleCoreTypeValue(value, daqCtInt);
        break;
    }
    case daqExample_propertyType_string:
    {
        printSimpleCoreTypeValue(value, daqCtString);
        break;
    }
    case daqExample_propertyType_boolean:
    {
        printSimpleCoreTypeValue(value, daqCtBool);
        break;
    }
    case daqExample_propertyType_floatObject:
    {
        printSimpleCoreTypeValue(value, daqCtFloat);
        break;
    }
    case daqExample_propertyType_ratio:
    {
        printSimpleCoreTypeValue(value, daqCtRatio);
        break;
    }
    case daqExample_propertyType_function:
    {
        printf("Function\n");
        if (value != NULL)
            daqReleaseRef(value);
        return;
    }
    case daqExample_propertyType_selection:
    {
        printSelectedListValue(property, defaultValue);
        break;
    }
    case daqExample_propertyType_sparseSelection:
    {
        printSelectedDictValue(property, defaultValue);
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
        if (value != NULL)
            daqReleaseRef(value);
        return;
    default:
    {
        printf("Property Value\n");
        if (value != NULL)
            daqReleaseRef(value);
        return;
    }
    }
    daqReleaseRef(value);
}

void printPropertyMetadata(daqProperty* property)
{
    enum daqExample_PropertyType propertyType = daq_getPropType(property);

    // Name
    /*
     * Mandatory field for all types of properties.
     * Never empty.
     */
    printDaqPropertyName(property);

    // Description
    /*
     * Optional field for all types of properties.
     * Can be empty.
     */
    printDaqPropertyDescription(property);

    // Visible
    /*
     * Mandatory field for all types of properties.
     * When calling getAllVisibleProperties will always be True.
     * Never empty.
     */
    printDaqPropertyVisible(property);

    // Read-only
    /*
     * Mandatory field for all types of properties.
     * Never empty.
     */
    printDaqPropertyReadOnly(property);

    // Value Type
    /*
     * Mandatory field for all types of properties.
     * Never empty.
     */
    printDaqPropertyValueType(property);

    // Default Value
    /*
     * Mandatory field for all aside from 
     * Function and Procedure Properties.
     * Will never be empty, aside from 
     * Function/Procedure Property where it is empty.
     */
    printDaqPropertyDefaultValue(property);

    // Key type
    /*
     * Mandatory field for a Dictionary property.
     * Will never be empty when required.
     */
    if (propertyType == daqExample_propertyType_dictionary)
        printDaqPropertyKeyType(property);

    // Item type
    /*
     * Mandatory field for Dictionary and List property.
     * Will never be empty when required.
     */
    if (propertyType == daqExample_propertyType_dictionary || propertyType == daqExample_propertyType_list)
        printDaqPropertyItemType(property);

    // Min/Max value
    /*
     * Optional field for Integer and Float Property.
     * May be empty even in Integer or Float Property.
     */
    if (propertyType == daqExample_propertyType_integer || propertyType == daqExample_propertyType_floatObject)
        printDaqPropertyMinMaxValue(property, propertyType);

    // Selection values (list)
    /*
     * Mandatory field for Selection Property.
     * Will never be empty when it is required.
     */
    if (propertyType == daqExample_propertyType_selection)
        printDaqPropertySelectionValues(property);

    // Selection values (dictionary)
    /*
     * Mandatory field for Sparse Selection Property.
     * Will never be empty when it is required.
     */
    if (propertyType == daqExample_propertyType_sparseSelection)
        printDaqPropertySparseSelectionValues(property);

    // Suggested values
    /*
     * Optional field for Integer, Float and String Property.
     * Can be empty even in Integer, Float and String Property.
     */
    if (propertyType == daqExample_propertyType_integer || propertyType == daqExample_propertyType_string || propertyType == daqExample_propertyType_floatObject)
        printDaqPropertySuggestedValues(property);

    // Unit
    /*
     * Optional field for Integer and Float Property.
     * Can be even in Integer and Float Property.
     */
    if (propertyType == daqExample_propertyType_integer || propertyType == daqExample_propertyType_floatObject)
        printDaqPropertyUnit(property);

    // Callable info
    /*
     * Mandatoty field for Function and Procedure Property.
     * Will never be empty when required. 
     * The return field within Callable Info will be defined 
     * only when the Property type is Function.
     */
    if (propertyType == daqExample_propertyType_function)
        printDaqPropertyCallableInfo(property);
}

void printDaqNumber(daqNumber* number, enum daqExample_PropertyType type)
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

void printDaqRatio(struct daqExample_Ratio ratio)
{
    printf("Ratio:\n");
    printf("  - Denominator: %lld\n", ratio.denominator);
    printf("  - Numerator: %lld\n", ratio.numerator);
}

void printSelectedListValue(daqProperty* property, daqBool defaultValue)
{
    daqBaseObject* value = NULL;

    if (defaultValue)
        daqProperty_getDefaultValue(property, &value);
    else
        daqProperty_getValue(property, &value);

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

    daqReleaseRef(value);
}

void printSelectedDictValue(daqProperty* property, daqBool defaultValue)
{
    daqBaseObject* value = NULL;

    if (defaultValue)
        daqProperty_getDefaultValue(property, &value);
    else
        daqProperty_getValue(property, &value);

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

    daqReleaseRef(value);
}

void printDaqPropertyName(daqProperty* property)
{
    daqString* name = NULL;
    daqProperty_getName(property, &name);
    printDaqFormattedString("Name: %s\n", name);
    daqReleaseRef(name);
}

void printDaqPropertyDescription(daqProperty* property)
{
    daqString* description = NULL;
    daqProperty_getDescription(property, &description);
    
    if (description == NULL)
        return;

    printDaqFormattedString("- Descriprion: %s\n", description);
    daqReleaseRef(description);
    
}

void printDaqPropertyVisible(daqProperty* property)
{
    daqBool visible = False;
    daqProperty_getVisible(property, &visible);
    printf("- Visible: %s\n", visible == True ? "True" : "False");
}

void printDaqPropertyReadOnly(daqProperty* property)
{
    daqBool readOnly = False;
    daqProperty_getReadOnly(property, &readOnly);
    printf("- Read only: %s\n", readOnly == True ? "True" : "False");
}

void printDaqPropertyValueType(daqProperty* property)
{
    daqCoreType coreType = daqCtUndefined;
    daqProperty_getValueType(property, &coreType);
    printf("- Value type: %s\n", daqCoreTypeToString(coreType));
}

void printDaqPropertyDefaultValue(daqProperty* property)
{
    printf("- Default value:");
    printPropertyValueOrDefault(property, True);
}

void printDaqPropertyKeyType(daqProperty* property)
{
    daqCoreType coreType = daqCtUndefined;
    daqProperty_getKeyType(property, &coreType);
    printf("- Key type: %s\n", daqCoreTypeToString(coreType));
}

void printDaqPropertyItemType(daqProperty* property)
{
    daqCoreType coreType = daqCtUndefined;
    daqProperty_getItemType(property, &coreType);
    printf("- Item type: %s\n", daqCoreTypeToString(coreType));
}

void printDaqPropertyMinMaxValue(daqProperty* property, enum daqExample_PropertyType propertyType)
{
    daqNumber* min = NULL;
    daqNumber* max = NULL;
    daqProperty_getMinValue(property, &min);
    daqProperty_getMaxValue(property, &max);
    if (min != NULL)
    {
        printf("- Min value:");
        printDaqNumber(min, propertyType);
        daqReleaseRef(min);
    }
    if (max != NULL)
    {
        printf("- Max value:");
        printDaqNumber(max, propertyType);
        daqReleaseRef(max);
    }
}

void printDaqPropertySelectionValues(daqProperty* property)
{
    daqCoreType coreType = daqCtUndefined;
    daqBaseObject* selectionValues = NULL;
    daqProperty_getSelectionValues(property, &selectionValues);
    daqProperty_getItemType(property, &coreType);
    printf("- Selection values:\n");
    printDaqList(selectionValues, coreType);
    daqReleaseRef(selectionValues);
}

void printDaqPropertySparseSelectionValues(daqProperty* property)
{
    daqCoreType coreType = daqCtUndefined;
    daqBaseObject* selectionValues = NULL;
    daqProperty_getSelectionValues(property, &selectionValues);
    daqProperty_getItemType(property, &coreType);
    printf("- Selection values: \n");
    printDaqDict(selectionValues, daqCtInt, coreType);
    daqReleaseRef(selectionValues);
}

void printDaqPropertySuggestedValues(daqProperty* property)
{
    daqList* suggestedValues = NULL;
    daqProperty_getSuggestedValues(property, &suggestedValues);

    if (suggestedValues == NULL)
        return;

    daqCoreType coreType = daqCtUndefined;
    daqProperty_getValueType(property, &coreType);
    printf("- Suggested values:\n");
    printDaqList((daqBaseObject*) suggestedValues, coreType);
    daqReleaseRef(suggestedValues);
}

void printDaqPropertyUnit(daqProperty* property)
{
    daqUnit* unit = NULL;
    daqProperty_getUnit(property, &unit);

    if (unit == NULL)
        return;

    printf("- Unit:\n");
    int64_t id = 0;
    daqUnit_getId(unit, &id);
    if (id > -1)
    {
        printf("  -- Id: '%lld'\n", id);
    }

    daqString* value = NULL;
    daqUnit_getName(unit, &value);
    if (value != NULL)
    {
        printDaqFormattedString("  -- Unit name: '%s'\n", value);
        daqReleaseRef(value);
    }

    daqUnit_getQuantity(unit, &value);
    if (value != NULL)
    {
        printDaqFormattedString("  -- Quantity: '%s'\n", value);
        daqReleaseRef(value);
    }

    daqUnit_getSymbol(unit, &value);
    if (value != NULL)
    {
        printDaqFormattedString("  -- Symbol: '%s'\n", value);
        daqReleaseRef(value);
    }
    daqReleaseRef(unit);
}

void printDaqPropertyCallableInfo(daqProperty* property)
{
    daqCallableInfo* callableInfo = NULL;
    daqProperty_getCallableInfo(property, &callableInfo);
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

    daqReleaseRef(callableInfo);
}
