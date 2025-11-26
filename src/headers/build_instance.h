#include <copendaq.h>
#include <stdio.h>

void checkErrorState()
{
    daqString* errorString = NULL;

    if (errorString != NULL)
    {
        daqCharPtr* errorCharConst = NULL;
        daqString_getCharPtr(errorString, errorCharConst);
        printf("%s", *errorCharConst);
        daqReleaseRef(errorString);
        daqReleaseRef(errorCharConst);
    }
}

void createInstance(daqInstance** instance)
{
    daqInstanceBuilder* builder = NULL;

    if (daqInstanceBuilder_createInstanceBuilder(&builder) != DAQ_SUCCESS)
    {
        checkErrorState();
    }

    daqString* modulePath = NULL;

    if (daqString_createString(&modulePath, MODULE_PATH) != DAQ_SUCCESS)
    {
        checkErrorState();
    }

    daqReleaseRef(modulePath);

    daqString* localId = NULL;

    daqString_createString(&localId, "");

    daqInstanceBuilder_setDefaultRootDeviceLocalId(builder, localId);

    daqReleaseRef(localId);

    if (daqInstance_createInstanceFromBuilder(instance, builder) != DAQ_SUCCESS)
    {
        checkErrorState();
        return;
    }

    daqReleaseRef(builder);
}