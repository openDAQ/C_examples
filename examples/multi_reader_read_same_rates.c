#include <copendaq.h>
#include <daq_utils.h>
#include <stdlib.h>
#include <Windows.h>

/**
* Set of parameters describing the time domain.
*/
struct DomainMetadata
{
    daqSizeT sampleRate;
    daqInt ruleStart;
    daqInt ruleDelta;
    daqInt referenceDomainOffset;

    daqString* origin;
    daqString* unitSymbol;

    daqInt resNum;
    daqInt resDen;
};

/**
* Creation utility function.
*/
struct DomainMetadata createDomainMetadata();

/**
* Destruction utility.
*/
void freeDomainMetadata(struct DomainMetadata* p);

/*
* Read data from a list of signals. The signals should have the same sample rate.
* 
* This function is the core of this example. It demonstrates how to use multi reader
* to read samples from multiple aligned signals. The reading process consists of a
* simple loop with a timed sleep to let samples accumulate. The multi reader user
* only needs to allocate appropriate buffers, but there is no need to handle individual
* packets.
*/
void readDataSameRateSignals(daqList* signals);

/**
* Add existing device.
*/
daqErrCode addDeviceWrapper(daqInstance** instance, daqDevice** device, const char* connectionStr);

/**
* Get the first signals of channels with tag "AI" from device.
* 
* Actual devices may have a lot of signals that are unsuitable for synchronized reading. One way
* of collecting appropriate signals is to filter channels by tag "AI" and take their first signal.
*/
void getAIChannelSignals(daqDevice* device, daqList** signals);

/**
* Create multi reader from a list of signals.
*/
daqErrCode createMultiReader(daqList* signals, daqMultiReader** reader);

/**
* Handle event in the daqMultiReaderStatus by updating the DomainMetadata object.
*/
daqErrCode handleEvent(daqMultiReaderStatus* status, struct DomainMetadata* metadata);

/**
* Extracts sample rate, start and delta from linear data rule, referenceDomainOffset from reference domain info
* and updates metadata fields.
*/
daqErrCode updateDomainMetadata(daqDataDescriptor* domainDataDescriptor, struct DomainMetadata* metadata);

/**
* Get reference domain info offset from reference domain info object.
*/
daqErrCode getReferenceDomainOffset(daqDataDescriptor* domainDataDescriptor, daqInt* offset);

/**
* Returns True if the packet represents data descriptor change.
*/
daqBool isDataDescriptorChangeEvent(daqEventPacket* eventPacket);

/**
* Print absolute time stamp in format: 20483d 14h 49min 10.934s since 1970-01-01T00:00:00Z. The precision
* of this format is 1ms, so two samples within a millisecond may have the same time stamp printed out.
*/
void printAbsoluteTimestamp(struct DomainMetadata* domain, daqInt tick);

/**
* Get offset to the first sample in the read buffer.
*/
daqInt getOffsetFromStatus(daqReaderStatus* status);

/**
* Get a daqNumber from daqDict object.
*/
daqErrCode getNumberFromDict(daqDict* dict, const char* key, daqNumber** out);

/**
* Allocate buffers to hold n doubles. It is assumed that buffers has enough allocated memory to hold count
* number of pointers.
*/
daqBool allocateBuffers(void** buffers, daqSizeT count, daqSizeT n);

/**
* Calls free on count number of pointer inside buffers if allocated is True.
*/
void freeIfAllocated(void** buffers, daqSizeT count, daqBool allocated);

int main(void) {
    daqInstance* simulatorInstance = NULL;
    daqInstance* instance = NULL;
    daqDevice* device = NULL;
    setupSimulator(&simulatorInstance);

    daqList* signals = NULL;

    // This example creates a simulator device with 8 channels and connects to it.
    // Connection string may be replaced with a connection string of an existing
    // (real) device.
    // 
    // All of the simulator's signals are appropriate for synchronized reading. For real
    // devices one may need to filter the signals. One of the options is to use the function
    // getAIChannelSignals.

    const char* connectionString = "daq://openDAQ_sim01";
    addDeviceWrapper(&instance, &device, connectionString);
    daqDevice_getSignalsRecursive(device, &signals, NULL);
    
    // Connect to an existing device
    // const char* customString = "daq://Dewesoft_DB24049746";
    // addDeviceWrapper(&instance, &device, customString);
    // getAIChannelSignals(device, &signals);

    // TODO: check sampling rates.

    // Start reading samples from the signals.
    readDataSameRateSignals(signals);

    daqReleaseRef(signals);
    daqReleaseRef(device);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);
	return 0;
}

void readDataSameRateSignals(daqList* signals)
{
    daqMultiReader* multireader = NULL;
    createMultiReader(signals, &multireader);
    daqReader* multireaderAsReader = daqQueryInterfacePtr(multireader, DAQ_READER_INTF_ID);

    daqSizeT signalCount = 0;
    daqList_getCount(signals, &signalCount);

    void** dataBuffers = calloc(signalCount, sizeof(void*));

    daqBool buffersAllocated = False;
    daqSizeT bufferSize = 0;

    struct DomainMetadata domain = createDomainMetadata();
    for (daqSizeT readCount = 0; readCount < 20; ++readCount) {
        daqSizeT availableCount = 0;
        daqReader_getAvailableCount(multireaderAsReader, &availableCount);

        daqSizeT count = min(bufferSize, availableCount);

        daqMultiReaderStatus* status = NULL;
        daqMultiReader_read(multireader, dataBuffers, &count, 0, &status);
        daqReaderStatus* statusAsReaderStatus = daqQueryInterfacePtr(status, DAQ_READER_STATUS_INTF_ID);

        daqReadStatus reportedStatus;
        daqReaderStatus_getReadStatus(statusAsReaderStatus, &reportedStatus);
        if (reportedStatus == daqReadStatusEvent) {
            daqErrCode err = handleEvent(status, &domain);

            if (err == DAQ_SUCCESS) {
                // Buffer size for 400ms worth of samples
                bufferSize = (domain.sampleRate * 2) / 5;
            }
            else {
                bufferSize = 0; // Allocate nothing
            }

            freeIfAllocated(dataBuffers, signalCount, buffersAllocated);
            buffersAllocated = allocateBuffers(dataBuffers, signalCount, bufferSize);
            bufferSize = buffersAllocated ? bufferSize : 0;
        }
        else if (reportedStatus == daqReadStatusOk && count > 0) {
            // Get offset of the first sample
            daqInt readOffset = getOffsetFromStatus(statusAsReaderStatus);

            // Get tick of the first sample
            daqInt readStartTick = domain.ruleStart + domain.referenceDomainOffset + readOffset;

            printf("\nRead successful (%lld).\nRead %lld samples. Printing samples 0, 1 and %lld.\n", readCount, count, count-1);
            printf("--------------- TIMESTAMP ------------------------- | -------- DATA -----------\n");
            for (daqSizeT sample = 0; sample < count; ++sample) {
                // Only print the first and the last samples
                if (sample == 2) {
                    printf("...\n");
                    continue;
                }
                else if (sample > 2 && sample < count - 1) {
                    continue;
                }

                // Calculate tick according to linear data rule.
                daqInt sampleTick = readStartTick + sample * domain.ruleDelta;

                printAbsoluteTimestamp(&domain, sampleTick);
                printf(" | ");
                for (daqSizeT i = 0; i < signalCount; ++i) {
                    if (dataBuffers[i] == NULL) {
                        continue;
                    }
                    double* buffer = (double*)dataBuffers[i];
                    printf("%lf; ", buffer[sample]);
                }
                printf("\n");
            }
        }

        daqReleaseRef(statusAsReaderStatus);
        daqReleaseRef(status);
        Sleep(200);
    }

    freeIfAllocated(dataBuffers, signalCount, buffersAllocated);
    free(dataBuffers);

    freeDomainMetadata(&domain);
    daqReleaseRef(multireaderAsReader);
    daqReleaseRef(multireader);
}

// Helper
void printSignalGlobalId(daqSignal* signal)
{
    daqString* localId = NULL;
    daqComponent_getGlobalId((daqComponent*)signal, &localId);

    printDaqFormattedString(" - %s\n", localId);
}

daqErrCode addDeviceWrapper(daqInstance** instance, daqDevice** device, const char* connectionStr)
{
    createInstance(instance, MODULE_PATH);

    daqString* connectionString = NULL;
    daqString_createString(&connectionString, connectionStr);
    daqErrCode err = daqDevice_addDevice((daqDevice*)*instance, device, connectionString, NULL);
    daqReleaseRef(connectionString);

    if (err != DAQ_SUCCESS)
        printf("Add device failed");
    return err;
}

void getAIChannelSignals(daqDevice* device, daqList** signals)
{
    // Create empty
    daqList_createListWithElementType(signals, DAQ_SIGNAL_INTF_ID);

    daqString* ai = NULL;
    daqString_createString(&ai, "AI");

    daqList* tags = NULL;
    daqList_createListWithElementType(&tags, DAQ_STRING_INTF_ID);
    daqList_pushBack(tags, ai);

    daqSearchFilter* filter;
    daqSearchFilter_createRequiredTagsSearchFilter(&filter, tags);

    // Get AI channels
    daqList* channels = NULL;
    daqDevice_getChannels(device, &channels, filter);

    daqIterator* iterator = NULL;
    daqList_createStartIterator(channels, &iterator);
    while (daqIterator_moveNext(iterator) == DAQ_SUCCESS)
    {
        daqFunctionBlock* ch = NULL;
        daqIterator_getCurrent(iterator, &ch);

        daqList* chSignals = NULL;
        daqFunctionBlock_getSignals(ch, &chSignals, NULL);

        daqSizeT cnt = 0;
        daqList_getCount(chSignals, &cnt);

        if (cnt > 0) {
            daqSignal* firstSignal = NULL;
            daqList_getItemAt(chSignals, 0, &firstSignal);
            daqList_pushBack(*signals, firstSignal);

            daqReleaseRef(firstSignal);
        }

        daqReleaseRef(chSignals);
        daqReleaseRef(ch);
    }

    daqReleaseRef(iterator);
    daqReleaseRef(channels);
    daqReleaseRef(tags);
    daqReleaseRef(ai);
    daqReleaseRef(filter);
}

daqErrCode createMultiReader(daqList* signals, daqMultiReader** reader)
{
    daqErrCode err = DAQ_SUCCESS;

    daqMultiReaderBuilder* builder = NULL;
    daqMultiReaderBuilder_createMultiReaderBuilder(&builder);

    daqMultiReaderBuilder_setValueReadType(builder, daqSampleTypeFloat64);
    daqMultiReaderBuilder_setDomainReadType(builder, daqSampleTypeInt64);

    printf("Signals to read:\n");

    daqSizeT signalCount = 0;
    daqList_getCount(signals, &signalCount);
    for (daqSizeT i = 0; i < signalCount; ++i) {
        daqSignal* signal = NULL;
        daqList_getItemAt(signals, i, &signal);
        if (signal == NULL) {
            err = DAQ_ERR_GENERALERROR;
            break;
        }
        daqMultiReaderBuilder_addSignal(builder, signal);
        printSignalGlobalId(signal);
        daqReleaseRef(signal);
    }
    printf("\n");

    daqMultiReaderBuilder_build(builder, reader);
    daqReleaseRef(builder);

    return err;
}

daqErrCode handleEvent(daqMultiReaderStatus* status, struct DomainMetadata* metadata)
{
    daqErrCode err = DAQ_SUCCESS;

    // NOTE: MultiReaderStatus cannot use the ReaderStatus interface for getting the event packet
    daqEventPacket* eventPacket = NULL;
    daqMultiReaderStatus_getMainDescriptor(status, &eventPacket);

    if (isDataDescriptorChangeEvent(eventPacket) == False)
    {
        daqReleaseRef(eventPacket);
        return DAQ_ERR_INVALID_DATA;
    }

    daqDict* parameters = NULL;
    daqEventPacket_getParameters(eventPacket, &parameters);

    daqString* domainDescriptorStr = NULL;
    daqString_createString(&domainDescriptorStr, "DomainDataDescriptor");

    daqDataDescriptor* domainDescriptor = NULL;
    daqDict_get(parameters, domainDescriptorStr, (daqBaseObject**)&domainDescriptor);

    err = updateDomainMetadata(domainDescriptor, metadata);

    daqReleaseRef(domainDescriptorStr);
    daqReleaseRef(domainDescriptor);
    daqReleaseRef(parameters);
    daqReleaseRef(eventPacket);
    return err;
}

daqErrCode updateDomainMetadata(daqDataDescriptor* descriptor, struct DomainMetadata* metadata)
{
    daqDataRule* dataRule = NULL;
    daqDataDescriptor_getRule(descriptor, &dataRule);

    if (!checkIsLinearRule(dataRule))
    {
        printf("Data rule of the signal is not linear, therefore we cannot calculate sample rate.");
        daqReleaseRef(dataRule);
        return DAQ_ERR_INVALID_DATA;
    }

    // Origin and unit
    daqDataDescriptor_getOrigin(descriptor, &(metadata->origin));

    daqUnit* unit = NULL;
    daqDataDescriptor_getUnit(descriptor, &unit);
    daqUnit_getSymbol(unit, &(metadata->unitSymbol));
    daqReleaseRef(unit);
    printDaqFormattedString("Origin: %s\n", metadata->origin);
    printDaqFormattedString("Unit: %s\n", metadata->unitSymbol);

    // Check if unitSymbo == "s"
    daqString* secondsSym = NULL;
    daqString_createString(&secondsSym, "s");

    daqBool check = False;
    daqBaseObject_equals(metadata->unitSymbol, secondsSym, &check);
    daqReleaseRef(secondsSym);
    if (!check) {
        printf("Unit symbol is not 's'!");
        daqReleaseRef(dataRule);
        return DAQ_ERR_INVALID_DATA;
    }

    // Sample rate, delta and start
    daqRatio* ratio = NULL;
    daqDataDescriptor_getTickResolution(descriptor, &ratio);

    daqRatio_getNumerator(ratio, &metadata->resNum);
    daqRatio_getDenominator(ratio, &metadata->resDen);

    daqDict* parametersDataRule = NULL;
    daqDataRule_getParameters(dataRule, &parametersDataRule);

    daqNumber* delta = NULL;
    getNumberFromDict(parametersDataRule, "delta", &delta);

    daqNumber* start = NULL;
    getNumberFromDict(parametersDataRule, "start", &start);

    calculateSampleRate(&(metadata->sampleRate), ratio, delta);
    daqNumber_getIntValue(delta, &(metadata->ruleDelta));
    daqNumber_getIntValue(start, &(metadata->ruleStart));

    daqReleaseRef(start);
    daqReleaseRef(delta);
    daqReleaseRef(parametersDataRule);
    daqReleaseRef(ratio);
    daqReleaseRef(dataRule);

    // Reference domain info is not mandatory
    getReferenceDomainOffset(descriptor, &(metadata->referenceDomainOffset));

    return DAQ_SUCCESS;
}

daqErrCode getReferenceDomainOffset(daqDataDescriptor* domainDataDescriptor, daqInt* offset)
{
    daqErrCode err = DAQ_SUCCESS;

    daqReferenceDomainInfo* domainInfo = NULL;
    err = daqDataDescriptor_getReferenceDomainInfo(domainDataDescriptor, &domainInfo);

    // Reference domain info is not mandatory
    if (err || domainInfo == NULL) {
        *offset = 0;
        return DAQ_SUCCESS;
    }

    daqInteger* refDomainOffset = NULL;
    err = daqReferenceDomainInfo_getReferenceDomainOffset(domainInfo, &refDomainOffset);

    // Reference domain info is not mandatory
    if (err || refDomainOffset == NULL) {
        *offset = 0;
        daqReleaseRef(domainInfo);
        return DAQ_SUCCESS;
    }

    err = daqInteger_getValue(refDomainOffset, offset);

    daqReleaseRef(refDomainOffset);
    daqReleaseRef(domainInfo);

    return err;
}

daqBool isDataDescriptorChangeEvent(daqEventPacket* eventPacket)
{
    daqString* eventId = NULL;
    daqEventPacket_getEventId(eventPacket, &eventId);

    daqString* checkStr = NULL;
    daqString_createString(&checkStr, "DATA_DESCRIPTOR_CHANGED");

    daqBool check = False;
    daqBaseObject_equals(eventId, checkStr, &check);

    daqReleaseRef(checkStr);
    daqReleaseRef(eventId);
    return check;
}

void printAbsoluteTimestamp(struct DomainMetadata* domain, daqInt tick)
{
    daqInt secondsSinceEpoch = (tick * domain->resNum) / domain->resDen;

    daqInt seconds = secondsSinceEpoch;
    daqInt days = secondsSinceEpoch / (60 * 60 * 24);
    seconds -= days * (60 * 60 * 24);

    daqInt hours = seconds / (60 * 60);
    seconds -= hours * (60 * 60);

    daqInt minutes = seconds / 60;
    seconds -= minutes * 60;

    daqInt leftoverTicks = tick - (secondsSinceEpoch * domain->resDen) / domain->resNum;
    daqInt milliseconds = (1000 * leftoverTicks * domain->resNum) / domain->resDen;

    char* origin = NULL;
    daqString_getCharPtr(domain->origin, &origin);
    printf("%lldd %lldh %lldmin %lld.%llds since %s", days, hours, minutes, seconds, milliseconds, origin);
}

daqInt getOffsetFromStatus(daqReaderStatus* status)
{
    daqNumber* offsetNum = NULL;
    daqReaderStatus_getOffset(status, &offsetNum);
    daqInt offset = 0;
    daqNumber_getIntValue(offsetNum, &offset);

    daqReleaseRef(offsetNum);
    return offset;
}

daqErrCode getNumberFromDict(daqDict* dict, const char* key, daqNumber** out)
{
    daqErrCode err = DAQ_SUCCESS;

    daqString* keyStr = NULL;
    err = daqString_createString(&keyStr, key);

    if (err)
        return err;

    daqBaseObject* obj = NULL;
    err = daqDict_get(dict, keyStr, &obj);
    daqReleaseRef(keyStr);

    if (err)
        return err;

    err = daqQueryInterface(obj, DAQ_NUMBER_INTF_ID, out);
    daqReleaseRef(obj);
    return err;
}

daqBool allocateBuffers(void** buffers, daqSizeT count, daqSizeT n)
{
    if (n == 0)
        return False;

    for (daqSizeT i = 0; i < count; ++i) {
        buffers[i] = malloc(n * sizeof(double));
        if (!buffers[i]) {
            for (daqSizeT j = 0; j < i; ++j) {
                free(buffers[j]);
                buffers[j] = NULL;
            }
            return False;
        }
    }
    return True;
}

void freeIfAllocated(void** buffers, daqSizeT count, daqBool allocated)
{
    if (allocated == False)
        return;
    
    for (daqSizeT i = 0; i < count; ++i) {
        free(buffers[i]);
        buffers[i] = NULL;
    }
}

struct DomainMetadata createDomainMetadata()
{
    struct DomainMetadata d = { 1, 0, 1, 0, NULL, NULL };
    return d;
}
void freeDomainMetadata(struct DomainMetadata* p)
{
    if (p->origin != NULL) {
        daqReleaseRef(p->origin);
        p->origin = NULL;
    }
    if (p->unitSymbol != NULL) {
        daqReleaseRef(p->unitSymbol);
        p->unitSymbol = NULL;
    }
}