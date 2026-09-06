# Set any rules.mk overrides for your specific keymap here.
# See rules at https://docs.qmk.fm/#/config_options?id=the-rulesmk-file
CAPS_WORD_ENABLE = yes
COMBO_ENABLE = yes
COMMAND_ENABLE = no
CONSOLE_ENABLE = no
DYNAMIC_MACRO_ENABLE = yes
DYNAMIC_TAPPING_TERM_ENABLE = no
KEY_OVERRIDE_ENABLE = no
LTO_ENABLE = yes
ORYX_ENABLE = yes
PROGRAMMABLE_BUTTON_ENABLE = no
RGB_MATRIX_ENABLE = no
UNICODE_ENABLE = no
WEBUSB_ENABLE = no

# Third-party (vendored) modules.
SRC += third_party/casemodes.c
SRC += third_party/custom_shift_keys.c

# Own keymap modules.
SRC += custom/casemodes_config.c
SRC += custom/custom_caps_lock.c
SRC += custom/debug_helper.c
SRC += custom/symbol_layer.c
SRC += custom/layer_rgb.c
SRC += custom/tap_hold.c
SRC += custom/turkish.c
SRC += custom/led_indicators.c

# Combos define key_combos[], which QMK's keymap_introspection.c reads via
# ARRAY_SIZE(). That macro needs the array visible in the introspection
# translation unit, so the file is pulled in there (alongside keymap.c) rather
# than compiled separately via SRC.
INTROSPECTION_KEYMAP_C = custom/combos.c

# Disable the following to save space
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
MAGIC_ENABLE = no
MUSIC_ENABLE = no
