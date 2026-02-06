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
    Ok = 0,
    Warning,
    Error
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

void addCustomStructAndEnumProp(daqDevice* device)
{
    daqTypeManager* typeMan = NULL;
    daqContext* context = NULL;
    daqComponent_getContext((daqComponent*)device, &context);
    daqContext_getTypeManager(context, &typeMan);
    daqStructBuilder* builder = NULL;
    daqString* nameTemp = daq_toDaqString("DAQ_Coordinates");
    daqStructBuilder_createStructBuilder(&builder, nameTemp, typeMan);
    daqReleaseRef(nameTemp);
    daqInteger* tempInt = NULL;
    daqString* temp = NULL;

    temp = daq_toDaqString("x");
    tempInt = daq_toDaqInteger(2);
    daqStructBuilder_set(builder, temp, (daqBaseObject*) tempInt);
    daqReleaseRef(temp);
    daqReleaseRef(tempInt);

    temp = daq_toDaqString("y");
    tempInt = daq_toDaqInteger(3);
    daqStructBuilder_set(builder, temp, (daqBaseObject*) tempInt);
    daqReleaseRef(temp);
    daqReleaseRef(tempInt);

    temp = daq_toDaqString("z");
    tempInt = daq_toDaqInteger(4);
    daqStructBuilder_set(builder, temp, (daqBaseObject*) tempInt);
    daqReleaseRef(temp);
    daqReleaseRef(tempInt);

    daqStruct* coordinatesStruct = NULL;
    daqStructBuilder_build(builder, &coordinatesStruct);

    nameTemp = daq_toDaqString("DAQ_CurrentPosition");
    daqPropertyBuilder* structBuilder = NULL;
    daqPropertyBuilder_createStructPropertyBuilder(&structBuilder, nameTemp,coordinatesStruct);
    daqProperty* structProp = NULL;
    daqPropertyBuilder_build(structBuilder, &structProp);
    daqPropertyObject_addProperty((daqPropertyObject*)device, structProp);
}

void addCustomTypes(daqInstance* instance)
{
    daqContext* context = NULL;
    daqComponent_getContext(instance, &context);
    addCoordinateStructToTypeManager(context);
}