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

    // Error handle
    if (daqInstanceBuilder_setModulePath(builder, modulePath) != DAQ_SUCCESS)
    {
        checkErrorState();
    }

    // Error handle
    daqReleaseRef(modulePath);

    daqString* localId = NULL;

    // Error handle
    daqString_createString(&localId, "");

    // Error handle
    daqInstanceBuilder_setDefaultRootDeviceLocalId(builder, localId);

    // Error handle
    daqReleaseRef(localId);

    // Error handle
    daqInstance_createInstanceFromBuilder(instance, builder);

    // Error handle
    daqReleaseRef(builder);
}