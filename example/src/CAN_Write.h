#pragma once
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <thread>
#include "targetver.h"
#include "PCANBasic.h"
#include "Struct.h"

namespace CAN_WriteMsg
{
   class Write
   {
   public:
      // Methods
      Write();    // Constructor
      ~Write();    // Destructor
      TPCANStatus WriteMessage(struct_s_Message* sMessage);

   private:
      // Attributes
      TPCANHandle PCANHandle_;
      TPCANBaudrate Baudrate_;
   };
}