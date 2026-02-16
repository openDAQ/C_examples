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
daqInteger* exdaq_toDaqInteger(daqInt c_value);

// String conversion
/*
 * Conversion from and to openDAQ String (daqString) core type
 * from C language (const char*). The function that converts to native C return a non-owning pointer.
 * Precautions should be taken, when using the received pointer.
 */
char* exdaq_fromDaqString(daqString* daq_value);
daqString* exdaq_toDaqString(const char* c_value);

// Bool conversion
/*
 * Conversion from and to openDAQ Boolean (daqString) core type from C language (uint8_t).
 */
uint8_t exdaq_fromDaqBoolean(daqBoolean* daq_value);
daqBoolean* exdaq_toDaqBoolean(uint8_t c_value);

// Float conversion
/*
 *Conversion from and to openDAQ String (daqFloat) core type from C language (double).
 */
daqFloat exdaq_fromDaqFloat(daqFloatObject* daq_value);
daqFloatObject* exdaq_toDaqFloat(daqFloat c_value);

// Complex number conversion
/*
 * Conversion from and to openDAQ Complex (daqComplex) core type
 * from C language (struct ComplexNumber).
 */
struct exdaq_ComplexNumber exdaq_fromDaqComplex(daqComplexNumber* daq_value);
daqComplexNumber* exdaq_toDaqComplex(struct exdaq_ComplexNumber* c_value);

// Range conversion
/*
 * Conversion from and to openDAQ Struct (daqRange) core type
 * from C language (struct Range).
 */
struct exdaq_Range exdaq_fromDaqRange(daqRange* daq_value);
daqRange* exdaq_toDaqRange(struct exdaq_Range c_value);

// Ratio conversion
/*
 * Conversion from and to openDAQ Ratio Struct (daqRatio)
 * from C language (struct Ratio).
 */
struct exdaq_Ratio exdaq_fromDaqRatio(daqRatio* daq_value);
daqRatio* exdaq_toDaqRatio(struct exdaq_Ratio c_value);

daqInt exdaq_fromDaqInteger(daqInteger* daq_value)
{
    daqInt c_value = 0;
    daqInteger_getValue(daq_value, &c_value);
    return c_value;
}

daqInteger* exdaq_toDaqInteger(daqInt c_value)
{
    daqInteger* daq_value = NULL;
    daqInteger_createInteger(&daq_value, c_value);
    return daq_value;
}

char* exdaq_fromDaqString(daqString* daq_value)
{
    char* c_value = NULL;
    daqString_getCharPtr(daq_value, &c_value);
    return c_value;
}

daqString* exdaq_toDaqString(const char* c_value)
{
    daqString* daq_value = NULL;
    daqString_createString(&daq_value, c_value);
    return daq_value;
}

uint8_t exdaq_fromDaqBoolean(daqBoolean* daq_value)
{
    uint8_t c_value = 0;
    daqBoolean_getValue(daq_value, &c_value);
    return c_value;
}

daqBoolean* exdaq_toDaqBoolean(uint8_t c_value)
{
    daqBoolean* daq_value = NULL;
    daqBoolean_createBoolean(&daq_value, c_value);
    return daq_value;
}

daqFloat exdaq_fromDaqFloat(daqFloatObject* daq_value)
{
    daqFloat c_value = 0;
    daqFloatObject_getValue(daq_value, &c_value);
    return c_value;
}

daqFloatObject* exdaq_toDaqFloat(daqFloat c_value)
{
    daqFloatObject* daq_value = NULL;
    daqFloatObject_createFloatObject(&daq_value, c_value);
    return daq_value;
}

struct exdaq_ComplexNumber exdaq_fromDaqComplex(daqComplexNumber* daq_value)
{
    double real = 0;
    double complex = 0;
    daqComplexNumber_getReal(daq_value, &real);
    daqComplexNumber_getImaginary(daq_value, &complex);
    struct exdaq_ComplexNumber c_value = { real, complex };
    return c_value;
}

daqComplexNumber* exdaq_toDaqComplex(struct exdaq_ComplexNumber* c_value)
{
    daqComplexNumber* daq_value = NULL;
    daqComplexNumber_createComplexNumber(&daq_value, c_value->real, c_value->imaginary);
    return daq_value;
}

struct exdaq_Range exdaq_fromDaqRange(daqRange* daq_value)
{
    daqNumber* number = NULL;
    double min = 0;
    double max = 0; 
    daqRange_getLowValue(daq_value, &number);
    daqNumber_getFloatValue(number, &min);
    daqReleaseRef(number);

    daqRange_getHighValue(daq_value, &number);
    daqNumber_getFloatValue(number, &max);
    daqReleaseRef(number);

    struct exdaq_Range c_value = { min, max };
    return c_value;
}

daqRange* exdaq_toDaqRange(struct exdaq_Range c_value)
{
    daqRange* daq_value = NULL;
    daqNumber* lowValue = NULL;
    daqNumber* highValue = NULL;
    daqFloatObject* lowFloat = exdaq_toDaqFloat(c_value.min);
    daqQueryInterface(lowFloat, DAQ_NUMBER_INTF_ID, &lowValue);
    daqReleaseRef(lowFloat);

    daqFloatObject* highFloat = exdaq_toDaqFloat(c_value.max);
    daqQueryInterface(highFloat, DAQ_NUMBER_INTF_ID, &highValue);
    daqReleaseRef(highFloat);

    // Create and populate the daqRange object
    daqRange_createRange(&daq_value, lowValue, highValue);
    daqReleaseRef(lowValue);
    daqReleaseRef(highValue);
    return daq_value;
}

struct exdaq_Ratio exdaq_fromDaqRatio(daqRatio* daq_value)
{
    int64_t denominator = 0;
    int64_t numerator = 0;
    daqRatio_getDenominator(daq_value, &denominator);
    daqRatio_getNumerator(daq_value, &numerator);
    struct exdaq_Ratio c_value = { denominator, numerator };
    return c_value;
}

daqRatio* exdaq_toDaqRatio(struct exdaq_Ratio c_value)
{
    daqRatio* daq_value = NULL;
    daqRatio_createRatio(&daq_value, c_value.numerator, c_value.denominator);
    return daq_value;
}
