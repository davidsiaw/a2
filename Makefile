ENV := $(firstword $(shell uname -s | tr A-Z a-z))
BUILD_DIR := build/$(ENV)

WORK_DIR=sample
TARGET_NAME=a2
TARGET=$(BUILD_DIR)/bin/$(TARGET_NAME)

-include $(BUILD_DIR)/Makefile

run: all
	cd $(WORK_DIR) && $(PWD)/$(TARGET)
