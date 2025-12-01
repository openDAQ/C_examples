#include <build_instance.h>

// When writing an openDAQ application building block from which everything else is derived from is an Instance object. It creation can be tailored via specifing where it need to look for modules that you wish loaded or by changing its local ID (or both).
// In this example we will demonstate the simplest way to create an instance object in C, where no custom parameters will be used.

int main()
{
    // First we create a builder (by adjusting its settings we change the created object (modulePath and localId ...))
    daqInstanceBuilder* builder = NULL;
    daqInstanceBuilder_createInstanceBuilder(&builder);

    // The created Instance gets its settings from the builder that was used to create it
    daqInstance* instance = NULL;
    daqInstance_createInstanceFromBuilder(&instance, builder);

    // A simple check so that we know instance actually got created
    if (instance == NULL)
    {
        printf("Failed to create instance.\r\n");
        return 0;
    }
    else
    {
        printf("Instance successfully created.\r\n");
    }

    // Due to C not having inherit smart pointer capabilities, we need to dereference object pointers ourselves, so that we do not create memory leaks.
    daqReleaseRef(builder);
    daqReleaseRef(instance);

    return 0;
}
