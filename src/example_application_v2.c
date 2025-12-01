#include <example_application.h>

// 

int main()
{
    daqInstanceBuilder* instanceBuilder = NULL;
    daqInstance* instance = NULL;
    
    daqInstanceBuilder_createInstanceBuilder(&instanceBuilder);

    daqInstanceBuilder_build(instanceBuilder, &instance);

    daqList* availableSignals = NULL;
    daqStreamReaderBuilder* streamReaderBuilder = NULL;

    setup(instance, &availableSignals, &streamReaderBuilder);

    daqSignal* signal = NULL;

    if (daqList_getItemAt(availableSignals, 0, (daqBaseObject**)&signal) != DAQ_SUCCESS)
    {
        printf("There has occured an error during the retrieval process of the signal");
        daqReleaseRef(availableSignals);
        daqReleaseRef(instanceBuilder);
        daqReleaseRef(instance);
        return 0;
    }

    daqStreamReader* streamReader = NULL;
    daqStreamReaderBuilder_build(streamReaderBuilder, &streamReader);

    daqSignal* signal = NULL;

    // Get the first signal from the list of available ones and then connect a stream reader onto it (similar to read_in_loop)

    // Stream reader

    // Multi reader

    // 


    daqReleaseRef(availableSignals);
    daqReleaseRef(streamReader);
    daqReleaseRef(signal);
    daqReleaseRef(instanceBuilder);
    daqReleaseRef(instance);
    return 0;
}