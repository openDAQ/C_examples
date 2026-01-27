#include <daq_utils.h>

// Int conversion
/*
 * The following two funtions represent the conversion between the openDAQ native
 * daqInt type and its coresponding C integer type.
 */
int64_t openDAQIntConversion(daqInteger* integer)
{
    daqInt* value;
    daqInteger_getValue(integer, &value);
    return *value;
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
    daqString* strOut = NULL;
    daqString_createString(&strOut, str);
    return strOut;
}

char* openDAQStringConversion(daqString* str)
{
    char* out = NULL;
    daqString_getCharPtr(str, &out);
    return out;
}

// Bool conversion
/*
 * Conversion from and to openDAQ Boolean (daqString) core type from C language (uint8_t).
 */
daqBoolean* booleanOpenDAQConversion(uint8_t value)
{
    daqBoolean* object = NULL;
    daqBoolean_createBoolean(object, value);
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
    daqFloatObject* fl = 0;
    daqFloatObject_createFloatObject(&fl, value);
    return fl;
}

double openDAQFloatConversion(daqFloatObject* value)
{
    double vl = 0;
    daqFloatObject_getValue(value, &vl);
    return vl;
}

// Complex number conversion
/*
 * Conversion from and to openDAQ Complex (daqComplex) core type 
 * from C language (struct ComplexNumber).
 */
struct ComplexNumber
{
    double real;
    double complex;
};

struct ComplexNumber complexOpenDAQConversion(daqComplexNumber* complexNumber)
{
    struct ComplexNumber cmp;
    double middle = 0;
    daqComplexNumber_getReal(complexNumber, &middle);
    cmp.real = middle;
    daqComplexNumber_getImaginary(complexNumber, &middle);
    cmp.complex = middle;
    return cmp;
}

daqComplexNumber* openDAQComplexConversion(struct ComplexNumber* complexNumber)
{
    daqComplexNumber* cmp = NULL;
    daqComplexNumber_createComplexNumber(&cmp, complexNumber->real, complexNumber->complex);
    return cmp;
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
    struct Range rng;
    daqNumber* number;
    daqRange_getLowValue(range, &number);
    double intermmidiate = 0;
    daqNumber_getFloatValue(number, &intermmidiate);
    rng.min = intermmidiate;
    daqReleaseRef(number);
    daqRange_getHighValue(range, &number);
    daqNumber_getFloatValue(number, &intermmidiate);
    rng.max = intermmidiate;
    daqReleaseRef(number);
    return rng;
}

daqRange* openDAQRangeConversion(struct Range range)
{
    daqRange* rng = NULL;
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
    daqRange_createRange(&rng, lowValue, highValue);

    daqReleaseRef(lowValue);
    daqReleaseRef(highValue);
    return rng;
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
    double x;
    double y;
    double z;
};

struct Coordinates structOpenDAQConvetsion(daqStruct* coordinates)
{
    daqList* names = NULL;
    daqList* values = NULL;
    daqStruct_getFieldNames(coordinates, &names);
    daqStruct_getFieldValues(coordinates, &values);

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

daqEnumeration* openDAQEnumConversion(enum ComponentStatusTypeEnum componentStatusType)
{
    daqEnumeration* mid = NULL;
    daqEnumerationType* enmType = NULL;
    daqInteger* inp = intOpenDAQConversion(componentStatusType);

    daqList* enumTypes = NULL;
    daqList_createList(&enumTypes);
    daqList_pushBack(enumTypes, stringOpenDAQConversion("Error"));
    daqList_pushBack(enumTypes, stringOpenDAQConversion("Ok"));
    daqList_pushBack(enumTypes, stringOpenDAQConversion("Warning"));

    daqEnumerationType_createEnumerationType(enmType, stringOpenDAQConversion("ComponentStatusType"), enumTypes, 0);

    daqEnumeration_createEnumerationWithIntValueAndType(&mid, enmType, inp);

    daqReleaseRef(enumTypes);
    daqReleaseRef(inp);
    daqReleaseRef(enmType);

    return mid;
}
