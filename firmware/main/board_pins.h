/* board_pins.h — the boards one image runs on, told apart at boot over I2C
 * (display_port_init; SDA / SCL, the QSPI data lines and LCD_CS are shared):
 *
 * Waveshare ESP32-S3-Touch-AMOLED-1.8 (V1: SH8601 + FT3168; V2: CO5300 +
 * CST816). Sources: Waveshare esp-idf examples and the official Arduino
 * variant. VERIFY I2C SDA/SCL on the bench: Waveshare's own code says
 * SDA=15/SCL=14, the Arduino variant says the reverse.
 *
 * Waveshare ESP32-S3-Touch-AMOLED-1.75C (2026-10-01; ROUND 466x466 CO5300 +
 * CST9217). Sources: resources/ESP32-S3-Touch-AMOLED-1.75C/ (the schematic's
 * GPIO table) and Waveshare's BSP (waveshare/esp32_s3_touch_amoled_1_75c).
 * No IO expander - the resets are GPIOs - no RTC chip, no SD card; an ES7210
 * microphone ADC the 1.8 does not have (its I2C address is how the board is
 * recognized). PMIC, IMU, codec, amp and every audio pin: as on the 1.8.
 *
 * Waveshare ESP32-S3-Touch-AMOLED-2.06, the WATCH (2026-10-02; 410x502
 * CO5300 + FT3168, the 1.8's panel family a size up; worn, so its own build
 * is a PORTRAIT 410x502 tank, the panel unturned). Sources: resources/ESP32-S3-Touch-AMOLED-2.06-Watch/ (schematic,
 * wiki page) and Waveshare's BSP (waveshare/esp32_s3_touch_amoled_2_06).
 * No IO expander: the resets are GPIOs 8 and 9 - the pins the 1.8 plays its
 * I2S bit clock and data on, so the AUDIO PINS differ here (audio_port) and
 * the 1.8's would hold the panel in reset. An ES7210 like the 1.75C's, and an
 * RTC chip unlike it: no expander + ES7210 + RTC = the watch. DSI_PWR_EN is
 * not a GPIO: it is pulled up to ALDO2, so that rail IS the panel's power
 * switch. PWR key sense on GPIO 10 (SYS_OUT), an SD slot (1/2/3/17) and a
 * vibration motor (GPIO 18, fed from ALDO3) the tank does not use. */
#ifndef BOARD_PINS_H
#define BOARD_PINS_H
#include "sdkconfig.h"

#if CONFIG_POCKET_TANK_BOARD_CYD_320X240
/* The 2.8" ESP32-S3 CYD, ES3C28P (lcdwiki.com, ES3C28P_ES2N28P_Specification_V1.0
 * section 4.2). A stock board, no modifications: everything below is as the
 * vendor wires it. The LCD's reset is CHIP_PU - it resets with the chip, so
 * there is no reset pin to drive. */
#define PIN_LCD_CS        10
#define PIN_LCD_DC        46       /* high = data, low = command */
#define PIN_LCD_SCLK      12
#define PIN_LCD_MOSI      11
#define PIN_LCD_MISO      13
#define PIN_LCD_BL        45       /* high = backlight on; PWM for brightness */
#define PIN_I2C_SDA       16       /* shared: touch, audio codec, the I2C socket */
#define PIN_I2C_SCL       15
#define PIN_TP_RST        18       /* low = reset */
#define PIN_TP_INT        17       /* low while touched (the port polls instead) */
#define I2C_ADDR_FT6336   0x38
#define PANEL_W           240      /* native portrait; the panel scans landscape (MADCTL) */
#define PANEL_H           320
/* audio: the ES8311 (I2C 0x18) on I2S, and the power amplifier's enable */
#define PIN_I2S_MCLK      4
#define PIN_I2S_BCLK      5
#define PIN_I2S_WS        7
#define PIN_I2S_DOUT      8        /* ESP -> codec DSDIN; GPIO6 is the microphone's way back, unused */
#define PIN_AMP_EN        1
#define AMP_EN_ON         0        /* the spec: "low level enable" */
#else
#define PIN_LCD_CS        12
#define PIN_LCD_PCLK      11
#define PIN_LCD_DATA0     4
#define PIN_LCD_DATA1     5
#define PIN_LCD_DATA2     6
#define PIN_LCD_DATA3     7
#define PIN_I2C_SDA       15
#define PIN_I2C_SCL       14
#define PIN_TP_INT        21
#define I2C_ADDR_EXPANDER 0x20     /* TCA9554: bit0 LCD_RST, bit1 DSI_PWR_EN, bit2 TOUCH_RST, bit7 SD_CS */
#define PANEL_W           368      /* native portrait */
#define PANEL_H           448
#define V2_PANEL_X_GAP    0x10
#endif  /* board */
/* every build: the touch addresses the shared touch code (touch_port_ft3168.c) names */
#define I2C_ADDR_FT3168   0x38     /* V1 touch, and the watch's */
#define I2C_ADDR_CST816   0x15     /* V2 touch (probe => V2 board) */

/* ---- the 1.75C ---- */
#define R_PIN_LCD_PCLK    38
#define R_PIN_LCD_RST     1
#define R_PIN_LCD_TE      13       /* unused */
#define R_PIN_TP_RST      2
#define R_PIN_TP_INT      11
#define R_PIN_IMU_INT1    21       /* unused */
#define R_PIN_PWR_SENSE   3        /* SYS_OUT: high while the PWR key is down (unused: the PMIC is asked, as on the 1.8) */
#define I2C_ADDR_ES7210   0x40     /* the microphone ADC (probe, with no expander => the 1.75C) */
#define I2C_ADDR_CST9217  0x5A
#define R_PANEL           466      /* round: 466 across, the corners of the square are not there */
#define R_PANEL_X_GAP     6

/* ---- the 2.06 watch ---- */
#define W_PIN_LCD_RST     8
#define W_PIN_TP_RST      9
#define W_PIN_TP_INT      38       /* unused: polled, like the others */
#define W_PIN_LCD_TE      13       /* unused */
#define W_PIN_PWR_SENSE   10       /* SYS_OUT: high while the PWR key is down */
#define W_PIN_I2S_BCLK    41
#define W_PIN_I2S_DOUT    40       /* ESP -> codec DSDIN */
#define W_PIN_MOTOR       18       /* unused (pulled off on the board) */
#define I2C_ADDR_RTC      0x51     /* PCF85063: the 1.8 and the watch have one, the 1.75C does not */
#define W_PANEL_W         410      /* native portrait */
#define W_PANEL_H         502
#define W_PANEL_X_GAP     0x16
#endif
