#ifndef plugin_gatewayc
#define plugin_gatewayc

#include <math.h>

#include "resc_joystick.hpp"
#include "resc_display.hpp"
#include "common_misc.hpp"
#include "loracomm_protocol.hpp"

int8_t plugin_gateway_request_weather(struct contact_t *device, struct payload_wethrpt_t *wethrpt);

int8_t plugin_gateway_request_location(struct contact_t *device, struct payload_locrpt_t *locrpt);

void plugin_gateway_display_weather(struct contact_t *device);

void plugin_gateway_main(struct contact_t *device);

#endif