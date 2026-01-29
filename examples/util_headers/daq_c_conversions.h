#include <daq_utils.h>

struct ComplexNumber
{
    double real;
    double imaginary;
};

struct Range
{
    double min;
    double max;
};

struct Coordinates
{
    int64_t x;
    int64_t y;
    int64_t z;
};

enum ComponentStatusTypeEnum
{
    Error = 0,
    Ok,
    Warning
};

// Adds Coodinates struct and ComponentStatusTypeEnum to types in Type Manager
void addCoordinateStructToTypeManager(daqContext* context);

// Int conversion
daqInt openDAQIntConversion(daqInteger* daq);
daqInteger* intOpenDAQConversion(daqInt native);

// String conversion
char* openDAQStringConversion(daqString* daq);
daqString* stringOpenDAQConversion(const char* native);

// Bool conversion
daqBoolean* booleanOpenDAQConversion(uint8_t native);
uint8_t openDAQBooleanConversion(daqBoolean* daq);

// Float conversion
daqFloatObject* floatOpenDAQConversion(double native);
double openDAQFloatConversion(daqFloatObject* daq);

// Complex number conversion
struct ComplexNumber openDAQComplexConversion(daqComplexNumber* daq);
daqComplexNumber* complexOpenDAQConversion(struct ComplexNumber* native);

// Range conversion
struct Range openDAQRangeConversion(daqRange* daq);
daqRange* RangeOpenDAQConversion(struct Range native);

// Struct conversion 
struct Coordinates openDAQCoordinatesStructConversion(daqStruct* daq);
daqStruct* coordinatesStructOpenDAQConversion(struct Coordinates native, daqTypeManager* typeManager);

// Enumeration conversion
enum ComponentStatusTypeEnum openDAQEnumConversion(daqEnumeration* daq);
daqEnumeration* EnumOpenDAQConversion(enum ComponentStatusTypeEnum native, daqTypeManager* typeManager);

void addCoordinateStructToTypeManager(daqContext* context)
{
    daqTypeManager* typeManager = NULL;
    daqContext_getTypeManager(context, &typeManager);
    
    daqList* names = NULL;
    daqList_createList(&names);

    daqList_pushBack(names, (daqBaseObject*) stringOpenDAQConversion("x"));
    daqList_pushBack(names, (daqBaseObject*) stringOpenDAQConversion("y"));
    daqList_pushBack(names, (daqBaseObject*) stringOpenDAQConversion("z"));

    daqList* defaultValues = NULL;
    daqList_createList(&defaultValues);
    daqList_pushBack(defaultValues, (daqBaseObject*) intOpenDAQConversion(0));
    daqList_pushBack(defaultValues, (daqBaseObject*) intOpenDAQConversion(0));
    daqList_pushBack(defaultValues, (daqBaseObject*) intOpenDAQConversion(0));

    daqSimpleType* simpleType = NULL;
    daqSimpleType_createSimpleType(&simpleType, daqCtInt);

    daqList* types = NULL;
    daqList_createList(&types);
    daqList_pushBack(types, simpleType);
    daqList_pushBack(types, simpleType);
    daqList_pushBack(types, simpleType);
    daqReleaseRef(simpleType);

    daqStructType* newType = NULL;
    daqStructType_createStructType(&newType, stringOpenDAQConversion("DAQ_Coordinates"), names, defaultValues, types);

    daqTypeManager_addType(typeManager, (daqType*) newType);
    daqReleaseRef(newType);
    daqReleaseRef(names);

    daqList_createList(&names);

    daqList_pushBack(names, (daqBaseObject*) stringOpenDAQConversion("Error"));
    daqList_pushBack(names, (daqBaseObject*) stringOpenDAQConversion("Ok"));
    daqList_pushBack(names, (daqBaseObject*) stringOpenDAQConversion("Warning"));

    daqEnumerationType* enumType = NULL;
    daqEnumerationType_createEnumerationType(&enumType, stringOpenDAQConversion("DAQ_ComponentStatusTypeEnum"), names, 0);

    daqTypeManager_addType(typeManager, (daqType*) enumType);
    daqReleaseRef(enumType);

    daqBool check = False;
    daqTypeManager_hasType(typeManager, stringOpenDAQConversion("DAQ_Coordinates"), &check);

    daqBool enumCheck = False;
    daqTypeManager_hasType(typeManager, stringOpenDAQConversion("DAQ_ComponentStatusTypeEnum"), &enumCheck);

    if(check && enumCheck)
        printf("\nNew types added.\n");
    else
        printf("\nFailed to add new types.\n");

    daqReleaseRef(types);
    daqReleaseRef(defaultValues);
    daqReleaseRef(names);
    daqReleaseRef(typeManager);
}

/*
 * The following two funtions represent the conversion between the openDAQ native
 * daqInt type and its coresponding C integer type.
 */
daqInt openDAQIntConversion(daqInteger* daq)
{
    daqInt native = 0;
    daqInteger_getValue(daq, &native);
    return native;
}

daqInteger* intOpenDAQConversion(daqInt native)
{
    daqInteger* daq = NULL;
    daqInteger_createInteger(&daq, native);
    return daq;
}

/*
 * Conversion from and to openDAQ String (daqString) core type
 * from C language (const char*).
 */
daqString* stringOpenDAQConversion(const char* native)
{
    daqString* daq = NULL;
    daqString_createString(&daq, native);
    return daq;
}

char* openDAQStringConversion(daqString* daq)
{
    char* native = NULL;
    daqString_getCharPtr(daq, &native);
    return native;
}

/*
 * Conversion from and to openDAQ Boolean (daqString) core type from C language (uint8_t).
 */
daqBoolean* booleanOpenDAQConversion(uint8_t native)
{
    daqBoolean* daq = NULL;
    daqBoolean_createBoolean(&daq, native);
    return daq;
}

uint8_t openDAQBooleanConversion(daqBoolean* daq)
{
    uint8_t native = 0;
    daqBoolean_getValue(daq, &native);
    return native;
}

/*
 *Conversion from and to openDAQ String (daqFloat) core type from C language (double).
 */
daqFloatObject* floatOpenDAQConversion(double native)
{
    daqFloatObject* daq = 0;
    daqFloatObject_createFloatObject(&daq, native);
    return daq;
}

double openDAQFloatConversion(daqFloatObject* daq)
{
    double native = 0;
    daqFloatObject_getValue(daq, &native);
    return native;
}

/*
 * Conversion from and to openDAQ Complex (daqComplex) core type 
 * from C language (struct ComplexNumber).
 */
struct ComplexNumber openDAQComplexConversion(daqComplexNumber* daq)
{
    struct ComplexNumber native = {0, 0};
    double tempDouble= 0;
    daqComplexNumber_getReal(daq, &tempDouble);
    native.real = tempDouble;
    daqComplexNumber_getImaginary(daq, &tempDouble);
    native.imaginary = tempDouble;
    return native;
}

daqComplexNumber* complexOpenDAQConversion(struct ComplexNumber* native)
{
    daqComplexNumber* daq = NULL;
    daqComplexNumber_createComplexNumber(&daq, native->real, native->imaginary);
    return daq;
}

/*
 * Conversion from and to openDAQ String (daqRange) core type 
 * from C language (struct Range).
 */
struct Range openDAQRangeConversion(daqRange* daq)
{
    struct Range native = {0, 0};
    daqNumber* temp;
    daqRange_getLowValue(daq, &temp);
    double intermmidiate = 0;
    daqNumber_getFloatValue(temp, &intermmidiate);
    native.min = intermmidiate;
    daqReleaseRef(temp);

    daqRange_getHighValue(daq, &temp);
    daqNumber_getFloatValue(temp, &intermmidiate);
    native.max = intermmidiate;
    daqReleaseRef(temp);
    return native;
}

daqRange* RangeOpenDAQConversion(struct Range native)
{
    daqRange* daq = NULL;
    daqNumber* lowValue = NULL;
    daqNumber* highValue = NULL;

    daqFloatObject* lowFloat = floatOpenDAQConversion(native.min);
    daqFloatObject* highFloat = floatOpenDAQConversion(native.max);

    daqQueryInterface(lowFloat, DAQ_NUMBER_INTF_ID, &lowValue);
    daqQueryInterface(highFloat, DAQ_NUMBER_INTF_ID, &highValue);

    daqReleaseRef(lowFloat);
    daqReleaseRef(highFloat);

    // Create and populate the daqRange object
    daqRange_createRange(&daq, lowValue, highValue);

    daqReleaseRef(lowValue);
    daqReleaseRef(highValue);
    return daq;
}

/*
 * Conversion from and to daqStruct objects to C style structs.
 * Warning: These types of conversions require prior knowledge of 
 * the struct structure and its definition in C. For conversion 
 * from C to openDAQ a pointer to the TypeManager is needed because
 * of the way openDAQ structs are implemented.
 */
struct Coordinates openDAQCoordinatesStructConversion(daqStruct* daq)
{
    struct Coordinates native = {0,0,0};

    daqBaseObject* tempObj = NULL;

    daqStruct_get(daq, stringOpenDAQConversion("x"), tempObj);
    native.x = openDAQIntConversion((daqInteger*) daqQueryInterfacePtr(tempObj, DAQ_INTEGER_INTF_ID));
    daqReleaseRef(tempObj);

    daqStruct_get(daq, stringOpenDAQConversion("y"), tempObj);
    native.y = openDAQIntConversion((daqInteger*) daqQueryInterfacePtr(tempObj, DAQ_INTEGER_INTF_ID));
    daqReleaseRef(tempObj);

    daqStruct_get(daq, stringOpenDAQConversion("z"), tempObj);
    native.z = openDAQIntConversion((daqInteger*) daqQueryInterfacePtr(tempObj, DAQ_INTEGER_INTF_ID));
    daqReleaseRef(tempObj);

    return native;
}

daqStruct* coordinatesStructOpenDAQConversion(struct Coordinates native, daqTypeManager* typeManager)
{
    daqStructBuilder* builder = NULL;
    daqStructBuilder_createStructBuilder(&builder, stringOpenDAQConversion("DAQ_Coordinates"), typeManager);

    if (0)
    {
        // We can set values in the struct either via directly assigning them to the
        // corresponding struct values or adding them in the correct sequence 
        // to a daqList and setting the list.
        daqList* values = NULL;
        daqList_createList(&values);

        daqList_pushBack(values, (daqBaseObject*) intOpenDAQConversion(native.x));
        daqList_pushBack(values, (daqBaseObject*) intOpenDAQConversion(native.y));
        daqList_pushBack(values, (daqBaseObject*) intOpenDAQConversion(native.z));

        daqStructBuilder_setFieldValues(builder, values);
        daqReleaseRef(values);
    }
    else
    {
        daqStructBuilder_set(builder, stringOpenDAQConversion("x"), (daqBaseObject*) intOpenDAQConversion(native.x));

        daqStructBuilder_set(builder, stringOpenDAQConversion("y"), (daqBaseObject*) intOpenDAQConversion(native.y));

        daqStructBuilder_set(builder, stringOpenDAQConversion("z"), (daqBaseObject*) intOpenDAQConversion(native.z));
    }
    daqStruct* daq = NULL;
    daqStructBuilder_build(builder, &daq);
    daqReleaseRef(builder);

    return daq;
}

/*
 * Conversion from and to openDAQ String (daqEnumeration) core type
 * from C language (enum ComponentStutesTypeEnum).
 * Warning: These types of conversions require prior knowledge of 
 * the enumeration structure and its definition in C. For conversion 
 * from C to openDAQ a pointer to the TypeManager is needed because
 * of the way openDAQ enumeration is implemented.
 */
enum ComponentStatusTypeEnum openDAQEnumConversion(daqEnumeration* daq)
{
    enum ComponentStatusTypeEnum native = Error;
    uint8_t temp;
    daqEnumeration_getIntValue(daq, &temp);

    // Sanity check
    if (temp < 2)
        native = temp;

    return native;
}

daqEnumeration* EnumOpenDAQConversion(enum ComponentStatusTypeEnum native, daqTypeManager* typeManager)
{
    daqEnumeration* daq = NULL;
    daqInteger* temp = intOpenDAQConversion(native);

    daqEnumeration_createEnumerationWithIntValue(&daq, stringOpenDAQConversion("ComponentStatusType"), temp, typeManager);

    daqReleaseRef(temp);
    return daq;
}
