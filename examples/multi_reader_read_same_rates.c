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
};

/**
* Create multi reader from a list of signals.
*/
daqErrCode createMultiReader(daqList* signals, daqMultiReader** reader);

/**
* Handle event in the daqMultiReaderStatus by updating the DomainMetadata object.
*/
daqErrCode handleEvent(daqMultiReaderStatus* status, struct DomainMetadata* metadata);

/**
* Extracts sample rate, start and delta from linear data rule and updates metadata fields.
*/
daqErrCode processDataRule(daqDataDescriptor* domainDataDescriptor, struct DomainMetadata* metadata);

/**
* Extract reference domain info offset from reference domain info object and updates the metadata field.
*/
daqErrCode processReferenceDomainInfo(daqDataDescriptor* domainDataDescriptor, struct DomainMetadata* metadata);

/**
* Returns True if the packet represents data descriptor change.
*/
daqBool isDataDescriptorChangeEvent(daqEventPacket* eventPacket);

/**
* Get offset to the first sample in the read buffer.
*/
daqInt getOffsetFromStatus(daqReaderStatus* status);

/**
* Get a daqNumber from daqDict object.
*/
daqErrCode getNumberFromDict(daqDict* dict, const char* key, daqNumber** out);

void readDataSameRateSignals(daqList* signals)
{
    daqMultiReader* multireader = NULL;
    createMultiReader(signals, &multireader);
    daqReader* multireaderAsReader = daqQueryInterfacePtr(multireader, DAQ_READER_INTF_ID);

    daqSizeT signalCount = 0;
    daqList_getCount(signals, &signalCount);

    daqSizeT bufferSize = 0;
    void** dataBuffers = malloc(signalCount * sizeof(void*));
    daqBool buffersAllocated = False;

    struct DomainMetadata domain = { 1, 0, 1, 0 };
    for (daqSizeT readCount = 0; readCount < 20; ++readCount){
        daqSizeT availableCount = 0;
        daqReader_getAvailableCount(multireaderAsReader, &availableCount);
        
        daqSizeT count = min(bufferSize, availableCount);

        daqMultiReaderStatus* status = NULL;
        daqMultiReader_read(multireader, dataBuffers, &count, 0, &status);
        daqReaderStatus* statusAsReaderStatus = daqQueryInterfacePtr(status, DAQ_READER_STATUS_INTF_ID);

        daqReadStatus reportedStatus;
        daqReaderStatus_getReadStatus(statusAsReaderStatus, &reportedStatus);
        if (reportedStatus == daqReadStatusEvent) {
            handleEvent(status, &domain);

            // Buffer size for 100ms worth of samples
            bufferSize = domain.sampleRate / 10;

            for (daqSizeT i = 0; i < signalCount; ++i) {
                if (bufferSize == 0) {
                    continue;
                }
                if (buffersAllocated) {
                    free(dataBuffers[i]);
                }
                dataBuffers[i] = malloc(bufferSize * sizeof(double));
            }
            buffersAllocated = bufferSize != 0;
        }
        else if (reportedStatus == daqReadStatusOk && count > 0) {
            daqInt readOffset = getOffsetFromStatus(statusAsReaderStatus);
            daqInt readStartTick = domain.ruleStart + domain.referenceDomainOffset + readOffset;

            printf("\n-- TIMESTAMP --- | -------- DATA (%lld) --------\n", readCount);
            for (daqSizeT sample = 0; sample < count; ++sample) {
                daqInt sampleTick = readStartTick + sample * domain.ruleDelta;
                printf("%lld | ", sampleTick);
                for (daqSizeT i = 0; i < signalCount; ++i) {
                    double* buffer = (double*)dataBuffers[i];
                    if (buffer == NULL) {
                        continue;
                    }
                    printf("%lf; ", buffer[sample]);
                }
                printf("\n");
            }
        }


        daqReleaseRef(statusAsReaderStatus);
        daqReleaseRef(status);
        Sleep(50);
    }

    if (buffersAllocated) {
        for (daqSizeT i = 0; i < signalCount; ++i) {
            free(dataBuffers[i]);
        }
    }
    free(dataBuffers);

    daqReleaseRef(multireaderAsReader);
    daqReleaseRef(multireader);
}

int main(void) {
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);
    daqInstance* instance = NULL;
    daqDevice* device = NULL;
    addSimulator(&device, &instance);

    daqList* signals;
    daqDevice_getSignalsRecursive(device, &signals, NULL);

    readDataSameRateSignals(signals);

    daqReleaseRef(signals);
    daqReleaseRef(device);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);
	return 0;
}

daqErrCode createMultiReader(daqList* signals, daqMultiReader** reader)
{
    daqErrCode err = DAQ_SUCCESS;

    daqMultiReaderBuilder* builder = NULL;
    daqMultiReaderBuilder_createMultiReaderBuilder(&builder);

    daqMultiReaderBuilder_setValueReadType(builder, daqSampleTypeFloat64);
    daqMultiReaderBuilder_setDomainReadType(builder, daqSampleTypeInt64);

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
        daqReleaseRef(signal);
    }
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

    processDataRule(domainDescriptor, metadata);
    processReferenceDomainInfo(domainDescriptor, metadata);

    daqReleaseRef(domainDescriptorStr);
    daqReleaseRef(domainDescriptor);
    daqReleaseRef(parameters);
    daqReleaseRef(eventPacket);
    return err;
}

daqErrCode processDataRule(daqDataDescriptor* domainDataDescriptor, struct DomainMetadata* metadata)
{
    daqDataRule* dataRule = NULL;
    daqDataDescriptor_getRule(domainDataDescriptor, &dataRule);

    if (!checkIsLinearRule(dataRule))
    {
        printf("Data rule of the signal is not linear, therefore we cannot calculate sample rate.");
        daqReleaseRef(dataRule);
        return DAQ_ERR_INVALID_DATA;
    }

    daqRatio* ratio = NULL;
    daqDataDescriptor_getTickResolution(domainDataDescriptor, &ratio);

    daqDict* parametersDataRule = NULL;
    daqDataRule_getParameters(dataRule, &parametersDataRule);


    daqNumber* delta = NULL;
    getNumberFromDict(parametersDataRule, "delta", &delta);

    daqNumber* start = NULL;
    getNumberFromDict(parametersDataRule, "start", &start);

    calculateSampleRate(&(metadata->sampleRate), ratio, delta);
    daqNumber_getIntValue(delta, &(metadata->ruleDelta));
    daqNumber_getIntValue(start, &(metadata->ruleStart));

    daqReleaseRef(delta);
    daqReleaseRef(start);
    daqReleaseRef(parametersDataRule);
    daqReleaseRef(ratio);
    daqReleaseRef(dataRule);

    return DAQ_SUCCESS;
}

daqErrCode processReferenceDomainInfo(daqDataDescriptor* domainDataDescriptor, struct DomainMetadata* metadata)
{
    daqErrCode err = DAQ_SUCCESS;

    daqReferenceDomainInfo* domainInfo = NULL;
    err = daqDataDescriptor_getReferenceDomainInfo(domainDataDescriptor, &domainInfo);

    // Reference domain info is not mandatory
    if (err || domainInfo == NULL) {
        printf("Reference domain info unavailable.");
        metadata->referenceDomainOffset = 0;
        return DAQ_SUCCESS;
    }

    daqInteger* refDomainOffset = NULL;
    err = daqReferenceDomainInfo_getReferenceDomainOffset(domainInfo, &refDomainOffset);

    // Reference domain info is not mandatory
    if (err || refDomainOffset == NULL) {
        printf("Reference domain info unavailable.");
        metadata->referenceDomainOffset = 0;
        daqReleaseRef(domainInfo);
        return DAQ_SUCCESS;
    }

    err = daqInteger_getValue(refDomainOffset, &(metadata->referenceDomainOffset));

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
