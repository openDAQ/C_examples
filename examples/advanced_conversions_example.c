/*
 * Example that shows how to create conversion functions for custom defined in openDAQ.
 * Functions demonstrate conversions to and from openDAQ structure to its 
 * equivalent in native C.
 */
#include <daq_example_utils.h>


struct daqExample_Coordinates;

enum daqExample_ComponentStatusTypeEnum;

// Struct conversion
/*
 * Conversion from and to daqStruct objects to C style structs.
 * Warning: These types of conversions require prior knowledge of
 * the struct structure and its definition in C. For conversion
 * from C to openDAQ a pointer to the TypeManager is needed because
 * of the way openDAQ structs are implemented.
 */
struct daqExample_Coordinates daqExample_fromDaqCoordinates(daqStruct* daq)
{
    struct daqExample_Coordinates native = { 0,0,0 };

    daqBaseObject* tempObj = NULL;
    daqBaseObject* tempMid = NULL;

    daqString* temp = daqExample_toDaqString("x");
    daqStruct_get(daq, temp, &tempObj);
    daqQueryInterface(tempObj, DAQ_INTEGER_INTF_ID, &tempMid);
    native.x = daqExample_fromDaqInteger((daqInteger*) tempMid);
    daqReleaseRef(tempMid);
    daqReleaseRef(tempObj);
    daqReleaseRef(temp);

    temp = daqExample_toDaqString("y");
    daqStruct_get(daq, temp, &tempObj);
    daqQueryInterface(tempObj, DAQ_INTEGER_INTF_ID, &tempMid);
    native.y = daqExample_fromDaqInteger((daqInteger*) tempMid);
    daqReleaseRef(tempMid);
    daqReleaseRef(tempObj);
    daqReleaseRef(temp);

    temp = daqExample_toDaqString("z");
    daqStruct_get(daq, temp, &tempObj);
    daqQueryInterface(tempObj, DAQ_INTEGER_INTF_ID, &tempMid);
    native.z = daqExample_fromDaqInteger((daqInteger*) tempMid);
    daqReleaseRef(tempMid);
    daqReleaseRef(tempObj);
    daqReleaseRef(temp);

    return native;
}

daqStruct* daqExample_toDaqCoordinates(struct daqExample_Coordinates native, daqTypeManager* typeManager)
{
    daqStructBuilder* builder = NULL;
    daqStructBuilder_createStructBuilder(&builder, daqExample_toDaqString("DAQ_Coordinates"), typeManager);

    daqString* temp = NULL;
    daqInteger* tempInt = NULL;

    temp = daqExample_toDaqString("x");
    tempInt = daqExample_toDaqInteger(native.x);
    daqStructBuilder_set(builder, temp, (daqBaseObject*) tempInt);
    daqReleaseRef(temp);
    daqReleaseRef(tempInt);

    temp = daqExample_toDaqString("y");
    tempInt = daqExample_toDaqInteger(native.y);
    daqStructBuilder_set(builder, temp, (daqBaseObject*) tempInt);
    daqReleaseRef(temp);
    daqReleaseRef(tempInt);

    temp = daqExample_toDaqString("z");
    tempInt = daqExample_toDaqInteger(native.z);
    daqStructBuilder_set(builder, temp, (daqBaseObject*) tempInt);
    daqReleaseRef(temp);
    daqReleaseRef(tempInt);

    daqStruct* daq = NULL;
    daqStructBuilder_build(builder, &daq);
    daqReleaseRef(builder);

    return daq;
}

// Enumeration conversion
/*
 * Conversion from and to openDAQ String (daqEnumeration) core type
 * from C language (enum ComponentStutesTypeEnum).
 * Warning: These types of conversions require prior knowledge of
 * the enumeration structure and its definition in C. For conversion
 * from C to openDAQ a pointer to the TypeManager is needed because
 * of the way openDAQ enumeration is implemented.
 */
enum daqExample_ComponentStatusTypeEnum daqExample_fromDaqCompStatusTypeEnum(daqEnumeration* daq)
{
    enum daqExample_ComponentStatusTypeEnum native = daqExample_ComponentStatusType_Error;
    int64_t temp = -1;
    daqEnumeration_getIntValue(daq, &temp);

    native = temp;

    return native;
}

daqEnumeration* daqExample_toCompStatusTypeEnum(enum daqExample_ComponentStatusTypeEnum native, daqTypeManager* typeManager)
{
    daqEnumeration* daq = NULL;
    daqInteger* tempInt = daqExample_toDaqInteger(native);
    daqString* temp = daqExample_toDaqString("ComponentStatusType");

    daqEnumeration_createEnumerationWithIntValue(&daq, temp, tempInt, typeManager);

    daqReleaseRef(temp);
    daqReleaseRef(tempInt);
    return daq;
}

int main()
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);
    daqExmaple_addCustomTypes(simulatorInstance);
    daqExample_addCustomStructAndEnumProp((daqDevice*)simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);
    
    daqContext* context = NULL;
    daqComponent_getContext((daqComponent*)simulator, &context);
    daqTypeManager* typeMan = NULL;
    daqContext_getTypeManager(context, &typeMan);

    enum daqExample_ComponentStatusTypeEnum enumStatusType = daqExample_ComponentStatusType_Error;
    daqEnumeration* daqEnum = daqExample_toCompStatusTypeEnum(enumStatusType, typeMan);
    enum daqExample_ComponentStatusTypeEnum enumStatusType2 = daqExample_fromDaqCompStatusTypeEnum(daqEnum);
    
    uint8_t check = enumStatusType == enumStatusType2;

    if (check)
        printf("Structs are the same after transforming them to and from openDAQ.\n");
    else
        printf("Structs are different!\n");

    struct daqExample_Coordinates nativeCoordinates = {4, 4, 4};
    daqStruct* tempStruct = daqExample_toDaqCoordinates(nativeCoordinates, typeMan);
    struct daqExample_Coordinates nativeCoordinates2 = daqExample_fromDaqCoordinates(tempStruct);
    
    check = nativeCoordinates.x == nativeCoordinates2.x && nativeCoordinates.y == nativeCoordinates2.y && nativeCoordinates.z == nativeCoordinates2.z;

    if (check)
        printf("Enums are the same after transforming them to and from openDAQ.\n");
    else
        printf("Enums are different!\n");

    daqReleaseRef(typeMan);
    daqReleaseRef(tempStruct);

    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);

    return 0;
}