/*
 * Disclamer: The following functions are used only for demonstration purposes 
 * and do not represent the corrent way of using openDAQ API.
 */
#include <daq_c_conversions.h>

 // Custom struct type
struct exdaq_Coordinates
{
    int64_t x;
    int64_t y;
    int64_t z;
};

// Standard enum
enum exdaq_ComponentStatusTypeEnum
{
    exdaq_ComponentStatusType_Ok = 0,
    exdaq_ComponentStatusType_Warning,
    exdaq_ComponentStatusType_Error
};

// Adds Coodinates struct and ComponentStatusTypeEnum to types in Type Manager
void exdaq_internal_addCoordinateStructToTypeManager(daqContext* context)
{
    daqTypeManager* typeManager = NULL;
    daqContext_getTypeManager(context, &typeManager);

    daqList* names = NULL;
    daqList_createList(&names);

    daqString* name = exdaq_toDaqString("x");
    daqList_pushBack(names, (daqBaseObject*) name);
    daqReleaseRef(name);

    name = exdaq_toDaqString("y");
    daqList_pushBack(names, (daqBaseObject*) name);
    daqReleaseRef(name);

    name = exdaq_toDaqString("z");
    daqList_pushBack(names, (daqBaseObject*) name);
    daqReleaseRef(name);

    daqSimpleType* simpleType = NULL;
    daqSimpleType_createSimpleType(&simpleType, daqCtInt);

    daqList* types = NULL;
    daqList_createList(&types);
    daqList_pushBack(types, simpleType);
    daqList_pushBack(types, simpleType);
    daqList_pushBack(types, simpleType);
    daqReleaseRef(simpleType);

    daqStructType* newType = NULL;
    daqString* typeName = exdaq_toDaqString("DAQ_Coordinates");
    daqStructType_createStructTypeNoDefaults(&newType, typeName, names, types);
    daqReleaseRef(typeName);

    daqTypeManager_addType(typeManager, (daqType*) newType);
    daqReleaseRef(newType);
    daqReleaseRef(names);
    daqReleaseRef(types);
    daqReleaseRef(typeManager);
}

void exdaq_internal_addCustomStructAndEnumProp(daqDevice* device)
{
    daqTypeManager* typeManager = NULL;
    daqContext* context = NULL;
    daqComponent_getContext((daqComponent*)device, &context);
    daqContext_getTypeManager(context, &typeManager);

    daqStructBuilder* builder = NULL;
    daqString* name = exdaq_toDaqString("DAQ_Coordinates");
    daqStructBuilder_createStructBuilder(&builder, name, typeManager);
    daqReleaseRef(name);

    daqInteger* value = NULL;
    daqString* fieldName = NULL;

    fieldName = exdaq_toDaqString("x");
    value = exdaq_toDaqInteger(2);
    daqStructBuilder_set(builder, fieldName, (daqBaseObject*) value);
    daqReleaseRef(fieldName);
    daqReleaseRef(value);

    fieldName = exdaq_toDaqString("y");
    value = exdaq_toDaqInteger(3);
    daqStructBuilder_set(builder, fieldName, (daqBaseObject*) value);
    daqReleaseRef(fieldName);
    daqReleaseRef(value);

    fieldName = exdaq_toDaqString("z");
    value = exdaq_toDaqInteger(4);
    daqStructBuilder_set(builder, fieldName, (daqBaseObject*) value);
    daqReleaseRef(fieldName);
    daqReleaseRef(value);

    daqStruct* coordinatesStruct = NULL;
    daqStructBuilder_build(builder, &coordinatesStruct);

    name = exdaq_toDaqString("DAQ_CurrentPosition");
    daqPropertyBuilder* structBuilder = NULL;
    daqPropertyBuilder_createStructPropertyBuilder(&structBuilder, name, coordinatesStruct);
    daqProperty* structProp = NULL;
    daqPropertyBuilder_build(structBuilder, &structProp);
    daqPropertyObject_addProperty((daqPropertyObject*)device, structProp);

    daqReleaseRef(structProp);
    daqReleaseRef(structBuilder);
    daqReleaseRef(builder);
}

void exdaq_internal_addCustomTypes(daqInstance* instance)
{
    daqContext* context = NULL;
    daqComponent_getContext((daqComponent*)instance, &context);
    exdaq_internal_addCoordinateStructToTypeManager(context);
}