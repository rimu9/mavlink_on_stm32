#ifndef __OPEN_TEL_MAVLINK_H//??:????  qq:624668529
#define __OPEN_TEL_MAVLINK_H


//#include "./minimal/minimal/minimal.h"
#include "define.h"
#include "mavlink_avoid_errors.h"
#include "stdint.h"
#include "mavlink_helpers.h"


void mavlink_send_message(mavlink_channel_t chan, enum ap_message id, uint16_t packet_drops);
void update(void);
void handleMessage(mavlink_message_t* msg);

#endif /*__OPENTEL_MAVLINK_H*/   
