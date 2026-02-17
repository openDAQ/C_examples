/*
 * In openDAQ devices are represented by a tree structure. Example demonstrates how to fully explore that tree and displays
 * the type of the object alongside its name. After saving every object into an dictionary, example displays all of saved objects
 * globalIds.
 */

#include <daq_utils.h>

enum exdaq_ComponentType
{
    exdaq_ComponentType_Unknown = 0,
    exdaq_ComponentType_Device,
    exdaq_ComponentTypeFunctionBlock,
    exdaq_ComponentType_IOFolder,
    exdaq_ComponentType_Channel,
    exdaq_ComponentType_Server,
    exdaq_ComponentType_Signal,
    exdaq_ComponentType_Folder,
    exdaq_ComponentType_Component,
    exdaq_ComponentType_SyncComponent,
    exdaq_ComponentType_InputPort
};

void printDaqObject(daqBaseObject* baseObject, enum exdaq_ComponentType compType, uint8_t indent);

void printDaqDevice(daqBaseObject* baseObject, uint8_t indent);

void printDaqChannel(daqBaseObject* baseObject, uint8_t indent);

void printDaqFunctionBlock(daqBaseObject* baseObject, uint8_t indent);

void printDaqIOFolder(daqBaseObject* baseObject, uint8_t indent);

void printDaqServer(daqBaseObject* baseObject, uint8_t indent);

void printDaqSyncComponent(daqBaseObject* baseObject, uint8_t indent);

void printDaqInputPort(daqBaseObject* baseObject, uint8_t indent);

void printDaqSignal(daqBaseObject* baseObject, uint8_t indent);

void printDaqObject(daqBaseObject* baseObject, enum exdaq_ComponentType compType, uint8_t indent)
{
    switch (compType)
    {
    case exdaq_ComponentType_Device:
        printDaqDevice(baseObject, indent);
        break;

    case exdaq_ComponentType_Server:
        printDaqServer(baseObject, indent);
        break;

    case exdaq_ComponentType_SyncComponent:
        printDaqSyncComponent(baseObject, indent);
        break;

    case exdaq_ComponentType_Channel:
        printDaqChannel(baseObject, indent);
        break;

    case exdaq_ComponentTypeFunctionBlock:
        printDaqFunctionBlock(baseObject, indent);
        break;

    case exdaq_ComponentType_Folder:
        printDaqIOFolder(baseObject, indent);
        break;

    case exdaq_ComponentType_InputPort:
        printDaqInputPort(baseObject, indent);
        break;

    case exdaq_ComponentType_Signal:
        printDaqSignal(baseObject, indent);
        break;

    default:
        break;
    }
}

void printObjectList(daqList* list, enum exdaq_ComponentType compType, uint8_t indent)
{
    daqBaseObject* listMember = NULL;
    daqSizeT count = 0;
    daqList_getCount(list, &count);
    if (count <= 0)
        return;
    daqList_getItemAt(list, 0, &listMember);

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

    // IOFolder is a special type of folder that only accepts
    // Channels and IOFolders components.
    daqFolder* ioFolder = NULL;
    daqDevice_getInputsOutputsFolder(device, &ioFolder);
    if (ioFolder != NULL)
    {
        printDaqIOFolder(ioFolder, indent+1);
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
        printObjectList(devices, exdaq_ComponentType_Device, indent+1);
        daqReleaseRef(devices);
    }

    daqList* functionBlocks = NULL;
    daqDevice_getFunctionBlocks(device, &functionBlocks, NULL);
    if (functionBlocks != NULL)
    {
        printObjectList(functionBlocks, exdaq_ComponentTypeFunctionBlock, indent+1);
        daqReleaseRef(functionBlocks);
    }

    daqList* servers = NULL;
    daqDevice_getServers(device, &servers);
    if (servers != NULL)
    {
        printObjectList(servers, exdaq_ComponentType_Server, indent+1);
        daqReleaseRef(servers);
    }

    daqReleaseRef(localId);
    daqReleaseRef(device);
}

void printDaqChannel(daqBaseObject* baseObject, uint8_t indent)
{
    // Channels represent Physical IO in openDAQ.
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
        printObjectList(functionBlocks, exdaq_ComponentTypeFunctionBlock, indent+1);
        daqReleaseRef(functionBlocks);
    }

    daqList* inputPorts = NULL;
    daqFunctionBlock_getInputPorts(functionBlock, &inputPorts, NULL);
    if (inputPorts != NULL)
    {
        printObjectList(inputPorts, exdaq_ComponentType_InputPort, indent+1);
        daqReleaseRef(inputPorts);
    }

    daqList* listOfSignals = NULL;
    daqFunctionBlock_getSignals(functionBlock, &listOfSignals, NULL);
    if (listOfSignals != NULL)
    {
        printObjectList(listOfSignals, exdaq_ComponentType_Signal, indent+1);
        daqReleaseRef(listOfSignals);
    }

    daqReleaseRef(localId);
    daqReleaseRef(functionBlock);
}

void printDaqIOFolder(daqBaseObject* baseObject, uint8_t indent)
{
    daqFolder* folder = NULL;
    daqQueryInterface(baseObject, DAQ_FOLDER_INTF_ID, &folder);
    daqString* localId = NULL;
    daqComponent_getName((daqComponent*) folder, &localId);

    if (localId != NULL)
    {
        printf("%*c", indent, ' ');
        printDaqFormattedString("IOFolder: %s\n", localId);
        daqReleaseRef(localId);
    }

    daqList* listOfItems = NULL;
    daqFolder_getItems(folder, &listOfItems, NULL);

    daqSizeT count = 0;
    daqList_getCount(listOfItems, &count);

    if (listOfItems == NULL)
    {
        daqReleaseRef(folder);
        return;
    }

    daqList* listOfFolders = NULL;
    daqList* listOfChannels = NULL;
    daqBaseObject* item = NULL;

    daqList_createList(&listOfFolders);
    daqList_createList(&listOfChannels);

    for (daqSizeT i = 0; i < count; i++)
    {
        daqList_getItemAt(listOfItems, i, &item);
        
        if (DAQ_SUPPORTS_INTERFACE(item, DAQ_FOLDER_INTF_ID))
        {
            daqList_pushBack(listOfFolders, item);
        }
        else if (DAQ_SUPPORTS_INTERFACE(item, DAQ_SIGNAL_INTF_ID))
        {
            daqList_pushBack(listOfChannels, item);
        }
    }

    printObjectList(listOfFolders, exdaq_ComponentType_Folder, indent+1);
    printObjectList(listOfChannels, exdaq_ComponentType_Signal, indent+1);
    daqReleaseRef(listOfFolders);
    daqReleaseRef(listOfChannels);
    daqReleaseRef(listOfItems);

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
        printObjectList(listOfSignals, exdaq_ComponentType_Signal, indent+1);
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

void printDaqInputPort(daqBaseObject* baseObject, uint8_t indent)
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
