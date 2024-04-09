#pragma once
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <thread>
#include "targetver.h"
#include "PCANBasic.h"

namespace CAN_WriteMsg
{
   class Write
   {
   public:
      // Methods
      Write();    // Constructor
      ~Write();    // Destructor
      TPCANStatus WriteMessage(void);
      void WriteCyclicMessage(void);

   private:
      // Attributes
      TPCANHandle PCANHandle_;
      TPCANBaudrate Baudrate_;

      // Methods
      void SetCanMessage(TPCANMsg* msgCanMessage);
      void GetTestMessage(TPCANMsg* msgCanMessage);
   };
}