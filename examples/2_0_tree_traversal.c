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

    // Repeat the procedure for all connected devices (recursively)

    daqReleaseRef(instance);
    daqReleaseRef(simulator);
    daqReleaseRef(simulatorInstance);
    return 0;
}