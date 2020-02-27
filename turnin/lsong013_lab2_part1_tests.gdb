# Test file for Lab2_introToAVR


# commands.gdb provides the following functions for ease:
#   test "<message>"
#       Where <message> is the message to print. Must call this at the beginning of every test
#       Example: test "PINA: 0x00 => expect PORTC: 0x01"
#   checkResult
#       Verify if the test passed or failed. Prints "passed." or "failed." accordingly, 
#       Must call this at the end of every test.
#   expectPORTx <val>
#       With x as the port (A,B,C,D)
#       The value the port is epected to have. If not it will print the erroneous actual value
#   setPINx <val>
#       With x as the port or pin (A,B,C,D)
#       The value to set the pin to (can be decimal or hexidecimal
#       Example: setPINA 0x01
#   printPORTx f OR printPINx f 
#       With x as the port or pin (A,B,C,D)
#       With f as a format option which can be: [d] decimal, [x] hexadecmial (default), [t] binary 
#       Example: printPORTC d
#   printDDRx
#       With x as the DDR (A,B,C,D)
#       Example: printDDRB

echo ======================================================\n
echo Running all tests..."\n\n

#   Partner(s) Name & E-mail: Lucas Song (lsong013@ucr.edu) Reuben D'cunha
#   Lab Section: 22
#   Assignment: Lab # 2 Exercise # 1
#   Exercise Description: [optional - include for your own benefit]
#   
#   I acknowledge all content contained herein, excluding template 
#    or example code, is my own original work.
#


test “not open/night time-> light not on”
setPINA 0x00
#setPINB 0x00
continue 5
expectPORTB 0x00
checkResult

test “not open/day time-> light not on”
setPINA 0x00
setPINB 0X01
continue 5
expectPORTB 0x00
checkResult

test “door open/day time-> light not on”
setPINA 0x00
setPINB 0X01
continue 5
expectPORTB 0x00
checkResult

test “door open/nighttime -> light on”
setPINA 0x01
setPINB 0X00
continue 5
expectPORTB 0x01
checkResult



# Add tests below

# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
