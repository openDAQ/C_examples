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

// Note: The case when getters fial should be handled so no crashes happen (at least not from this)

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
        daqSizeT numberOfSubdevices = 0;
        daqList_getCount(devices, &numberOfSubdevices);

        for (daqSizeT i = 0; i < numberOfSubdevices; i++)
        {
            daqDevice* currentSubdevice = NULL;
            daqList_getItemAt(devices, i, &currentSubdevice);

            printDevice(currentSubdevice);
            daqReleaseRef(currentSubdevice);
        }
        daqReleaseRef(devices);
    }

    daqList* functionBlocks = NULL;
    daqDevice_getFunctionBlocks(device, &functionBlocks, NULL);
    if (functionBlocks != NULL) {
        daqSizeT numberOfFunctionBlocks = 0;
        daqList_getCount(functionBlocks, &numberOfFunctionBlocks);

        for (daqSizeT i = 0; i < numberOfFunctionBlocks; i++)
        {
            daqFunctionBlock* currentFunctionBlock = NULL;
            daqList_getItemAt(functionBlocks, i, &currentFunctionBlock);

            printFunctionBlock(currentFunctionBlock);
            daqReleaseRef(currentFunctionBlock);
        }
        daqReleaseRef(functionBlocks);
    }

    daqList* servers = NULL;
    daqDevice_getServers(device, &servers);
    if (servers != NULL)
    {
        daqSizeT numberOfServers = 0;
        daqList_getCount(servers, &numberOfServers);

        for (daqSizeT i = 0; i < numberOfServers; i++)
        {
            daqServer* currentServer = NULL;
            daqList_getItemAt(servers, i, &currentServer);

            printServer(currentServer);
            daqReleaseRef(currentServer);
        }
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
        daqSizeT numberOfFunctionBlocks = 0;
        daqList_getCount(functionBlocks, &numberOfFunctionBlocks);
        
        for (daqSizeT i = 0; i < numberOfFunctionBlocks; i++)
        {
            daqFunctionBlock* currentFunctionBlock = NULL;
            daqList_getItemAt(functionBlocks, i, &currentFunctionBlock);

            printFunctionBlock(currentFunctionBlock);
            daqReleaseRef(currentFunctionBlock);
        }
        daqReleaseRef(functionBlocks);
    }

    daqList* inputPorts = NULL;
    daqFunctionBlock_getInputPorts(functionBlock, &inputPorts, NULL);
    if (inputPorts != NULL)
    {
        daqSizeT numberOfInputPorts = 0;
        daqList_getCount(inputPorts, &numberOfInputPorts);

        for (daqSizeT i = 0; i < numberOfInputPorts; i++)
        {
            daqInputPort* currentInputPort = NULL;
            daqList_getItemAt(inputPorts, i, &currentInputPort);

            printInputPort(currentInputPort);
            daqReleaseRef(currentInputPort);
        }
        daqReleaseRef(inputPorts);
    }

    daqList* listOfSignals = NULL;
    daqFunctionBlock_getSignals(functionBlock, &listOfSignals, NULL);
    if (listOfSignals != NULL)
    {
        daqSizeT numberOfSignals = 0;
        daqList_getCount(listOfSignals, &numberOfSignals);

        for (daqSizeT i = 0; i < numberOfSignals; i++)
        {
            daqSignal* currentSignal = NULL;
            daqList_getItemAt(listOfSignals, i, &currentSignal);

            printSignal(currentSignal);
            daqReleaseRef(currentSignal);
        }
        daqReleaseRef(listOfSignals);
    }
}

void printFolder(daqFolder* folder)
{
    // Self descriptrion

    // Items
}

void printServer(daqServer* sserver)
{

}

void printSyncComponent(daqSyncComponent* syncComp)
{

}

void printInputPort(daqInputPort* inputPort)
{

}

void printSignal(daqSignal* signal)
{

}

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