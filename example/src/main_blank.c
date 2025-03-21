#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include "CANopen.h"
#include "OD.h"

// Default values for CO_CANopenInit()
#define NMT_CONTROL \
            CO_NMT_STARTUP_TO_OPERATIONAL \
          | CO_NMT_ERR_ON_ERR_REG   \
          | CO_ERR_REG_GENERIC_ERR  \
          | CO_ERR_REG_COMMUNICATION

#define FIRST_HB_TIME 500
#define SDO_SRV_TIMEOUT_TIME 1000
#define SDO_CLI_TIMEOUT_TIME 500
#define SDO_CLI_BLOCK false
#define OD_STATUS_BITS NULL

// Gloabel variables
CO_t* pCO = NULL;  // CANopen-Object

void CANrx_callback(void *object, void *rxMsg);
void tmrTask_thread(void);

int main(void)
{
   CO_ReturnError_t Err;
   uint32_t u32HeapMemoryUsed = 0U;
   CO_config_t* pConfigPtr = NULL;
   void* pCANptr = NULL;               // CAN module address, pointer to CAN device
   uint8_t u8PendingNodeId = 10;       // read from dip switches or nonvolatile memory, configurable by LSS slave
   uint8_t u8ActiveNodeId = 10;        // Copied from CO_pendingNodeId in the communication reset section
   uint16_t u16PendingBitRate = 125;   // read from dip switches or nonvolatile memory, configurable by LSS slave
   CO_NMT_reset_cmd_t Reset = CO_RESET_NOT;

   pCO = CO_new(pConfigPtr, &u32HeapMemoryUsed);
   if (pCO == NULL)
   {
       printf("Can't allocate memory\n");
       return 0;
   }
   else
   {
       printf("Allocated %u bytes of heap\n", u32HeapMemoryUsed);
   }



   //======================================================================================================================================
   // CANopen communication reset - initialize CANopen objects

   // Enter CAN configuration
   CO_CANsetConfigurationMode((void *)&pCANptr);
   CO_CANmodule_disable(pCO->CANmodule);

   // Initialize CANopen
   Err = CO_CANinit(pCO, pCANptr, u16PendingBitRate);
   if (Err != CO_ERROR_NO)
   {
       printf("Error: CAN initialization failed: %d\n", Err);
       return 0;
   }

   // Keine Ahnung was das macht
   CO_LSS_address_t lssAdress =
   {
       .identity =
       {
           .vendorID       = OD_PERSIST_COMM.x1018_identity.vendor_ID,
           .productCode    = OD_PERSIST_COMM.x1018_identity.productCode,
           .revisionNumber = OD_PERSIST_COMM.x1018_identity.revisionNumber,
           .serialNumber   = OD_PERSIST_COMM.x1018_identity.serialNumber
       }
   };

   // LSS Slave brauche ich glaube ich nicht
   Err = CO_LSSinit(pCO, &lssAdress, &u8PendingNodeId, &u16PendingBitRate);
   if (Err != CO_ERROR_NO)
   {
       printf("Error : LSS slave initialization failed : % d\n ", Err);
       return 0;
   }

   u8ActiveNodeId = u8PendingNodeId;
   uint32_t u32ErrorInfo;

   Err = CO_CANopenInit(pCO,                       // CANopen-Object
                        NULL,                      // alternate NMT
                        NULL,                      // alternate em
                        OD,                        // Object dictionary
                        OD_STATUS_BITS,            // Optional OD_statsBits
                        NMT_CONTROL,               // CO_NMT_control_t
                        FIRST_HB_TIME,             // firstHBTime_ms
                        SDO_SRV_TIMEOUT_TIME,      // SDOserverTimeoutTime_ms
                        SDO_CLI_TIMEOUT_TIME,      // SDOclientTimeoutTime_ms
                        SDO_CLI_BLOCK,             // SDOclientBlockTransfer
                        u8ActiveNodeId,
                        &u32ErrorInfo);

   if ((Err != CO_ERROR_NO) && (Err != CO_ERROR_NODE_ID_UNCONFIGURED_LSS))
   {
      if (Err == CO_ERROR_OD_PARAMETERS)
      {
           printf("Error: Object Dictionary entry 0x%X\n", u32ErrorInfo);
      }
      else
      {
          printf("Error: PDO initialization failed: %d\n", Err);
      }

      return 0;
   }

   Err = CO_CANopenInitPDO(pCO, pCO->em, OD, u8ActiveNodeId, &u32ErrorInfo);
   if (Err != CO_ERROR_NO) 
   {
       if (Err == CO_ERROR_OD_PARAMETERS)
       {
           printf("Error: Object Dictionary entry 0x%X\n", u32ErrorInfo);
       }
       else 
       {
           printf("Error: PDO initialization failed: %d\n", Err);
       }
       return 0;
   }

   // Ich wei� noch nicht genau f�r was das ist, Prgramm verh�lt sich ohne init exakt gleich
   // evtl ist es nachher zum Empfangen wichtig
   // Aus AN13121: Before CAN messages can be received, each member in CO_CANrx_t must be initialized.
   //              CO_CANrxBufferInit() is called by CANopen module, which uses specific member. 
   //              Main arguments to the CO_CANrxBufferInit() function are CAN identifier and a pointer to callback function.
   //              Those two arguments(and some others) are copied to the member of the CO_CANrx_t array.
   
   //Err = CO_CANmodule_init(pCO->CANmodule,
   //                  pCANptr,
   //                  pCO->CANrx,
   //                  pCO->CANmodule->rxSize,
   //                  pCO->CANtx,
   //                  pCO->CANmodule->txSize,
   //                  u16PendingBitRate);

   if (Err == CO_ERROR_NO)
   {
       printf("Initialized CANmodule successful\n");
   }
   else
   {
       printf("Could not initialize CANmodule\n");
   }

   //======================================================================================================================================
   // Configure Timer interrupt function for execution every 1 millisecond

   //HANDLE hThread1 = CreateThread(NULL, 0, tmrTask_thread, NULL, 0, NULL);
   //if (Err != CO_ERROR_NO)
   //{
   //    printf("Error configuring callback\n");
   //    return 0;
   //}
   //else
   //{
   //    printf("Callback configured successful\n");
   //}


   //======================================================================================================================================
   // Configure CAN transmit and receive interrupt


   //======================================================================================================================================
   // Configure CANopen callbacks, etc

   // void (*pReceiveBuffer)(void *object, void *message);  // Funktionszeiger
   // f�r CANrx_callback pReceiveBuffer = &CANrx_callback; Err =
   // CO_CANrxBufferInit(pCO->CANmodule, 0, 0x77F, 0x7FF, false, pCO,
   // pReceiveBuffer); // Konfiguration CAN Empfangs buffer



   CO_CANtx_t *pBuffer = CO_CANtxBufferInit(pCO->CANmodule, 0, 0x203, false, 8, false);
   pBuffer->DLC = 8;

   for (int i = 0; i < pBuffer->DLC; i++)
   {
       pBuffer->data[i] = i + 1;
   }



   //======================================================================================================================================
   // Start CAN
   CO_CANsetNormalMode(pCO->CANmodule);
   Reset = CO_RESET_NOT;
   printf("CANopenNode running...\n");
   //tmrTask_thread();


   int32_t s32Input = 13;

   while (s32Input == 13) 
   {
       printf("Do you want to send message? Press enter: ");
       s32Input = _getch();
       CO_CANsend(pCO->CANmodule, pBuffer);
       // Data bytes must be written in buffer before function call.
   }
   

   // Loop for normal program execution
   //while (Reset == CO_RESET_NOT)
   //{
   //   uint32_t u32TimeDifference = 500;
   //   // CANopen Process
   //   Reset = CO_process(pCO, false, u32TimeDifference, NULL);
   //}

   // Program exit
   // Delete objects from Memory
   CO_CANsetConfigurationMode((void *)&pCANptr);
   CO_delete(pCO);

   return 0;
}

/* timer thread executes in constant intervals ********************************/
void tmrTask_thread(void)
{

    for (;;) 
    {
        CO_LOCK_OD(CO->CANmodule);
        if (!pCO->nodeIdUnconfigured && pCO->CANmodule->CANnormal) {
            bool_t syncWas = false;
            /* get time difference since last function call */
            uint32_t timeDifference_us = 1000;

#if (CO_CONFIG_SYNC) & CO_CONFIG_SYNC_ENABLE
            syncWas = CO_process_SYNC(pCO, timeDifference_us, NULL);
#endif
#if (CO_CONFIG_PDO) & CO_CONFIG_RPDO_ENABLE
            CO_process_RPDO(CO, syncWas, timeDifference_us, NULL);
#endif
#if (CO_CONFIG_PDO) & CO_CONFIG_TPDO_ENABLE
            CO_process_TPDO(CO, syncWas, timeDifference_us, NULL);
#endif

            /* Further I/O or nonblocking application code may go here. */
        }
        CO_UNLOCK_OD(CO->CANmodule);
    }
}