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

test “1or2”
setPINA 0x01
continue 5
expectPORTC 0x60
checkResult

test “3or4”
setPINA 0x03
continue 5
expectPORTC 0x70
checkResult

test “5or6”
setPINA 0x05
continue 5
expectPORTC 0x38
checkResult

test “7or9”
setPINA 0x07
continue 5
expectPORTC 0x3C
checkResult

test “10or12”
setPINA 0x0A
continue 5
expectPORTC 0x3E
checkResult

test “13or15”
setPINA 0x0F
continue 5
expectPORTC 0x3F
checkResult





# Add tests below

# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
