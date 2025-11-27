// Basic example of how to create an Instance in openDAQ

#include <build_instance.h>

int main()
{
    daqInstance* instance = NULL;
    createInstance(&instance);

    if (instance == NULL)
    {
        printf("Failed to create instance.\r\n");
        return 0;
    }
    else
    {
        printf("Instance successfully created.\r\n");
    }

    daqReleaseRef(instance);

    return 0;
}
