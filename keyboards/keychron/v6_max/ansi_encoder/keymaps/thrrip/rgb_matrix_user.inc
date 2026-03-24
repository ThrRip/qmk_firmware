RGB_MATRIX_EFFECT(thrrip_1)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

#    define RGB_MATRIX_thrrip_1_IDLE_BAR_LED_MIN 20
#    define RGB_MATRIX_thrrip_1_IDLE_BAR_LED_MAX 33
#    define RGB_MATRIX_thrrip_1_IDLE_BAR_LED_MIN_PTX 0
#    define RGB_MATRIX_thrrip_1_IDLE_BAR_LED_MAX_PTX 141
#    define RGB_MATRIX_thrrip_1_IDLE_BAR_ACCENT_PTX 52
// Each cycle is a sequence of run -> rest -> run (backwards) -> rest. Duration
// of each phase, in milliseconds, should not exceed the range of uint16.
// Durations defined here scales based on "rgb_matrix_get_speed()", where the
// defined values directly reflect when speed is 0.
#    define RGB_MATRIX_thrrip_1_IDLE_BAR_DURA_RUN 20000
#    define RGB_MATRIX_thrrip_1_IDLE_BAR_DURA_REST 3000

#    define RGB_MATRIX_thrrip_1_LAYER_LED 0

static bool thrrip_1(effect_params_t *params) {
    enum ib_phase {
        IB_RUN_1,
        IB_REST_1,
        IB_RUN_2,
        IB_REST_2,
    };

    enum layers {
        MAC_BASE,
        MAC_FN,
        WIN_BASE,
        WIN_FN,
    };

    static uint8_t       speed;
    static uint16_t      ib_width;
    static uint16_t      ib_duration_run;
    static uint16_t      ib_duration_rest;
    static uint16_t      ib_time_phase_start;
    static enum ib_phase ib_phase;

    if (params->init || speed != rgb_matrix_get_speed()) {
        speed = rgb_matrix_get_speed();
        // clang-format off
        ib_width =
            RGB_MATRIX_thrrip_1_IDLE_BAR_LED_MAX_PTX - RGB_MATRIX_thrrip_1_IDLE_BAR_LED_MIN_PTX +
            RGB_MATRIX_thrrip_1_IDLE_BAR_ACCENT_PTX * 2; // Runoff areas
        // clang-format on
        ib_duration_run     = scale16by8(RGB_MATRIX_thrrip_1_IDLE_BAR_DURA_RUN, 255 - speed);
        ib_duration_rest    = scale16by8(RGB_MATRIX_thrrip_1_IDLE_BAR_DURA_REST, qadd8(255 - speed, 10));
        ib_time_phase_start = timer_read();
        ib_phase            = IB_RUN_1;
    }

    uint16_t ib_time_phase_elapsed = timer_elapsed(ib_time_phase_start);
    if ((ib_phase == IB_RUN_1 || ib_phase == IB_RUN_2) && ib_time_phase_elapsed > ib_duration_run) {
        ib_time_phase_start   = timer_read();
        ib_time_phase_elapsed = 0;
        ib_phase              = ib_phase == IB_RUN_1 ? IB_REST_1 : IB_REST_2;
    } else if ((ib_phase == IB_REST_1 || ib_phase == IB_REST_2) && ib_time_phase_elapsed > ib_duration_rest) {
        ib_time_phase_start   = timer_read();
        ib_time_phase_elapsed = 0;
        ib_phase              = ib_phase == IB_REST_1 ? IB_RUN_2 : IB_RUN_1;
    }

    RGB_MATRIX_USE_LIMITS(led_min, led_max);
    for (uint8_t i = led_min; i < led_max; i++) {
        if (RGB_MATRIX_thrrip_1_IDLE_BAR_LED_MIN <= i && i <= RGB_MATRIX_thrrip_1_IDLE_BAR_LED_MAX) {
            hsv_t hsv = rgb_matrix_get_hsv();
            if (ib_phase == IB_RUN_1) {
                // Saturation to reduce per point * distance to current center of idle bar
                // clang-format off
                uint16_t s_reduce = hsv.s / RGB_MATRIX_thrrip_1_IDLE_BAR_ACCENT_PTX * abs(
                    // Physical location + left runoff area padding
                    g_led_config.point[i].x + RGB_MATRIX_thrrip_1_IDLE_BAR_ACCENT_PTX -
                    ib_width * ib_time_phase_elapsed / ib_duration_run
                );
                // clang-format on
                hsv.s = s_reduce >= hsv.s ? 0 : hsv.s - s_reduce;
            } else if (ib_phase == IB_RUN_2) {
                // clang-format off
                uint16_t s_reduce = hsv.s / RGB_MATRIX_thrrip_1_IDLE_BAR_ACCENT_PTX * abs(
                    g_led_config.point[i].x + RGB_MATRIX_thrrip_1_IDLE_BAR_ACCENT_PTX -
                    ib_width * (ib_duration_run - ib_time_phase_elapsed) / ib_duration_run
                );
                // clang-format on
                hsv.s = s_reduce >= hsv.s ? 0 : hsv.s - s_reduce;
            } else {
                hsv.s = 0;
            }
            rgb_t rgb = hsv_to_rgb(hsv);
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        } else if (i == RGB_MATRIX_thrrip_1_LAYER_LED) {
            hsv_t hsv = rgb_matrix_get_hsv();
            if (layer_state_is(MAC_BASE)) {
                hsv.s = 0;
            }
            rgb_t rgb = hsv_to_rgb(hsv);
            rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
        } else {
            rgb_matrix_set_color(i, RGB_BLACK);
        }
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
