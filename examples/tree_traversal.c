/*
 * In this example we will demonstrate the structure of openDAQ tree structure
 * by traversing across it.
 */

#include <daq_utils.h>

enum ComponentType
{
    Unk = 0,
    Device,
    FunctionBlock,
    IOFolder,
    Channel,
    Server,
    Signal,
    Folder,
    Component,
    SyncComponent,
    InputPort
};

void componentTreePrintOut(daqDevice* headDevice, daqDict** listOfAvailableDevices, daqBool printout);

void printDaqSignal(daqSignal* signal, daqDict* listOfAvailableDevices, daqBool printout);
void printInputPort(daqInputPort* inputPort, daqDict* listOfAvailableDevices, daqBool printout);
void printDaqSyncComponent(daqSyncComponent* syncComp, daqDict* listOfAvailableDevices, daqBool printout);
void printDaqServer(daqServer* server, daqDict* listOfAvailableDevices, daqBool printout);
void printDaqFolder(daqFolder* folder, daqDict* listOfAvailableDevices, daqBool printout);
void printDaqFunctionBlock(daqFunctionBlock* functionBlock, daqDict* listOfAvailableDevices, daqBool printout);
void printDaqDevice(daqDevice* device, daqDict* listOfAvailableDevices, daqBool printout);

void addDaqComponentToDict(daqDict* listOfAvailableDevices, daqComponent* component);


void addDaqComponentToDict(daqDict* listOfAvailableDevices, daqComponent* component)
{
    daqString* globalId = NULL;
    daqString* description = NULL;
    daqComponent_getGlobalId(component, &globalId);
    daqComponent_getDescription(component, &description);

    daqDict_set(listOfAvailableDevices, globalId, description);

    daqReleaseRef(globalId);
    daqReleaseRef(description);
}

void componentTreePrintOut(daqDevice* headDevice, daqDict** listOfAvailableDevices, daqBool printout)
{
    daqDict_createDict(listOfAvailableDevices);
    printDaqDevice(headDevice, *listOfAvailableDevices, printout);
}

void printObjectList(daqList* list, daqDict* listOfAvailableDevices, daqBool printout)
{
    daqBaseObject* listMember = NULL;
    daqSizeT count = 0;
    daqList_getCount(list, &count);
    if (count <= 0)
        return;
    daqList_getItemAt(list, 0, &listMember);
    enum ComponentType componentType = Unk;

    if (DAQ_SUPPORTS_INTERFACE(listMember, DAQ_DEVICE_INTF_ID) && (componentType == Unk))
        componentType = Device;

    if (DAQ_SUPPORTS_INTERFACE(listMember, DAQ_SERVER_INTF_ID) && (componentType == Unk))
        componentType = Server;

    if (DAQ_SUPPORTS_INTERFACE(listMember, DAQ_SYNC_COMPONENT_INTF_ID) && (componentType == Unk))
        componentType = SyncComponent;

    if (DAQ_SUPPORTS_INTERFACE(listMember, DAQ_FUNCTION_BLOCK_INTF_ID) && (componentType == Unk))
        componentType = FunctionBlock;

    if (DAQ_SUPPORTS_INTERFACE(listMember, DAQ_FOLDER_INTF_ID) && (componentType == Unk))
        componentType = Folder;

    if (DAQ_SUPPORTS_INTERFACE(listMember, DAQ_INPUT_PORT_INTF_ID) && (componentType == Unk))
        componentType = InputPort;

    if (DAQ_SUPPORTS_INTERFACE(listMember, DAQ_SIGNAL_INTF_ID) && (componentType == Unk))
        componentType = Signal;

    daqSizeT numberOfObjects = 0;
    daqList_getCount(list, &numberOfObjects);

    for (daqSizeT i = 0; i < numberOfObjects; i++)
    {
        daqList_getItemAt(list, i, &listMember);

        switch (componentType)
        {
        case Device:
        {
            daqDevice* device = NULL;
            daqQueryInterface(listMember, DAQ_DEVICE_INTF_ID, &device);
            printDaqDevice(device, listOfAvailableDevices, printout);
            daqReleaseRef(device);
            daqReleaseRef(listMember);
            break;
        }
        case Server:
        {
            daqServer* server = NULL;
            daqQueryInterface(listMember, DAQ_SERVER_INTF_ID, &server);
            printDaqServer(server, listOfAvailableDevices, printout);
            daqReleaseRef(server);
            daqReleaseRef(listMember);
            break;
        }
        case SyncComponent:
        {
            daqSyncComponent* syncComponent = NULL;
            daqQueryInterface(listMember, DAQ_SYNC_COMPONENT_INTF_ID, &syncComponent);
            printDaqSyncComponent(syncComponent, listOfAvailableDevices, printout);
            daqReleaseRef(syncComponent);
            daqReleaseRef(listMember);
            break;
        }
        case FunctionBlock:
        {
            daqFunctionBlock* functionBlock = NULL;
            daqQueryInterface(listMember, DAQ_FUNCTION_BLOCK_INTF_ID, &functionBlock);
            printDaqFunctionBlock(functionBlock, listOfAvailableDevices, printout);
            daqReleaseRef(functionBlock);
            daqReleaseRef(listMember);
            break;
        }
        case Folder:
        {
            daqFolder* folder = NULL;
            daqQueryInterface(listMember, DAQ_FOLDER_INTF_ID, &folder);
            printDaqFolder(folder, listOfAvailableDevices, printout);
            daqReleaseRef(folder);
            daqReleaseRef(listMember);
            break;
        }
        case InputPort:
        {
            daqInputPort* inputPort = NULL;
            daqQueryInterface(listMember, DAQ_INPUT_PORT_INTF_ID, &inputPort);
            printInputPort(inputPort, listOfAvailableDevices, printout);
            daqReleaseRef(inputPort);
            daqReleaseRef(listMember);
            break;
        }
        case Signal:
        {
            daqSignal* signal = NULL;
            daqQueryInterface(listMember, DAQ_SIGNAL_INTF_ID, &signal);
            printDaqSignal(signal, listOfAvailableDevices, printout);
            daqReleaseRef(signal);
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

void printDaqDevice(daqDevice* device, daqDict* listOfAvailableDevices, daqBool printout)
{
    daqString* name = NULL;
    daqComponent_getName((daqComponent*)device, &name);

    if (printout && name != NULL)
        printDaqFormattedString("Device: %s\n", name);

    daqReleaseRef(name);

    addDaqComponentToDict(listOfAvailableDevices, (daqComponent*) device);

    daqFolder* ioFolder = NULL;
    daqDevice_getInputsOutputsFolder(device, &ioFolder);
    if (ioFolder != NULL)
    {
        printDaqFolder(ioFolder, listOfAvailableDevices, printout);
        daqReleaseRef(ioFolder);
    }

    daqSyncComponent* syncComponent = NULL;
    daqDevice_getSyncComponent(device, &syncComponent);
    if (syncComponent != NULL)
    {
        printDaqSyncComponent(syncComponent, listOfAvailableDevices, printout);
        daqReleaseRef(syncComponent);
    }

    daqList* devices = NULL;
    daqDevice_getDevices(device, &devices, NULL);
    if (devices != NULL)
    {
        printObjectList(devices, listOfAvailableDevices, printout);
        daqReleaseRef(devices);
    }

    daqList* functionBlocks = NULL;
    daqDevice_getFunctionBlocks(device, &functionBlocks, NULL);
    if (functionBlocks != NULL)
    {
        printObjectList(functionBlocks, listOfAvailableDevices, printout);
        daqReleaseRef(functionBlocks);
    }

    daqList* servers = NULL;
    daqDevice_getServers(device, &servers);
    if (servers != NULL)
    {
        printObjectList(servers, listOfAvailableDevices, printout);
        daqReleaseRef(servers);
    }
}

void printDaqFunctionBlock(daqFunctionBlock* functionBlock, daqDict* listOfAvailableDevices, daqBool printout)
{
    daqString* name = NULL;
    daqComponent_getName((daqComponent*)functionBlock, &name);

    if (printout && name != NULL)
        printDaqFormattedString("Function block: %s\n", name);

    daqReleaseRef(name);

    addDaqComponentToDict(listOfAvailableDevices, (daqComponent*) functionBlock);

    daqList* functionBlocks = NULL;
    daqFunctionBlock_getFunctionBlocks(functionBlock, &functionBlocks, NULL);
    if (functionBlocks != NULL)
    {
        printObjectList(functionBlocks, listOfAvailableDevices, printout);
        daqReleaseRef(functionBlocks);
    }

    daqList* inputPorts = NULL;
    daqFunctionBlock_getInputPorts(functionBlock, &inputPorts, NULL);
    if (inputPorts != NULL)
    {
        printObjectList(inputPorts, listOfAvailableDevices, printout);
        daqReleaseRef(inputPorts);
    }

    daqList* listOfSignals = NULL;
    daqFunctionBlock_getSignals(functionBlock, &listOfSignals, NULL);
    if (listOfSignals != NULL)
    {
        printObjectList(listOfSignals, listOfAvailableDevices, printout);
        daqReleaseRef(listOfSignals);
    }
}

void printDaqFolder(daqFolder* folder, daqDict* listOfAllDevices, daqBool printout)
{
    daqString* name = NULL;
    daqComponent_getName((daqComponent*)folder, &name);


    if (printout&& name!=NULL)
        printDaqFormattedString("Folder: %s\n", name);

    daqReleaseRef(name);

    addDaqComponentToDict(listOfAllDevices, (daqComponent*) folder);

    daqList* listOfItems = NULL;
    daqFolder_getItems(folder, &listOfItems, NULL);
    if (listOfItems != NULL)
    {
        printObjectList(listOfItems, listOfAllDevices, printout);
        daqReleaseRef(listOfItems);
    }
}

void printDaqServer(daqServer* server, daqDict* listOfAllAvailableDevices, daqBool printout)
{
    daqString* name = NULL;
    daqComponent_getName((daqComponent*)server, &name);

    if(printout && name != NULL)
        printDaqFormattedString("Server: %s\n", name);

    daqReleaseRef(name);

    addDaqComponentToDict(listOfAllAvailableDevices, (daqComponent*) server);

    daqList* listOfSignals = NULL;
    daqServer_getSignals(server, &listOfSignals, NULL);
    if (listOfSignals != NULL)
    {
        printObjectList(listOfSignals, listOfAllAvailableDevices, printout);
        daqReleaseRef(listOfSignals);
    }
}

void printDaqSyncComponent(daqSyncComponent* syncComp, daqDict* listOfAvailableDevices, daqBool printout)
{
    daqString* name = NULL;
    daqComponent_getName((daqComponent*)syncComp, &name);

    if(printout && name != NULL)
        printDaqFormattedString("Sync component: %s\n", name);

    daqReleaseRef(name);

    addDaqComponentToDict(listOfAvailableDevices, (daqComponent*) syncComp);
}

void printInputPort(daqInputPort* inputPort, daqDict* listOfAvailableDevices, daqBool printout)
{
    daqString* name = NULL;
    daqComponent_getName((daqComponent*)inputPort, &name);

    if (printout && name != NULL)
        printDaqFormattedString("Input port: %s\n", name);

    daqReleaseRef(name);

    addDaqComponentToDict(listOfAvailableDevices, (daqComponent*) inputPort);
}

void printDaqSignal(daqSignal* signal, daqDict* listOfAvailableDevices, daqBool printout)
{
    daqString* name = NULL;
    daqComponent_getName((daqComponent*)signal, &name);

    if (printout && name != NULL)
        printDaqFormattedString("Signal: %s\n", name);

    daqReleaseRef(name);

    addDaqComponentToDict(listOfAvailableDevices, (daqComponent*) signal);
}

int main()
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    daqDict* listOfComponents = NULL;

    componentTreePrintOut((daqDevice*)instance, &listOfComponents, True);

    daqSizeT count = 0;
    daqDict_getCount(listOfComponents, &count);
    printf("\nNumber of components in the openDAQ tree: %llu\n\n", count);

    daqList* keys = NULL;
    daqDict_getKeyList(listOfComponents, &keys);
    daqIterator* iterator = NULL;
    daqList_createStartIterator(keys, &iterator);

    while(daqIterator_moveNext(iterator) == DAQ_SUCCESS)
    {
        daqBaseObject* current = NULL;
        daqIterator_getCurrent(iterator, &current);
        daqString* comp = NULL;
        if(DAQ_SUPPORTS_INTERFACE(current, DAQ_STRING_INTF_ID))
        {
            daqQueryInterface(current, DAQ_STRING_INTF_ID, &comp);
            daqConstCharPtr constChar = NULL;
            daqString_getCharPtr(comp, &constChar);
            printf("%s\n", constChar);
        }
        daqReleaseRef(current);
    }

    daqReleaseRef(iterator);
    daqReleaseRef(keys);
    daqReleaseRef(listOfComponents);
    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);
    return 0;
}
