#include "CAN_Write.h"

#ifdef __cplusplus
extern "C" {
#endif

   void Send(void)
   {
      
      CAN_WriteMsg::Write WriteObj{};
       WriteObj.WriteMessage();
   }


}