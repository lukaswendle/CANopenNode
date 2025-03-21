#include "CAN_Write.h"

namespace CAN_WriteMsg
{
   Write::Write()
   {
      PCANHandle_ = PCAN_USBBUS1;
      Baudrate_ = PCAN_BAUD_125K;
      TPCANStatus dwStatus = CAN_Initialize(PCANHandle_, Baudrate_);

      if (dwStatus == PCAN_ERROR_OK)
      {
         std::cout << "Initialized successfully." << std::endl << std::endl;
      }
      else
      {
         std::cout << "Can not initialize." << std::endl << std::endl;
      }
   }

   Write::~Write()
   {
      CAN_Uninitialize(PCAN_NONEBUS);
   }

   TPCANStatus Write::WriteMessage(struct_s_Message* sMessage)
   {
      TPCANStatus dwStatus = PCAN_ERROR_OVERRUN;
      TPCANMsg msgCANMessage;
      int32_t s32Input = 0;

      msgCANMessage.ID = sMessage->cob_id;
      msgCANMessage.LEN = sMessage->len;
      msgCANMessage.MSGTYPE = PCAN_MESSAGE_STANDARD;

      for (int i = 0; i < msgCANMessage.LEN; i++)
      {
          msgCANMessage.DATA[i] = sMessage->data[i];
      }

      if (CAN_Write(PCANHandle_, &msgCANMessage) == PCAN_ERROR_OK)
      {
         std::cout << "Sent message successfully!" << std::endl << std::endl;
         dwStatus = PCAN_ERROR_OK;
      }

      return dwStatus;
   }
}