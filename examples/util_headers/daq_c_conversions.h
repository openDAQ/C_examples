#include <daq_utils.h>

// Core struct type
struct ComplexNumber
{
    double real;
    double imaginary;
};

// Core struct type
struct Range
{
    double min;
    double max;
};

// Int conversion
/*
 * The following two funtions represent the conversion between the openDAQ native
 * daqInt type and its coresponding C integer type.
 */
daqInt daq_fromDaqInteger(daqInteger* daq);
daqInteger* daq_toDaqInteger(daqInt native);

// String conversion
/*
 * Conversion from and to openDAQ String (daqString) core type
 * from C language (const char*). The function that converts to native C return a non-owning pointer.
 * Precautions should be taken, when using the received pointer.
 */
char* daq_fromDaqString(daqString* daq);
daqString* daq_toDaqString(const char* native);

// Bool conversion
/*
 * Conversion from and to openDAQ Boolean (daqString) core type from C language (uint8_t).
 */
uint8_t daq_fromDaqBoolean(daqBoolean* daq);
daqBoolean* daq_toDaqBoolean(uint8_t native);

// Float conversion
/*
 *Conversion from and to openDAQ String (daqFloat) core type from C language (double).
 */
daqFloat daq_fromDaqFloat(daqFloatObject* daq);
daqFloatObject* daq_toDaqFloat(daqFloat native);

// Complex number conversion
/*
 * Conversion from and to openDAQ Complex (daqComplex) core type
 * from C language (struct ComplexNumber).
 */
struct ComplexNumber daq_fromDaqComplex(daqComplexNumber* daq);
daqComplexNumber* daq_toDaqComplex(struct ComplexNumber* native);

// Range conversion
/*
 * Conversion from and to openDAQ String (daqRange) core type
 * from C language (struct Range).
 */
struct Range daq_fromDaqRange(daqRange* daq);
daqRange* daq_toDaqRange(struct Range native);

daqInt daq_fromDaqInteger(daqInteger* daq)
{
    daqInt native = 0;
    daqInteger_getValue(daq, &native);
    return native;
}

daqInteger* daq_toDaqInteger(daqInt native)
{
    daqInteger* daq = NULL;
    daqInteger_createInteger(&daq, native);
    return daq;
}

char* daq_fromDaqString(daqString* daq)
{
    char* native = NULL;
    daqString_getCharPtr(daq, &native);
    return native;
}

daqString* daq_toDaqString(const char* native)
{
    daqString* daq = NULL;
    daqString_createString(&daq, native);
    return daq;
}

uint8_t daq_fromDaqBoolean(daqBoolean* daq)
{
    uint8_t native = 0;
    daqBoolean_getValue(daq, &native);
    return native;
}

daqBoolean* daq_toDaqBoolean(uint8_t native)
{
    daqBoolean* daq = NULL;
    daqBoolean_createBoolean(&daq, native);
    return daq;
}

daqFloat daq_fromDaqFloat(daqFloatObject* daq)
{
    daqFloat native = 0;
    daqFloatObject_getValue(daq, &native);
    return native;
}

daqFloatObject* daq_toDaqFloat(daqFloat native)
{
    daqFloatObject* daq = 0;
    daqFloatObject_createFloatObject(&daq, native);
    return daq;
}

struct ComplexNumber daq_fromDaqComplex(daqComplexNumber* daq)
{
    struct ComplexNumber native = { 0, 0 };
    double tempDouble = 0;
    daqComplexNumber_getReal(daq, &tempDouble);
    native.real = tempDouble;
    daqComplexNumber_getImaginary(daq, &tempDouble);
    native.imaginary = tempDouble;
    return native;
}

daqComplexNumber* daq_toDaqComplex(struct ComplexNumber* native)
{
    daqComplexNumber* daq = NULL;
    daqComplexNumber_createComplexNumber(&daq, native->real, native->imaginary);
    return daq;
}

struct Range daq_fromDaqRange(daqRange* daq)
{
    struct Range native = { 0, 0 };
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

daqRange* daq_toDaqRange(struct Range native)
{
    daqRange* daq = NULL;
    daqNumber* lowValue = NULL;
    daqNumber* highValue = NULL;

    daqFloatObject* lowFloat = daq_toDaqFloat(native.min);
    daqFloatObject* highFloat = daq_toDaqFloat(native.max);

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
