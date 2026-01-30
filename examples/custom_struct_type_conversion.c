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

// Adds Coodinates struct and ComponentStatusTypeEnum to types in Type Manager
void addCoordinateStructToTypeManager(daqContext* context)
{
    daqTypeManager* typeManager = NULL;
    daqContext_getTypeManager(context, &typeManager);

    daqList* names = NULL;
    daqList_createList(&names);

    daqList_pushBack(names, (daqBaseObject*) daq_toDaqString("x"));
    daqList_pushBack(names, (daqBaseObject*) daq_toDaqString("y"));
    daqList_pushBack(names, (daqBaseObject*) daq_toDaqString("z"));

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

    daqList_createList(&names);

    daqList_pushBack(names, (daqBaseObject*) daq_toDaqString("Error"));
    daqList_pushBack(names, (daqBaseObject*) daq_toDaqString("Ok"));
    daqList_pushBack(names, (daqBaseObject*) daq_toDaqString("Warning"));

    daqEnumerationType* enumType = NULL;
    daqEnumerationType_createEnumerationType(&enumType, daq_toDaqString("DAQ_ComponentStatusTypeEnum"), names, 0);

    daqTypeManager_addType(typeManager, (daqType*) enumType);
    daqReleaseRef(enumType);

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

    daqStruct_get(daq, daq_toDaqString("x"), tempObj);
    native.x = daq_fromDaqInteger((daqInteger*) daqQueryInterfacePtr(tempObj, DAQ_INTEGER_INTF_ID));
    daqReleaseRef(tempObj);

    daqStruct_get(daq, daq_toDaqString("y"), tempObj);
    native.y = daq_fromDaqInteger((daqInteger*) daqQueryInterfacePtr(tempObj, DAQ_INTEGER_INTF_ID));
    daqReleaseRef(tempObj);

    daqStruct_get(daq, daq_toDaqString("z"), tempObj);
    native.z = daq_fromDaqInteger((daqInteger*) daqQueryInterfacePtr(tempObj, DAQ_INTEGER_INTF_ID));
    daqReleaseRef(tempObj);

    return native;
}

daqStruct* daq_toDaqCoordinates(struct Coordinates native, daqTypeManager* typeManager)
{
    daqStructBuilder* builder = NULL;
    daqStructBuilder_createStructBuilder(&builder, daq_toDaqString("DAQ_Coordinates"), typeManager);

    if (0)
    {
        // We can set values in the struct either via directly assigning them to the
        // corresponding struct values or adding them in the correct sequence 
        // to a daqList and setting the list.
        daqList* values = NULL;
        daqList_createList(&values);

        daqList_pushBack(values, (daqBaseObject*) daq_toDaqInteger(native.x));
        daqList_pushBack(values, (daqBaseObject*) daq_toDaqInteger(native.y));
        daqList_pushBack(values, (daqBaseObject*) daq_toDaqInteger(native.z));

        daqStructBuilder_setFieldValues(builder, values);
        daqReleaseRef(values);
    }
    else
    {
        daqStructBuilder_set(builder, daq_toDaqString("x"), (daqBaseObject*) daq_toDaqInteger(native.x));

        daqStructBuilder_set(builder, daq_toDaqString("y"), (daqBaseObject*) daq_toDaqInteger(native.y));

        daqStructBuilder_set(builder, daq_toDaqString("z"), (daqBaseObject*) daq_toDaqInteger(native.z));
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
    daqInteger* temp = daq_toDaqInteger(native);

    daqEnumeration_createEnumerationWithIntValue(&daq, daq_toDaqString("ComponentStatusType"), temp, typeManager);

    daqReleaseRef(temp);
    return daq;
}

int main()
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);



    daqReleaseRef(simulator);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);

    return 0;
}