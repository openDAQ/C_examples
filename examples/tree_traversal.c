/*
 * In openDAQ devices are represented by a tree structure. Example demonstrates how to fully explore that tree and displays
 * the type of the object alongside its name. After saving every object into an dictionary, example displays all of saved objects
 * globalIds.
 */

#include <daq_utils.h>

enum ComponentType
{
    DaqUnknown = 0,
    DaqDevice,
    DaqFunctionBlock,
    DaqIOFolder,
    DaqChannel,
    DaqServer,
    DaqSignal,
    DaqFolder,
    DaqComponent,
    DaqSyncComponent,
    DaqInputPort
};

void printDaqObject(daqBaseObject* baseObject, enum ComponentType compType, uint8_t indent);

enum ComponentType getComponentType(daqBaseObject* baseObject);

void printDaqDevice(daqBaseObject* baseObject, uint8_t indent);

void printDaqChannel(daqBaseObject* baseObject, uint8_t indent);

void printDaqFunctionBlock(daqBaseObject* baseObject, uint8_t indent);

void printDaqFolder(daqBaseObject* baseObject, uint8_t indent);

void printDaqServer(daqBaseObject* baseObject, uint8_t indent);

void printDaqSyncComponent(daqBaseObject* baseObject, uint8_t indent);

void printInputPort(daqBaseObject* baseObject, uint8_t indent);

void printDaqSignal(daqBaseObject* baseObject, uint8_t indent);

void printDaqObject(daqBaseObject* baseObject, enum ComponentType compType, uint8_t indent)
{
    switch (compType)
    {
    case DaqDevice:
        printDaqDevice(baseObject, indent + 1);
        break;

    case DaqServer:
        printDaqServer(baseObject, indent + 1);
        break;

    case DaqSyncComponent:
        printDaqSyncComponent(baseObject, indent + 1);
        break;

    case DaqChannel:
        printDaqChannel(baseObject, indent + 1);
        break;

    case DaqFunctionBlock:
        printDaqFunctionBlock(baseObject, indent + 1);
        break;

    case DaqFolder:
        printDaqFolder(baseObject, indent + 1);
        break;

    case DaqInputPort:
        printInputPort(baseObject, indent + 1);
        break;

    case DaqSignal:
        printDaqSignal(baseObject, indent + 1);
        break;

    default:
        break;
    }
}

enum ComponentType getComponentType(daqBaseObject* baseObject)
{
    if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_DEVICE_INTF_ID))
        return DaqDevice;

    else if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_SERVER_INTF_ID))
        return DaqServer;

    else if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_SYNC_COMPONENT_INTF_ID))
        return DaqSyncComponent;

    else if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_CHANNEL_INTF_ID))
        return DaqChannel;

    else if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_FUNCTION_BLOCK_INTF_ID))
        return DaqFunctionBlock;

    else if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_FOLDER_INTF_ID))
        return DaqFolder;

    else if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_INPUT_PORT_INTF_ID))
        return DaqInputPort;

    else if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_SIGNAL_INTF_ID))
        return DaqSignal;

    return DaqUnknown;
}

void printObjectList(daqList* list, enum ComponentType compType, uint8_t indent)
{
    daqBaseObject* listMember = NULL;
    daqSizeT count = 0;
    daqList_getCount(list, &count);
    if (count <= 0)
        return;
    daqList_getItemAt(list, 0, &listMember);

    if (compType == DaqUnknown)
        compType = getComponentType(listMember);

    count = 0;
    daqList_getCount(list, &count);

    for (daqSizeT i = 0; i < count; i++)
    {
        daqList_getItemAt(list, i, &listMember);

        printDaqObject(listMember, compType, indent);

        daqReleaseRef(listMember);
    }
}

void printDaqDevice(daqBaseObject* baseObject, uint8_t indent)
{
    daqDevice* device = NULL;
    daqQueryInterface(baseObject, DAQ_DEVICE_INTF_ID, &device);
    daqString* localId = NULL;
    daqComponent_getLocalId((daqComponent*)device, &localId);

    if (localId != NULL)
    {
        printf("%*c", indent, ' ');
        printDaqFormattedString("Device: %s\n", localId);
    }

    // IOFolder is a SPECIAL TYPE of FOLDER that only accepts
    // IChannel and IIoFolderConfig components.
    daqFolder* ioFolder = NULL;
    daqDevice_getInputsOutputsFolder(device, &ioFolder);
    if (ioFolder != NULL)
    {
        printDaqFolder(ioFolder, indent+1);
        daqReleaseRef(ioFolder);
    }

    daqSyncComponent* syncComponent = NULL;
    daqDevice_getSyncComponent(device, &syncComponent);
    if (syncComponent != NULL)
    {
        printDaqSyncComponent(syncComponent, indent+1);
        daqReleaseRef(syncComponent);
    }

    daqList* devices = NULL;
    daqDevice_getDevices(device, &devices, NULL);
    if (devices != NULL)
    {
        printObjectList(devices, DaqDevice, indent+1);
        daqReleaseRef(devices);
    }

    daqList* functionBlocks = NULL;
    daqDevice_getFunctionBlocks(device, &functionBlocks, NULL);
    if (functionBlocks != NULL)
    {
        printObjectList(functionBlocks, DaqFunctionBlock, indent+1);
        daqReleaseRef(functionBlocks);
    }

    daqList* servers = NULL;
    daqDevice_getServers(device, &servers);
    if (servers != NULL)
    {
        printObjectList(servers, DaqServer, indent+1);
        daqReleaseRef(servers);
    }

    daqReleaseRef(localId);
    daqReleaseRef(device);
}

void printDaqChannel(daqBaseObject* baseObject, uint8_t indent)
{
    // Channels represent physical sensors in openDAQ.
    // Their internal structure is the same as that of the function block.
    printDaqFunctionBlock(baseObject, indent);
}

void printDaqFunctionBlock(daqBaseObject* baseObject, uint8_t indent)
{
    daqFunctionBlock* functionBlock = NULL;
    daqQueryInterface(baseObject, DAQ_FUNCTION_BLOCK_INTF_ID, &functionBlock);
    daqString* localId = NULL;
    daqComponent_getLocalId((daqComponent*)functionBlock, &localId);

    if (localId != NULL)
    {
        printf("%*c", indent, ' ');
        printDaqFormattedString("Function block: %s\n", localId);
    }

    daqList* functionBlocks = NULL;
    daqFunctionBlock_getFunctionBlocks(functionBlock, &functionBlocks, NULL);
    if (functionBlocks != NULL)
    {
        printObjectList(functionBlocks, DaqFunctionBlock, indent+1);
        daqReleaseRef(functionBlocks);
    }

    daqList* inputPorts = NULL;
    daqFunctionBlock_getInputPorts(functionBlock, &inputPorts, NULL);
    if (inputPorts != NULL)
    {
        printObjectList(inputPorts, DaqInputPort, indent+1);
        daqReleaseRef(inputPorts);
    }

    daqList* listOfSignals = NULL;
    daqFunctionBlock_getSignals(functionBlock, &listOfSignals, NULL);
    if (listOfSignals != NULL)
    {
        printObjectList(listOfSignals, DaqSignal, indent+1);
        daqReleaseRef(listOfSignals);
    }

    daqReleaseRef(localId);
    daqReleaseRef(functionBlock);
}

void printDaqFolder(daqBaseObject* baseObject, uint8_t indent)
{
    daqFolder* folder = NULL;
    daqQueryInterface(baseObject, DAQ_FOLDER_INTF_ID, &folder);
    daqString* localId = NULL;
    daqComponent_getName((daqComponent*) folder, &localId);

    if (localId != NULL)
    {
        printf("%*c", indent, ' ');
        printDaqFormattedString("Folder: %s\n", localId);
    }

    daqList* listOfItems = NULL;
    daqFolder_getItems(folder, &listOfItems, NULL);
    if (listOfItems != NULL)
    {
        printObjectList(listOfItems, DaqUnknown, indent+1);
        daqReleaseRef(listOfItems);
    }

    daqReleaseRef(localId);
    daqReleaseRef(folder);
}

void printDaqServer(daqBaseObject* baseObject, uint8_t indent)
{
    daqServer* server = NULL;
    daqQueryInterface(baseObject, DAQ_SERVER_INTF_ID, &server);

    daqString* localId = NULL;
    daqComponent_getLocalId((daqComponent*)server, &localId);

    if (localId != NULL)
    {
        printf("%*c", indent, ' ');
        printDaqFormattedString("Server: %s\n", localId);
    }

    daqList* listOfSignals = NULL;
    daqServer_getSignals(server, &listOfSignals, NULL);
    if (listOfSignals != NULL)
    {
        printObjectList(listOfSignals, DaqSignal, indent+1);
        daqReleaseRef(listOfSignals);
    }

    daqReleaseRef(localId);
    daqReleaseRef(server);
}

void printDaqSyncComponent(daqBaseObject* baseObject, uint8_t indent)
{
    daqSyncComponent* syncComp = NULL;
    daqQueryInterface(baseObject, DAQ_SYNC_COMPONENT_INTF_ID, &syncComp);
    daqString* localId = NULL;
    daqComponent_getLocalId((daqComponent*)syncComp, &localId);

    if (localId != NULL)
    {
        printf("%*c", indent, ' ');
        printDaqFormattedString("Sync component: %s\n", localId);
    }

    daqReleaseRef(localId);
    daqReleaseRef(syncComp);
}

void printInputPort(daqBaseObject* baseObject, uint8_t indent)
{
    daqInputPort* inputPort = NULL;
    daqQueryInterface(baseObject, DAQ_INPUT_PORT_INTF_ID, &inputPort);
    daqString* localId = NULL;
    daqComponent_getLocalId((daqComponent*)inputPort, &localId);

    if (localId != NULL)
    {
        printf("%*c", indent, ' ');
        printDaqFormattedString("Input port: %s\n", localId);
    }

    daqReleaseRef(localId);
    daqReleaseRef(inputPort);
}

void printDaqSignal(daqBaseObject* baseObject, uint8_t indent)
{
    daqSignal* signal = NULL;
    daqQueryInterface(baseObject, DAQ_SIGNAL_INTF_ID, &signal);
    daqString* localId = NULL;
    daqComponent_getLocalId((daqComponent*)signal, &localId);

    if (localId != NULL)
    {
        printf("%*c", indent, ' ');
        printDaqFormattedString("Signal: %s\n", localId);
    }

    daqReleaseRef(localId);
    daqReleaseRef(signal);
}

int main()
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    printDaqDevice((daqDevice*) instance, 0);

    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);
    return 0;
}
