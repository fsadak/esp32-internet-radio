#pragma once
#include <cstddef>
#include <cstdint>
using esp_ota_handle_t=uint32_t;
using esp_err_t=int;
constexpr int ESP_OK=0;
constexpr size_t OTA_WITH_SEQUENTIAL_WRITES=0xfffffffe;
struct esp_partition_t{size_t size;};
const esp_partition_t* esp_ota_get_next_update_partition(const void*);
const esp_partition_t* esp_ota_get_running_partition();
esp_err_t esp_ota_begin(const esp_partition_t*,size_t,esp_ota_handle_t*);
esp_err_t esp_ota_write(esp_ota_handle_t,const void*,size_t);
esp_err_t esp_ota_end(esp_ota_handle_t);
esp_err_t esp_ota_abort(esp_ota_handle_t);
esp_err_t esp_ota_set_boot_partition(const esp_partition_t*);
using esp_ota_img_states_t=int;
constexpr int ESP_OTA_IMG_PENDING_VERIFY=1;
esp_err_t esp_ota_get_state_partition(const esp_partition_t*,esp_ota_img_states_t*);
esp_err_t esp_ota_mark_app_invalid_rollback_and_reboot();
esp_err_t esp_ota_mark_app_valid_cancel_rollback();
