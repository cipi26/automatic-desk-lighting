#include "freertos/idf_additions.h"

#define BTN_SELECT (1 << 0)
#define BTN_NEXT (1 << 1)

void buttons_init(TaskHandle_t notify_task);