/*
 * This example will demonstrate how to create and read into buffers
 * that are created based on the calculated sample rate
 */
#include <daq_utils.h>

int main(void)
{
    daqInstance* simulatorInstance = NULL;
    setupSimulator(&simulatorInstance);

    daqInstance* instance = NULL;
    daqDevice* simulator = NULL;
    addSimulator(&simulator, &instance);

    // Calculate sample rate
    // Connect to the first available signal on the simulator
    daqList* availableSignals = NULL;
    daqDevice_getSignalsRecursive(simulator, &availableSignals, NULL);

    daqSignal* connectedSignal = NULL;
    daqList_getItemAt(availableSignals, 0, (daqBaseObject**)&connectedSignal);

    daqSignal* domainSignal = NULL;
    daqSignal_getDomainSignal(connectedSignal, &domainSignal);

    daqDataDescriptor* domainDataDescriptor = NULL;
    daqSignal_getDescriptor(domainSignal, &domainDataDescriptor);

    // Create sample rate buffer
    daqRatio* ratio = NULL;
    daqDataDescriptor_getTickResolution(domainDataDescriptor, &ratio);

    daqInt numerator = 0;
    daqRatio_getNumerator(ratio, &numerator);
    
    daqInt denominator = 0;
    daqRatio_getDenominator(ratio, &denominator);

    daqFloat sampleRate = (daqFloat)denominator / (daqFloat)numerator;

    // 2 seconds of buffer (?)
    daqFloat samples[sampleRate * 2];
    daqUInt domainSamples[sampleRate * 2];

    daqBlockReaderBuilder* blockReaderBuilder = NULL;

    // Create reader
    // Stream reader (??)
    daqStreamReader* streamReader = NULL;
    daqStreamReader_createStreamReader(&streamReader, connectedSignal, daqSampleTypeFloat64, daqSampleTypeInt64, daqReadModeRawValue, daqReadTimeoutTypeAny);

    // Read into the created buffer

    daqReleaseRef(instance);
    daqReleaseRef(simulator);
    daqReleaseRef(simulatorInstance);

    return 0;
}
