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

    // We can just read like in read_in_loop
    read_from_stream_reader(/*Fix the signal (cannot be an array, must be a single signal)*/availableSignals, 1000);

    // Alternatively construct a different reader and connect it to the signal

    daqReleaseRef(availableSignals);
    daqReleaseRef(streamReader);
    daqReleaseRef(signal);
    daqReleaseRef(instanceBuilder);
    daqReleaseRef(instance);
    return 0;
}