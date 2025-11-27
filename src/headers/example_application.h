#include <read_in_loop.h>

void setup(daqInstance* instance, daqList** availableSignals, daqStreamReaderBuilder** streamReader)
{
    // In the example we will connect to the first available device and connect the streamReader to the first available signal
    
}

void reading(daqStreamReader* reader, daqSignal* signal, daqUInt duration)
{
    // Here we will read with domain from the signal that is connected to the 
}
