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

void addDaqComponentToDict(daqDict* listOfAvailableComponents, daqComponent* component);

void printComponentTree(daqDevice* headDevice, daqDict** listOfAvailableComponents, daqBool printout);

void printDaqDevice(daqBaseObject* baseObject, daqDict* listOfAvailableComponents, daqBool printout);

void printDaqFunctionBlock(daqBaseObject* baseObject, daqDict* listOfAvailableComponents, daqBool printout);

void printDaqFolder(daqBaseObject* baseObject, daqDict* listOfAvailableComponents, daqBool printout);

void printDaqServer(daqBaseObject* baseObject, daqDict* listOfAvailableComponents, daqBool printout);

void printDaqSyncComponent(daqBaseObject* baseObject, daqDict* listOfAvailableComponents, daqBool printout);

void printInputPort(daqBaseObject* baseObject, daqDict* listOfAvailableComponents, daqBool printout);

void printDaqSignal(daqBaseObject* baseObject, daqDict* listOfAvailableComponents, daqBool printout);

void addDaqComponentToDict(daqDict* listOfAvailableComponents, daqComponent* component)
{
    daqString* globalId = NULL;
    daqString* description = NULL;
    daqComponent_getGlobalId(component, &globalId);
    daqComponent_getDescription(component, &description);

    daqDict_set(listOfAvailableComponents, globalId, description);

    daqReleaseRef(globalId);
    daqReleaseRef(description);
}

void printComponentTree(daqDevice* headDevice, daqDict** listOfAvailableComponents, daqBool printout)
{
    daqDict_createDict(listOfAvailableComponents);
    printDaqDevice(headDevice, *listOfAvailableComponents, printout);
}

enum ComponentType getComponentType(daqBaseObject* baseObject)
{
    enum ComponentType componentType = DaqUnknown;

    if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_DEVICE_INTF_ID) && (componentType == DaqUnknown))
        componentType = DaqDevice;

    if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_SERVER_INTF_ID) && (componentType == DaqUnknown))
        componentType = DaqServer;

    if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_SYNC_COMPONENT_INTF_ID) && (componentType == DaqUnknown))
        componentType = DaqSyncComponent;

    if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_FUNCTION_BLOCK_INTF_ID) && (componentType == DaqUnknown))
        componentType = DaqFunctionBlock;

    if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_FOLDER_INTF_ID) && (componentType == DaqUnknown))
        componentType = DaqFolder;

    if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_INPUT_PORT_INTF_ID) && (componentType == DaqUnknown))
        componentType = DaqInputPort;

    if (DAQ_SUPPORTS_INTERFACE(baseObject, DAQ_SIGNAL_INTF_ID) && (componentType == DaqUnknown))
        componentType = DaqSignal;

    return componentType;
}

void printObjectList(daqList* list, daqDict* listOfAvailableComponents, daqBool printout)
{
    daqBaseObject* listMember = NULL;
    daqSizeT count = 0;
    daqList_getCount(list, &count);
    if (count <= 0)
        return;
    daqList_getItemAt(list, 0, &listMember);
    
    enum ComponentType componentType = getComponentType(listMember);

    daqSizeT numberOfObjects = 0;
    daqList_getCount(list, &numberOfObjects);

    for (daqSizeT i = 0; i < numberOfObjects; i++)
    {
        daqList_getItemAt(list, i, &listMember);

        switch (componentType)
        {
        case DaqDevice:
        {
            printDaqDevice(listMember, listOfAvailableComponents, printout);
            daqReleaseRef(listMember);
            break;
        }
        case DaqServer:
        {
            printDaqServer(listMember, listOfAvailableComponents, printout);
            daqReleaseRef(listMember);
            break;
        }
        case DaqSyncComponent:
        {
            printDaqSyncComponent(listMember, listOfAvailableComponents, printout);
            daqReleaseRef(listMember);
            break;
        }
        case DaqFunctionBlock:
        {
            printDaqFunctionBlock(listMember, listOfAvailableComponents, printout);
            daqReleaseRef(listMember);
            break;
        }
        case DaqFolder:
        {
            printDaqFolder(listMember, listOfAvailableComponents, printout);
            daqReleaseRef(listMember);
            break;
        }
        case DaqInputPort:
        {
            printInputPort(listMember, listOfAvailableComponents, printout);
            daqReleaseRef(listMember);
            break;
        }
        case DaqSignal:
        {
            printDaqSignal(listMember, listOfAvailableComponents, printout);
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

void printDaqDevice(daqBaseObject* baseObject, daqDict* listOfAvailableComponents, daqBool printout)
{
    daqDevice* device = NULL;
    daqQueryInterface(baseObject, DAQ_DEVICE_INTF_ID, &device);
    daqString* name = NULL;
    daqComponent_getName((daqComponent*)device, &name);

    if (printout && name != NULL)
        printDaqFormattedString("Device: %s\n", name);

    daqReleaseRef(name);

    addDaqComponentToDict(listOfAvailableComponents, (daqComponent*) device);

    daqFolder* ioFolder = NULL;
    daqDevice_getInputsOutputsFolder(device, &ioFolder);
    if (ioFolder != NULL)
    {
        printDaqFolder(ioFolder, listOfAvailableComponents, printout);
        daqReleaseRef(ioFolder);
    }

    daqSyncComponent* syncComponent = NULL;
    daqDevice_getSyncComponent(device, &syncComponent);
    if (syncComponent != NULL)
    {
        printDaqSyncComponent(syncComponent, listOfAvailableComponents, printout);
        daqReleaseRef(syncComponent);
    }

    daqList* devices = NULL;
    daqDevice_getDevices(device, &devices, NULL);
    if (devices != NULL)
    {
        printObjectList(devices, listOfAvailableComponents, printout);
        daqReleaseRef(devices);
    }

    daqList* functionBlocks = NULL;
    daqDevice_getFunctionBlocks(device, &functionBlocks, NULL);
    if (functionBlocks != NULL)
    {
        printObjectList(functionBlocks, listOfAvailableComponents, printout);
        daqReleaseRef(functionBlocks);
    }

    daqList* servers = NULL;
    daqDevice_getServers(device, &servers);
    if (servers != NULL)
    {
        printObjectList(servers, listOfAvailableComponents, printout);
        daqReleaseRef(servers);
    }

    daqReleaseRef(device);
}

void printDaqFunctionBlock(daqBaseObject* baseObject, daqDict* listOfAvailableComponents, daqBool printout)
{
    daqFunctionBlock* functionBlock = NULL;
    daqQueryInterface(baseObject, DAQ_FUNCTION_BLOCK_INTF_ID, &functionBlock);
    daqString* name = NULL;
    daqComponent_getName((daqComponent*)functionBlock, &name);

    if (printout && name != NULL)
        printDaqFormattedString("Function block: %s\n", name);

    daqReleaseRef(name);

    addDaqComponentToDict(listOfAvailableComponents, (daqComponent*) functionBlock);

    daqList* functionBlocks = NULL;
    daqFunctionBlock_getFunctionBlocks(functionBlock, &functionBlocks, NULL);
    if (functionBlocks != NULL)
    {
        printObjectList(functionBlocks, listOfAvailableComponents, printout);
        daqReleaseRef(functionBlocks);
    }

    daqList* inputPorts = NULL;
    daqFunctionBlock_getInputPorts(functionBlock, &inputPorts, NULL);
    if (inputPorts != NULL)
    {
        printObjectList(inputPorts, listOfAvailableComponents, printout);
        daqReleaseRef(inputPorts);
    }

    daqList* listOfSignals = NULL;
    daqFunctionBlock_getSignals(functionBlock, &listOfSignals, NULL);
    if (listOfSignals != NULL)
    {
        printObjectList(listOfSignals, listOfAvailableComponents, printout);
        daqReleaseRef(listOfSignals);
    }

    daqReleaseRef(functionBlock);
}

void printDaqFolder(daqBaseObject* baseObject, daqDict* listOfAllDevices, daqBool printout)
{
    daqFolder* folder = NULL;
    daqQueryInterface(baseObject, DAQ_FOLDER_INTF_ID, &folder);
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

    daqReleaseRef(folder);
}

void printDaqServer(daqBaseObject* baseObject, daqDict* listOfAvailableComponents, daqBool printout)
{
    daqServer* server = NULL;
    daqQueryInterface(baseObject, DAQ_SERVER_INTF_ID, &server);

    daqString* name = NULL;
    daqComponent_getName((daqComponent*)server, &name);

    if(printout && name != NULL)
        printDaqFormattedString("Server: %s\n", name);

    daqReleaseRef(name);

    addDaqComponentToDict(listOfAvailableComponents, (daqComponent*) server);

    daqList* listOfSignals = NULL;
    daqServer_getSignals(server, &listOfSignals, NULL);
    if (listOfSignals != NULL)
    {
        printObjectList(listOfSignals, listOfAvailableComponents, printout);
        daqReleaseRef(listOfSignals);
    }

    daqReleaseRef(server);
}

void printDaqSyncComponent(daqBaseObject* baseObject, daqDict* listOfAvailableComponents, daqBool printout)
{
    daqSyncComponent* syncComp = NULL;
    daqQueryInterface(baseObject, DAQ_SYNC_COMPONENT_INTF_ID, &syncComp);
    daqString* name = NULL;
    daqComponent_getName((daqComponent*)syncComp, &name);

    if(printout && name != NULL)
        printDaqFormattedString("Sync component: %s\n", name);

    daqReleaseRef(name);

    addDaqComponentToDict(listOfAvailableComponents, (daqComponent*) syncComp);

    daqReleaseRef(syncComp);
}

void printInputPort(daqBaseObject* baseObject, daqDict* listOfAvailableComponents, daqBool printout)
{
    daqInputPort* inputPort = NULL;
    daqQueryInterface(baseObject, DAQ_INPUT_PORT_INTF_ID, &inputPort);
    daqString* name = NULL;
    daqComponent_getName((daqComponent*)inputPort, &name);

    if (printout && name != NULL)
        printDaqFormattedString("Input port: %s\n", name);

    daqReleaseRef(name);

    addDaqComponentToDict(listOfAvailableComponents, (daqComponent*) inputPort);

    daqReleaseRef(inputPort);
}

void printDaqSignal(daqBaseObject* baseObject, daqDict* listOfAvailableComponents, daqBool printout)
{
    daqSignal* signal = NULL;
    daqQueryInterface(baseObject, DAQ_SIGNAL_INTF_ID, &signal);
    daqString* name = NULL;
    daqComponent_getName((daqComponent*)signal, &name);

    if (printout && name != NULL)
        printDaqFormattedString("Signal: %s\n", name);

    daqReleaseRef(name);

    addDaqComponentToDict(listOfAvailableComponents, (daqComponent*) signal);

    daqReleaseRef(signal);
}

int main()
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    daqDict* listOfComponents = NULL;

    printComponentTree((daqDevice*)instance, &listOfComponents, True);

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
