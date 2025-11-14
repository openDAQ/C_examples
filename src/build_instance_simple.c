#include <copendaq.h>
#include <stdio.h>

void createInstance(daqInstance* instance)
{
    daqInstanceBuilder* builder = NULL;
    daqInstanceBuilder_createInstanceBuilder(&builder);

    daqString* modulePath = NUL;
    daqString_createString(&modulePath, MODULE_PATH);
    daqInstanceBuilder_setModulePath(builder, modulePath);
    daqReleaseRef(modulePath);

    daqString* localId = NULL;
    daqString_createString(&localId, "");
    daqInstanceBuilder_setDefaultRootDeviceLocalId(builder, localId);
    daqReleaseRef(localId);
    daqInstance_createInstanceFromBuilder(&instance, builder);
    daqReleaseRef(builder);
}


int main()
{
    // Create an instance
    daqInstance* instance = NULL;
    createInstance(instance);

    if (instance == NULL)
    {
        printf("Failed to create instance.\r\n");
    }
    else
    {
        printf("Instance successfully created.\r\n");
    }

    daqReleaseRef(instance);

    return 0;
}