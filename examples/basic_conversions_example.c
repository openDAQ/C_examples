/*
 *
 */
#include <daq_c_conversions.h>

int main()
{
    daqInteger* daq = NULL;
    daqInteger_createInteger(&daq, 7);

    int64_t native = daq_fromDaqInteger(daq);
    
    daqInteger* daqCreated = daq_toDaqInteger(native);

    uint8_t check = False;
    daqBaseObject_equals(daq, daqCreated, &check);

    if (check)
        printf("We create an openDAQ Object and converted it from and to C.\n");
    else
        printf("Something went wrong.\n");
}