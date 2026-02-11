/*
 * Example that displays Conversions between native C types and their openDAQ equvalents.
 */
#include <daq_c_conversions.h>

int main()
{
    // Integer example
    int64_t i = 10;
    daqInteger* daqIn = daqExample_toDaqInteger(i);
    int64_t i2 = daqExample_fromDaqInteger(daqIn);
    printf("Integer is equal: %s\n", i == i2 ? "True": "False");
    daqReleaseRef(daqIn);

    // Float example
    double j = 1.0;
    daqFloatObject* daqFl = daqExample_toDaqFloat(j);
    double j2 = daqExample_fromDaqFloat(daqFl);
    printf("Float is equal: %s\n", j == j2 ? "True" : "False");
    daqReleaseRef(daqFl);

    // String example
    char* k = "check";
    daqString* daqStr = daqExample_toDaqString(k);
    char* k2 = daqExample_fromDaqString(daqStr);
    printf("String is equal: %s\n", strcmp(k, k2) == 0 ? "True" : "False");
    daqReleaseRef(daqStr);

    // Bool example
    uint8_t l = True;
    daqBoolean* daqBl = daqExample_toDaqBoolean(l);
    uint8_t l2 = daqExample_fromDaqBoolean(daqBl);
    printf("Boolean is equal: %s\n", l == l2 ? "True" : "False");
    daqReleaseRef(daqBl);

    // Complex Number example
    struct daqExample_ComplexNumber m = { 1,5 };
    daqComplexNumber* daqComplex = daqExample_toDaqComplex(&m);
    struct daqExample_ComplexNumber m2 = daqExample_fromDaqComplex(daqComplex);
    printf("Complex Number is equal: %s\n", ((m.imaginary == m2.imaginary) && (m.real == m2.real)) ? "True" : "False");
    daqReleaseRef(daqComplex);

    // Range example
    struct daqExample_Range n = { 1,10 };
    daqRange* daqRng = daqExample_toDaqRange(n);
    struct daqExample_Range n2 = daqExample_fromDaqRange(daqRng);
    printf("Range is equal: %s\n", ((n.max == n2.max) && (n.min == n2.min)) ? "True" : "False");
    daqReleaseRef(daqRng);
}