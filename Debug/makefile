################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Example10_2.axf

# Tool invocations
Example10_2.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\Giovani\Documents\LPCXpresso_6.1.4_194\workspace\CMSISv1p30_LPC17xx\Debug" -L"C:\Users\Giovani\Documents\LPCXpresso_6.1.4_194\workspace\FreeRTOS_Library\Debug" -Xlinker --gc-sections -Xlinker -Map=Example10_2.map -mcpu=cortex-m3 -mthumb -T "Example10_2_Debug.ld" -o "Example10_2.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) Example10_2.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size Example10_2.axf; # arm-none-eabi-objcopy -O ihex Example10_2.axf Example10_2.hex ;
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
