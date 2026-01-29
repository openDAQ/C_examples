#include <daq_utils.h>

daqString* stringOpenDAQConversion(const char* native);
daqInteger* intOpenDAQConversion(daqInt native);

// add types to typeManager
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


    daqBool check = False;
    daqTypeManager_hasType(typeManager, stringOpenDAQConversion("DAQ_Coordinates"), &check);

    if(check)
        printf("\nNew type added.\n");
    else
        printf("\nFailed to add a new type.\n");

    daqReleaseRef(types);
    daqReleaseRef(defaultValues);
    daqReleaseRef(names);
    daqReleaseRef(typeManager);
}


// Int conversion
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

// String conversion
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

// Bool conversion
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

// Float conversion
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

struct ComplexNumber openDAQComplexConversion(daqComplexNumber* daq)
{
    struct ComplexNumber native;
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

struct Range openDAQRangeConversion(daqRange* daq)
{
    struct Range native;
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

struct Coordinates openDAQStructConversion(daqStruct* daq)
{
    daqList* values = NULL;
    daqStruct_getFieldValues(daq, &values);

    struct Coordinates native = {0,0,0};

    daqInteger* temp = NULL;

    daqSizeT count = 0;
    daqList_getCount(values, &count);

    daqBaseObject* tempObj = NULL;
    for (daqSizeT i = 0; i<count; i++)
    {
        daqList_getItemAt(values, i, &tempObj);
        daqQueryInterface(tempObj, DAQ_INTEGER_INTF_ID, &temp);
        switch(i)
        {
        case 0:
        {
            native.x = openDAQIntConversion(temp);
            daqReleaseRef(temp);
            daqReleaseRef(tempObj);
            break;
        }
        case 1:
        {
            native.y = openDAQIntConversion(temp);
            daqReleaseRef(temp);
            daqReleaseRef(tempObj);
            break;
        }
        case 2:
        {
            native.z = openDAQIntConversion(temp);
            daqReleaseRef(temp);
            daqReleaseRef(tempObj);
            break;
        }
        default:
        {
            daqReleaseRef(temp);
            daqReleaseRef(tempObj);
            break;
        }
        }
    }
    return native;
}

daqStruct* structOpenDAQConversion(struct Coordinates native, daqTypeManager* typeManager)
{
    daqStructBuilder* builder = NULL;
    daqStructBuilder_createStructBuilder(&builder, stringOpenDAQConversion("DAQ_Coordinates"), typeManager);

    daqStructBuilder_set(builder, stringOpenDAQConversion("x"), (daqBaseObject*)intOpenDAQConversion(native.x));
    daqStructBuilder_set(builder, stringOpenDAQConversion("y"), (daqBaseObject*) intOpenDAQConversion(native.y));
    daqStructBuilder_set(builder, stringOpenDAQConversion("z"), (daqBaseObject*) intOpenDAQConversion(native.z));

    // Alternative way of setting the values in a struct
    if (0)
    {
        daqList* valuesList = NULL;
        daqList_createList(&valuesList);

        daqList_pushBack(valuesList, (daqBaseObject*) intOpenDAQConversion(native.x));
        daqList_pushBack(valuesList, (daqBaseObject*) intOpenDAQConversion(native.y));
        daqList_pushBack(valuesList, (daqBaseObject*) intOpenDAQConversion(native.z));

        daqStructBuilder_setFieldValues(builder, valuesList);
        daqReleaseRef(valuesList);
    }
    daqStruct* daq = NULL;
    daqStructBuilder_build(builder, &daq);
    daqReleaseRef(builder);

    return daq;
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

enum ComponentStatusTypeEnum openDAQEnumConversion(daqEnumeration* componentStatusType)
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

daqEnumeration* EnumOpenDAQConversion(enum ComponentStatusTypeEnum native, daqTypeManager* typeManager)
{
    daqEnumeration* daq = NULL;
    daqInteger* inp = intOpenDAQConversion(native);

    daqEnumeration_createEnumerationWithIntValue(&daq, stringOpenDAQConversion("ComponentStatusType"), inp, typeManager);

    daqReleaseRef(inp);
    return daq;
}
