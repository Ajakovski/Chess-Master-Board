idf_component_register(
    SRCS
        "main.c"
        "hal/sensors.c"
        "hal/leds.c"
        "hal/display.c"
        "hal/buttons.c"
        "hal/battery.c"
        "chess/board.c"
        "chess/rules.c"
        "engine/uci.c"
        "game/game.c"
        "mcu_max/mcu-max.c"

    INCLUDE_DIRS
        "."
        "hal"
        "chess"
        "engine"
        "game"
        "mcu_max"

    REQUIRES
        driver
        esp_adc
        esp_timer
        freertos
        log
        soc
        hal
        esp_hw_support
        esp_system
        esp_rom
        nvs_flash
        newlib
)

target_compile_options(${COMPONENT_LIB} PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-std=gnu17 -O2>
)