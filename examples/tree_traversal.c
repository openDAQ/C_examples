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

void printComponentTree(daqDevice* headDevice, daqBool printout);

enum ComponentType getComponentType(daqBaseObject* baseObject);

void printDaqDevice(daqBaseObject* baseObject, daqBool printout);

void printDaqFunctionBlock(daqBaseObject* baseObject, daqBool printout);

void printDaqFolder(daqBaseObject* baseObject, daqBool printout);

void printDaqServer(daqBaseObject* baseObject, daqBool printout);

void printDaqSyncComponent(daqBaseObject* baseObject, daqBool printout);

void printInputPort(daqBaseObject* baseObject, daqBool printout);

void printDaqSignal(daqBaseObject* baseObject, daqBool printout);

void printComponentTree(daqDevice* headDevice, daqBool printout)
{
    printDaqDevice(headDevice, printout);
}

enum ComponentType getComponentType(daqBaseObject* baseObject)
{
    enum ComponentType componentType = DaqUnknown;

    if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_DEVICE_INTF_ID))
        componentType = DaqDevice;

    else if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_SERVER_INTF_ID))
        componentType = DaqServer;

    else if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_SYNC_COMPONENT_INTF_ID))
        componentType = DaqSyncComponent;

    else if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_FUNCTION_BLOCK_INTF_ID))
        componentType = DaqFunctionBlock;

    else if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_FOLDER_INTF_ID))
        componentType = DaqFolder;

    else if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_INPUT_PORT_INTF_ID))
        componentType = DaqInputPort;

    else if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_SIGNAL_INTF_ID))
        componentType = DaqSignal;

    return componentType;
}

void printObjectList(daqList* list, enum ComponentType compType, daqBool printout)
{
    daqBaseObject* listMember = NULL;
    daqSizeT count = 0;
    daqList_getCount(list, &count);
    if (count <= 0)
        return;
    daqList_getItemAt(list, 0, &listMember);
    
    enum ComponentType componentType = compType;

    if (componentType == DaqUnknown)
        componentType = getComponentType(listMember);

    count = 0;
    daqList_getCount(list, &count);

    for (daqSizeT i = 0; i < count; i++)
    {
        daqList_getItemAt(list, i, &listMember);

        switch (componentType)
        {
        case DaqDevice:
        {
            printDaqDevice(listMember, printout);
            daqReleaseRef(listMember);
            break;
        }
        case DaqServer:
        {
            printDaqServer(listMember, printout);
            daqReleaseRef(listMember);
            break;
        }
        case DaqSyncComponent:
        {
            printDaqSyncComponent(listMember, printout);
            daqReleaseRef(listMember);
            break;
        }
        case DaqFunctionBlock:
        {
            printDaqFunctionBlock(listMember, printout);
            daqReleaseRef(listMember);
            break;
        }
        case DaqFolder:
        {
            printDaqFolder(listMember, printout);
            daqReleaseRef(listMember);
            break;
        }
        case DaqInputPort:
        {
            printInputPort(listMember, printout);
            daqReleaseRef(listMember);
            break;
        }
        case DaqSignal:
        {
            printDaqSignal(listMember, printout);
            daqReleaseRef(listMember);
            break;
        }
        default:
        {
            daqReleaseRef(listMember);
            break;
        }
        }
    }
}

void printDaqDevice(daqBaseObject* baseObject, daqBool printout)
{
    daqDevice* device = NULL;
    daqQueryInterface(baseObject, DAQ_DEVICE_INTF_ID, &device);
    daqString* localId = NULL;
    daqComponent_getLocalId((daqComponent*)device, &localId);

    if (printout && localId != NULL)
        printDaqFormattedString("Device: %s\n", localId);

    daqReleaseRef(localId);

    daqFolder* ioFolder = NULL;
    daqDevice_getInputsOutputsFolder(device, &ioFolder);
    if (ioFolder != NULL)
    {
        printDaqFolder(ioFolder, printout);
        daqReleaseRef(ioFolder);
    }

    daqSyncComponent* syncComponent = NULL;
    daqDevice_getSyncComponent(device, &syncComponent);
    if (syncComponent != NULL)
    {
        printDaqSyncComponent(syncComponent, printout);
        daqReleaseRef(syncComponent);
    }

    daqList* devices = NULL;
    daqDevice_getDevices(device, &devices, NULL);
    if (devices != NULL)
    {
        printObjectList(devices, DaqDevice, printout);
        daqReleaseRef(devices);
    }

    daqList* functionBlocks = NULL;
    daqDevice_getFunctionBlocks(device, &functionBlocks, NULL);
    if (functionBlocks != NULL)
    {
        printObjectList(functionBlocks, DaqFunctionBlock, printout);
        daqReleaseRef(functionBlocks);
    }

    daqList* servers = NULL;
    daqDevice_getServers(device, &servers);
    if (servers != NULL)
    {
        printObjectList(servers, DaqServer, printout);
        daqReleaseRef(servers);
    }

    daqReleaseRef(device);
}

void printDaqFunctionBlock(daqBaseObject* baseObject, daqBool printout)
{
    daqFunctionBlock* functionBlock = NULL;
    daqQueryInterface(baseObject, DAQ_FUNCTION_BLOCK_INTF_ID, &functionBlock);
    daqString* localId = NULL;
    daqComponent_getLocalId((daqComponent*)functionBlock, &localId);

    if (printout && localId != NULL)
        printDaqFormattedString("Function block: %s\n", localId);

    daqReleaseRef(localId);

    daqList* functionBlocks = NULL;
    daqFunctionBlock_getFunctionBlocks(functionBlock, &functionBlocks, NULL);
    if (functionBlocks != NULL)
    {
        printObjectList(functionBlocks, DaqFunctionBlock, printout);
        daqReleaseRef(functionBlocks);
    }

    daqList* inputPorts = NULL;
    daqFunctionBlock_getInputPorts(functionBlock, &inputPorts, NULL);
    if (inputPorts != NULL)
    {
        printObjectList(inputPorts, DaqInputPort, printout);
        daqReleaseRef(inputPorts);
    }

    daqList* listOfSignals = NULL;
    daqFunctionBlock_getSignals(functionBlock, &listOfSignals, NULL);
    if (listOfSignals != NULL)
    {
        printObjectList(listOfSignals, DaqSignal, printout);
        daqReleaseRef(listOfSignals);
    }

    daqReleaseRef(functionBlock);
}

void printDaqFolder(daqBaseObject* baseObject, daqBool printout)
{
    daqFolder* folder = NULL;
    daqQueryInterface(baseObject, DAQ_FOLDER_INTF_ID, &folder);
    daqString* localId = NULL;
    daqComponent_getName((daqComponent*)folder, &localId);

    if (printout && localId!=NULL)
        printDaqFormattedString("Folder: %s\n", localId);

    daqReleaseRef(localId);

    daqList* listOfItems = NULL;
    daqFolder_getItems(folder, &listOfItems, NULL);
    if (listOfItems != NULL)
    {
        printObjectList(listOfItems, DaqUnknown, printout);
        daqReleaseRef(listOfItems);
    }

    daqReleaseRef(folder);
}

void printDaqServer(daqBaseObject* baseObject, daqBool printout)
{
    daqServer* server = NULL;
    daqQueryInterface(baseObject, DAQ_SERVER_INTF_ID, &server);

    daqString* localId = NULL;
    daqComponent_getLocalId((daqComponent*)server, &localId);

    if(printout && localId != NULL)
        printDaqFormattedString("Server: %s\n", localId);

    daqReleaseRef(localId);

    daqList* listOfSignals = NULL;
    daqServer_getSignals(server, &listOfSignals, NULL);
    if (listOfSignals != NULL)
    {
        printObjectList(listOfSignals, DaqSignal, printout);
        daqReleaseRef(listOfSignals);
    }

    daqReleaseRef(server);
}

void printDaqSyncComponent(daqBaseObject* baseObject, daqBool printout)
{
    daqSyncComponent* syncComp = NULL;
    daqQueryInterface(baseObject, DAQ_SYNC_COMPONENT_INTF_ID, &syncComp);
    daqString* localId = NULL;
    daqComponent_getLocalId((daqComponent*)syncComp, &localId);

    if(printout && localId != NULL)
        printDaqFormattedString("Sync component: %s\n", localId);

    daqReleaseRef(localId);

    daqReleaseRef(syncComp);
}

void printInputPort(daqBaseObject* baseObject, daqBool printout)
{
    daqInputPort* inputPort = NULL;
    daqQueryInterface(baseObject, DAQ_INPUT_PORT_INTF_ID, &inputPort);
    daqString* localId = NULL;
    daqComponent_getLocalId((daqComponent*)inputPort, &localId);

    if (printout && localId != NULL)
        printDaqFormattedString("Input port: %s\n", localId);

    daqReleaseRef(localId);

    daqReleaseRef(inputPort);
}

void printDaqSignal(daqBaseObject* baseObject, daqBool printout)
{
    daqSignal* signal = NULL;
    daqQueryInterface(baseObject, DAQ_SIGNAL_INTF_ID, &signal);
    daqString* localId = NULL;
    daqComponent_getLocalId((daqComponent*)signal, &localId);

    if (printout && localId != NULL)
        printDaqFormattedString("Signal: %s\n", localId);

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

    printDaqDevice((daqDevice*) instance, True);

    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);
    return 0;
}
