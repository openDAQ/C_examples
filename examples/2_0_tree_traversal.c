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

struct ComponentInfo
{
    daqComponent* reference;
    enum ComponentType type;
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
    // Check for emptyness of list should be done outside
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
    daqDeviceInfo* deviceInfo = NULL;
    daqDevice_getInfo(device, &deviceInfo);
    daqCharPtr* str = NULL;
    daqBaseObject_toString((daqBaseObject*)deviceInfo, str);

    addDaqComponentToDict(listOfAvailableDevices, (daqComponent*) device);

    if(printout && str != NULL)
        printf("Hello from device. Device Info: \n%s\n", *str);

    daqReleaseRef(deviceInfo);

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

    // The following Structures can appear in multiples, so we handle them all.
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
    // Missing self display
    daqCharPtr* str = NULL;
    daqBaseObject_toString((daqBaseObject*)functionBlock, str);
    
    addDaqComponentToDict(listOfAvailableDevices, (daqComponent*) functionBlock);

    if (printout && str != NULL)
        printf("Hello from function block. Function Block: \n%s\n", *str);

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
    daqCharPtr* str = NULL;
    daqBaseObject_toString((daqBaseObject*)folder, str);

    addDaqComponentToDict(listOfAllDevices, (daqComponent*) folder);

    if (printout&& str!=NULL)
        printf("Hello from folder. Folder:\n%s\n", *str);

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
    daqCharPtr* str = NULL;
    daqBaseObject_toString((daqBaseObject*)server, str);

    addDaqComponentToDict(listOfAllAvailableDevices, (daqComponent*) server);

    if(printout && str != NULL)
        printf("Hello from server. Server:\n%s\n", *str);

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
    // Leaf node
    daqCharPtr* str = NULL;
    daqBaseObject_toString((daqBaseObject*)syncComp, str);

    addDaqComponentToDict(listOfAvailableDevices, (daqComponent*) syncComp);

    if(printout && str != NULL)
        printf("Hello from sync component. Sync component:\n%s\n", *str);
}

void printInputPort(daqInputPort* inputPort, daqDict* listOfAvailableDevices, daqBool printout)
{
    daqCharPtr* str = NULL;
    daqBaseObject_toString((daqBaseObject*)inputPort, str);

    addDaqComponentToDict(listOfAvailableDevices, (daqComponent*) inputPort);

    if (printout && str != NULL)
        printf("Hello from input port. InputPort:\n%s\n", *str);
}

void printDaqSignal(daqSignal* signal, daqDict* listOfAvailableDevices, daqBool printout)
{
    // Leaf node
    daqCharPtr* str = NULL;
    daqBaseObject_toString((daqBaseObject*)signal, str);

    addDaqComponentToDict(listOfAvailableDevices, (daqComponent*) signal);

    if (printout && str != NULL)
        printf("Hello from signal. Signal:\n%s\n", *str);
}

void searchComponentTree(daqString** componentDescription, daqDict* listOfComponents, const char* searchQuery)
{
    daqString* queryString = NULL;
    daqString_createString(&queryString, searchQuery);
    daqDict_get(listOfComponents, queryString, componentDescription);
    daqReleaseRef(queryString);
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
    printf("%llu\n", count);

    daqList* keys = NULL;
    daqDict_getKeyList(listOfComponents, &keys);
    daqIterator* it = NULL;
    daqList_createStartIterator(keys, &it);

    while(daqIterator_moveNext(it) == DAQ_SUCCESS)
    {
        daqBaseObject* current = NULL;
        daqIterator_getCurrent(it, &current);
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

    daqReleaseRef(it);
    daqReleaseRef(keys);
    daqReleaseRef(listOfComponents);
    daqReleaseRef(instance);
    daqReleaseRef(simulator);
    daqReleaseRef(simulatorInstance);
    return 0;
}
