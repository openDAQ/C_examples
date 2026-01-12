
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

void printDaqSignal(daqSignal* signal);
void printInputPort(daqInputPort* inputPort);
void printDaqSyncComponent(daqSyncComponent* syncComp);
void printDaqServer(daqServer* server);
void printDaqFolder(daqFolder* folder);
void printDaqFunctionBlock(daqFunctionBlock* functionBlock);
void printDaqDevice(daqDevice* device);


// Note: The case when getters fail should be handled so no crashes happen (at least not from this). 

    /*
     * daqDevice_getDevices()
     * Device checklist:
     * daqDevice_getInputsOutputsFolder()
     * daqDevice_getFunctionBlocks()
     * daqDevice_getSyncComponent()
     * daqDevice_getServers()
     */

    /*
     * daqFolder_getItems
     * IOFolder checklist:
     * daqList zapelji se skozi celoten seznam in pošlji vsako posebej naprej
     * (Spremeni pogled na function block na pogled na kanal (Channel) in iz njega pridobi zaznamke (tags) in si jih shrani)
     * daqFunctionBlock_getFunctionBlocks()
     * daqFunctionBlock_getInputPorts()
     * daqFunctionBlock_getSignals()
     */

    /*
     * daqFunctionBlock_getFunctionBlocks()
     * Function Block checklist:
     * daqFunctionBlock_getInputPorts()
     * daqFunctionBlock_getSignals()
     */

    /*
     * Server folder checklist:
     * daqServer_getSignals()
     * daqServer_getStreaming()
     */

    /*
     * Input ports checklist:
     * daqInputPort_getSignals()
     */

    /*
     * Sync Component checklist:
     *  samo to kar je na njemu (torej samo pointer do objekta je dovolj)
     */

    /*
     * Signal checklist:
     *  tisto kar je, veè ni potrebno
     */

void componentTreePrintOut(daqDevice* headDevice)
{
    // Device -> Folder -> FB/Channel -> Signal
    //   \-> SyncComponent

    /*daqFolder* ioFolder = NULL;
    daqDevice_getInputsOutputsFolder(headDevice, &ioFolder);

    daqServer* server;

    daqStreaming* streaming = NULL;
    daqServer_getStreaming(server, &streaming);
    */
}

void printObjectList(daqList* list)
{
    // Check for emptyness of list should be done outside
    daqBaseObject* listMember = NULL;
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
            printDaqDevice(device);
            daqReleaseRef(device);
            daqReleaseRef(listMember);
            break;
        }
        case Server:
        {
            daqServer* server = NULL;
            daqQueryInterface(listMember, DAQ_SERVER_INTF_ID, &server);
            printDaqServer(server);
            daqReleaseRef(server);
            daqReleaseRef(listMember);
            break;
        }
        case SyncComponent:
        {
            daqSyncComponent* syncComponent = NULL;
            daqQueryInterface(listMember, DAQ_SYNC_COMPONENT_INTF_ID, &syncComponent);
            printDaqSyncComponent(syncComponent);
            daqReleaseRef(syncComponent);
            daqReleaseRef(listMember);
            break;
        }
        case FunctionBlock:
        {
            daqFunctionBlock* functionBlock = NULL;
            daqQueryInterface(listMember, DAQ_FUNCTION_BLOCK_INTF_ID, &functionBlock);
            printDaqFunctionBlock(functionBlock);
            daqReleaseRef(functionBlock);
            daqReleaseRef(listMember);
            break;
        }
        case Folder:
        {
            daqFolder* folder = NULL;
            daqQueryInterface(listMember, DAQ_FOLDER_INTF_ID, &folder);
            printDaqFolder(folder);
            daqReleaseRef(folder);
            daqReleaseRef(listMember);
            break;
        }
        case InputPort:
        {
            daqInputPort* inputPort = NULL;
            daqQueryInterface(listMember, DAQ_INPUT_PORT_INTF_ID, &inputPort);
            printInputPort(inputPort);
            daqReleaseRef(inputPort);
            daqReleaseRef(listMember);
            break;
        }
        case Signal:
        {
            daqSignal* signal = NULL;
            daqQueryInterface(listMember, DAQ_SIGNAL_INTF_ID, &signal);
            printDaqSignal(signal);
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

void printDaqDevice(daqDevice* device)
{
    // Missing printing of itself.
    // Direct single children and the device itself needs to be printed.
    daqDeviceInfo* deviceInfo = NULL;
    daqDevice_getInfo(device, &deviceInfo);
    daqCharPtr* str = NULL;
    daqBaseObject_toString(deviceInfo, str);
    printf("Hello from device. Device Info: \n%s\n", *str);

    daqFolder* ioFolder = NULL;
    daqDevice_getInputsOutputsFolder(device, &ioFolder);
    if (ioFolder != NULL)
    {
        printDaqFolder(ioFolder);
        daqReleaseRef(ioFolder);
    }

    daqSyncComponent* syncComponent = NULL;
    daqDevice_getSyncComponent(device, &syncComponent);
    if (syncComponent != NULL)
    {
        printDaqSyncComponent(syncComponent);
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

void printDaqFunctionBlock(daqFunctionBlock* functionBlock)
{
    // Missing self display
    daqCharPtr* str = NULL;
    daqBaseObject_toString(functionBlock, str);
    printf("Hello from function blcok. Function Block: \n%s\n", *str);

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

void printDaqFolder(daqFolder* folder)
{
    // Self descriptrion
    daqCharPtr* str = NULL;
    daqBaseObject_toString(folder, str);
    printf("Hello from folder. Folder:\n%s\n", *str);

    daqList* listOfItems = NULL;
    daqFolder_getItems(folder, &listOfItems, NULL);
    if (listOfItems != NULL)
    {
        printObjectList(listOfItems);
        daqReleaseRef(listOfItems);
    }
}

void printDaqServer(daqServer* server)
{
    // Self description
    daqCharPtr* str = NULL;
    daqBaseObject_toString(server, str);
    printf("Hello from server. Server:\n%s\n", *str);

    daqList* listOfSignals = NULL;
    daqServer_getSignals(server, &listOfSignals, NULL);
    if (listOfSignals != NULL)
    {
        printObjectList(listOfSignals);
        daqReleaseRef(listOfSignals);
    }
}

void printDaqSyncComponent(daqSyncComponent* syncComp)
{
    // Leaf node
    // Self description
    daqCharPtr* str = NULL;
    daqBaseObject_toString(syncComp, str);
    printf("Hello from sync component. Sync component:\n%s\n", *str);
}

void printInputPort(daqInputPort* inputPort)
{
    // Self description
    daqCharPtr* str = NULL;
    daqBaseObject_toString(inputPort, str);
    printf("Hello from input port. InputPort:\n%s\n", *str);
}

void printDaqSignal(daqSignal* signal)
{
    // Leaf node
    // Self Description
    daqCharPtr* str = NULL;
    daqBaseObject_toString(signal, str);
    printf("Hello from signal. Signal:\n%s\n", *str);
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

    printDaqDevice((daqDevice*) instance);

    daqReleaseRef(instance);
    daqReleaseRef(simulator);
    daqReleaseRef(simulatorInstance);
    return 0;
}