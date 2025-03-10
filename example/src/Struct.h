#pragma once

#include <stdint.h>

struct struct_s_Message 
{
    uint16_t cob_id; /**< message's ID */
    uint8_t rtr; /**< remote transmission request. (0 if not rtr message, 1 if rtr message) */
    uint8_t len; /**< message's length (0 to 8) */
    uint8_t data[8]; /**< message's datas */
};
