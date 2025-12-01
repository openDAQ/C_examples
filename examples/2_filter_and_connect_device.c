#include <daq_utils.h>

daqErrCode create_instance_and_return_all_available_devices(daqInstance** instance, daqList** availableDevices)
{
    daqInstanceBuilder* builder = NULL;
    daqErrCode err = daqInstanceBuilder_createInstanceBuilder(&builder);

    if (err != DAQ_SUCCESS)
        return err;

    daqErrCode err2 = daqInstance_createInstanceFromBuilder(instance, builder);

    if (err2 != DAQ_SUCCESS)
    {
    daqReleaseRef(builder);
    return err2;
    }

    daqErrCode err3 = daqDevice_getAvailableDevices((daqDevice*)*instance, availableDevices);
    if (err3 != DAQ_SUCCESS)
    {
        daqReleaseRef(builder);
        return err3;
    }

    return DAQ_SUCCESS;
}


// In this example we will search through the devices to find one that has its manufacturer set to 'openDAQ'.
// Then connect our created instance to the selected device.

int main()
{
    daqInstance* instance = NULL;
    daqList* availableDevices = NULL;

    if (create_instance_and_return_all_available_devices(&instance, &availableDevices) != DAQ_SUCCESS)
    {
        return 1;
    }

    daqIterator* iterator = NULL;
    daqList_createStartIterator(availableDevices, &iterator);

    daqDevice* connectedDevice = NULL;

    int connected = 0;

    while(daqIterator_moveNext(iterator) == DAQ_SUCCESS)
    {
        daqDeviceInfo* currentDevInfo = NULL;
        daqIterator_getCurrent(iterator, (daqBaseObject*)&currentDevInfo);


        daqString* manufacturer = NULL;
        daqDeviceInfo_getManufacturer(currentDevInfo, &manufacturer);


        daqConstCharPtr manufacturer_constChar = NULL;
        daqString_getCharPtr(manufacturer, &manufacturer_constChar);

        int result = strcmp(manufacturer_constChar, "openDAQ");

        if (!result)
        {
            daqString* connectionString = NULL;
            daqDeviceInfo_getConnectionString(currentDevInfo, &connectionString);

            daqDevice_addDevice((daqDevice*)instance, &connectedDevice, connectionString, NULL);
            daqReleaseRef(connectionString);
            
            connected = 1;
        }

        daqReleaseRef(currentDevInfo);
        daqReleaseRef(manufacturer);

        if (connected)
            break;
    }

    if (connected) {
        daqDeviceInfo* connectedDeviceInfo = NULL;
        daqDevice_getInfo(connectedDevice, &connectedDeviceInfo);

        daqString* name = NULL;
        daqDeviceInfo_getName(connectedDeviceInfo, &name);

        daqConstCharPtr nameConstChar = NULL;
        daqString_getCharPtr(name, &nameConstChar);

        printf("The name of the connected device is: %s\n", nameConstChar);

        daqReleaseRef(name);
        daqReleaseRef(connectedDevice);
    }
    daqReleaseRef(instance);
    daqReleaseRef(availableDevices);
    return 0;
}
