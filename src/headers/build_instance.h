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

// Method for creating an Instance of openDAQ in its default state.
void createInstanceWithDefaultParameters(daqInstance** instance)
{
    daqInstanceBuilder* builder = NULL;

    if (daqInstanceBuilder_createInstanceBuilder(&builder) != DAQ_SUCCESS)
    {
        checkErrorState();
    }

    if (daqInstance_createInstanceFromBuilder(instance, builder) != DAQ_SUCCESS)
    {
        checkErrorState();
        return;
    }

    daqReleaseRef(builder);
}


// Method for creating Instance with custom modulePath and LocalId parameters
void createInstanceWithCustomParameters(daqString* moduleCustomPath, daqString* customLocalId, daqInstance** instance)
{
    daqInstanceBuilder* builder = NULL;

    daqInstanceBuilder_createInstanceBuilder(&builder);

    daqString* modulePath = NULL;

    daqString_createString(&modulePath, MODULE_PATH);

    daqBool equalPaths = NULL;

    daqBaseObject_equals((daqBaseObject*)modulePath, (daqBaseObject*)moduleCustomPath, &equalPaths);

    if (equalPaths == True) 
    {
        daqInstanceBuilder_addModulePath(builder, modulePath);
    }
    else
    {
        // Here we will assume that moduleCustomPath is non-empty
        daqInstanceBuilder_addModulePath(builder, moduleCustomPath);
    }

    daqString* localId = NULL;

    daqString_createString(&localId, "");

    daqBool equalId = NULL;

    daqBaseObject_equals((daqBaseObject*)localId, (daqBaseObject*)customLocalId, &equalId);

    if (equalId == True)
    {
        daqInstanceBuilder_getDefaultRootDeviceLocalId(builder, localId);
    }
    else
    {
        // Again the same assumption as above. (customLocalId is non-empty)
        daqInstanceBuilder_getDefaultRootDeviceLocalId(builder, customLocalId);
    }

    daqInstance_createInstanceFromBuilder(instance, builder);

    daqReleaseRef(builder);
    daqReleaseRef(modulePath);
    daqReleaseRef(localId);
}
