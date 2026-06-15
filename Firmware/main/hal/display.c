#include "display.h"
#include "config.h"
#include "driver/i2c.h"
#include "esp_log.h"
#include <string.h>
#include <stdio.h>

static const char *TAG = "display";

// SSD1309
#define SSD_CTRL_CMD  0x00
#define SSD_CTRL_DATA 0x40


static uint8_t s_fb[OLED_WIDTH * OLED_PAGES];  /* 1024 bytes */

/* ============================================================ USED CODE REFERENCES FROM WEB FOR THIS SECTION
 * 5×7 ASCII FONT  (characters 0x20–0x7E)
 * Each entry: 5 column bytes. Bit0 = top row, Bit6 = bottom row.
 * Standard Adafruit/Arduino 5×7 font.
 * ============================================================ */
static const uint8_t font5x7[][5] = {
    {0x00,0x00,0x00,0x00,0x00}, /* 20  */  {0x00,0x00,0x5F,0x00,0x00}, /* 21 ! */
    {0x00,0x07,0x00,0x07,0x00}, /* 22 " */  {0x14,0x7F,0x14,0x7F,0x14}, /* 23 # */
    {0x24,0x2A,0x7F,0x2A,0x12}, /* 24 $ */  {0x23,0x13,0x08,0x64,0x62}, /* 25 % */
    {0x36,0x49,0x55,0x22,0x50}, /* 26 & */  {0x00,0x05,0x03,0x00,0x00}, /* 27 ' */
    {0x00,0x1C,0x22,0x41,0x00}, /* 28 ( */  {0x00,0x41,0x22,0x1C,0x00}, /* 29 ) */
    {0x08,0x2A,0x1C,0x2A,0x08}, /* 2A * */  {0x08,0x08,0x3E,0x08,0x08}, /* 2B + */
    {0x00,0x50,0x30,0x00,0x00}, /* 2C , */  {0x08,0x08,0x08,0x08,0x08}, /* 2D - */
    {0x00,0x60,0x60,0x00,0x00}, /* 2E . */  {0x20,0x10,0x08,0x04,0x02}, /* 2F / */
    {0x3E,0x51,0x49,0x45,0x3E}, /* 30 0 */  {0x00,0x42,0x7F,0x40,0x00}, /* 31 1 */
    {0x42,0x61,0x51,0x49,0x46}, /* 32 2 */  {0x21,0x41,0x45,0x4B,0x31}, /* 33 3 */
    {0x18,0x14,0x12,0x7F,0x10}, /* 34 4 */  {0x27,0x45,0x45,0x45,0x39}, /* 35 5 */
    {0x3C,0x4A,0x49,0x49,0x30}, /* 36 6 */  {0x01,0x71,0x09,0x05,0x03}, /* 37 7 */
    {0x36,0x49,0x49,0x49,0x36}, /* 38 8 */  {0x06,0x49,0x49,0x29,0x1E}, /* 39 9 */
    {0x00,0x36,0x36,0x00,0x00}, /* 3A : */  {0x00,0x56,0x36,0x00,0x00}, /* 3B ; */
    {0x00,0x08,0x14,0x22,0x41}, /* 3C < */  {0x14,0x14,0x14,0x14,0x14}, /* 3D = */
    {0x41,0x22,0x14,0x08,0x00}, /* 3E > */  {0x02,0x01,0x51,0x09,0x06}, /* 3F ? */
    {0x32,0x49,0x79,0x41,0x3E}, /* 40 @ */  {0x7E,0x11,0x11,0x11,0x7E}, /* 41 A */
    {0x7F,0x49,0x49,0x49,0x36}, /* 42 B */  {0x3E,0x41,0x41,0x41,0x22}, /* 43 C */
    {0x7F,0x41,0x41,0x22,0x1C}, /* 44 D */  {0x7F,0x49,0x49,0x49,0x41}, /* 45 E */
    {0x7F,0x09,0x09,0x09,0x01}, /* 46 F */  {0x3E,0x41,0x49,0x49,0x7A}, /* 47 G */
    {0x7F,0x08,0x08,0x08,0x7F}, /* 48 H */  {0x00,0x41,0x7F,0x41,0x00}, /* 49 I */
    {0x20,0x40,0x41,0x3F,0x01}, /* 4A J */  {0x7F,0x08,0x14,0x22,0x41}, /* 4B K */
    {0x7F,0x40,0x40,0x40,0x40}, /* 4C L */  {0x7F,0x02,0x04,0x02,0x7F}, /* 4D M */
    {0x7F,0x04,0x08,0x10,0x7F}, /* 4E N */  {0x3E,0x41,0x41,0x41,0x3E}, /* 4F O */
    {0x7F,0x09,0x09,0x09,0x06}, /* 50 P */  {0x3E,0x41,0x51,0x21,0x5E}, /* 51 Q */
    {0x7F,0x09,0x19,0x29,0x46}, /* 52 R */  {0x46,0x49,0x49,0x49,0x31}, /* 53 S */
    {0x01,0x01,0x7F,0x01,0x01}, /* 54 T */  {0x3F,0x40,0x40,0x40,0x3F}, /* 55 U */
    {0x1F,0x20,0x40,0x20,0x1F}, /* 56 V */  {0x3F,0x40,0x38,0x40,0x3F}, /* 57 W */
    {0x63,0x14,0x08,0x14,0x63}, /* 58 X */  {0x07,0x08,0x70,0x08,0x07}, /* 59 Y */
    {0x61,0x51,0x49,0x45,0x43}, /* 5A Z */  {0x00,0x7F,0x41,0x41,0x00}, /* 5B [ */
    {0x02,0x04,0x08,0x10,0x20}, /* 5C \ */  {0x00,0x41,0x41,0x7F,0x00}, /* 5D ] */
    {0x04,0x02,0x01,0x02,0x04}, /* 5E ^ */  {0x40,0x40,0x40,0x40,0x40}, /* 5F _ */
    {0x00,0x01,0x02,0x04,0x00}, /* 60 ` */  {0x20,0x54,0x54,0x54,0x78}, /* 61 a */
    {0x7F,0x48,0x44,0x44,0x38}, /* 62 b */  {0x38,0x44,0x44,0x44,0x20}, /* 63 c */
    {0x38,0x44,0x44,0x48,0x7F}, /* 64 d */  {0x38,0x54,0x54,0x54,0x18}, /* 65 e */
    {0x08,0x7E,0x09,0x01,0x02}, /* 66 f */  {0x0C,0x52,0x52,0x52,0x3E}, /* 67 g */
    {0x7F,0x08,0x04,0x04,0x78}, /* 68 h */  {0x00,0x44,0x7D,0x40,0x00}, /* 69 i */
    {0x20,0x40,0x44,0x3D,0x00}, /* 6A j */  {0x7F,0x10,0x28,0x44,0x00}, /* 6B k */
    {0x00,0x41,0x7F,0x40,0x00}, /* 6C l */  {0x7C,0x04,0x18,0x04,0x78}, /* 6D m */
    {0x7C,0x08,0x04,0x04,0x78}, /* 6E n */  {0x38,0x44,0x44,0x44,0x38}, /* 6F o */
    {0x7C,0x14,0x14,0x14,0x08}, /* 70 p */  {0x08,0x14,0x14,0x18,0x7C}, /* 71 q */
    {0x7C,0x08,0x04,0x04,0x08}, /* 72 r */  {0x48,0x54,0x54,0x54,0x20}, /* 73 s */
    {0x04,0x3F,0x44,0x40,0x20}, /* 74 t */  {0x3C,0x40,0x40,0x20,0x7C}, /* 75 u */
    {0x1C,0x20,0x40,0x20,0x1C}, /* 76 v */  {0x3C,0x40,0x30,0x40,0x3C}, /* 77 w */
    {0x44,0x28,0x10,0x28,0x44}, /* 78 x */  {0x0C,0x50,0x50,0x50,0x3C}, /* 79 y */
    {0x44,0x64,0x54,0x4C,0x44}, /* 7A z */  {0x00,0x08,0x36,0x41,0x00}, /* 7B { */
    {0x00,0x00,0x7F,0x00,0x00}, /* 7C | */  {0x00,0x41,0x36,0x08,0x00}, /* 7D } */
    {0x08,0x08,0x2A,0x1C,0x08}, /* 7E ~ */
};

// Helper

static esp_err_t i2c_write_cmds(const uint8_t *cmds, size_t n)
{
    i2c_cmd_handle_t h = i2c_cmd_link_create();
    i2c_master_start(h);
    i2c_master_write_byte(h, (OLED_I2C_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(h, SSD_CTRL_CMD, true);
    i2c_master_write(h, (uint8_t*)cmds, n, true);
    i2c_master_stop(h);
    esp_err_t err = i2c_master_cmd_begin(I2C_PORT, h, pdMS_TO_TICKS(50));
    i2c_cmd_link_delete(h);
    return err;
}

static void display_flush(void)
{
    
    static const uint8_t setup[] = {
        0x21, 0x00, 0x7F,   
        0x22, 0x00, 0x07,   
    };
    i2c_write_cmds(setup, sizeof(setup));

    i2c_cmd_handle_t h = i2c_cmd_link_create();
    i2c_master_start(h);
    i2c_master_write_byte(h, (OLED_I2C_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(h, SSD_CTRL_DATA, true);
    i2c_master_write(h, s_fb, sizeof(s_fb), true);
    i2c_master_stop(h);
    i2c_master_cmd_begin(I2C_PORT, h, pdMS_TO_TICKS(200));
    i2c_cmd_link_delete(h);
}

// Framebuffer

static void fb_clear(void) { memset(s_fb, 0, sizeof(s_fb)); }

/* Set/clear a single pixel */
static void fb_pixel(int x, int y, bool on)
{
    if (x < 0 || x >= OLED_WIDTH || y < 0 || y >= OLED_HEIGHT) return;
    int page = y / 8;
    int bit  = y % 8;
    int idx  = page * OLED_WIDTH + x;
    if (on) s_fb[idx] |=  (1 << bit);
    else    s_fb[idx] &= ~(1 << bit);
}

/* Draw 5×7 character at pixel x,y*/
static int fb_char(int x, int y, char c, int scale)
{
    if (c < 0x20 || c > 0x7E) c = '?';
    const uint8_t *glyph = font5x7[c - 0x20];
    for (int col = 0; col < 5; col++) {
        for (int row = 0; row < 7; row++) {
            if (glyph[col] & (1 << row)) {
                for (int sx = 0; sx < scale; sx++)
                    for (int sy = 0; sy < scale; sy++)
                        fb_pixel(x + col * scale + sx, y + row * scale + sy, true);
            }
        }
    }
    return 6 * scale;
}


static int fb_str(int x, int y, const char *s, int scale)
{
    int cx = x;
    while (*s) cx += fb_char(cx, y, *s++, scale);
    return cx - x;
}

// Draw a horizontal line of width pixels
static void fb_hline(int x, int y, int width)
{
    for (int i = 0; i < width; i++) fb_pixel(x + i, y, true);
}

//CLK FORMatter
static void fmt_clock(int32_t ms, char *out)
{
    if (ms < 0) ms = 0;
    int total_s = ms / 1000;
    int m = total_s / 60;
    int s = total_s % 60;
    if (m > 99) m = 99;
    out[0] = '0' + (m / 10);
    out[1] = '0' + (m % 10);
    out[2] = ':';
    out[3] = '0' + (s / 10);
    out[4] = '0' + (s % 10);
    out[5] = '\0';
}

/Init

esp_err_t display_init(void)
{

    i2c_config_t conf = {
        .mode             = I2C_MODE_MASTER,
        .sda_io_num       = PIN_I2C_SDA,
        .scl_io_num       = PIN_I2C_SCL,
        .sda_pullup_en    = GPIO_PULLUP_ENABLE,
        .scl_pullup_en    = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_FREQ_HZ,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_PORT, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_PORT, I2C_MODE_MASTER, 0, 0, 0));

    /* SSD1306 initialisation sequence */
    static const uint8_t init_seq[] = {
        0xAE,       /* Display OFF                                    */
        0x00,       /* Set lower column start = 0                     */
        0x10,       /* Set higher column start = 0                    */
        0x40,       /* Set display start line = 0                     */
        0xB0,       /* Set page start address = 0                     */
        0x81, 0xCF, /* Set contrast (0xCF = high)                     */
        0xA1,       /* Set segment remap: col127→SEG0                 */
        0xA6,       /* Normal display (not inverted)                  */
        0xA8, 0x3F, /* Multiplex ratio = 64 rows                      */
        0xC8,       /* COM scan direction: remapped (top→bottom)      */
        0xD3, 0x00, /* Display offset = 0                             */
        0xD5, 0x80, /* Clock divide ratio / oscillator freq           */
        0xD9, 0xF1, /* Pre-charge period                              */
        0xDA, 0x12, /* COM pins hardware config (alternative, not seq)*/
        0xDB, 0x40, /* VCOMH deselect level                           */
        0x20, 0x00, /* Memory addressing mode = Horizontal            */
        0x8D, 0x14, /* Charge pump = Enable                           */
        0xAF,       /* Display ON                                     */
    };
    esp_err_t err = i2c_write_cmds(init_seq, sizeof(init_seq));
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "SSD1306 init failed: %s", esp_err_to_name(err));
        return err;
    }

    fb_clear();
    display_flush();
    ESP_LOGI(TAG, "SSD1306 ready");
    return ESP_OK;
}

// PI PUBLIC

void display_clear(void)
{
    fb_clear();
    display_flush();
}

void display_update(int32_t  clock_ms_w,
                    int32_t  clock_ms_b,
                    int      active,
                    int      move_number,
                    int      battery_pct,
                    bool     sf_thinking,
                    int      sf_depth,
                    int      sf_score_cp)
{
    fb_clear();

    // ROw 0-7
    if (active == 0) {
        /* Fill white's label area with white pixels (invert) */
        for (int x = 0; x < 62; x++)
            for (int y = 0; y < 8; y++)
                fb_pixel(x, y, true);
        /* Text over white fill (pixels OFF on white background = black text) */
        const char *wlabel = "WHITE";
        int wx = (62 - 5 * 6) / 2;
        int cx = wx;
        for (const char *p = wlabel; *p; p++) {
            const uint8_t *g = font5x7[*p - 0x20];
            for (int col = 0; col < 5; col++)
                for (int row = 0; row < 7; row++)
                    if (g[col] & (1 << row))
                        fb_pixel(cx + col, 1 + row, false);
            cx += 6;
        }
        fb_str(66, 1, "BLACK", 1);
    } else {
        fb_str(2,  1, "WHITE", 1);
        for (int x = 64; x < 128; x++)
            for (int y = 0; y < 8; y++)
                fb_pixel(x, y, true);
        const char *blabel = "BLACK";
        int cx = 66;
        for (const char *p = blabel; *p; p++) {
            const uint8_t *g = font5x7[*p - 0x20];
            for (int col = 0; col < 5; col++)
                for (int row = 0; row < 7; row++)
                    if (g[col] & (1 << row))
                        fb_pixel(cx + col, 1 + row, false);
            cx += 6;
        }
    }

    /*  Row 8-39: Clock digits (scale=3, 3×5-pixel font → ~15×21px)  */
    
    char tw[8], tb[8];
    fmt_clock(clock_ms_w, tw);
    fmt_clock(clock_ms_b, tb);

    /* White clock: left half (0–62) at scale 2 */
    fb_str(1, 10, tw, 2);

    /* Black clock: right half (64–127) at scale 2 */
    fb_str(65, 10, tb, 2);

    
    for (int y = 0; y < 48; y++) fb_pixel(63, y, true);

   //Separator
    fb_hline(0, 40, OLED_WIDTH);

    /*  Row 48-55: Move count + battery*/
    char status[32];
    snprintf(status, sizeof(status), "M:%d  BAT:%d%%", move_number, battery_pct);
    fb_str(1, 49, status, 1);

    /* Row 56-63: Machine status*/
    if (sf_thinking) {
        fb_str(1, 57, "SF: thinking...", 1);
    } else if (sf_depth > 0) {
        char sf_info[32];
       
        float pawns = (float)sf_score_cp / 100.0f;
        snprintf(sf_info, sizeof(sf_info), "SF d%d %+.1fp", sf_depth, pawns);
        fb_str(1, 57, sf_info, 1);
    } else {
        fb_str(1, 57, "SF: ready", 1);
    }

    display_flush();
}

void display_message(const char *line1, const char *line2)
{
    fb_clear();
    if (line1) {
        int w = (int)(strlen(line1) * 6);
        int x = (OLED_WIDTH - w) / 2;
        if (x < 0) x = 0;
        fb_str(x, 20, line1, 1);
    }
    if (line2) {
        int w = (int)(strlen(line2) * 6);
        int x = (OLED_WIDTH - w) / 2;
        if (x < 0) x = 0;
        fb_str(x, 36, line2, 1);
    }
    display_flush();
}
