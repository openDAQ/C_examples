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

int propertyValueTypeCheck(daqProperty* property, daqCoreType coreType)
{
    daqCoreType propCoreType = daqCtUndefined;
    daqProperty_getValueType(property, propCoreType);

    if (propCoreType == coreType)
        return 1;
    return 0;
}

// Missing handling for IntProp, SelectionProp, SparseSelectionProp, ReferenceProp, FunctionProp
// Note on universal attributes (default value, read-only, visible, ...):
// - They will be seen on all visualizations of properties
// - 


// BoolProperty
void boolProp(daqProperty* property)
{
    // Retrive metadata, check what type the value is...
    if (propertyValueTypeCheck(property, daqCtBool))
    {
        // We have to check the default value and the current value that the property is set to
    }
}

// FloatValue
void floatProp(daqProperty* property)
{
    // Retrive metadata, check what type the value is...
    if (propertyValueTypeCheck(property, daqCtFloat))
    {
        // Min value, max value, default value, suggested values, visible
        daqFloat minValue = 0;
        daqProperty_getMinValue(property, &minValue);

        daqFloat maxValue = 0;
        daqProperty_getMaxValue(property, &minValue);

        daqBaseObject* defaultValueObj = NULL;
        daqProperty_getDefaultValue(property, &defaultValueObj);
        daqFloat defaultValue = 0;
        daqQueryInterface(defaultValueObj, DAQ_FLOAT_OBJECT_INTF_ID, &defaultValue);
        daqReleaseRef(defaultValueObj);

        daqList* listOfRecommendedValues = NULL;
        daqProperty_getSuggestedValues(property, &listOfRecommendedValues);


        daqBool visible = False;
        daqProperty_getVisible(property, &visible);
        // Display min, max, default and suggested values

    }
}

// StringProperty
void stringProp(daqProperty* property)
{
    // Retrive metadata, check what type the value is...
    if(propertyValueTypeCheck(property, daqCtString))
    {
        // Default value, suggested values, visible
    }
}

// RatioProperty
void ratioProp(daqProperty* property)
{
    // Retrive metadata, check what type the value is...
    if(propertyValueTypeCheck(property, daqCtRatio))
    {
        
    }
}

// ListProperty
void listProp(daqProperty* property)
{
    if (propertyValueTypeCheck(property, daqCtList))
    {
        
    }
}

// DictProperty
void dictProp(daqProperty* property)
{
    if(propertyValueTypeCheck(property, daqCtDict))
    {
        
    }
}

// ObjectProperty
void objectProp(daqProperty* property)
{
    if(propertyValueTypeCheck(property, daqCtDict))
    {
        
    }
}

// StructureProperty
void structProp(daqProperty* property)
{
    if(propertyValueTypeCheck(property, daqCtStruct))
    {
        
    }
}

// EnumerationProperty
void enumProp(daqProperty* property)
{
    if(propertyValueTypeCheck(property, daqCtEnumeration))
    {
        
    }
}

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
