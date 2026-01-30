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

void functionProp(daqProperty* property);
void sparseSelectionProp(daqProperty* property);
void selectionProp(daqProperty* property);
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

// Missing handling for IntProp, SelectionProp, SparseSelectionProp, ReferenceProp, FunctionProp
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
    // Retrive metadata, check what type the value is...
    if (propertyValueTypeCheck(property, daqCtBool))
    {
        // We have to check the default value and the current value that the property is set to
    }

    daqBaseObject* defaultVisibleObj = NULL;
    daqProperty_getDefaultValue(property, &defaultVisibleObj);

    daqBool* defaultVisible = NULL;
    daqQueryInterface(property, DAQ_BOOLEAN_INTF_ID, &defaultVisible);


}

// FloatProperty
void floatProp(daqProperty* property)
{
    // Retrive metadata, check what type the value is...
    if (propertyValueTypeCheck(property, daqCtFloat))
    {
        // Min value, max value, default value, suggested values, visible
        daqNumber* minValueNum = NULL;
        daqProperty_getMinValue(property, &minValueNum);

        daqNumber* maxValueNum = NULL;
        daqProperty_getMaxValue(property, &maxValueNum);

        daqBaseObject* defaultValueObj = NULL;
        daqProperty_getDefaultValue(property, &defaultValueObj);

        if (defaultValueObj != NULL)
        {
            daqFloatObject* defaultValue = NULL;
            daqQueryInterface(defaultValueObj, DAQ_FLOAT_OBJECT_INTF_ID, &defaultValue);
            daqReleaseRef(defaultValueObj);
            daqFloat value = 0;
            daqFloatObject_getValue(defaultValue, &value);
            printf("- Default Value: %f\n", value);
            daqReleaseRef(defaultValue);
        }

        daqList* listOfRecommendedValues = NULL;
        daqProperty_getSuggestedValues(property, &listOfRecommendedValues);

        // Display min, max, default and suggested values
        if(minValueNum != NULL)
        {
            daqFloat value = 0;
            daqNumber_getFloatValue(minValueNum, &value);
            printf("- Minimum Value: %f\n", value);
            daqReleaseRef(minValueNum);
            if (maxValueNum != NULL)
            {
                value = 0;
                daqNumber_getFloatValue(maxValueNum, &value);
                printf("- Maximum Value: %f\n", value);
                daqReleaseRef(maxValueNum);
            }
        }
    }
}

// IntProperty
void intProp(daqProperty* property)
{
    daqNumber* minValueNum = NULL;
    daqProperty_getMinValue(property, &minValueNum);

    daqNumber* maxValueNum = NULL;
    daqProperty_getMaxValue(property, maxValueNum);

    daqBaseObject* defaultValueObj = NULL;
    daqProperty_getDefaultValue(property, &defaultValueObj);

    if (defaultValueObj != NULL)
    {
        daqInteger* defaultValueOb = NULL;
        daqQueryInterface(defaultValueObj, DAQ_INTEGER_INTF_ID, &defaultValueOb);
        daqInt defaultValue = 0;
        daqInteger_getValue(defaultValueOb, &defaultValue);
        daqReleaseRef(defaultValueOb);

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
    // Retrive metadata, check what type the value is...
    if(propertyValueTypeCheck(property, daqCtString))
    {
        // Default value, suggested values, visible
    }
    daqBaseObject* defaultStringObj = NULL;
    daqProperty_getDefaultValue(property, &defaultStringObj);
    daqString* defaultString = NULL;
    daqQueryInterface(defaultStringObj, DAQ_STRING_INTF_ID, &defaultString);
    printDaqFormattedString("\n- String default value: %s\n", defaultString);
    daqList* suggestedStrings = NULL;
    daqProperty_getSuggestedValues(property, &suggestedStrings);
    // Display
    daqSizeT count = 0;
    if (suggestedStrings)
        daqList_getCount(suggestedStrings, &count);
    daqBaseObject* currentObj = NULL;
    daqString* currentStr = NULL;

    for (daqSizeT i = 0; i< count; i++)
    {
        daqList_getItemAt(suggestedStrings, i, &currentObj);
        daqString* temp = daqQueryInterfacePtr(currentObj, DAQ_STRING_INTF_ID);

        printf("\n%s\n", openDAQStringConversion(temp));

        daqReleaseRef(temp);
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

    // Min and max are included here
    daqNumber* minValue = NULL;
    daqNumber* maxValue = NULL;
    daqProperty_getMinValue(property, &minValue);
    daqProperty_getMaxValue(property, &maxValue);

    // When displaying min/max, there needs to be an emptiness check

    daqBool visible = False;
    daqProperty_getVisible(property, &visible);
    // Not sure about suggested values....
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
    daqBool visible = False;
    daqProperty_getVisible(property, &visible);

    daqBaseObject* defaultObjectObj = NULL;
    daqProperty_getDefaultValue(property, &defaultObjectObj);

    // PropertyObject value
    daqBaseObject* valueObj = NULL;
    daqProperty_getValue(property, &valueObj);

    daqPropertyObject* defaultPropObj = NULL;
    daqQueryInterface(defaultObjectObj, DAQ_PROPERTY_OBJECT_INTF_ID, &defaultPropObj);

    // Check if empty and then rerun the objectProp on the defaultPropObj
}

// StructureProperty
void structProp(daqProperty* property)
{
    if(propertyValueTypeCheck(property, daqCtStruct))
    {}
    // Value is Struct Core Type
    daqBaseObject* defValueObj = NULL;
    daqProperty_getDefaultValue(property, &defValueObj);

    daqStruct* defValue = NULL;
    if (defValueObj)
        daqQueryInterface(defValueObj, DAQ_STRUCT_INTF_ID, &defValue);

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
            printDaqFormattedString("\n\nProperty name: %s", propName);
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
