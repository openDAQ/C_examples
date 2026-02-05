#include <daq_c_conversions.h>

 // Custom struct type
struct Coordinates
{
    int64_t x;
    int64_t y;
    int64_t z;
};

// Standard enum
enum ComponentStatusTypeEnum
{
    Error = 0,
    Ok,
    Warning
};

// Adds Coodinates struct and ComponentStatusTypeEnum to types in Type Manager
void addCoordinateStructToTypeManager(daqContext* context)
{
    daqTypeManager* typeManager = NULL;
    daqContext_getTypeManager(context, &typeManager);

    daqList* names = NULL;
    daqList_createList(&names);

    daqString* temp = daq_toDaqString("x");

    daqList_pushBack(names, (daqBaseObject*)temp);
    daqReleaseRef(temp);
    temp = daq_toDaqString("y");
    daqList_pushBack(names, (daqBaseObject*) temp);
    daqReleaseRef(temp);
    temp = daq_toDaqString("z");
    daqList_pushBack(names, (daqBaseObject*)temp);
    daqReleaseRef(temp);

    daqSimpleType* simpleType = NULL;
    daqSimpleType_createSimpleType(&simpleType, daqCtInt);

    daqList* types = NULL;
    daqList_createList(&types);
    daqList_pushBack(types, simpleType);
    daqList_pushBack(types, simpleType);
    daqList_pushBack(types, simpleType);
    daqReleaseRef(simpleType);

    daqStructType* newType = NULL;
    daqString* typeName = daq_toDaqString("DAQ_Coordinates");
    daqStructType_createStructTypeNoDefaults(&newType, typeName, names, types);
    daqReleaseRef(typeName);

    daqTypeManager_addType(typeManager, (daqType*) newType);
    daqReleaseRef(newType);
    daqReleaseRef(names);
    daqReleaseRef(types);
    daqReleaseRef(typeManager);
}

void advancedSimulatorAddition(daqDevice** device, daqInstance** instance)
{
    addSimulator(device, instance);

    daqContext* context = NULL;
    daqComponent_getContext((daqComponent*) *instance, &context);
    addCoordinateStructToTypeManager(context);
}