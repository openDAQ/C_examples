#include <copendaq.h>

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


void 

int main()
{
	// Create an instance
	daqInstance* instance = NULL;
	createInstance(instance);

	// TODO: Display that the insance itself was created

	daqReleaseRef(instance);
	
    return 0;
}