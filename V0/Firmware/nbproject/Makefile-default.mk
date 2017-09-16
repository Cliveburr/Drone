#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=--mafrlcsj
else
COMPARISON_BUILD=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=BLDC_Esc/BLDC_Esc.c BLDC_Esc/Timer_Events.c BLDC_Esc/BLDC_Esc_WorkHighLow.c BLDC_Esc/BLDC_Esc_PWM.c USB_Config/usb_descriptors.c USB_Config/usb_app.c USB_Framework/usb_device.c USB_Framework/usb_device_hid.c main.c system.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/BLDC_Esc/BLDC_Esc.p1 ${OBJECTDIR}/BLDC_Esc/Timer_Events.p1 ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_WorkHighLow.p1 ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_PWM.p1 ${OBJECTDIR}/USB_Config/usb_descriptors.p1 ${OBJECTDIR}/USB_Config/usb_app.p1 ${OBJECTDIR}/USB_Framework/usb_device.p1 ${OBJECTDIR}/USB_Framework/usb_device_hid.p1 ${OBJECTDIR}/main.p1 ${OBJECTDIR}/system.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/BLDC_Esc/BLDC_Esc.p1.d ${OBJECTDIR}/BLDC_Esc/Timer_Events.p1.d ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_WorkHighLow.p1.d ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_PWM.p1.d ${OBJECTDIR}/USB_Config/usb_descriptors.p1.d ${OBJECTDIR}/USB_Config/usb_app.p1.d ${OBJECTDIR}/USB_Framework/usb_device.p1.d ${OBJECTDIR}/USB_Framework/usb_device_hid.p1.d ${OBJECTDIR}/main.p1.d ${OBJECTDIR}/system.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/BLDC_Esc/BLDC_Esc.p1 ${OBJECTDIR}/BLDC_Esc/Timer_Events.p1 ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_WorkHighLow.p1 ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_PWM.p1 ${OBJECTDIR}/USB_Config/usb_descriptors.p1 ${OBJECTDIR}/USB_Config/usb_app.p1 ${OBJECTDIR}/USB_Framework/usb_device.p1 ${OBJECTDIR}/USB_Framework/usb_device_hid.p1 ${OBJECTDIR}/main.p1 ${OBJECTDIR}/system.p1

# Source Files
SOURCEFILES=BLDC_Esc/BLDC_Esc.c BLDC_Esc/Timer_Events.c BLDC_Esc/BLDC_Esc_WorkHighLow.c BLDC_Esc/BLDC_Esc_PWM.c USB_Config/usb_descriptors.c USB_Config/usb_app.c USB_Framework/usb_device.c USB_Framework/usb_device_hid.c main.c system.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/Firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F4550
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/BLDC_Esc/BLDC_Esc.p1: BLDC_Esc/BLDC_Esc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/BLDC_Esc" 
	@${RM} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc.p1.d 
	@${RM} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/BLDC_Esc/BLDC_Esc.p1  BLDC_Esc/BLDC_Esc.c 
	@-${MV} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc.d ${OBJECTDIR}/BLDC_Esc/BLDC_Esc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/BLDC_Esc/Timer_Events.p1: BLDC_Esc/Timer_Events.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/BLDC_Esc" 
	@${RM} ${OBJECTDIR}/BLDC_Esc/Timer_Events.p1.d 
	@${RM} ${OBJECTDIR}/BLDC_Esc/Timer_Events.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/BLDC_Esc/Timer_Events.p1  BLDC_Esc/Timer_Events.c 
	@-${MV} ${OBJECTDIR}/BLDC_Esc/Timer_Events.d ${OBJECTDIR}/BLDC_Esc/Timer_Events.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/BLDC_Esc/Timer_Events.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/BLDC_Esc/BLDC_Esc_WorkHighLow.p1: BLDC_Esc/BLDC_Esc_WorkHighLow.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/BLDC_Esc" 
	@${RM} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_WorkHighLow.p1.d 
	@${RM} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_WorkHighLow.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/BLDC_Esc/BLDC_Esc_WorkHighLow.p1  BLDC_Esc/BLDC_Esc_WorkHighLow.c 
	@-${MV} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_WorkHighLow.d ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_WorkHighLow.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_WorkHighLow.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/BLDC_Esc/BLDC_Esc_PWM.p1: BLDC_Esc/BLDC_Esc_PWM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/BLDC_Esc" 
	@${RM} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_PWM.p1.d 
	@${RM} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_PWM.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/BLDC_Esc/BLDC_Esc_PWM.p1  BLDC_Esc/BLDC_Esc_PWM.c 
	@-${MV} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_PWM.d ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_PWM.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_PWM.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/USB_Config/usb_descriptors.p1: USB_Config/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/USB_Config" 
	@${RM} ${OBJECTDIR}/USB_Config/usb_descriptors.p1.d 
	@${RM} ${OBJECTDIR}/USB_Config/usb_descriptors.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/USB_Config/usb_descriptors.p1  USB_Config/usb_descriptors.c 
	@-${MV} ${OBJECTDIR}/USB_Config/usb_descriptors.d ${OBJECTDIR}/USB_Config/usb_descriptors.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/USB_Config/usb_descriptors.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/USB_Config/usb_app.p1: USB_Config/usb_app.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/USB_Config" 
	@${RM} ${OBJECTDIR}/USB_Config/usb_app.p1.d 
	@${RM} ${OBJECTDIR}/USB_Config/usb_app.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/USB_Config/usb_app.p1  USB_Config/usb_app.c 
	@-${MV} ${OBJECTDIR}/USB_Config/usb_app.d ${OBJECTDIR}/USB_Config/usb_app.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/USB_Config/usb_app.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/USB_Framework/usb_device.p1: USB_Framework/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/USB_Framework" 
	@${RM} ${OBJECTDIR}/USB_Framework/usb_device.p1.d 
	@${RM} ${OBJECTDIR}/USB_Framework/usb_device.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/USB_Framework/usb_device.p1  USB_Framework/usb_device.c 
	@-${MV} ${OBJECTDIR}/USB_Framework/usb_device.d ${OBJECTDIR}/USB_Framework/usb_device.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/USB_Framework/usb_device.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/USB_Framework/usb_device_hid.p1: USB_Framework/usb_device_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/USB_Framework" 
	@${RM} ${OBJECTDIR}/USB_Framework/usb_device_hid.p1.d 
	@${RM} ${OBJECTDIR}/USB_Framework/usb_device_hid.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/USB_Framework/usb_device_hid.p1  USB_Framework/usb_device_hid.c 
	@-${MV} ${OBJECTDIR}/USB_Framework/usb_device_hid.d ${OBJECTDIR}/USB_Framework/usb_device_hid.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/USB_Framework/usb_device_hid.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/main.p1  main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/system.p1: system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system.p1.d 
	@${RM} ${OBJECTDIR}/system.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  -D__DEBUG=1 --debugger=pickit3  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/system.p1  system.c 
	@-${MV} ${OBJECTDIR}/system.d ${OBJECTDIR}/system.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/system.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/BLDC_Esc/BLDC_Esc.p1: BLDC_Esc/BLDC_Esc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/BLDC_Esc" 
	@${RM} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc.p1.d 
	@${RM} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/BLDC_Esc/BLDC_Esc.p1  BLDC_Esc/BLDC_Esc.c 
	@-${MV} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc.d ${OBJECTDIR}/BLDC_Esc/BLDC_Esc.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/BLDC_Esc/Timer_Events.p1: BLDC_Esc/Timer_Events.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/BLDC_Esc" 
	@${RM} ${OBJECTDIR}/BLDC_Esc/Timer_Events.p1.d 
	@${RM} ${OBJECTDIR}/BLDC_Esc/Timer_Events.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/BLDC_Esc/Timer_Events.p1  BLDC_Esc/Timer_Events.c 
	@-${MV} ${OBJECTDIR}/BLDC_Esc/Timer_Events.d ${OBJECTDIR}/BLDC_Esc/Timer_Events.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/BLDC_Esc/Timer_Events.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/BLDC_Esc/BLDC_Esc_WorkHighLow.p1: BLDC_Esc/BLDC_Esc_WorkHighLow.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/BLDC_Esc" 
	@${RM} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_WorkHighLow.p1.d 
	@${RM} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_WorkHighLow.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/BLDC_Esc/BLDC_Esc_WorkHighLow.p1  BLDC_Esc/BLDC_Esc_WorkHighLow.c 
	@-${MV} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_WorkHighLow.d ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_WorkHighLow.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_WorkHighLow.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/BLDC_Esc/BLDC_Esc_PWM.p1: BLDC_Esc/BLDC_Esc_PWM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/BLDC_Esc" 
	@${RM} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_PWM.p1.d 
	@${RM} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_PWM.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/BLDC_Esc/BLDC_Esc_PWM.p1  BLDC_Esc/BLDC_Esc_PWM.c 
	@-${MV} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_PWM.d ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_PWM.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/BLDC_Esc/BLDC_Esc_PWM.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/USB_Config/usb_descriptors.p1: USB_Config/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/USB_Config" 
	@${RM} ${OBJECTDIR}/USB_Config/usb_descriptors.p1.d 
	@${RM} ${OBJECTDIR}/USB_Config/usb_descriptors.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/USB_Config/usb_descriptors.p1  USB_Config/usb_descriptors.c 
	@-${MV} ${OBJECTDIR}/USB_Config/usb_descriptors.d ${OBJECTDIR}/USB_Config/usb_descriptors.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/USB_Config/usb_descriptors.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/USB_Config/usb_app.p1: USB_Config/usb_app.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/USB_Config" 
	@${RM} ${OBJECTDIR}/USB_Config/usb_app.p1.d 
	@${RM} ${OBJECTDIR}/USB_Config/usb_app.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/USB_Config/usb_app.p1  USB_Config/usb_app.c 
	@-${MV} ${OBJECTDIR}/USB_Config/usb_app.d ${OBJECTDIR}/USB_Config/usb_app.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/USB_Config/usb_app.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/USB_Framework/usb_device.p1: USB_Framework/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/USB_Framework" 
	@${RM} ${OBJECTDIR}/USB_Framework/usb_device.p1.d 
	@${RM} ${OBJECTDIR}/USB_Framework/usb_device.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/USB_Framework/usb_device.p1  USB_Framework/usb_device.c 
	@-${MV} ${OBJECTDIR}/USB_Framework/usb_device.d ${OBJECTDIR}/USB_Framework/usb_device.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/USB_Framework/usb_device.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/USB_Framework/usb_device_hid.p1: USB_Framework/usb_device_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/USB_Framework" 
	@${RM} ${OBJECTDIR}/USB_Framework/usb_device_hid.p1.d 
	@${RM} ${OBJECTDIR}/USB_Framework/usb_device_hid.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/USB_Framework/usb_device_hid.p1  USB_Framework/usb_device_hid.c 
	@-${MV} ${OBJECTDIR}/USB_Framework/usb_device_hid.d ${OBJECTDIR}/USB_Framework/usb_device_hid.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/USB_Framework/usb_device_hid.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/main.p1: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.p1.d 
	@${RM} ${OBJECTDIR}/main.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/main.p1  main.c 
	@-${MV} ${OBJECTDIR}/main.d ${OBJECTDIR}/main.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/main.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/system.p1: system.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system.p1.d 
	@${RM} ${OBJECTDIR}/system.p1 
	${MP_CC} --pass1 $(MP_EXTRA_CC_PRE) --chip=$(MP_PROCESSOR_OPTION) -Q -G  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib $(COMPARISON_BUILD)  --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"    -o${OBJECTDIR}/system.p1  system.c 
	@-${MV} ${OBJECTDIR}/system.d ${OBJECTDIR}/system.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/system.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/Firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/Firmware.${IMAGE_TYPE}.map  -D__DEBUG=1 --debugger=pickit3  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     --rom=default,-7dc0-7fff --ram=default,-3f4-3ff,-f9c-f9c,-fd4-fd4,-fdb-fdf,-fe3-fe7,-feb-fef,-ffd-fff  $(COMPARISON_BUILD) --memorysummary dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -odist/${CND_CONF}/${IMAGE_TYPE}/Firmware.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/Firmware.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/Firmware.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) --chip=$(MP_PROCESSOR_OPTION) -G -mdist/${CND_CONF}/${IMAGE_TYPE}/Firmware.${IMAGE_TYPE}.map  --double=32 --float=32 --emi=wordwrite --opt=default,+asm,+asmfile,-speed,+space,-debug --addrqual=ignore --mode=pro -P -N255 -I"USB_Framework" -I"./" -I"USB_Config" -I"BLDC_Esc" --warn=-3 --asmlist -DXPRJ_default=$(CND_CONF)  --summary=default,-psect,-class,+mem,-hex,-file --output=default,-inhx032 --runtime=default,+clear,+init,-keep,-no_startup,-download,+config,+clib,-plib --output=-mcof,+elf:multilocs --stack=compiled:auto:auto:auto "--errformat=%f:%l: error: (%n) %s" "--warnformat=%f:%l: warning: (%n) %s" "--msgformat=%f:%l: advisory: (%n) %s"     $(COMPARISON_BUILD) --memorysummary dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -odist/${CND_CONF}/${IMAGE_TYPE}/Firmware.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
