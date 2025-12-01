#include <copendaq.h>
#include <stdio.h>
#include <string.h>

/*
 * TODO: Slightly longer description
 */
static inline daqErrCode setupSimulator(daqInstance** instance);

/*
 * TODO: Slightly longer description
 */
static inline daqErrCode addSimulator(daqDevice** device, daqInstance** instance);

/*
 * TODO: Slightly longer description
 */
static inline daqErrCode createInstance(daqInstance** instance, const char* modulePath);

/*
 * TODO: Slightly longer description
 */
static inline void printDaqFormattedString(const char* string, daqString* daqString);

static inline daqErrCode setupSimulator(daqInstance** instance)
{
    daqPropertyObject* config = NULL;

    daqPropertyObject_createPropertyObject(&config);

    // Create String property 'Name'
    daqProperty* nameProperty = NULL;

    daqBoolean* nameVisible = NULL;
    daqBoolean_createBoolean(&nameVisible, True);

    daqString* namePropertyName = NULL;
    daqString_createString(&namePropertyName, "Name");

    daqString* nameDefaultValue = NULL;
    daqString_createString(&nameDefaultValue, "Reference device simulator");

    daqProperty_createStringProperty(&nameProperty, namePropertyName, nameDefaultValue, nameVisible);

    daqReleaseRef(nameVisible);
    daqReleaseRef(nameDefaultValue);
    daqReleaseRef(namePropertyName);

    // Create String property 'LocalId'
    daqProperty* localIdProperty = NULL;

    daqBoolean* localIdVisible = NULL;
    daqBoolean_createBoolean(&localIdVisible, True);

    daqString* localIdPropertyName = NULL;
    daqString_createString(&localIdPropertyName, "LocalId");

    daqString* localIdDefaultValue = NULL;
    daqString_createString(&localIdDefaultValue, "RefDevSimulator");

    daqProperty_createStringProperty(&localIdProperty, localIdPropertyName, localIdDefaultValue, localIdVisible);

    daqReleaseRef(localIdVisible);
    daqReleaseRef(localIdDefaultValue);
    daqReleaseRef(localIdPropertyName);

    // Create String property 'SerialNumber'
    daqProperty* serialNumberProperty = NULL;

    daqBoolean* serialNumberVisible = NULL;
    daqBoolean_createBoolean(&serialNumberVisible, True);

    daqString* serialNumberPropertyName = NULL;
    daqString_createString(&serialNumberPropertyName, "Serial number");

    daqString* serialNumberDefaultValue = NULL;
    daqString_createString(&serialNumberDefaultValue, "sim01");

    daqProperty_createStringProperty(&serialNumberProperty, serialNumberPropertyName, serialNumberDefaultValue, serialNumberVisible);

    daqReleaseRef(serialNumberVisible);
    daqReleaseRef(serialNumberDefaultValue);
    daqReleaseRef(serialNumberPropertyName);

    daqPropertyObject_addProperty(config, nameProperty);
    daqPropertyObject_addProperty(config, localIdProperty);
    daqPropertyObject_addProperty(config, serialNumberProperty);

    daqReleaseRef(nameProperty);
    daqReleaseRef(localIdProperty);
    daqReleaseRef(serialNumberProperty);

    daqInstanceBuilder* instanceBuilder = NULL;

    daqInstanceBuilder_createInstanceBuilder(&instanceBuilder);

    daqString* discoveryServer = NULL;
    daqString_createString(&discoveryServer, "mdns");

    daqInstanceBuilder_addDiscoveryServer(instanceBuilder, discoveryServer);

    daqString* rootDeviceName = NULL;
    daqString_createString(&rootDeviceName, "daqref://device0");

    daqInstanceBuilder_setRootDevice(instanceBuilder, rootDeviceName, config);

    daqReleaseRef(config);
    daqReleaseRef(discoveryServer);
    daqReleaseRef(rootDeviceName);

    daqString* modulePath = NULL;
    daqString_createString(&modulePath, MODULE_PATH);

    daqInstanceBuilder_setModulePath(instanceBuilder, modulePath);

    daqInstance_createInstanceFromBuilder(instance, instanceBuilder);

    daqReleaseRef(modulePath);
    daqReleaseRef(instanceBuilder);


    daqPropertyObject* serverConfig = NULL;
    daqPropertyObject_createPropertyObject(&serverConfig);

    daqString* typeStr = NULL;
    daqString_createString(&typeStr, "");

    daqServer* server;
    daqDevice_addServer((daqDevice*)*instance, typeStr, serverConfig, &server);
    daqServer_enableDiscovery(server);

    daqReleaseRef(typeStr);
    daqReleaseRef(server);
    daqReleaseRef(serverConfig);

    return DAQ_SUCCESS;
}

static inline daqErrCode addSimulator(daqDevice** device, daqInstance** instance)
{
    createInstance(instance, MODULE_PATH);

    daqList* availableDevices = NULL;

    daqDevice_getAvailableDevices((daqDevice*)*instance, availableDevices);

    daqIterator* iterator = NULL;
    daqList_createStartIterator(availableDevices, &iterator);

    daqBool sameSerial = False;
    daqBool sameManufacturer = False;

    while (daqIterator_moveNext(iterator) == DAQ_SUCCESS)
    {
        daqDeviceInfo* currentDeviceInfo = NULL;
        daqIterator_getCurrent(iterator, &currentDeviceInfo);

        daqString* serialNumberCurrent = NULL;
        daqDeviceInfo_getSerialNumber(currentDeviceInfo, &serialNumberCurrent);

        daqString* manufacturerCurrent = NULL;
        daqDeviceInfo_getManufacturer(currentDeviceInfo, &manufacturerCurrent);

        daqConstCharPtr serialNumberCurrentConstChar = NULL;
        daqString_getCharPtr(serialNumberCurrent, &serialNumberCurrentConstChar);

        daqConstCharPtr manufacturerCurrentConstChar = NULL;
        daqString_getCharPtr(manufacturerCurrent, &manufacturerCurrentConstChar);

        sameSerial = !strcmp(serialNumberCurrentConstChar, "sim01");
        sameManufacturer = !strcmp(manufacturerCurrentConstChar, "openDAQ");

        daqReleaseRef(serialNumberCurrent);
        daqReleaseRef(manufacturerCurrent);

        if (sameSerial && sameManufacturer)
        {
            daqString* connectionString = NULL;
            daqDeviceInfo_getConnectionString(currentDeviceInfo, &connectionString);

            daqDevice_addDevice((daqDevice*)*instance, device, connectionString, NULL);

            daqReleaseRef(currentDeviceInfo);
            break;
        }

        daqReleaseRef(currentDeviceInfo);
    }

    daqReleaseRef(iterator);
    daqReleaseRef(availableDevices);

    return DAQ_SUCCESS;
}

static inline daqErrCode createInstance(daqInstance** instance, const char* modulePath)
{
    daqInstanceBuilder* instanceBuilder = NULL;
    daqInstanceBuilder_createInstanceBuilder(&instanceBuilder);

    daqString* modulePathStr = NULL;
    daqString_createString(&modulePathStr, modulePath);

    daqInstanceBuilder_addModulePath(instanceBuilder, modulePathStr);

    daqInstance_createInstanceFromBuilder(instance, instanceBuilder);

    daqReleaseRef(modulePathStr);
    daqReleaseRef(instanceBuilder);

    return DAQ_SUCCESS;
}

static inline void printDaqFormattedString(const char* outputFormatString, daqString* daqString)
{
    // TODO: Maybe refactor this again if a nicer way is known...
    daqConstCharPtr stringConstChar = NULL;
    daqString_getCharPtr(daqString, &stringConstChar);

    const char* combined = malloc(strlen(outputFormatString) + strlen("%s\n" + 1));
    strcpy(combined, outputFormatString);
    strcat(combined, "%s\n");

    printf(combined, stringConstChar);
}