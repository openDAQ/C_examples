/*
 * Example that displays Conversions between native C types and their openDAQ equvalents.
 */
#include <daq_c_conversions.h>

int main()
{
    // Integer example
    int64_t i = 10;
    daqInteger* daqInt = daq_toDaqInteger(i);
    int64_t i2 = daq_fromDaqInteger(daqInt);
    printf("Integer is equal: %s\n", i == i2 ? "True": "False");
    daqReleaseRef(daqInt);

    // Float example
    double j = 1.0;
    daqFloatObject* daqFloat = daq_toDaqFloat(j);
    double j2 = daq_fromDaqFloat(daqFloat);
    printf("Float is equal: %s\n", j == j2 ? "True" : "False");
    daqReleaseRef(daqFloat);

    // String example
    char* k = "check";
    daqString* daqStr = daq_toDaqString(k);
    char* k2 = daq_fromDaqString(daqStr);
    printf("String is equal: %s\n", strcmp(k, k2) == 0 ? "True" : "False");
    daqReleaseRef(daqStr);

    // Bool example
    uint8_t l = True;
    daqBoolean* daqBl = daq_toDaqBoolean(l);
    uint8_t l2 = daq_fromDaqBoolean(daqBl);
    printf("Boolean is equal: %s\n", l == l2 ? "True" : "False");
    daqReleaseRef(daqBl);

    // Complex Number example
    struct ComplexNumber m = { 1,5 };
    daqComplexNumber* daqComplex = daq_toDaqComplex(&m);
    struct ComplexNumber m2 = daq_fromDaqComplex(daqComplex);
    printf("Complex Number is equal: %s\n", ((m.imaginary == m2.imaginary) && (m.real == m2.real)) ? "True" : "False");
    daqReleaseRef(daqComplex);

    // Range example
    struct Range n = { 1,10 };
    daqRange* daqRng = daq_toDaqRange(n);
    struct Range n2 = daq_fromDaqRange(daqRng);
    printf("Range is equal: %s\n", ((n.max == n2.max) && (n.min == n2.min)) ? "True" : "False");
    daqReleaseRef(daqRng);
}