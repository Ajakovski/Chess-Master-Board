#include "uci.h"
#include "config.h"
#include "board.h"
#include "mcu-max.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/message_buffer.h"
#include "esp_heap_caps.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const char *TAG = "uci";


#define CMD_BUF_BYTES    2048
#define REPLY_BUF_BYTES  4096

static MessageBufferHandle_t s_cmd_buf   = NULL;
static MessageBufferHandle_t s_reply_buf = NULL;
static volatile bool         s_ready     = false;

/*
 ENGINE-SIDE I/O HOOKS  (called from within mcu-max.c)*/

void uci_engine_getline(char *buf, size_t sz)
{
    size_t n;
    do {
        n = xMessageBufferReceive(s_cmd_buf, buf, sz - 1, portMAX_DELAY);
    } while (n == 0);
    buf[n] = '\0';
    /* Strip trailing newline */
    size_t l = strlen(buf);
    if (l > 0 && (buf[l-1] == '\n' || buf[l-1] == '\r')) buf[--l] = '\0';
}

void uci_engine_puts(const char *line)
{
    if (!line) return;
    xMessageBufferSend(s_reply_buf, line, strlen(line), pdMS_TO_TICKS(20));
}

/*
HOST-SIDE HELPERS*/

static void send_cmd(const char *cmd)
{
    if (!s_cmd_buf) return;
    xMessageBufferSend(s_cmd_buf, cmd, strlen(cmd), pdMS_TO_TICKS(100));
    ESP_LOGD(TAG, "→ %s", cmd);
}

static bool recv_reply(char *buf, size_t sz, uint32_t timeout_ms)
{
    size_t n = xMessageBufferReceive(s_reply_buf, buf, sz - 1,
                                     pdMS_TO_TICKS(timeout_ms));
    if (n == 0) return false;
    buf[n] = '\0';
    ESP_LOGD(TAG, "← %s", buf);
    return true;
}

// Engine task

void stockfish_task(void *arg)
{
    (void)arg;
    ESP_LOGI(TAG, "mcu-max task started on core %d", (int)xPortGetCoreID());
    mcumax_uci_loop();           /* Blocks forever */
    vTaskDelete(NULL);
}

// INIT  
esp_err_t uci_init(void)
{
    static StaticMessageBuffer_t cmd_sb, reply_sb;
    static uint8_t cmd_storage  [CMD_BUF_BYTES];
    static uint8_t reply_storage[REPLY_BUF_BYTES];

    s_cmd_buf   = xMessageBufferCreateStatic(CMD_BUF_BYTES,
                                              cmd_storage,   &cmd_sb);
    s_reply_buf = xMessageBufferCreateStatic(REPLY_BUF_BYTES,
                                              reply_storage, &reply_sb);
    if (!s_cmd_buf || !s_reply_buf) {
        ESP_LOGE(TAG, "Message buffer creation failed");
        return ESP_ERR_NO_MEM;
    }

    // Launch mcu-max task on Core 1
    static StackType_t  engine_stack[MCU_MAX_TASK_STACK / sizeof(StackType_t)];
    static StaticTask_t engine_tcb;
    xTaskCreateStaticPinnedToCore(
        stockfish_task,
        "mcu-max",
        MCU_MAX_TASK_STACK / sizeof(StackType_t),
        NULL,
        TASK_PRIO_STOCKFISH,
        engine_stack,
        &engine_tcb,
        CORE_STOCKFISH
    );

    // UCI
    char reply[256];

    send_cmd("uci");
    bool got_uciok = false;
    for (int i = 0; i < 20 && !got_uciok; i++) {
        if (recv_reply(reply, sizeof(reply), 500)) {
            if (strncmp(reply, "uciok", 5) == 0) got_uciok = true;
        }
    }
    if (!got_uciok) {
        ESP_LOGE(TAG, "mcu-max did not respond with uciok");
        return ESP_FAIL;
    }


    char opt[64];
    snprintf(opt, sizeof(opt), "setoption name Hash value %d",
             MCU_MAX_HASH_KB / 1024);
    send_cmd(opt);

    send_cmd("isready");
    bool got_readyok = false;
    for (int i = 0; i < 20 && !got_readyok; i++) {
        if (recv_reply(reply, sizeof(reply), 500)) {
            if (strncmp(reply, "readyok", 7) == 0) got_readyok = true;
        }
    }
    if (!got_readyok) {
        ESP_LOGE(TAG, "mcu-max did not respond with readyok");
        return ESP_FAIL;
    }

    s_ready = true;
    ESP_LOGI(TAG, "mcu-max ready (hash=%d KB, depth=%d)",
             MCU_MAX_HASH_KB, SF_SEARCH_DEPTH);
    return ESP_OK;
}

// API PUBLIC

void uci_send_position(const move_history_t *history)
{
    char cmd[600];
    if (!history || history->count == 0) {
        send_cmd("position startpos");
        return;
    }
    int pos = snprintf(cmd, sizeof(cmd), "position startpos moves");
    for (int i = 0; i < history->count && pos < (int)sizeof(cmd) - 8; i++) {
        pos += snprintf(cmd + pos, sizeof(cmd) - (size_t)pos,
                        " %s", history->entries[i]);
    }
    send_cmd(cmd);
}

void uci_go_depth(int depth)
{
    char cmd[32];
    snprintf(cmd, sizeof(cmd), "go depth %d", depth);
    send_cmd(cmd);
}

void uci_stop(void)
{
    send_cmd("stop");
}

bool uci_ready(void) { return s_ready; }

bool uci_wait_bestmove(move_t *best_move, int *depth, int *score_cp,
                       uint32_t timeout_ms)
{
    if (timeout_ms == 0) timeout_ms = SF_MOVE_TIMEOUT_MS;
    if (depth)    *depth    = 0;
    if (score_cp) *score_cp = 0;
    *best_move = MOVE_NULL;

    char line[SF_REPLY_BUF_SIZE];
    uint32_t deadline = (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS)
                         + timeout_ms;

    while ((uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS) < deadline) {
        uint32_t remaining = deadline -
                             (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
        if (!recv_reply(line, sizeof(line), remaining < 50 ? remaining : 50))
            continue;

        /* "info depth N score cp S pv <move>" */
        if (strncmp(line, "info ", 5) == 0) {
            const char *d = strstr(line, "depth ");
            const char *s = strstr(line, "score cp ");
            if (d && depth)    *depth    = atoi(d + 6);
            if (s && score_cp) *score_cp = atoi(s + 9);
            continue;
        }

        /* "bestmove <move>" */
        if (strncmp(line, "bestmove ", 9) == 0) {
            char token[8] = {0};
            sscanf(line + 9, "%7s", token);
            if (strcmp(token, "(none)") == 0 || strcmp(token, "0000") == 0)
                return false;
            return move_from_uci(token, best_move);
        }
    }

    ESP_LOGW(TAG, "uci_wait_bestmove: timeout after %lu ms", timeout_ms);
    return false;
}