#include <copendaq.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

/*
 * Method boots up an openDAQ simulator with openDAQ native streaming enabled and
 * connects it to the reference device, which it uses as root.
 * The method returns the instance created, so we can control the lifetime of the simulator running.
 */
static inline daqErrCode setupSimulator(daqInstance** instance);

/*
 * Method connects a simulator device to a newly created instance and returns a pointer to the created
 * instance and a pointer to the connected simulator device.
 */
static inline daqErrCode addSimulator(daqDevice** device, daqInstance** instance);

/*
 * Method creates a simple instance that has its log level set at warning 
 * and a custom provided module path.
 */
static inline daqErrCode createInstance(daqInstance** instance, const char* modulePath);

/*
 * Method for easing printing daqStrings to standard output.
 * For specifiying the output format the const char* (the first argument) is used.
 */
static inline void printDaqFormattedString(const char* string, daqString* daqString);

/*
 * Gets the Domain Data Descriptor contained within the event-type reader status.
 */
static inline daqErrCode domainDescriptorFromReaderStatus(daqReaderStatus* status, daqDataDescriptor** domainDescriptor);

/*
 * Performs a zero-count read and returns the reader status obtained from the read operation.
 */
static inline daqErrCode zeroCountRead(daqStreamReader* reader, daqReaderStatus** status);

/*
 * Gets the descriptor's sample rate. Requires linear rule signal and uses delta.
 */
static inline daqErrCode getSampleRate(daqSizeT* sampleRate, daqDataDescriptor* domainDataDescriptor);

/*
 * Divides the inverse of tickResolution with the given delta to calculate the sample rate.
 */
static inline daqErrCode calculateSampleRate(daqSizeT* sampleRate, daqRatio* tickResolution, daqNumber* delta);

/*
 * Checks if the provided DataRule is linear.
 */
static inline int checkIsLinearRule(daqDataRule* dataRule);

void daqSleepMs(int milliseconds)
{
#ifdef _WIN32
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

static inline daqErrCode setupSimulator(daqInstance** instance)
{
    daqPropertyObject* config = NULL;

    daqPropertyObject_createPropertyObject(&config);

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

    daqProperty* serialNumberProperty = NULL;

    daqBoolean* serialNumberVisible = NULL;
    daqBoolean_createBoolean(&serialNumberVisible, True);

    daqString* serialNumberPropertyName = NULL;
    daqString_createString(&serialNumberPropertyName, "SerialNumber");

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

    daqInstanceBuilder_setGlobalLogLevel(instanceBuilder, daqLogLevelOff);

    daqErrCode err = 0;
    err = daqInstanceBuilder_build(instanceBuilder, instance);

    if (err != 0)
    {
        printf("Error occured when creating simulator device.");
        return DAQ_FAILED(err);
    }

    daqReleaseRef(modulePath);
    daqReleaseRef(instanceBuilder);


    daqPropertyObject* serverConfig = NULL;
    daqPropertyObject_createPropertyObject(&serverConfig);

    daqString* typeStr = NULL;
    daqString_createString(&typeStr, "OpenDAQNativeStreaming");

    daqServer* server = NULL;
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

    daqDevice_getAvailableDevices((daqDevice*)*instance, &availableDevices);

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

    daqInstanceBuilder_setGlobalLogLevel(instanceBuilder, daqLogLevelOff);

    daqInstance_createInstanceFromBuilder(instance, instanceBuilder);

    daqReleaseRef(modulePathStr);
    daqReleaseRef(instanceBuilder);

    return DAQ_SUCCESS;
}

static inline void printDaqFormattedString(const char* outputFormatString, daqString* daqString)
{
    daqConstCharPtr stringConstChar = NULL;
    daqString_getCharPtr(daqString, &stringConstChar);

    printf(outputFormatString, stringConstChar);
}

static inline daqErrCode zeroCountRead(daqStreamReader* reader, daqReaderStatus** status)
{
    daqSizeT count = 0;
    daqFloat samples[1];
    return daqStreamReader_read(reader, &samples, &count, 1000, status);
}

static inline daqErrCode domainDescriptorFromReaderStatus(daqReaderStatus* status, daqDataDescriptor** domainDescriptor)
{
    daqEventPacket* eventPacket = NULL;
    daqReaderStatus_getEventPacket(status, &eventPacket);
    daqString* eventId = NULL;
    daqEventPacket_getEventId(eventPacket, &eventId);

    daqBool check = False;
    daqString* checkStr = NULL;
    daqString_createString(&checkStr, "DATA_DESCRIPTOR_CHANGED");

    daqBaseObject_equals(eventId, checkStr, &check);

    daqReleaseRef(checkStr);
    daqReleaseRef(eventId);

    if (check == False)
    {
        daqReleaseRef(eventPacket);
        return DAQ_ERR_INVALID_DATA;
    }

    daqDict* parameters = NULL;
    daqEventPacket_getParameters(eventPacket, &parameters);
    daqString* domainDescriptorStr = NULL;
    daqString_createString(&domainDescriptorStr, "DomainDataDescriptor");

    daqDict_get(parameters, domainDescriptorStr, (daqBaseObject**) domainDescriptor);
    daqReleaseRef(domainDescriptorStr);
    daqReleaseRef(parameters);

    daqReleaseRef(eventPacket);

    return DAQ_SUCCESS;
}

static inline daqErrCode calculateSampleRate(daqSizeT* sampleRate, daqRatio* tickResolution, daqNumber* delta)
{
    daqFloat deltaFloat = 0;
    daqNumber_getFloatValue(delta, &deltaFloat);

    daqInt numerator = 1;
    daqRatio_getNumerator(tickResolution, &numerator);

    daqInt denominator = 1;
    daqRatio_getDenominator(tickResolution, &denominator);

    *sampleRate = (daqSizeT) ((daqFloat) denominator / (daqFloat) numerator / deltaFloat);

    return DAQ_SUCCESS;
}

static inline daqErrCode getSampleRate(daqSizeT* sampleRate, daqDataDescriptor* domainDataDescriptor)
{
    daqDataRule* dataRule = NULL;
    daqDataDescriptor_getRule(domainDataDescriptor, &dataRule);

    if (!checkIsLinearRule(dataRule))
    {
        printf("Data rule of the signal is not linear, therefore we cannot calculate sample rate.");
        daqReleaseRef(dataRule);
        return DAQ_ERR_INVALID_DATA;
    }

    daqRatio* ratio = NULL;
    daqDataDescriptor_getTickResolution(domainDataDescriptor, &ratio);

    daqDict* parametersDataRule = NULL;
    daqDataRule_getParameters(dataRule, &parametersDataRule);

    daqString* deltaString = NULL;
    daqString_createString(&deltaString, "delta");

    daqBaseObject* deltaObj = NULL;
    daqDict_get(parametersDataRule, deltaString, &deltaObj);

    daqNumber* delta = NULL;
    daqQueryInterface(deltaObj, DAQ_NUMBER_INTF_ID, &delta);

    calculateSampleRate(sampleRate, ratio, delta);

    daqReleaseRef(delta);
    daqReleaseRef(deltaObj);
    daqReleaseRef(deltaString);
    daqReleaseRef(parametersDataRule);
    daqReleaseRef(ratio);

    daqReleaseRef(dataRule);

    return DAQ_SUCCESS;
}

static inline int checkIsLinearRule(daqDataRule* dataRule)
{
    daqDataRuleType dataRuleType;
    daqDataRule_getType(dataRule, &dataRuleType);

    return dataRuleType == daqDataRuleTypeLinear;
}

struct PropFields
{
    daqCoreType valueType;
    uint8_t name;
    uint8_t description;
    uint8_t unit;
    uint8_t minValue;
    uint8_t maxValue;
    uint8_t defaultValue;
    uint8_t visible;
    uint8_t realOnly;
    uint8_t selectionValue;
    uint8_t suggestedValues;
    uint8_t refProp;
    uint8_t callableInfo;
};

void printCommonMetadata(daqProperty* property)
{
    daqBool visible = False;
    daqBool readOnly = False;
    daqProperty_getVisible(property, &visible);
    daqProperty_getReadOnly(property, &readOnly);

    printf("- Visible: %s\n", visible == True ? "True" : "False");
    printf("- Read only: %s\n", readOnly == True ? "True" : "False");
}

void printIntProp(daqProperty* property, daqBool metadata, daqBool val)
{
    daqBaseObject* temp = NULL;
    daqInteger* value = NULL;

    if (metadata)
    {
        printCommonMetadata(property);

        // Default value
        daqProperty_getDefaultValue(property, &temp);
        daqQueryInterface(temp, DAQ_INTEGER_INTF_ID, &value);
        int64_t valueNative = daq_fromDaqInteger(value);
        printf("- Default value: %lld\n", valueNative);
        daqReleaseRef(value);
        daqReleaseRef(temp);

        // Min/Max value
        daqNumber* edgeCase = NULL;
        int64_t minNativeValue = 0;
        int64_t maxNativeValue = 0;
        daqProperty_getMinValue(property, &edgeCase);
        if(edgeCase != NULL)
        {
            daqNumber_getIntValue(edgeCase, &minNativeValue);
            printf("- Minimal value: %lld\n", minNativeValue);
            daqReleaseRef(edgeCase);
            daqProperty_getMaxValue(property, &edgeCase);
            if(edgeCase!=NULL)
            {
                daqNumber_getIntValue(edgeCase, &maxNativeValue);
                printf("- Maximum value: %lld\n", maxNativeValue);
                daqReleaseRef(edgeCase);
            }
        }

        // Suggested values
        daqList* suggestedValues = NULL;
        daqProperty_getSuggestedValues(property, &suggestedValues);
        if (suggestedValues != NULL)
        {
            printf("- Suggested values:\n");

            daqSizeT count = 0;
            daqList_getCount(suggestedValues, &count);
            for (daqSizeT i = 0; i < count; i++)
            {
                daqList_getItemAt(suggestedValues, i, &temp);
                daqQueryInterface(temp, DAQ_INTEGER_INTF_ID, &value);
                valueNative = daq_fromDaqFloat(value);
                daqReleaseRef(value);
                daqReleaseRef(temp);
                printf(" -- %lld\n", valueNative);
            }
            daqReleaseRef(suggestedValues);
        }
    }

    if (val)
    {
        // Value
        daqProperty_getValue(property, &temp);
        daqQueryInterface(temp, DAQ_INTEGER_INTF_ID, &value);
        int64_t valueNative = daq_fromDaqInteger(value);
        printf("- Value: %lld\n", valueNative);
        daqReleaseRef(value);
        daqReleaseRef(temp);
    }
}

void printBoolProp(daqProperty* property, daqBool metadata, daqBool val)
{
    daqBaseObject* temp = NULL;
    daqBoolean* value = NULL;
    if (metadata)
    {
        printCommonMetadata(property);

        // Default value
        daqProperty_getDefaultValue(property, &temp);
        daqQueryInterface(temp, DAQ_BOOLEAN_INTF_ID, &value);
        daqBool valueNative = False;
        daqBoolean_getValue(value, &valueNative);
        daqReleaseRef(value);
        daqReleaseRef(temp);

        printf("- Default value: %s\n", valueNative == True ? "True" : "False");
    }
    if (val)
    {
        // Value
        daqProperty_getValue(property, &temp);
        daqQueryInterface(temp, DAQ_BOOLEAN_INTF_ID, &value);
        daqBool valueNative = False;
        daqBoolean_getValue(value, &valueNative);
        daqReleaseRef(value);
        daqReleaseRef(temp);

        printf("- Value: %s\n", valueNative);
    }
}

void printFloatProp(daqProperty* property, daqBool metadata, daqBool val)
{
    daqBaseObject* temp = NULL;
    daqFloatObject* value = NULL;
    if (metadata)
    {
        // Default value
        daqProperty_getDefaultValue(property, &temp);
        daqQueryInterface(temp, DAQ_FLOAT_OBJECT_INTF_ID, &value);
        double valueNative = 0;
        daqFloatObject_getValue(value, &valueNative);
        daqReleaseRef(value);
        daqReleaseRef(temp);

        printf("- Default value: %f\n", valueNative);

        // Min/Max value
        daqNumber* edgeCase = NULL;
        double minNativeValue = 0;
        double maxNativeValue = 0;
        daqProperty_getMinValue(property, &edgeCase);
        if(edgeCase != NULL)
        {
            daqNumber_getFloatValue(edgeCase, &minNativeValue);
            printf("- Minimal value: %f\n", minNativeValue);
            daqReleaseRef(edgeCase);
            daqProperty_getMaxValue(property, &edgeCase);
            if (edgeCase != NULL)
            {
                daqNumber_getFloatValue(edgeCase, &maxNativeValue);
                printf("- Maximum value: %f\n", maxNativeValue);
                daqReleaseRef(edgeCase);
            }
        }

        // Suggested values
        daqList* suggestedValues = NULL;
        daqProperty_getSuggestedValues(property, &suggestedValues);
        if (suggestedValues != NULL)
        {
            printf("- Suggested values:\n");

            daqSizeT count = 0;
            daqList_getCount(suggestedValues, &count);
            for(daqSizeT i = 0; i<count;i++)
            {
                daqList_getItemAt(suggestedValues, i, &temp);
                daqQueryInterface(temp, DAQ_FLOAT_OBJECT_INTF_ID, &value);
                valueNative = daq_fromDaqFloat(value);
                daqReleaseRef(value);
                daqReleaseRef(temp);
                printf(" -- %f\n", valueNative);
            }
            daqReleaseRef(suggestedValues);
        }
    }
    if(val)
    {
        
    }
}