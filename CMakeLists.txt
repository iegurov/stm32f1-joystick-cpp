cmake_minimum_required(VERSION 4.0.2)

project(Flash)
enable_language(C CXX ASM)

set(DEVICE "STM32F103xB")

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/arm-toolchain.cmake)
set(LD_SCRIPT ${CMAKE_SOURCE_DIR}/CMSIS/Device/gcc/STM32F103.ld)

#set(
#    SOURCES 
#    src/*.c
#    CMSIS/Device/system_stm32f1xx.c
#    CMSIS/Device/gcc/startup_stm32f103xb.s
#)

file(GLOB SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.c
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/CMSIS/Device/system_stm32f1xx.c
    ${CMAKE_SOURCE_DIR}/CMSIS/Device/gcc/startup_stm32f103xb.s
)

add_executable(${PROJECT_NAME} ${SOURCES})

#add_executable(
#    ${PROJECT_NAME}
#    ${SOURCES}
#)

target_include_directories(
    ${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/CMSIS/Core
    ${CMAKE_SOURCE_DIR}/CMSIS/Device/Includes
    ${CMAKE_SOURCE_DIR}/Inc
)

target_link_directories(
    ${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/CMSIS/Core
    ${CMAKE_SOURCE_DIR}/CMSIS/Device/Includes
)

target_compile_definitions(
    ${PROJECT_NAME} PRIVATE
    ${DEVICE}
)

target_compile_options(
    ${PROJECT_NAME} PRIVATE
    -mcpu=cortex-m3
    -mthumb
    -msoft-float
    -mfloat-abi=soft
    -Wall 
    -Wextra 
    -Wpedantic
    -ffunction-sections 
    -fdata-sections
    -O0
    -fno-exceptions
)

target_link_options(
    ${PROJECT_NAME} PRIVATE
    -Wl,--gc-sections
    -Wl,-T${LD_SCRIPT}
    -Wl,-Map=Flash.map
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}>
)

add_custom_command(
    OUTPUT ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    DEPENDS ${PROJECT_NAME}.elf
)

add_custom_target(generate_bin ALL
    DEPENDS ${PROJECT_NAME}.bin
)