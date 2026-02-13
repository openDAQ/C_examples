#include <daq_utils.h>

// Core struct type
struct exdaq_ComplexNumber
{
    double real;
    double imaginary;
};

// Core struct type
struct exdaq_Range
{
    double min;
    double max;
};

// Core struct type
struct exdaq_Ratio
{
    int64_t denominator;
    int64_t numerator;
};

// Int conversion
/*
 * The following two funtions represent the conversion between the openDAQ native
 * daqInt type and its coresponding C integer type.
 */
daqInt exdaq_fromDaqInteger(daqInteger* daq);
daqInteger* exdaq_toDaqInteger(daqInt native);

// String conversion
/*
 * Conversion from and to openDAQ String (daqString) core type
 * from C language (const char*). The function that converts to native C return a non-owning pointer.
 * Precautions should be taken, when using the received pointer.
 */
char* exdaq_fromDaqString(daqString* daq);
daqString* exdaq_toDaqString(const char* native);

// Bool conversion
/*
 * Conversion from and to openDAQ Boolean (daqString) core type from C language (uint8_t).
 */
uint8_t exdaq_fromDaqBoolean(daqBoolean* daq);
daqBoolean* exdaq_toDaqBoolean(uint8_t native);

// Float conversion
/*
 *Conversion from and to openDAQ String (daqFloat) core type from C language (double).
 */
daqFloat exdaq_fromDaqFloat(daqFloatObject* daq);
daqFloatObject* exdaq_toDaqFloat(daqFloat native);

// Complex number conversion
/*
 * Conversion from and to openDAQ Complex (daqComplex) core type
 * from C language (struct ComplexNumber).
 */
struct exdaq_ComplexNumber exdaq_fromDaqComplex(daqComplexNumber* daq);
daqComplexNumber* exdaq_toDaqComplex(struct exdaq_ComplexNumber* native);

// Range conversion
/*
 * Conversion from and to openDAQ Struct (daqRange) core type
 * from C language (struct Range).
 */
struct exdaq_Range exdaq_fromDaqRange(daqRange* daq);
daqRange* exdaq_toDaqRange(struct exdaq_Range native);

// Ratio conversion
/*
 * Conversion from and to openDAQ Ratio Struct (daqRatio)
 * from C language (struct Ratio).
 */
struct exdaq_Ratio exdaq_fromDaqRatio(daqRatio* daq);
daqRatio* exdaq_toDaqRatio(struct exdaq_Ratio native);

daqInt exdaq_fromDaqInteger(daqInteger* daq)
{
    daqInt native = 0;
    daqInteger_getValue(daq, &native);
    return native;
}

daqInteger* exdaq_toDaqInteger(daqInt native)
{
    daqInteger* daq = NULL;
    daqInteger_createInteger(&daq, native);
    return daq;
}

char* exdaq_fromDaqString(daqString* daq)
{
    char* native = NULL;
    daqString_getCharPtr(daq, &native);
    return native;
}

daqString* exdaq_toDaqString(const char* native)
{
    daqString* daq = NULL;
    daqString_createString(&daq, native);
    return daq;
}

uint8_t exdaq_fromDaqBoolean(daqBoolean* daq)
{
    uint8_t native = 0;
    daqBoolean_getValue(daq, &native);
    return native;
}

daqBoolean* exdaq_toDaqBoolean(uint8_t native)
{
    daqBoolean* daq = NULL;
    daqBoolean_createBoolean(&daq, native);
    return daq;
}

daqFloat exdaq_fromDaqFloat(daqFloatObject* daq)
{
    daqFloat native = 0;
    daqFloatObject_getValue(daq, &native);
    return native;
}

daqFloatObject* exdaq_toDaqFloat(daqFloat native)
{
    daqFloatObject* daq = 0;
    daqFloatObject_createFloatObject(&daq, native);
    return daq;
}

struct exdaq_ComplexNumber exdaq_fromDaqComplex(daqComplexNumber* daq)
{
    double real;
    double complex;
    daqComplexNumber_getReal(daq, &real);
    daqComplexNumber_getImaginary(daq, &complex);
    struct exdaq_ComplexNumber native = { real, complex };
    return native;
}

daqComplexNumber* exdaq_toDaqComplex(struct exdaq_ComplexNumber* native)
{
    daqComplexNumber* daq = NULL;
    daqComplexNumber_createComplexNumber(&daq, native->real, native->imaginary);
    return daq;
}

struct exdaq_Range exdaq_fromDaqRange(daqRange* daq)
{
    daqNumber* number = NULL;
    double min = 0;
    double max = 0; 

    daqRange_getLowValue(daq, &number);
    daqNumber_getFloatValue(number, &min);
    daqReleaseRef(number);

    daqRange_getHighValue(daq, &number);
    daqNumber_getFloatValue(number, &max);
    struct exdaq_Range native = { min, max };
    daqReleaseRef(number);
    return native;
}

daqRange* exdaq_toDaqRange(struct exdaq_Range native)
{
    daqRange* daq = NULL;
    daqNumber* lowValue = NULL;
    daqNumber* highValue = NULL;

    daqFloatObject* lowFloat = exdaq_toDaqFloat(native.min);
    daqFloatObject* highFloat = exdaq_toDaqFloat(native.max);

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

struct exdaq_Ratio exdaq_fromDaqRatio(daqRatio* daq)
{
    int64_t denominator = 0;
    int64_t numerator = 0;
    daqRatio_getDenominator(daq, &denominator);
    daqRatio_getNumerator(daq, &numerator);
    struct exdaq_Ratio native = { denominator, numerator };
    return native;
}

daqRatio* exdaq_toDaqRatio(struct exdaq_Ratio native)
{
    daqRatio* daq = NULL;
    daqRatio_createRatio(&daq, native.numerator, native.denominator);
    return daq;
}
