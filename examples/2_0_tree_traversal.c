
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
     *  tisto kar je, vec ni potrebno
     */


    // Device -> Folder -> FB/Channel -> Signal
    //   \-> SyncComponent

    /*daqFolder* ioFolder = NULL;
    daqDevice_getInputsOutputsFolder(headDevice, &ioFolder);

    daqServer* server;

    daqStreaming* streaming = NULL;
    daqServer_getStreaming(server, &streaming);
    */

void componentTreePrintOut(daqDevice* headDevice, daqDict** listOfAvailableDevices, daqBool printout)
{
    daqDict_createDict(listOfAvailableDevices);
    printDaqDevice(headDevice, listOfAvailableDevices, printout);
}

void printObjectList(daqList* list, daqDict* listOfAvailableDevices, daqBool printout)
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
    daqBaseObject_toString(deviceInfo, str);

    addDaqComponentToDict(listOfAvailableDevices, (daqComponent*) device);

    if(printout)
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
    daqBaseObject_toString(functionBlock, str);
    
    addDaqComponentToDict(listOfAvailableDevices, (daqComponent*) functionBlock);

    if (printout)
        printf("Hello from function blcok. Function Block: \n%s\n", *str);

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
    daqBaseObject_toString(folder, str);

    addDaqComponentToDict(listOfAllDevices, (daqComponent*) folder);

    if (printout)
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
    daqBaseObject_toString(server, str);

    addDaqComponentToDict(listOfAllAvailableDevices, (daqComponent*) server);

    if(printout)
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
    daqBaseObject_toString(syncComp, str);

    addDaqComponentToDict(listOfAvailableDevices, (daqComponent*) syncComp);

    if(printout)
        printf("Hello from sync component. Sync component:\n%s\n", *str);
}

void printInputPort(daqInputPort* inputPort, daqDict* listOfAvailableDevices, daqBool printout)
{
    daqCharPtr* str = NULL;
    daqBaseObject_toString(inputPort, str);

    addDaqComponentToDict(listOfAvailableDevices, (daqComponent*) inputPort);

    if (printout)
        printf("Hello from input port. InputPort:\n%s\n", *str);
}

void printDaqSignal(daqSignal* signal, daqDict* listOfAvailableDevices, daqBool printout)
{
    // Leaf node
    daqCharPtr* str = NULL;
    daqBaseObject_toString(signal, str);

    addDaqComponentToDict(listOfAvailableDevices, (daqComponent*) signal);

    if (printout)
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

    daqDict* listOfComponenets = NULL;
    daqBool printout = True;

    componentTreePrintOut(instance, listOfComponenets, printout);

    daqReleaseRef(listOfComponenets);
    daqReleaseRef(instance);
    daqReleaseRef(simulator);
    daqReleaseRef(simulatorInstance);
    return 0;
}


// TODOs:
//  - Add caching of key-value pairs <CopmonentGlobalId, ComponentInfo>  DONE
//  - Add display of tree                                                DONE(ISH)
//  - Add display info for specified component                           IN PROGRESS
