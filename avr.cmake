SET(CMAKE_SYSTEM_NAME avr)

SET(CMAKE_C_COMPILER avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)

SET(CSTANDARD "-std=gnu99")
SET(CDEBUG "-gstabs")
SET(CWARN "-Wall -Wstrict-prototypes")
SET(CTUNING "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums")
SET(COPT "-Os")
SET(CINCS "")
SET(CMCU "-mmcu=attiny13a")
SET(CDEFS "-DF_CPU=9600000")


SET(CFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${CINCS} ${COPT} ${CWARN} ${CSTANDARD} ${CEXTRA}")
SET(CXXFLAGS "${CMCU} ${CDEFS} ${CINCS} ${COPT}")

SET(CMAKE_C_FLAGS  ${CFLAGS})
SET(CMAKE_CXX_FLAGS ${CXXFLAGS})

# workaround for error linking with -rdynamic option
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")



find_program(AVRDUDE avrdude)
IF(${AVRDUDE-NOTFOUND})
        message(WARNING "'avrdude' program not found. 'upload' and 'info' targets will not be available!")
ELSE(${AVRDUDE-NOTFOUND})

SET(PROG_ID "usbtiny")
SET(PROG_PART "t13")
SET(HEXFORMAT "ihex")

add_custom_target(upload
        ${AVRDUDE}
                -c ${PROG_ID}
                -p ${PROG_PART}
                #-P ${PROGR_PORT} -e
                -U flash:w:${PROJECT_NAME}.hex
        DEPENDS ${PROJECT_NAME}.hex ${PROJECT_NAME}.ee.hex
        VERBATIM)

add_custom_target(info
        ${AVRDUDE} -v
                -c ${PROG_ID}
                -p ${PROG_PART}
                #-P ${PROGR_PORT} -e
                -U -U hfuse:r:high.txt:r -U lfuse:r:low.txt:r
        VERBATIM)

add_custom_command(
        OUTPUT ${PROJECT_NAME}.hex
        COMMAND /usr/bin/${CMAKE_SYSTEM_NAME}-objcopy --strip-all -j .text -j .data -O ${HEXFORMAT} ${PROJECT_NAME} ${PROJECT_NAME}.hex
        DEPENDS ${PROJECT_NAME}
        VERBATIM
)

add_custom_command(
        OUTPUT ${PROJECT_NAME}.ee.hex
        COMMAND /usr/bin/${CMAKE_SYSTEM_NAME}-objcopy --strip-all -j .eeprom --change-section-lma .eeprom=0
                           -O ${HEXFORMAT} ${PROJECT_NAME} ${PROJECT_NAME}.ee.hex
        DEPENDS ${PROJECT_NAME}
        VERBATIM
)

ENDIF(${AVRDUDE-NOTFOUND})
