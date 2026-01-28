#include <daq_utils.h>

// Int conversion
/*
 * The following two funtions represent the conversion between the openDAQ native
 * daqInt type and its coresponding C integer type.
 */
int64_t openDAQIntConversion(daqInteger* integer)
{
    daqInt value = 0;
    daqInteger_getValue(integer, &value);
    return value;
}

daqInteger* intOpenDAQConversion(int64_t integer)
{
    daqInteger* object = NULL;
    daqInteger_createInteger(&object, integer);
    return object;
}

// String conversion
/*
 * Conversion from and to openDAQ String (daqString) core type
 * from C language (const char*).
 */
daqString* stringOpenDAQConversion(const char* str)
{
    daqString* stringOutput = NULL;
    daqString_createString(&stringOutput, str);
    return stringOutput;
}

char* openDAQStringConversion(daqString* str)
{
    char* cString = NULL;
    daqString_getCharPtr(str, &cString);
    return cString;
}

// Bool conversion
/*
 * Conversion from and to openDAQ Boolean (daqString) core type from C language (uint8_t).
 */
daqBoolean* booleanOpenDAQConversion(uint8_t value)
{
    daqBoolean* object = NULL;
    daqBoolean_createBoolean(&object, value);
    return object;
}

uint8_t openDAQBooleanConversion(daqBoolean* boolean)
{
    uint8_t value = 0;
    daqBoolean_getValue(boolean, &value);
    return value;
}

// Float conversion
/*
 *Conversion from and to openDAQ String (daqFloat) core type from C language (double).
 */
daqFloatObject* floatOpenDAQConversion(double value)
{
    daqFloatObject* floatObject = 0;
    daqFloatObject_createFloatObject(&floatObject, value);
    return floatObject;
}

double openDAQFloatConversion(daqFloatObject* value)
{
    double cDouble = 0;
    daqFloatObject_getValue(value, &cDouble);
    return cDouble;
}

// Complex number conversion
/*
 * Conversion from and to openDAQ Complex (daqComplex) core type 
 * from C language (struct ComplexNumber).
 */
struct ComplexNumber
{
    double real;
    double imaginary;
};

struct ComplexNumber complexOpenDAQConversion(daqComplexNumber* complexNumber)
{
    struct ComplexNumber structComplexNumber;
    double tempDouble= 0;
    daqComplexNumber_getReal(complexNumber, &tempDouble);
    structComplexNumber.real = tempDouble;
    daqComplexNumber_getImaginary(complexNumber, &tempDouble);
    structComplexNumber.imaginary = tempDouble;
    return structComplexNumber;
}

daqComplexNumber* openDAQComplexConversion(struct ComplexNumber* complexNumber)
{
    daqComplexNumber* complexNumberObject = NULL;
    daqComplexNumber_createComplexNumber(&complexNumberObject, complexNumber->real, complexNumber->imaginary);
    return complexNumberObject;
}

// Range conversion
/*
 * Conversion from and to openDAQ String (daqRange) core type 
 * from C language (struct Range).
 */
struct Range
{
    double min;
    double max;
};

struct Range rangeOpenDAQConversion(daqRange* range)
{
    struct Range rangeStruct;
    daqNumber* number;
    daqRange_getLowValue(range, &number);
    double intermmidiate = 0;
    daqNumber_getFloatValue(number, &intermmidiate);
    rangeStruct.min = intermmidiate;
    daqReleaseRef(number);
    daqRange_getHighValue(range, &number);
    daqNumber_getFloatValue(number, &intermmidiate);
    rangeStruct.max = intermmidiate;
    daqReleaseRef(number);
    return rangeStruct;
}

daqRange* openDAQRangeConversion(struct Range range)
{
    daqRange* rangeObject = NULL;
    daqNumber* lowValue = NULL;
    daqNumber* highValue = NULL;

    daqFloatObject* lowFloat = NULL;
    daqFloatObject_createFloatObject(&lowFloat, range.min);
    daqFloatObject* highFloat = NULL;
    daqFloatObject_createFloatObject(&highFloat, range.max);

    daqBaseObject* lowBaseObject = NULL;
    daqQueryInterface(lowFloat, DAQ_BASE_OBJECT_INTF_ID, &lowBaseObject);
    daqBaseObject* highBaseObject = NULL;
    daqQueryInterface(highFloat, DAQ_BASE_OBJECT_INTF_ID, &highBaseObject);

    if (DAQ_SUPPORTS_INTERFACE(lowBaseObject, DAQ_NUMBER_INTF_ID))
    {
        daqQueryInterface(lowBaseObject, DAQ_NUMBER_INTF_ID, &lowValue);
        daqQueryInterface(highBaseObject, DAQ_NUMBER_INTF_ID, &highValue);
    }

    daqReleaseRef(lowFloat);
    daqReleaseRef(highFloat);
    daqReleaseRef(lowBaseObject);
    daqReleaseRef(highBaseObject);

    // Create and populate the daqRange object
    daqRange_createRange(&rangeObject, lowValue, highValue);

    daqReleaseRef(lowValue);
    daqReleaseRef(highValue);
    return rangeObject;
}

// Struct conversion 
/*
 * Conversion from and to daqStruct objects to C style structs.
 * Warning: These types of conversions require prior knowledge of 
 * the struct structure and its definition in C. For conversion 
 * from C to openDAQ a pointer to the TypeManager is needed because
 * of the way openDAQ structs are implemented.
 */
struct Coordinates
{
    int64_t x;
    int64_t y;
    int64_t z;
};

struct Coordinates structOpenDAQConvetsion(daqStruct* coordinates)
{
    daqList* values = NULL;
    daqStruct_getFieldValues(coordinates, &values);

    struct Coordinates coordinate = {0,0,0};

    daqInteger* value = NULL;

    daqSizeT count = 0;
    daqList_getCount(values, &count);

    daqBaseObject* valueObj = NULL;
    for (daqSizeT i = 0; i<count; i++)
    {
        daqList_getItemAt(values, i, &valueObj);
        daqQueryInterface(valueObj, DAQ_INTEGER_INTF_ID, &value);
        switch(i)
        {
        case 0:
        {
            coordinate.x = openDAQIntConversion(value);
            daqReleaseRef(value);
            daqReleaseRef(valueObj);
            break;
        }
        case 1:
        {
            coordinate.y = openDAQIntConversion(value);
            daqReleaseRef(value);
            daqReleaseRef(valueObj);
            break;
        }
        case 2:
        {
            coordinate.z = openDAQIntConversion(value);
            daqReleaseRef(value);
            daqReleaseRef(valueObj);
            break;
        }
        default:
        {
            daqReleaseRef(value);
            daqReleaseRef(valueObj);
            break;
        }
        }
    }
    return coordinate;
}

daqStruct* openDAQStructConversion(struct Coordinates coordinates, daqTypeManager* typeManager)
{
    daqStructBuilder* coordinatesBuilder = NULL;
    daqStructBuilder_createStructBuilder(&coordinatesBuilder, stringOpenDAQConversion("Coordinates"), typeManager);

    daqStructBuilder_set(coordinatesBuilder, stringOpenDAQConversion("x"), (daqBaseObject*)intOpenDAQConversion(coordinates.x));
    daqStructBuilder_set(coordinatesBuilder, stringOpenDAQConversion("y"), (daqBaseObject*) intOpenDAQConversion(coordinates.y));
    daqStructBuilder_set(coordinatesBuilder, stringOpenDAQConversion("z"), (daqBaseObject*) intOpenDAQConversion(coordinates.z));

    // Alternative way of setting the values in a struct
    if (0)
    {
        daqList* valuesList = NULL;
        daqList_createList(&valuesList);

        daqList_pushBack(valuesList, (daqBaseObject*) intOpenDAQConversion(coordinates.x));
        daqList_pushBack(valuesList, (daqBaseObject*) intOpenDAQConversion(coordinates.y));
        daqList_pushBack(valuesList, (daqBaseObject*) intOpenDAQConversion(coordinates.z));

        daqStructBuilder_setFieldValues(coordinatesBuilder, valuesList);
        daqReleaseRef(valuesList);
    }
    daqStruct* coordinatesStruct = NULL;
    daqStructBuilder_build(coordinatesBuilder, &coordinatesStruct);
    daqReleaseRef(coordinatesBuilder);

    return coordinatesStruct;
}

// Enumeration conversion (needs an additional run through)
/*
 * Conversion from and to openDAQ String (daqEnumeration) core type
 * from C language (enum ComponentStutesTypeEnum).
 * Warning: These types of conversions require prior knowledge of 
 * the enumeration structure and its definition in C. For conversion 
 * from C to openDAQ a pointer to the TypeManager is needed because
 * of the way openDAQ enumeration is implemented.
 */
enum ComponentStatusTypeEnum
{
    Error = 0,
    Ok,
    Warning
};

enum ComponentStatusTypeEnum enumOpenDAQConversion(daqEnumeration* componentStatusType)
{
    enum ComponentStatusTypeEnum compStatusType = Error;
    daqInt check;
    daqEnumeration_getIntValue(componentStatusType, &check);
    if (check < 2)
    {
        compStatusType = check;
    }
    return compStatusType;
}

daqEnumeration* openDAQEnumConversion(enum ComponentStatusTypeEnum componentStatusType, daqTypeManager* typeManager)
{
    daqEnumeration* mid = NULL;
    daqInteger* inp = intOpenDAQConversion(componentStatusType);

    daqEnumeration_createEnumerationWithIntValue(&mid, stringOpenDAQConversion("ComponentStatusType"), inp, typeManager);

    daqReleaseRef(inp);
    return mid;
}
