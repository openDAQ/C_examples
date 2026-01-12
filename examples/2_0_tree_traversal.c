/*
 * In this example we will demonstrate the structure of openDAQ tree structure
 * by traversing across it.
 */

#include <daq_utils.h>

typedef enum ComponentType
{
    Unknown = 0,
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

// Note: The case when getters fail should be handled so no crashes happen (at least not from this). 

void componentTreePrintOut(daqDevice* headDevice)
{
    // Device -> Folder -> FB/Channel -> Signal
    //   \-> SyncComponent
    
    /*
     * daqDevice_getDevices()
     * Device checklist:
     * daqDevice_getInputsOutputsFolder()
     * daqDevice_getFunctionBlocks()
     * daqDevice_getSyncComponent()
     * daqDevice_getServers()
     */

    daqFolder* ioFolder = NULL;
    daqDevice_getInputsOutputsFolder(headDevice, &ioFolder);

    daqList* devices = NULL;
    daqDevice_getDevices(headDevice, &devices, NULL);

    daqList* functionBlocks = NULL;
    daqDevice_getFunctionBlocks(headDevice, &functionBlocks, NULL);

    daqSyncComponent* syncComponent = NULL;
    daqDevice_getSyncComponent(headDevice, &syncComponent);

    daqList* servers = NULL;
    daqDevice_getServers(headDevice, &servers);

    /*
     * daqFolder_getItems
     * IOFolder checklist:
     * daqList zapelji se skozi celoten seznam in pošlji vsako posebej naprej
     * (Spremeni pogled na function block na pogled na kanal (Channel) in iz njega pridobi zaznamke (tags) in si jih shrani)
     * daqFunctionBlock_getFunctionBlocks()
     * daqFunctionBlock_getInputPorts()
     * daqFunctionBlock_getSignals()
     */

    daqList* itemsIOFolder = NULL;
    daqFolder_getItems(ioFolder, &itemsIOFolder, NULL);

    daqList* functionBlocks = NULL;
    daqFunctionBlock_getFunctionBlocks((daqFunctionBlock*) ioFolder, &functionBlocks, NULL);

    daqList* inputPorts = NULL;
    daqFunctionBlock_getInputPorts((daqFunctionBlock*) ioFolder, &inputPorts, NULL);

    daqList* listOfSignals = NULL;
    daqFunctionBlock_getSignals((daqFunctionBlock*) ioFolder, &listOfSignals, NULL);
    
    /*
     * daqFunctionBlock_getFunctionBlocks()
     * Function Block checklist:
     * daqFunctionBlock_getInputPorts()
     * daqFunctionBlock_getSignals()
     */

    // Function block checks are the same as above

    /*
     * Input ports checklist:
     * daqInputPort_getSignals()
     */

    daqInputPort* inputPort = NULL;
    daqBaseObject* inputPortObj = NULL;
    daqList_getItemAt(inputPorts, 0, &inputPortObj);
    daqQueryInterface(inputPortObj, DAQ_INPUT_PORT_INTF_ID, &inputPort);

    daqSignal* signal = NULL;
    daqInputPort_getSignal(inputPort, &signal);

    /*
     * Server folder checklist:
     * daqServer_getSignals()
     * daqServer_getStreaming()
     */
    
    daqServer* server = NULL;
    daqBaseObject* serverObj = NULL;
    daqList_getItemAt(servers, 0, &serverObj);
    daqQueryInterface(serverObj, DAQ_SERVER_INTF_ID, &server);

    daqList* signals = NULL;
    daqServer_getSignals(server, &signals, NULL);

    daqStreaming* streaming = NULL;
    daqServer_getStreaming(server, &streaming);

    /*
     * Sync Component checklist:
     *  samo to kar je na njemu (torej samo pointer do objekta je dovolj)
     */

    /*
     * Signal checklist:
     *  tisto kar je, veè ni potrebno
     */
    
}

void printObjectList(daqList* list)
{
    // Check for emptyness of list should be done outside
    daqBaseObject* listMember = NULL;
    daqList_getItemAt(list, 0, &listMember);
    enum ComponentType componentType = Unknown;

    if (DAQ_SUPPORTS_INTERFACE(listMember, DAQ_DEVICE_INTF_ID) && (componentType == Unknown))
        componentType = Device;

    if (DAQ_SUPPORTS_INTERFACE(listMember, DAQ_SERVER_INTF_ID) && (componentType == Unknown))
        componentType = Server;

    if (DAQ_SUPPORTS_INTERFACE(listMember, DAQ_SYNC_COMPONENT_INTF_ID) && (componentType == Unknown))
        componentType = SyncComponent;

    if (DAQ_SUPPORTS_INTERFACE(listMember, DAQ_FUNCTION_BLOCK_INTF_ID) && (componentType == Unknown))
        componentType = FunctionBlock;

    if (DAQ_SUPPORTS_INTERFACE(listMember, DAQ_FOLDER_INTF_ID) && (componentType == Unknown))
        componentType = Folder;

    if (DAQ_SUPPORTS_INTERFACE(listMember, DAQ_INPUT_PORT_INTF_ID) && (componentType == Unknown))
        componentType = InputPort;

    if (DAQ_SUPPORTS_INTERFACE(listMember, DAQ_SIGNAL_INTF_ID) && (componentType == Unknown))
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
            printDevice(device);
            daqReleaseRef(device);
            daqReleaseRef(listMember);
            break;
        }
        case Server:
        {
            daqServer* server = NULL;
            daqQueryInterface(listMember, DAQ_SERVER_INTF_ID, &server);
            printServer(server);
            daqReleaseRef(server);
            daqReleaseRef(listMember);
            break;
        }
        case SyncComponent:
        {
            daqSyncComponent* syncComponent = NULL;
            daqQueryInterface(listMember, DAQ_SYNC_COMPONENT_INTF_ID, &syncComponent);
            printSyncComponent(syncComponent);
            daqReleaseRef(syncComponent);
            daqReleaseRef(listMember);
            break;
        }
        case FunctionBlock:
        {
            daqFunctionBlock* functionBlock = NULL;
            daqQueryInterface(listMember, DAQ_FUNCTION_BLOCK_INTF_ID, &functionBlock);
            printFunctionBlock(functionBlock);
            daqReleaseRef(functionBlock);
            daqReleaseRef(listMember);
            break;
        }
        case Folder:
        {
            daqFolder* folder = NULL;
            daqQueryInterface(listMember, DAQ_FOLDER_INTF_ID, &folder);
            printFolder(folder);
            daqReleaseRef(folder);
            daqReleaseRef(listMember);
            break;
        }
        case InputPort:
        {
            daqInputPort* inputPort = NULL;
            daqQueryInterface(listMember, DAQ_FOLDER_INTF_ID, &inputPort);
            printInputPort(inputPort);
            daqReleaseRef(inputPort);
            daqReleaseRef(listMember);
            break;
        }
        case Signal:
        {
            daqSignal* signal = NULL;
            daqQueryInterface(listMember, DAQ_SIGNAL_INTF_ID, &signal);
            printSignal(signal);
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

void printDevice(daqDevice* device)
{
    // Missing printing of itself.

    daqFolder* ioFolder = NULL;
    daqDevice_getInputsOutputsFolder(device, &ioFolder);
    if (ioFolder != NULL)
    {
        printFolder(ioFolder);
        daqReleaseRef(ioFolder);
    }

    daqSyncComponent* syncComponent = NULL;
    daqDevice_getSyncComponent(device, &syncComponent);
    if (syncComponent != NULL)
    {
        printSyncComponent(syncComponent);
        daqReleaseRef(syncComponent);
    }

    // The following Structures can appear in multiples, so we handle each and everyone of them.
    daqList* devices = NULL;
    daqDevice_getDevices(device, &devices, NULL);
    if (devices != NULL)
    {
        printObjectList(devices);
        daqReleaseRef(devices);
    }

    daqList* functionBlocks = NULL;
    daqDevice_getFunctionBlocks(device, &functionBlocks, NULL);
    if (functionBlocks != NULL)
    {
        printObjectList(functionBlocks);
        daqReleaseRef(functionBlocks);
    }

    daqList* servers = NULL;
    daqDevice_getServers(device, &servers);
    if (servers != NULL)
    {
        printObjectList(servers);
        daqReleaseRef(servers);
    }
}

void printFunctionBlock(daqFunctionBlock* functionBlock)
{
    // Missing self display

    daqList* functionBlocks = NULL;
    daqFunctionBlock_getFunctionBlocks(functionBlock, &functionBlocks, NULL);
    if (functionBlocks != NULL)
    {
        printObjectList(functionBlocks);
        daqReleaseRef(functionBlocks);
    }

    daqList* inputPorts = NULL;
    daqFunctionBlock_getInputPorts(functionBlock, &inputPorts, NULL);
    if (inputPorts != NULL)
    {
        printObjectList(inputPorts);
        daqReleaseRef(inputPorts);
    }

    daqList* listOfSignals = NULL;
    daqFunctionBlock_getSignals(functionBlock, &listOfSignals, NULL);
    if (listOfSignals != NULL)
    {
        printObjectList(listOfSignals);
        daqReleaseRef(listOfSignals);
    }
}

void printFolder(daqFolder* folder)
{
    // Self descriptrion

    daqList* listOfItems = NULL;
    daqFolder_getItems(folder, &listOfItems, NULL);
    if (listOfItems != NULL)
    {
        printObjectList(listOfItems);
        daqReleaseRef(listOfItems);
    }
}

void printServer(daqServer* server)
{
    // Self description

    daqList* listOfSignals = NULL;
    daqServer_getSignals(server, &listOfSignals, NULL);
    if (listOfSignals != NULL)
    {
        printObjectList(listOfSignals);
        daqReleaseRef(listOfSignals);
    }
}

void printSyncComponent(daqSyncComponent* syncComp)
{
    // Leaf node
    // Self description
}

void printInputPort(daqInputPort* inputPort)
{
    // Self description
}

void printSignal(daqSignal* signal)
{
    // Leaf node
    // Self Description
}

// There is an argument for including the a specific print function for ioFolder (as it is itself a specific type of a folder)
// The IOFolder would use get signals as a substitute for a recursive search with getItems, but presenting a flat structure in return.
// It can be used to display an alterantive way of parsing and recieving items in a folder
// (due to limitations imposed upon IOFolder, we could afford it in this givin function).

int main()
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    daqList* listOfComponents = NULL;
    daqList_createList(&listOfComponents);
    // Go through all the folders that are available on the device
    daqFolder* simulatorIOFolder = NULL;
    daqDevice_getInputsOutputsFolder(simulator, &simulatorIOFolder);

    daqList* simulatorIOItems = NULL;
    daqSearchFilter* filterIOAny = NULL;
    daqSearchFilter_createAnySearchFilter(&filterIOAny);
    daqFolder_getItems(simulatorIOFolder, &simulatorIOItems, filterIOAny);

    // Going through the components in IOFolder
    daqIterator* ioItemsIterator = NULL;
    daqList_createStartIterator(simulatorIOItems, &ioItemsIterator);

    while (daqIterator_moveNext(ioItemsIterator) == DAQ_SUCCESS)
    {
        daqDataDescriptor* ioItem = NULL;
        daqIterator_getCurrent(ioItemsIterator, &ioItem);

        daqDict* itemMetadata = NULL;
        daqDataDescriptor_getMetadata(ioItem, &itemMetadata);

        daqReleaseRef(itemMetadata);
        daqReleaseRef(ioItem);
    }

    daqReleaseRef(ioItemsIterator);
    daqReleaseRef(filterIOAny);
    daqReleaseRef(simulatorIOItems);
    daqReleaseRef(simulatorIOFolder);
    // Search for all the signals

    // Check for additional devices

    // Repeat the procedure for all connected devices

    daqReleaseRef(instance);
    daqReleaseRef(simulator);
    daqReleaseRef(simulatorInstance);
    return 0;
}