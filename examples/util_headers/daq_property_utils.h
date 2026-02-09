#include <daq_c_conversions.h>

struct PropFields
{
    daqCoreType valueType;
    uint8_t name;
    uint8_t description;
    uint8_t unit;
    uint8_t minValue;
    uint8_t maxValue;
    uint8_t defaultValue;
    uint8_t visible;
    uint8_t realOnly;
    uint8_t selectionValue;
    uint8_t suggestedValues;
    uint8_t refProp;
    uint8_t callableInfo;
};

void printCommonMetadata(daqProperty* property)
{
    daqBool visible = False;
    daqBool readOnly = False;
    daqProperty_getVisible(property, &visible);
    daqProperty_getReadOnly(property, &readOnly);

    printf("- Visible: %s\n", visible == True ? "True" : "False");
    printf("- Read only: %s\n", readOnly == True ? "True" : "False");
}

void printIntProp(daqProperty* property, daqBool metadata, daqBool val)
{
    daqBaseObject* temp = NULL;
    daqInteger* value = NULL;

    if (metadata)
    {
        printCommonMetadata(property);

        // Default value
        daqProperty_getDefaultValue(property, &temp);
        daqQueryInterface(temp, DAQ_INTEGER_INTF_ID, &value);
        int64_t valueNative = daq_fromDaqInteger(value);
        printf("- Default value: %lld\n", valueNative);
        daqReleaseRef(value);
        daqReleaseRef(temp);

        // Min/Max value
        daqNumber* edgeCase = NULL;
        int64_t minNativeValue = 0;
        int64_t maxNativeValue = 0;
        daqProperty_getMinValue(property, &edgeCase);
        if (edgeCase != NULL)
        {
            daqNumber_getIntValue(edgeCase, &minNativeValue);
            printf("- Minimal value: %lld\n", minNativeValue);
            daqReleaseRef(edgeCase);
            daqProperty_getMaxValue(property, &edgeCase);
            if (edgeCase != NULL)
            {
                daqNumber_getIntValue(edgeCase, &maxNativeValue);
                printf("- Maximum value: %lld\n", maxNativeValue);
                daqReleaseRef(edgeCase);
            }
        }

        // Suggested values
        daqList* suggestedValues = NULL;
        daqProperty_getSuggestedValues(property, &suggestedValues);
        if (suggestedValues != NULL)
        {
            printf("- Suggested values:\n");

            daqSizeT count = 0;
            daqList_getCount(suggestedValues, &count);
            for (daqSizeT i = 0; i < count; i++)
            {
                daqList_getItemAt(suggestedValues, i, &temp);
                daqQueryInterface(temp, DAQ_INTEGER_INTF_ID, &value);
                valueNative = daq_fromDaqFloat(value);
                daqReleaseRef(value);
                daqReleaseRef(temp);
                printf(" -- %lld\n", valueNative);
            }
            daqReleaseRef(suggestedValues);
        }
    }

    if (val)
    {
        // Value
        daqProperty_getValue(property, &temp);
        daqQueryInterface(temp, DAQ_INTEGER_INTF_ID, &value);
        int64_t valueNative = daq_fromDaqInteger(value);
        printf("- Value: %lld\n", valueNative);
        daqReleaseRef(value);
        daqReleaseRef(temp);
    }
}

void printBoolProp(daqProperty* property, daqBool metadata, daqBool val)
{
    daqBaseObject* temp = NULL;
    daqBoolean* value = NULL;
    if (metadata)
    {
        printCommonMetadata(property);

        // Default value
        daqProperty_getDefaultValue(property, &temp);
        daqQueryInterface(temp, DAQ_BOOLEAN_INTF_ID, &value);
        daqBool valueNative = False;
        daqBoolean_getValue(value, &valueNative);
        daqReleaseRef(value);
        daqReleaseRef(temp);

        printf("- Default value: %s\n", valueNative == True ? "True" : "False");
    }
    if (val)
    {
        // Value
        daqProperty_getValue(property, &temp);
        daqQueryInterface(temp, DAQ_BOOLEAN_INTF_ID, &value);
        daqBool valueNative = False;
        daqBoolean_getValue(value, &valueNative);
        daqReleaseRef(value);
        daqReleaseRef(temp);

        printf("- Value: %s\n", valueNative == True ? "True" : "False");
    }
}

void printFloatProp(daqProperty* property, daqBool metadata, daqBool val)
{
    daqBaseObject* temp = NULL;
    daqFloatObject* value = NULL;
    if (metadata)
    {
        printCommonMetadata(property);

        // Default value
        daqProperty_getDefaultValue(property, &temp);
        daqQueryInterface(temp, DAQ_FLOAT_OBJECT_INTF_ID, &value);
        double valueNative = 0;
        daqFloatObject_getValue(value, &valueNative);
        daqReleaseRef(value);
        daqReleaseRef(temp);

        printf("- Default value: %f\n", valueNative);

        // Min/Max value
        daqNumber* edgeCase = NULL;
        double minNativeValue = 0;
        double maxNativeValue = 0;
        daqProperty_getMinValue(property, &edgeCase);
        if (edgeCase != NULL)
        {
            daqNumber_getFloatValue(edgeCase, &minNativeValue);
            printf("- Minimal value: %f\n", minNativeValue);
            daqReleaseRef(edgeCase);
            daqProperty_getMaxValue(property, &edgeCase);
            if (edgeCase != NULL)
            {
                daqNumber_getFloatValue(edgeCase, &maxNativeValue);
                printf("- Maximum value: %f\n", maxNativeValue);
                daqReleaseRef(edgeCase);
            }
        }

        // Suggested values
        daqList* suggestedValues = NULL;
        daqProperty_getSuggestedValues(property, &suggestedValues);
        if (suggestedValues != NULL)
        {
            printf("- Suggested values:\n");

            daqSizeT count = 0;
            daqList_getCount(suggestedValues, &count);
            for (daqSizeT i = 0; i < count; i++)
            {
                daqList_getItemAt(suggestedValues, i, &temp);
                daqQueryInterface(temp, DAQ_FLOAT_OBJECT_INTF_ID, &value);
                valueNative = daq_fromDaqFloat(value);
                daqReleaseRef(value);
                daqReleaseRef(temp);
                printf(" -- %f\n", valueNative);
            }
            daqReleaseRef(suggestedValues);
        }
    }
    if (val)
    {
        daqProperty_getValue(property, &temp);
        daqQueryInterface(temp, DAQ_FLOAT_OBJECT_INTF_ID, &value);
        double valueNative = daq_fromDaqFloat(value);
        daqReleaseRef(temp);
        daqReleaseRef(value);

        printf("- Value: %f\n", valueNative);
    }
}

void printStringProp(daqProperty* property, daqBool metadata, daqBool val)
{
    daqBaseObject* temp = NULL;
    daqString* value = NULL;

    if (metadata)
    {
        printCommonMetadata(property);

        // Default value
        daqProperty_getDefaultValue(property, &temp);
        daqQueryInterface(temp, DAQ_STRING_INTF_ID, &value);
        printDaqFormattedString("- Default value: %s\n", value);
        daqReleaseRef(temp);
        daqReleaseRef(value);

        // Suggested values
        daqList* suggestedValues = NULL;
        daqProperty_getSuggestedValues(property, &suggestedValues);
        if (suggestedValues != NULL)
        {
            printf("- Sugggested values:\n");
            daqSizeT count = 0;
            daqList_getCount(suggestedValues, &count);
            for (daqSizeT i = 0; i < count; i++)
            {
                daqList_getItemAt(suggestedValues, i, &temp);
                daqQueryInterface(temp, DAQ_STRING_INTF_ID, &value);
                printDaqFormattedString(" -- %s\n", value);
                daqReleaseRef(temp);
                daqReleaseRef(value);
            }
            daqReleaseRef(suggestedValues);
        }
    }

    if (val)
    {
        // Value
        daqProperty_getValue(property, &temp);
        daqQueryInterface(temp, DAQ_STRING_INTF_ID, &value);
        printDaqFormattedString("- Value: %s\n", value);
        daqReleaseRef(temp);
        daqReleaseRef(value);
    }
}

void printRatioProp(daqProperty* property, daqBool metadata, daqBool val)
{
    daqBaseObject* temp = NULL;
    daqRatio* value = NULL;
    struct Range range = { 1, 1 };

    if (metadata)
    {
        printCommonMetadata(property);

        // Default value
        daqProperty_getDefaultValue(property, &temp);
        daqQueryInterface(temp, DAQ_RATIO_INTF_ID, &value);
        range = daq_fromDaqRange(value);
        daqReleaseRef(temp);
        daqReleaseRef(value);

        printf("- Default value:\n");
        printf(" -- Denominator: %f\n", range.max);
        printf(" -- Numerator: %f\n", range.min);
    }
    if (val)
    {
        // Value
        daqProperty_getValue(property, &temp);
        daqQueryInterface(temp, DAQ_RATIO_INTF_ID, &value);
        range = daq_fromDaqRange(value);
        daqReleaseRef(temp);
        daqReleaseRef(value);

        printf("- Value:\n");
        printf(" -- Denominator: %lld\n", range.max);
        printf(" -- Numerator: %lld\n", range.min);
    }
}
