/*
 *
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

    daqString* temp = daq_toDaqString("x");
    daqStruct_get(daq, temp, tempObj);
    tempMid = daqQueryInterfacePtr(tempObj, DAQ_INTEGER_INTF_ID);
    native.x = daq_fromDaqInteger((daqInteger*) tempMid);
    daqReleaseRef(tempMid);
    daqReleaseRef(tempObj);
    daqReleaseRef(temp);

    temp = daq_toDaqString("y");
    daqStruct_get(daq, temp, tempObj);
    tempMid = daqQueryInterfacePtr(tempObj, DAQ_INTEGER_INTF_ID);
    native.y = daq_fromDaqInteger((daqInteger*) tempMid);
    daqReleaseRef(tempMid);
    daqReleaseRef(tempObj);
    daqReleaseRef(temp);

    temp = daq_toDaqString("z");
    daqStruct_get(daq, temp, tempObj);
    tempMid = daqQueryInterfacePtr(tempObj, DAQ_INTEGER_INTF_ID);
    native.z = daq_fromDaqInteger((daqInteger*) tempMid);
    daqReleaseRef(tempMid);
    daqReleaseRef(tempObj);
    daqReleaseRef(temp);

    return native;
}

daqStruct* daq_toDaqCoordinates(struct Coordinates native, daqTypeManager* typeManager)
{
    daqStructBuilder* builder = NULL;
    daqStructBuilder_createStructBuilder(&builder, daq_toDaqString("DAQ_Coordinates"), typeManager);

    daqString* temp = NULL;
    daqInteger* tempInt = NULL;
    if (0)
    {
        // We can set values in the struct either via directly assigning them to the
        // corresponding struct values or adding them in the correct sequence 
        // to a daqList and setting the list.
        daqList* values = NULL;
        daqList_createList(&values);

        tempInt = daq_toDaqInteger(native.x);
        daqList_pushBack(values, (daqBaseObject*) tempInt);
        daqReleaseRef(tempInt);
        tempInt = daq_toDaqInteger(native.y);
        daqList_pushBack(values, (daqBaseObject*) tempInt);
        daqReleaseRef(tempInt);
        tempInt = daq_toDaqInteger(native.z);
        daqList_pushBack(values, (daqBaseObject*) tempInt);
        daqReleaseRef(tempInt);

        daqStructBuilder_setFieldValues(builder, values);
        daqReleaseRef(values);
    }
    else
    {

        temp = daq_toDaqString("x");
        tempInt = daq_toDaqInteger(native.x);
        daqStructBuilder_set(builder, temp, (daqBaseObject*) tempInt);
        daqReleaseRef(temp);
        daqReleaseRef(tempInt);

        temp = daq_toDaqString("y");
        tempInt = daq_toDaqInteger(native.y);
        daqStructBuilder_set(builder, temp, (daqBaseObject*) tempInt);
        daqReleaseRef(temp);
        daqReleaseRef(tempInt);

        temp = daq_toDaqString("z");
        tempInt = daq_toDaqInteger(native.z);
        daqStructBuilder_set(builder, temp, (daqBaseObject*) tempInt);
        daqReleaseRef(temp);
        daqReleaseRef(tempInt);
    }
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
    int64_t temp;
    daqEnumeration_getIntValue(daq, &temp);

    // Sanity check
    if (temp < 2 && temp >= 0)
        native = temp;

    return native;
}

daqEnumeration* daq_toCompStatusTypeEnum(enum ComponentStatusTypeEnum native, daqTypeManager* typeManager)
{
    daqEnumeration* daq = NULL;
    daqInteger* tempInt = daq_toDaqInteger(native);
    daqString* temp = daq_toDaqString("ComponentStatusType");

    daqEnumeration_createEnumerationWithIntValue(&daq, temp, tempInt, typeManager);

    daqReleaseRef(temp);
    daqReleaseRef(tempInt);
    return daq;
}

int main()
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    advancedSimulatorAddition(&simulator, &instance);



    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);

    return 0;
}