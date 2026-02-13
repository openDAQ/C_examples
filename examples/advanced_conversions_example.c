/*
 * Example that shows how to create conversion functions for custom defined in openDAQ.
 * Functions demonstrate conversions to and from openDAQ structure to its 
 * equivalent in native C.
 */
#include <daq_example_utils.h>

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

    daqBaseObject* value = NULL;
    daqBaseObject* integerValue = NULL;

    daqString* fieldName = exdaq_toDaqString("x");
    daqStruct_get(daq, fieldName, &value);
    daqQueryInterface(value, DAQ_INTEGER_INTF_ID, &integerValue);
    native.x = exdaq_fromDaqInteger((daqInteger*) integerValue);
    daqReleaseRef(integerValue);
    daqReleaseRef(value);
    daqReleaseRef(fieldName);

    fieldName = exdaq_toDaqString("y");
    daqStruct_get(daq, fieldName, &value);
    daqQueryInterface(value, DAQ_INTEGER_INTF_ID, &integerValue);
    native.y = exdaq_fromDaqInteger((daqInteger*) integerValue);
    daqReleaseRef(integerValue);
    daqReleaseRef(value);
    daqReleaseRef(fieldName);

    fieldName = exdaq_toDaqString("z");
    daqStruct_get(daq, fieldName, &value);
    daqQueryInterface(value, DAQ_INTEGER_INTF_ID, &integerValue);
    native.z = exdaq_fromDaqInteger((daqInteger*) integerValue);
    daqReleaseRef(integerValue);
    daqReleaseRef(value);
    daqReleaseRef(fieldName);

    return native;
}

daqStruct* daqExample_toDaqCoordinates(struct daqExample_Coordinates native, daqTypeManager* typeManager)
{
    daqStructBuilder* builder = NULL;
    daqStructBuilder_createStructBuilder(&builder, exdaq_toDaqString("DAQ_Coordinates"), typeManager);

    daqString* fieldName = NULL;
    daqInteger* fieldValue = NULL;

    fieldName = exdaq_toDaqString("x");
    fieldValue = exdaq_toDaqInteger(native.x);
    daqStructBuilder_set(builder, fieldName, (daqBaseObject*) fieldValue);
    daqReleaseRef(fieldName);
    daqReleaseRef(fieldValue);

    fieldName = exdaq_toDaqString("y");
    fieldValue = exdaq_toDaqInteger(native.y);
    daqStructBuilder_set(builder, fieldName, (daqBaseObject*) fieldValue);
    daqReleaseRef(fieldName);
    daqReleaseRef(fieldValue);

    fieldName = exdaq_toDaqString("z");
    fieldValue = exdaq_toDaqInteger(native.z);
    daqStructBuilder_set(builder, fieldName, (daqBaseObject*) fieldValue);
    daqReleaseRef(fieldName);
    daqReleaseRef(fieldValue);

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
    int64_t intValue = -1;
    daqEnumeration_getIntValue(daq, &intValue);

    return intValue;
}

daqEnumeration* daqExample_toCompStatusTypeEnum(enum daqExample_ComponentStatusTypeEnum native, daqTypeManager* typeManager)
{
    daqEnumeration* daq = NULL;
    daqInteger* valueInt = exdaq_toDaqInteger(native);
    daqString* valueName = exdaq_toDaqString("ComponentStatusType");

    daqEnumeration_createEnumerationWithIntValue(&daq, valueName, valueInt, typeManager);

    daqReleaseRef(valueName);
    daqReleaseRef(valueInt);
    return daq;
}

int main()
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);
    daqExmaple_addCustomTypes(simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);
    
    daqContext* context = NULL;
    daqComponent_getContext((daqComponent*)simulator, &context);
    daqTypeManager* typeManager = NULL;
    daqContext_getTypeManager(context, &typeManager);

    enum daqExample_ComponentStatusTypeEnum enumStatusType = daqExample_ComponentStatusType_Error;
    daqEnumeration* daqEnum = daqExample_toCompStatusTypeEnum(enumStatusType, typeManager);
    enum daqExample_ComponentStatusTypeEnum enumStatusType2 = daqExample_fromDaqCompStatusTypeEnum(daqEnum);

    if (enumStatusType == enumStatusType2)
        printf("Structs are the same after transforming them to and from openDAQ.\n");
    else
        printf("Structs are different!\n");

    struct daqExample_Coordinates nativeCoordinates = {4, 4, 4};
    daqStruct* tempStruct = daqExample_toDaqCoordinates(nativeCoordinates, typeManager);
    struct daqExample_Coordinates nativeCoordinates2 = daqExample_fromDaqCoordinates(tempStruct);

    if (nativeCoordinates.x == nativeCoordinates2.x &&
        nativeCoordinates.y == nativeCoordinates2.y && 
        nativeCoordinates.z == nativeCoordinates2.z)
        printf("Enums are the same after transforming them to and from openDAQ.\n");
    else
        printf("Enums are different!\n");

    daqReleaseRef(typeManager);
    daqReleaseRef(tempStruct);

    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);

    return 0;
}