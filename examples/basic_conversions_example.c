/*
 * Example that demonstrates conversions between native C types 
 * and their openDAQ equvalents.
 */
#include <daq_c_conversions.h>

int main()
{
    // Integer example
    int64_t integer = 10;
    daqInteger* daqIn = exdaq_toDaqInteger(integer);
    int64_t integer2 = exdaq_fromDaqInteger(daqIn);
    printf("Integer is equal: %s\n", integer == integer2 ? "True": "False");
    daqReleaseRef(daqIn);

    // Float example
    double float1 = 1.0;
    daqFloatObject* daqFl = exdaq_toDaqFloat(float1);
    double float2 = exdaq_fromDaqFloat(daqFl);
    printf("Float is equal: %s\n", float1 == float2 ? "True" : "False");
    daqReleaseRef(daqFl);

    // String example
    char* string = "check";
    daqString* daqStr = exdaq_toDaqString(string);
    char* string2 = exdaq_fromDaqString(daqStr);
    printf("String is equal: %s\n", strcmp(string, string2) == 0 ? "True" : "False");
    daqReleaseRef(daqStr);

    // Bool example
    uint8_t boolean1 = True;
    daqBoolean* daqBl = exdaq_toDaqBoolean(boolean1);
    uint8_t boolean2 = exdaq_fromDaqBoolean(daqBl);
    printf("Boolean is equal: %s\n", boolean1 == boolean2 ? "True" : "False");
    daqReleaseRef(daqBl);

    // Complex Number example
    struct exdaq_ComplexNumber complexNumber = { 1,5 };
    daqComplexNumber* daqComplex = exdaq_toDaqComplex(&complexNumber);
    struct exdaq_ComplexNumber complexNumber2 = exdaq_fromDaqComplex(daqComplex);
    printf("Complex Number is equal: %s\n", ((complexNumber.imaginary == complexNumber2.imaginary) && (complexNumber.real == complexNumber2.real)) ? "True" : "False");
    daqReleaseRef(daqComplex);

    // Range example
    struct exdaq_Range range = { 1,10 };
    daqRange* daqRng = exdaq_toDaqRange(range);
    struct exdaq_Range range2 = exdaq_fromDaqRange(daqRng);
    printf("Range is equal: %s\n", ((range.max == range2.max) && (range.min == range2.min)) ? "True" : "False");
    daqReleaseRef(daqRng);
}