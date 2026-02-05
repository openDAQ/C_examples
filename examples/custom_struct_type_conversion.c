/*
 * 
 */
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
    daqStructType_createStructTypeNoDefaults(&newType, daq_toDaqString("DAQ_Coordinates"), names, types);

    daqTypeManager_addType(typeManager, (daqType*) newType);
    daqReleaseRef(newType);
    daqReleaseRef(names);

    daqBool check = False;
    daqTypeManager_hasType(typeManager, daq_toDaqString("DAQ_Coordinates"), &check);

    daqBool enumCheck = False;
    daqTypeManager_hasType(typeManager, daq_toDaqString("DAQ_ComponentStatusTypeEnum"), &enumCheck);

    if (check && enumCheck)
        printf("\nNew types added.\n");
    else
        printf("\nFailed to add new types.\n");

    daqReleaseRef(types);
    daqReleaseRef(names);
    daqReleaseRef(typeManager);
}

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
    uint8_t temp;
    daqEnumeration_getIntValue(daq, &temp);

    // Sanity check
    if (temp < 2)
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
    addSimulator(&simulator, &instance);

    daqContext* context = NULL;
    daqComponent_getContext((daqComponent*) instance, &context);
    addCoordinateStructToTypeManager(context);
    daqReleaseRef(context);

    daqStructBuilder* builder = NULL;
    
    // Set a new property Object that is the new type and write its value and read it
    // Might need to set up a custom simulator that will have daq_coordinates as a field
    
    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);

    return 0;
}