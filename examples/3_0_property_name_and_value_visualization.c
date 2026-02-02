/*
 * This example demonstrates how to search through properties, 
 * determine their (core)type and display their value.
 */

#include <daq_c_conversions.h>

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

void displayPropertyTypes(daqPropertyObject* propertyObject);
void functionProp(daqProperty* property);
void sparseSelectionProp(daqProperty* property);
void selectionProp(daqProperty* property);
void objectProp(daqProperty* property);
void boolProp(daqProperty* property);
void floatProp(daqProperty* property);
void intProp(daqProperty* property);
void stringProp(daqProperty* property);
void ratioProp(daqProperty* property);
void listProp(daqProperty* property);
void dictProp(daqProperty* property);
void structProp(daqProperty* property);
void enumProp(daqProperty* property);
void prop(daqProperty* property);

int propertyValueTypeCheck(daqProperty* property, daqCoreType coreType)
{
    daqCoreType propCoreType = daqCtUndefined;
    daqProperty_getValueType(property, &propCoreType);

    if (propCoreType == coreType)
        return 1;
    return 0;
}

// Note on universal attributes (default value, read-only, visible, ...):
// - They will be seen on all visualizations of properties
// - 

void displayCommonMetadata(daqProperty* property)
{
    // Read-only
    // Visible
    daqBool visible = False;
    daqBool readOnly = False;
    daqProperty_getVisible(property, &visible);
    daqProperty_getReadOnly(property, &readOnly);

    printf("\n\n- Visible: %s\n", visible == True ? "True" : "False");
    printf("- ReadOnly: %s\n", readOnly == True ? "True" : "False");
}

void determinePropertyTypeAndPrint(daqProperty* property)
{
    displayCommonMetadata(property);
    daqCoreType propCoreType = daqCtUndefined;
    daqProperty_getValueType(property, &propCoreType);

    switch(propCoreType)
    {
        case daqCtBool:
        {
            boolProp(property);
            break;
        }
        case daqCtFloat:
        {
            floatProp(property);
            break;
        }
        case daqCtString:
        {
            stringProp(property);
            break;
        }
        case daqCtList:
        {
            listProp(property);
            break;
        }
        case daqCtDict:
        {
            dictProp(property);
            break;
        }
        case daqCtRatio:
        {
            ratioProp(property);
            break;
        }
        case daqCtFunc:
        {
            // Function prop
            functionProp(property);
            break;
        }
        case daqCtProc:
        {
            // Function prop w/ no output
            functionProp(property);
            break;
        }
        case daqCtObject:
        {
            objectProp(property);
            break;
        }
        case daqCtStruct:
        {
            structProp(property);
            break;
        }
        case daqCtEnumeration:
        {
            enumProp(property);
            break;
        }
        case daqCtInt:
        {
            // Check between selection, sparse selection and int property
            daqBaseObject* selectionValues = NULL;
            daqProperty_getSelectionValues(property, &selectionValues);
            if (selectionValues == NULL)
            {
                intProp(property);
                break;
            }

            if(DAQ_SUPPORTS_INTERFACE(selectionValues, DAQ_DICT_INTF_ID))
                sparseSelectionProp(property);
            else
                selectionProp(property);

            break;
        }
        default:
        {
            prop(property);
            break;
        }
    }
}

// FunctionProperty (also handles ProcedureProperty)
void functionProp(daqProperty* property)
{
    daqCallableInfo* callInfo = NULL;
    daqProperty_getCallableInfo(property, &callInfo);
}

// SparseSelectionProperty
void sparseSelectionProp(daqProperty* property)
{

}

// SelectionProperty
void selectionProp(daqProperty* property)
{
    
}

// Property
void prop(daqProperty* property)
{

}

// BoolProperty
void boolProp(daqProperty* property)
{
    daqBaseObject* temp = NULL;
    daqBoolean* defaultValue = NULL;
    daqProperty_getDefaultValue(property, &temp);
    daqQueryInterface(temp, DAQ_BOOLEAN_INTF_ID, &defaultValue);
    daqReleaseRef(temp);

    daqBoolean* value = NULL;
    daqProperty_getValue(property, &temp);
    daqQueryInterface(temp, DAQ_BOOLEAN_INTF_ID, &value);
    daqReleaseRef(temp);

    daqBool nativeValue = False;
    daqBoolean_getValue(value, &nativeValue);
    daqReleaseRef(value);
    daqBool nativeDefaultValue = False;
    daqBoolean_getValue(defaultValue, &nativeDefaultValue);
    daqReleaseRef(defaultValue);

    printf("- Value: %s\n", nativeValue == True ? "True" : "False");
    printf("- Default Value: %s\n", nativeDefaultValue == True ? "True" : "False");
}

// FloatProperty
void floatProp(daqProperty* property)
{
    // Min value, max value, default value, suggested values, visible
    daqNumber* minValueNum = NULL;
    daqProperty_getMinValue(property, &minValueNum);

    daqNumber* maxValueNum = NULL;
    daqProperty_getMaxValue(property, &maxValueNum);

    daqBaseObject* defaultValueObj = NULL;
    daqProperty_getDefaultValue(property, &defaultValueObj);

    daqBaseObject* valueObj = NULL;
    daqProperty_getValue(property, &valueObj);
    daqFloatObject* value = NULL;
    daqQueryInterface(valueObj, DAQ_FLOAT_OBJECT_INTF_ID, &value);
    daqReleaseRef(valueObj);

    daqFloat valueNative = 0;
    daqFloatObject_getValue(value, &valueNative);
    daqReleaseRef(value);

    printf("- Value: %f\n", valueNative);

    valueNative = 0;
    if (defaultValueObj != NULL)
    {
        daqQueryInterface(defaultValueObj, DAQ_FLOAT_OBJECT_INTF_ID, &value);
        daqReleaseRef(defaultValueObj);
        daqFloatObject_getValue(value, &valueNative);
        printf("- Default Value: %f\n", valueNative);
        daqReleaseRef(value);
    }

    daqList* listOfRecommendedValues = NULL;
    daqProperty_getSuggestedValues(property, &listOfRecommendedValues);

    // Display min, max, default and suggested values
    if(minValueNum != NULL)
    {
         valueNative = 0;
        daqNumber_getFloatValue(minValueNum, &valueNative);
        printf("- Minimum Value: %f\n", valueNative);
        daqReleaseRef(minValueNum);
        if (maxValueNum != NULL)
        {
            valueNative = 0;
            daqNumber_getFloatValue(maxValueNum, &valueNative);
            printf("- Maximum Value: %f\n", valueNative);
            daqReleaseRef(maxValueNum);
        }
    }
}

// IntProperty
void intProp(daqProperty* property)
{
    daqNumber* minValueNum = NULL;
    daqProperty_getMinValue(property, &minValueNum);

    daqNumber* maxValueNum = NULL;
    daqProperty_getMaxValue(property, &maxValueNum);

    daqBaseObject* defaultValueObj = NULL;
    daqProperty_getDefaultValue(property, &defaultValueObj);

    daqBaseObject* valueObj = NULL;
    daqProperty_getValue(property, &valueObj);
    daqInteger* value = NULL;
    daqQueryInterface(valueObj, DAQ_INTEGER_INTF_ID, &value);
    daqInt valueNative = openDAQIntConversion(value);
    daqReleaseRef(value);
    daqReleaseRef(valueObj);

    printf("- Value: %lld\n", valueNative);

    if (defaultValueObj != NULL)
    {
        daqInteger* defaultValueOb = NULL;
        daqQueryInterface(defaultValueObj, DAQ_INTEGER_INTF_ID, &defaultValueOb);
        daqInt defaultValue = 0;
        daqInteger_getValue(defaultValueOb, &defaultValue);
        daqReleaseRef(defaultValueOb);
        daqReleaseRef(defaultValueObj);
        printf("- Default value: %lld\n", defaultValue);
    }
    daqList* listOfSuggestedValues = NULL;
    daqProperty_getSuggestedValues(property, &listOfSuggestedValues);

    // Display attributes if they are not empty
    if (minValueNum != NULL)
    {
        daqInt minValue = 0;
        daqNumber_getIntValue(minValueNum, &minValue);
        printf("- Minimum value: %lld\n", minValue);
        daqReleaseRef(minValueNum);
        if (maxValueNum != NULL)
        {
            daqInt maxValue = 0;
            daqNumber_getIntValue(maxValueNum, &maxValue);
            printf("- Maximum value: %lld\n", maxValue);
            daqReleaseRef(maxValueNum);
        }
    }
    if(listOfSuggestedValues!=NULL)
        daqReleaseRef(listOfSuggestedValues);
}

// StringProperty
void stringProp(daqProperty* property)
{
    daqBaseObject* valueObj = NULL;
    daqProperty_getValue(property, &valueObj);
    
    daqString* value = NULL;
    daqQueryInterface(valueObj, DAQ_STRING_INTF_ID, &value);
    printDaqFormattedString("- Value: %s\n", value);
    daqReleaseRef(value);
    daqReleaseRef(valueObj);

    daqProperty_getDefaultValue(property, &valueObj);
    daqQueryInterface(valueObj, DAQ_STRING_INTF_ID, &value);
    printDaqFormattedString("- Default value: %s\n", value);
    daqList* suggestedStrings = NULL;
    daqProperty_getSuggestedValues(property, &suggestedStrings);
    daqReleaseRef(value);
    daqReleaseRef(valueObj);

    daqSizeT count = 0;
    if (suggestedStrings)
        daqList_getCount(suggestedStrings, &count);
    daqBaseObject* currentObj = NULL;

    for (daqSizeT i = 0; i< count; i++)
    {
        daqList_getItemAt(suggestedStrings, i, &currentObj);
        value = daqQueryInterfacePtr(currentObj, DAQ_STRING_INTF_ID);

        printf("\n%s\n", openDAQStringConversion(value));

        daqReleaseRef(value);
        daqReleaseRef(currentObj);
    }
}

// RatioProperty
void ratioProp(daqProperty* property)
{
    // Retrive metadata, check what type the value is...
    daqBaseObject* defaultRatioObj = NULL;
    daqProperty_getDefaultValue(property, &defaultRatioObj);
    daqRatio* defaultRatio = NULL;
    daqQueryInterface(defaultRatioObj, DAQ_RATIO_INTF_ID, &defaultRatio);
    daqReleaseRef(defaultRatioObj);

    // Default value Display
    if (defaultRatio == NULL)
        return;

    daqInt denominator = 0;
    daqInt numerator = 0;
    daqRatio_getDenominator(defaultRatio, &denominator);
    daqRatio_getNumerator(defaultRatio, &numerator);

    printf("- Default denominator: %lld\n", denominator);
    printf("- Default numerator: %lld\n", numerator);

    daqReleaseRef(defaultRatio);
}

// ListProperty
void listProp(daqProperty* property)
{
    daqBool visible = False;
    daqProperty_getVisible(property, &visible);

    daqCoreType itemType = daqCtUndefined;
    daqProperty_getItemType(property, &itemType);
}

// DictProperty
void dictProp(daqProperty* property)
{
    daqBool visible = False;
    daqProperty_getVisible(property, &visible);

    daqCoreType itemType = daqCtUndefined;
    daqProperty_getItemType(property, &itemType);

    daqCoreType keyType = daqCtUndefined;
    daqProperty_getKeyType(property, &keyType);
}

// ObjectProperty
void objectProp(daqProperty* property)
{
    daqBaseObject* defaultObjectObj = NULL;
    daqProperty_getDefaultValue(property, &defaultObjectObj);

    // PropertyObject value
    daqBaseObject* valueObj = NULL;
    daqProperty_getValue(property, &valueObj);

    daqPropertyObject* defaultPropObj = NULL;
    daqQueryInterface(defaultObjectObj, DAQ_PROPERTY_OBJECT_INTF_ID, &defaultPropObj);

    // Display both (default Value and actual value, even if they are the same)
    if (valueObj != NULL)
    {
        daqPropertyObject* value = NULL;
        daqQueryInterface(valueObj, DAQ_PROPERTY_OBJECT_INTF_ID, &value);
        printf("\n--- Object property Value ---\n");
        displayPropertyTypes(value);
        daqReleaseRef(value);
        if (defaultPropObj != NULL)
        {
            daqQueryInterface(defaultPropObj, DAQ_PROPERTY_OBJECT_INTF_ID, &value);
            printf("\n--- Object property DEFAULT Value ----\n");
            displayPropertyTypes(value);
            daqReleaseRef(value);
        }
        printf("\n---\n");
    }
}

// StructureProperty
void structProp(daqProperty* property)
{
    // Value is Struct Core Type
    daqBaseObject* defValueObj = NULL;
    daqProperty_getDefaultValue(property, &defValueObj);

    daqStruct* defValue = NULL;
    daqQueryInterface(defValueObj, DAQ_STRUCT_INTF_ID, &defValue);

    daqBaseObject* valueObj = NULL;
    daqProperty_getValue(property, &valueObj);

    daqStruct* value = NULL;
    daqQueryInterface(valueObj, DAQ_STRUCT_INTF_ID, &value);

    daqList* valueNames = NULL;
    daqStruct_getFieldNames(value, &valueNames);
    // daqString* name = NULL;
    daqSizeT count = 0;
    daqBaseObject* temp = NULL;
    daqList_getCount(valueNames, &count);
    for (daqSizeT i = 0; i< count;i++)
    {
        daqList_getItemAt(valueNames, i, &temp);

    }
    daqReleaseRef(valueNames);
    daqReleaseRef(value);
    daqReleaseRef(defValueObj);
}

// EnumerationProperty
void enumProp(daqProperty* property)
{
    daqBaseObject* selectionValueObj = NULL;
    daqProperty_getSelectionValues(property, &selectionValueObj);

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
            daqString* propName = NULL;
            daqProperty_getName(prop, &propName);
            printDaqFormattedString("\nProperty name: %s", propName);
            daqReleaseRef(propName);

            if (propCoreType == daqCtObject)
            {
                // Write them up first
                determinePropertyTypeAndPrint(prop);
            }
            else if (propCoreType == daqCtUndefined)
            {
                // Maybe needed (don't know yet)
            }
            else
                determinePropertyTypeAndPrint(prop);
            
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
