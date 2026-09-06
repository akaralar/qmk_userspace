# Set any rules.mk overrides for your specific keymap here.
# See rules at https://docs.qmk.fm/#/config_options?id=the-rulesmk-file
CAPS_WORD_ENABLE = yes
COMBO_ENABLE = yes
COMMAND_ENABLE = no
CONSOLE_ENABLE = yes
DYNAMIC_MACRO_ENABLE = yes
DYNAMIC_TAPPING_TERM_ENABLE = no
KEY_OVERRIDE_ENABLE = no
LTO_ENABLE = yes
ORYX_ENABLE = yes
PROGRAMMABLE_BUTTON_ENABLE = no
RGB_MATRIX_ENABLE = no
UNICODE_ENABLE = no
WEBUSB_ENABLE = no

SRC += features/casemodes.c
SRC += features/custom_caps_lock.c
SRC += features/custom_shift_keys.c
SRC += features/debug_helper.c
SRC += features/symbol_layer.c
SRC += features/layer_rgb.c

# Disable the following to save space
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
MAGIC_ENABLE = no
MUSIC_ENABLE = no
