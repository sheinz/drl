CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

SET(CMAKE_SYSTEM_NAME Generic)

SET(CMAKE_C_COMPILER avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)

SET(CSTANDARD "-std=gnu99")
SET(CDEBUG "-gstabs")
SET(CWARN "-Wall -Wstrict-prototypes")
SET(CTUNING "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums")
SET(COPT "-Os")
SET(CMCU "-mmcu=attiny13a")
SET(CDEFS "-DF_CPU=9600000UL")


SET(CFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${CINCS} ${COPT} ${CWARN} ${CSTANDARD} ${CEXTRA}")
SET(CXXFLAGS "${CMCU} ${CDEFS} ${CINCS} ${COPT}")

SET(CMAKE_C_FLAGS ${CFLAGS})
SET(CMAKE_CXX_FLAGS ${CXXFLAGS})

ADD_DEFINITIONS(${CFLAGS})

message(status ${CMAKE_C_FLAGS})

find_program(AVR_OBJCOPY avr-objcopy)
find_program(AVR_SIZE_TOOL avr-size)

set(hex_file "drl.hex")
set(elf_file "drl")

add_custom_command(
    OUTPUT ${hex_file}
    COMMAND
	${AVR_OBJCOPY} -j .text -j .data -O ihex ${elf_file} ${hex_file}
    COMMAND
	${AVR_SIZE_TOOL} ${elf_file}
    DEPENDS ${elf_file}
)


find_program(AVRDUDE avrdude)
set(AVR_PROGRAMMER usbtiny)
set(AVR_MCU t13)
set(upload_file ${hex_file})


add_custom_target(
        upload
        ${AVRDUDE} -p ${AVR_MCU} -c ${AVR_PROGRAMMER}
            -U flash:w:${upload_file}
        DEPENDS ${upload_file}
        COMMENT "Uploading ${upload_file} to ${AVR_MCU} using programmer ${AVR_PROGRAMMER}"
    )


PROJECT(drl)

SET(SOURCE_LIST
        main.c)
 
ADD_EXECUTABLE(drl ${SOURCE_LIST})
