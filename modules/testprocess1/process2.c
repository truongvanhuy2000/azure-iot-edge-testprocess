#include <stdio.h>
#include <stdlib.h>
#include "azureiot/iothub.h"
#include "azureiot/iothub_client_core_common.h"
#include "azureiot/iothubtransportmqtt.h"
#include "azure_c_shared_utility/threadapi.h"
#include "azureiot/iothub_module_client.h"
#include <unistd.h>
#include <sys/wait.h>

static const int g_interval = 5000;  // 10 sec send interval initially

static int method_callback(const char* method_name, const unsigned char* payload, size_t size, unsigned char** response, size_t* response_size, void* userContextCallback){
    pid_t pid = getpid();
    (void)userContextCallback;
    // When a message is sent this callback will get invoked1
    char * tempMessage = (char *)malloc(100 + 1);
    sprintf(tempMessage, "\n%d : %s called with %s\n", pid, method_name, payload);
    fprintf(stderr, tempMessage);
    free(tempMessage);

    *response = (unsigned char*)malloc(100);
    sprintf((char*)(*response), "{ \"Response\": \"This is the response from the method\" }");
    *response_size = strlen((char*)(*response));

    return 200;
}
void doSomething(char* tempMessage){
    while(1){
        ThreadAPI_Sleep(g_interval);
        fprintf(stderr, tempMessage);
    }
}
void iotHubProccess1(){
    // const char* connectionString = "HostName=testHub0411.azure-devices.net;DeviceId=testDevice1481136;ModuleId=doABC;SharedAccessKey=ltJRC2BLSVX2dmS4dsO8QFjy3J8ak7caiEjROQpMZoY=";
    IoTHub_Init();
    // #ifdef USE_EDGE_MODULES
    
    IOTHUB_CLIENT_TRANSPORT_PROVIDER protocol = MQTT_Protocol;
    IOTHUB_MODULE_CLIENT_HANDLE moduleHandler = IoTHubModuleClient_CreateFromEnvironment(protocol);

    fprintf(stderr, "\nStart proccess 1 \n");
    if (moduleHandler == NULL){
        fprintf(stderr, "\nFailure creating IotHub device for process 1. Hint: Check your connection string.\r\n");
        return;
    }
    IOTHUB_CLIENT_RESULT invokeAsyncResult = IoTHubModuleClient_SetModuleMethodCallback(moduleHandler, method_callback, NULL);
    if (invokeAsyncResult == IOTHUB_CLIENT_OK)
    {
        // Because we're using the convenience layer, the callback happens asynchronously.  Wait here for it to complete.
        doSomething("\nprocess 1 interval task\n");
    }
    else
    {
        (void)printf("IoTHubModuleClient_LL_ModuleMethodInvoke failed with result: %d\n", invokeAsyncResult);
    }
    IoTHubModuleClient_Destroy(moduleHandler);
    // #endif
    
    IoTHub_Deinit();
}

void iotHubProccess2(){
    fprintf(stderr, "\nStart proccess 2\n");
    IOTHUB_CLIENT_TRANSPORT_PROVIDER protocol = MQTT_Protocol;
    IoTHub_Init();
    IOTHUB_MODULE_CLIENT_HANDLE moduleHandler = IoTHubModuleClient_CreateFromEnvironment(protocol);
    if (moduleHandler == NULL){
        fprintf(stderr, "\nFailure creating IotHub device for process 2. Hint: Check your connection string.\r\n");
        return;
    }
    doSomething("process 2 interval task\n");
}
void forkChild(){
    pid_t pid1, pid2; // Process IDs

    fprintf(stderr, "\nHey, im'starting here\n");
    // Fork the first child process
    pid1 = fork();

    if (pid1 < 0) {
        perror("Fork 1 failed");
        exit(EXIT_FAILURE);
    } else if (pid1 == 0) {
        // This code is executed by the first child process
        iotHubProccess1();
        fprintf(stderr, "\nFirst child process: PID = %d\n", getpid());
    } else {
        // This code is executed by the parent process
        fprintf(stderr, "\nParent process: PID = %d, First Child PID = %d\n", getpid(), pid1);

        // Fork the second child process
        pid2 = fork();

        if (pid2 < 0) {
            perror("Fork 2 failed");
        } else if (pid2 == 0) {
            // This code is executed by the second child process
            fprintf(stderr, "\nSecond child process: PID = %d\n", getpid());
            iotHubProccess2();
        } else {
            // This code is executed by the parent process
            fprintf(stderr, "\nParent process: PID = %d, Second Child PID = %d\n", getpid(), pid2);
            waitpid(pid1, NULL, 0);
            waitpid(pid2, NULL, 0);

            fprintf(stderr, "\nParent process: Child processes have finished.\n");
        }
    }
}
int main() {
    (void)fprintf(stderr, "Process 2\n");
    // forkChild();
    iotHubProccess2();
    return 0;
}


