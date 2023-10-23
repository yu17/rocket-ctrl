#ifndef app_loracommc
#define app_loracommc 1.0

#include "resc_display.hpp"
#include "resc_lora.hpp"
#include "plugin_gateway.hpp"
#include "loracomm_protocol.hpp"

#define RX_WAITTIME_MS 60000

struct contact_list_t{
	uint8_t count;
	struct contact_t *contacts;
};

void func_loracomm_listener(void *param);

void func_contact_list_render(struct contact_list_t *contact_list, struct contact_t *contact, uint8_t pos, uint8_t scale);

void func_loracomm_send_announcement();

void func_loracomm_main(struct contact_list_t *contact_list);

struct contact_t* func_contact_list_find(struct contact_list_t *contact_list, uint16_t id);

void func_contact_list_destroy(struct contact_list_t *contact_list);

void* app_loracomm(const void *param);

#endif