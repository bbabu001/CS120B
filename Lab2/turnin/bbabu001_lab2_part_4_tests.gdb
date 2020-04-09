# Test file for "Lab2"


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



# Add tests below

#all 0
test "PINA: 0x00, PINB: 0x00, PINC: 0x00 => PORTD: 0x00"
setPINA 0x00
setPINB 0x00
setPINC 0x00
continue 2
expectPORTD 0x00
checkResult

#unbalanced
test "PINA: 0x55, PINB: 0x0A, PINC: 0x01 => PORTD: 0xC2"
setPINA 0x55
setPINB 0x0A
setPINC 0x01
continue 2
expectPORTD 0xC2
checkResult

#unbalanced
test "PINA: 0x01, PINB: 0x0A, PINC: 0x55 => PORTD: 0xC2" 
setPINA 0x01
setPINB 0x0A
setPINC 0x55
continue 2
expectPORTD 0xC2
checkResult

#balanced, overweight
test "PINA: 0x41, PINB: 0x14, PINC: 0x3C => PORTD: 0x91"
setPINA 0x41
setPINB 0x14 
setPINC 0x3C
continue 2
expectPORTD 0x91
checkResult

#unbalanced, overweight
test "PINA: 0x6E, PINB: 0x14, PINC: 0x3C => PORTD: 0xBF" 
setPINA 0x6E
setPINB 0x14
setPINC 0x3C
continue 2
expectPORTD 0xBF
checkResult


# Report on how many tests passed/tests ran
set $passed=$tests-$failed
eval "shell echo Passed %d/%d tests.\n",$passed,$tests
echo ======================================================\n
