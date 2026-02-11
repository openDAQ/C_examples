/*
 * Example that shows how to create conversion functions for custom defined in openDAQ.
 * Functions demonstrate conversions to and from openDAQ structure to its 
 * equivalent in native C.
 */
#include <daq_example_utils.h>


struct Coordinates;

enum ComponentStatusTypeEnum;

// Struct conversion
/*
 * Conversion from and to daqStruct objects to C style structs.
 * Warning: These types of conversions require prior knowledge of
 * the struct structure and its definition in C. For conversion
 * from C to openDAQ a pointer to the TypeManager is needed because
 * of the way openDAQ structs are implemented.
 */
struct Coordinates daq_fromDaqCoordinates(daqStruct* daq)
{
    struct Coordinates native = { 0,0,0 };

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

daqStruct* daq_toDaqCoordinates(struct Coordinates native, daqTypeManager* typeManager)
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
enum ComponentStatusTypeEnum daq_fromDaqCompStatusTypeEnum(daqEnumeration* daq)
{
    enum ComponentStatusTypeEnum native = Error;
    int64_t temp = -1;
    daqEnumeration_getIntValue(daq, &temp);

    // Sanity check
    if (temp < 2 && temp >= 0)
        native = temp;

    return native;
}

daqEnumeration* daq_toCompStatusTypeEnum(enum ComponentStatusTypeEnum native, daqTypeManager* typeManager)
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
    addCustomTypes(simulatorInstance);
    addCustomStructAndEnumProp(simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    daqComponentStatusContainer* statusContainer = NULL;
    daqComponent_getStatusContainer((daqComponent*)simulator, &statusContainer);
    daqString* tempStr = daqExample_toDaqString("ConnectionStatus");
    daqEnumeration* temp = NULL;
    daqComponentStatusContainer_getStatus(statusContainer, tempStr, &temp);
    daqReleaseRef(tempStr);

    enum ComponentStatusTypeEnum status = daq_fromDaqCompStatusTypeEnum(temp);
    printf("Status is: %d\n", status);
    daqReleaseRef(temp);
    
    daqProperty* currentPosition = NULL;
    daqStruct* tempStruct = NULL;
    struct Coordinates nativeCoordinates = {4, 4, 4};

    daqContext* context = NULL;
    daqComponent_getContext((daqComponent*)simulator, &context);
    daqTypeManager* typeMan = NULL;
    daqContext_getTypeManager(context, &typeMan);

    tempStr = daqExample_toDaqString("DAQ_CurrentPosition");
    daqPropertyObject_getProperty((daqPropertyObject*) simulator,tempStr, &currentPosition);
    daqReleaseRef(tempStr);
    daqBaseObject* tempObj = NULL;
    daqProperty_getValue(currentPosition, &tempObj);
    daqReleaseRef(currentPosition);
    if (tempObj != NULL)
    {
        daqQueryInterface(tempObj, DAQ_STRUCT_INTF_ID, &tempStruct);
        daqReleaseRef(tempObj);
        nativeCoordinates = daq_fromDaqCoordinates(tempStruct);

        printf("x: %lld,\ny: %lld,\nz: %lld\n", nativeCoordinates.x, nativeCoordinates.y, nativeCoordinates.z);

        daqStruct* tempStruct2 = daq_toDaqCoordinates(nativeCoordinates, typeMan);
        uint8_t check = False;
        daqBaseObject_equals(tempStruct, tempStruct2, &check);

        if (check)
            printf("OpenDAQ structs are the same after being translated to C and back.");
        else
            printf("Structs are different!");

        daqReleaseRef(typeMan);
        daqReleaseRef(tempStruct);
        daqReleaseRef(tempStruct2);
    }

    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);

    return 0;
}