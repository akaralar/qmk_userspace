# Set any rules.mk overrides for your specific keymap here.
# See rules at https://docs.qmk.fm/#/config_options?id=the-rulesmk-file
LTO_ENABLE = yes
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
WEBUSB_ENABLE = no
ORYX_ENABLE = no
DYNAMIC_MACRO_ENABLE = yes
PROGRAMMABLE_BUTTON_ENABLE = no
SRC = matrix.c
SRC += features/achordion.c
SRC += features/debug_helper.c
CAPS_WORD_ENABLE = yes
RGB_MATRIX_ENABLE = yes
DYNAMIC_TAPPING_TERM_ENABLE = yes
UNICODE_ENABLE = no
KEY_OVERRIDE_ENABLE = no

# Disable the following to save space
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
MAGIC_ENABLE = no
MUSIC_ENABLE = no
