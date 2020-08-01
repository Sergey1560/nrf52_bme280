SRC_FILES += \
$(SDK_ROOT)/external/segger_rtt/SEGGER_RTT.c \
$(SDK_ROOT)/external/segger_rtt/SEGGER_RTT_printf.c \
$(SDK_ROOT)/external/segger_rtt/SEGGER_RTT_Syscalls_GCC.c \
$(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_rtt.c \
$(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_serial.c \
$(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_uart.c \
$(SDK_ROOT)/components/libraries/log/src/nrf_log_default_backends.c \
$(SDK_ROOT)/components/libraries/log/src/nrf_log_frontend.c \
$(SDK_ROOT)/components/libraries/log/src/nrf_log_str_formatter.c


INC_FOLDERS +=  $(SDK_ROOT)/external/segger_rtt \
$(SDK_ROOT)/components/libraries/log/src \
$(SDK_ROOT)/components/libraries/delay

