#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := hexo-clock

EXTRA_COMPONENT_DIRS:=../ESP32_led_strip/components
	EXCLUDE_COMPONENTS:= strip
export EXTRA_COMPONENT_DIRS


include $(IDF_PATH)/make/project.mk

