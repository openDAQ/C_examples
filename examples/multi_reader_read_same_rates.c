#include <copendaq.h>
#include <daq_utils.h>
#include <stdlib.h>
#include <Windows.h>

// Configure the simulator device to have 4 channels
daqErrCode setNumberOfChannels(daqDevice* device, daqInt num);

daqErrCode createMultiReader(daqList* signals, daqMultiReader** reader) {
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

daqErrCode handleEvent(daqMultiReaderStatus* status, daqSizeT* sampleRate) {
    daqErrCode err = DAQ_SUCCESS;

    // NOTE: MultiReaderStatus cannot use the ReaderStatus interface for getting the event packet
    daqEventPacket* eventPacket = NULL;
    daqMultiReaderStatus_getMainDescriptor(status, &eventPacket);
    
    daqString* eventId = NULL;
    daqEventPacket_getEventId(eventPacket, &eventId);

    daqBool check = False;
    daqString* checkStr = NULL;
    daqString_createString(&checkStr, "DATA_DESCRIPTOR_CHANGED");

    daqBaseObject_equals(eventId, checkStr, &check);

    daqReleaseRef(checkStr);
    daqReleaseRef(eventId);

    if (check == False)
    {
        daqReleaseRef(eventPacket);
        return DAQ_ERR_INVALID_DATA;
    }

    daqDict* parameters = NULL;
    daqEventPacket_getParameters(eventPacket, &parameters);

    daqString* domainDescriptorStr = NULL;
    daqString_createString(&domainDescriptorStr, "DomainDataDescriptor");

    daqDataDescriptor* domainDescriptor = NULL;
    daqDict_get(parameters, domainDescriptorStr, (daqBaseObject**) &domainDescriptor);

    getSampleRate(sampleRate, domainDescriptor);

    daqReleaseRef(domainDescriptorStr);
    daqReleaseRef(domainDescriptor);
    daqReleaseRef(parameters);
    daqReleaseRef(eventPacket);
    return err;
}

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
            daqSizeT sampleRate;
            handleEvent(status, &sampleRate);

            // Buffer size for 100ms worth of samples
            bufferSize = sampleRate / 10;

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
            printf("----- DATA -----\n");
            for (daqSizeT sample = 0; sample < count; ++sample) {
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

    //setNumberOfChannels(device, 4);

    daqList* signals;
    daqDevice_getSignalsRecursive(device, &signals, NULL);

    readDataSameRateSignals(signals);

    daqReleaseRef(signals);
    daqReleaseRef(device);
    daqReleaseRef(instance);
    daqReleaseRef(simulatorInstance);
	return 0;
}

// Configure the simulator device to have 4 channels
daqErrCode setNumberOfChannels(daqDevice* device, daqInt num) {
    daqString* numOfChannelsStr = NULL;
    daqString_createString(&numOfChannelsStr, "NumberOfChannels");

    daqInteger* number = NULL;
    daqInteger_createInteger(&number, 4);

    daqPropertyObject* propObj = NULL;
    daqQueryInterface(device, DAQ_PROPERTY_OBJECT_INTF_ID, &propObj);

    daqErrCode err = daqPropertyObject_setPropertyValue(propObj, numOfChannelsStr, number);
    printf("Set number of channels exited with: %d", (int)err);

    daqReleaseRef(propObj);
    daqReleaseRef(number);
    daqReleaseRef(numOfChannelsStr);

    return err;
}