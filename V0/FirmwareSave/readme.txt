
C Data Types

unsigned int = 2 bytes = 16bits
unsigned long = 4 bytes = 32bits

char = 1 byte = 8bits
short = 2 byte = 16bits
int = 4 byte = 32bits
long = 8 bytes = 64bits


CHAR_BIT   = 8
MB_LEN_MAX = 16
 
CHAR_MIN   = -128
CHAR_MAX   = +127
SCHAR_MIN  = -128
SCHAR_MAX  = +127
UCHAR_MAX  = 255
 
SHRT_MIN   = -32768
SHRT_MAX   = +32767
USHRT_MAX  = 65535
 
INT_MIN    = -2147483648
INT_MAX    = +2147483647
UINT_MAX   = 4294967295
 
LONG_MIN   = -9223372036854775808
LONG_MAX   = +9223372036854775807
ULONG_MAX  = 18446744073709551615
 
LLONG_MIN  = -9223372036854775808
LLONG_MAX  = +9223372036854775807
ULLONG_MAX = 18446744073709551615

// 65,535
// 48Mhz = 48.000.000hz / 4 = 12.000.000hz = 83,33ns
// Preescale 256 * 83,33ns = 21.333,33ns = 21,33us
// Preescale 128 * 83,33ns = 10.666,66ns = 10,66us
// Preescale 16 * 83,33ns = 1.333,33ns = 1,333us
// Timer0 8-bit = 256 * 21,33us = 35.460,48us = 35,46ms
// Timer0 16-bit - Preescale 128 - Maximum idle time = 65535 * 10,66us = 698.603,10us = 698,60ms

// 128 preescale 1s = 1.000.000us / 10,66us = 93750
// 16 preescale 1s = 1.000.000us / 1,33us = 750000
// 0 preescale 12000004

// 500rpm / 60 =  8,333hz = 0,12s = 120ms
// 120ms = 120.000us / 0,08333us = 144000

// 10khz = 0,0001s = 0,1ms = 100us
// 16pre = 100us / 1,33us = 75
// 0pre = 100us / 0,08333us = 1200


18khz = 0,00005555555s = 0,05555555ms = 55,55555us
0 preescale = 55,55555us / 0,08333us = 666 ticks
pwmOnBeforeAdc = (666 * 50%) * 50% = 167 ticks
pwmOnAfterAdc = (666 * 50%) * 50% = 166 ticks
pwmOff = 666 * 50% = 333 ticks




Stating count

step_tick_start = 334031
step_tick_target = 20000

diff = step_tick_start - step_tick_target = 314031

pulse to increment = 500hz = 0,002s = 2ms = 2.000us
ticks = 24.000

time to reach = 3s
total pulse = time to reach / pulse to increment = 3 / 0,002 = 1500

314031 / 1500 = 209

