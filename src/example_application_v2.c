#include <example_application.h>

int main()
{
    daqInstanceBuilder* instanceBuilder = NULL;
    daqInstance* instance = NULL;
    
    daqInstanceBuilder_createInstanceBuilder(&instanceBuilder);

    daqInstanceBuilder_build(instanceBuilder, &instance);

    daqList* availableSignals = NULL;
    daqStreamReader* streamReader = NULL;

    setup(instance, &availableSignals, &streamReader);

    daqSignal* signal = NULL;

    if (daqList_getItemAt(availableSignals, 0, (daqBaseObject**)&signal) != DAQ_SUCCESS)
    {
        printf("There has occured an error during the retrieval process of the signal");
        daqReleaseRef(availableSignals);
        daqReleaseRef(instanceBuilder);
        daqReleaseRef(instance);
        return 0;
    }

    reading(streamReader, 0, signal);

    daqReleaseRef(availableSignals);
    daqReleaseRef(streamReader);
    daqReleaseRef(signal);
    daqReleaseRef(instanceBuilder);
    daqReleaseRef(instance);
    return 0;
}