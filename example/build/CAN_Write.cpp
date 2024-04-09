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

   void Write::SetCanMessage(TPCANMsg* msgCanMessage)
   {
      int32_t s32Input = 0;

      std::cout << "Type in CAN ID (hex): ";
      std::cin >> std::hex >> s32Input;
      msgCanMessage->ID = (DWORD)s32Input;

      std::cout << "Type in Message Length (dec): ";
      std::cin >> std::dec >> s32Input;
      msgCanMessage->LEN = (BYTE)s32Input;

      for (BYTE i = 0; i < msgCanMessage->LEN; i++)
      {
         std::cout << "Type in " << (i + 1) << ". byte of Data (hex): ";
         std::cin >> std::hex >> s32Input;
         msgCanMessage->DATA[i] = (BYTE)s32Input;
      }
      std::cout << std::endl;

      msgCanMessage->MSGTYPE = PCAN_MESSAGE_STANDARD;
   }

   TPCANStatus Write::WriteMessage(void)
   {
      TPCANStatus dwStatus = PCAN_ERROR_OVERRUN;
      TPCANMsg msgCANMessage;
      int32_t s32Input = 0;

      std::cout << "Do you want to load test message? [y] for yes: ";
      s32Input = _getch();
      std::cout << std::endl;

      if (s32Input == 121)
      {
         GetTestMessage(&msgCANMessage);
      }
      else
      {
         SetCanMessage(&msgCANMessage);
      }

      std::cout << "Press enter to send message ";
      s32Input = _getch();

      if (s32Input == 13) // 13 = Enter (Carriage Return)
      {
         if (CAN_Write(PCANHandle_, &msgCANMessage) == PCAN_ERROR_OK)
         {
            std::cout << "Sent message successfully!" << std::endl << std::endl;
            dwStatus = PCAN_ERROR_OK;
         }
      }
      else
      {
         std::cout << "Did not sent message!" << std::endl << std::endl;
         dwStatus = PCAN_ERROR_UNKNOWN;
      }

      return dwStatus;
   }

   void Write::GetTestMessage(TPCANMsg* msgCanMessage)
   {
      msgCanMessage->ID = (DWORD)0x281;
      msgCanMessage->LEN = (BYTE)4;
      msgCanMessage->MSGTYPE = PCAN_MESSAGE_STANDARD;

      msgCanMessage->DATA[0] = (BYTE)0x60;
      msgCanMessage->DATA[1] = (BYTE)0x80;
      msgCanMessage->DATA[2] = (BYTE)0x10;
      msgCanMessage->DATA[3] = (BYTE)0x15;

      std::cout << std::endl << "CAN Message:" << std::endl;
      std::cout << std::hex << "\tCAN ID: " << msgCanMessage->ID << std::endl;
      std::cout << std::dec << "\tCAN Length: " << (int32_t)msgCanMessage->LEN << std::endl;
      
      for (int i = 0; i < msgCanMessage->LEN; i++)
      {
         std::cout << "\t" << (i + 1) << ". byte of Data (hex): " << std::hex << (int32_t)msgCanMessage->DATA[i] << std::dec << std::endl;
      }

      std::cout << std::endl;
   }

   void Write::WriteCyclicMessage(void)
   {
      TPCANMsg msgCANMessage;

      msgCANMessage.ID = (DWORD)0x281;
      msgCANMessage.LEN = (BYTE)4;
      msgCANMessage.MSGTYPE = PCAN_MESSAGE_STANDARD;

      msgCANMessage.DATA[0] = (BYTE)0x2;
      msgCANMessage.DATA[1] = (BYTE)0x3;
      msgCANMessage.DATA[2] = (BYTE)0x08;
      msgCANMessage.DATA[3] = (BYTE)0x18;

      std::cout << std::endl << "CAN Message:" << std::endl;
      std::cout << std::hex << "\tCAN ID: " << msgCANMessage.ID << std::endl;
      std::cout << std::dec << "\tCAN Length: " << (int32_t)msgCANMessage.LEN << std::endl;

      for (int i = 0; i < msgCANMessage.LEN; i++)
      {
         std::cout << "\t" << (i + 1) << ". byte of Data (hex): " << std::hex << (int32_t)msgCANMessage.DATA[i] << std::dec << std::endl;
      }

      if (CAN_Write(PCANHandle_, &msgCANMessage) == PCAN_ERROR_OK)
      {
         std::cout << "Sent message successfully!" << std::endl << std::endl;
      }
   }
}