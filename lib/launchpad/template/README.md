# Getting started with the Launchpad Gem

## Step 0: Install build tools

Make sure you've installed the MSP430 build tools for OS X.  If you haven't
already done so, [go here][1] and download the MSP430 tool chain and the
VCP CDC driver.  The tool chain allows you to compile your MSP430 projects,
and the VCP CDC driver lets you interact with your MSP430 via serial port.

## Step 1: Edit your Rakefile

Make sure to specify the CPU you're using.  New projects default to using the
msp430g2553 processor, but your project may use something different.

## Step 2: Compiling and installing

To compile your project, run `rake compile`.  You should see something like
this:

```
$ rake compile
mkdir -p build
msp430-gcc -I. -I/Users/aaron/git/launchpad/lib/launchpad/native/include  -Os -Wall -g -mmcu=msp430g2553 -c -o build/main.o src/main.c
msp430-gcc -I. -I/Users/aaron/git/launchpad/lib/launchpad/native/include  -Os -Wall -g -mmcu=msp430g2553 -c -o build/launchpad.o /Users/aaron/git/launchpad/lib/launchpad/native/src/launchpad.c
msp430-gcc -I. -I/Users/aaron/git/launchpad/lib/launchpad/native/include  -Os -Wall -g -mmcu=msp430g2553 -o build/main.elf build/main.o build/launchpad.o
msp430-objdump -DS build/main.elf >build/main.lst
```

To compile and install your project, run `rake install`.  You should see
something like this:

```
$ rake install
mspdebug rf2500 "prog build/main.elf"
MSPDebug version 0.17 - debugging tool for MSP430 MCUs
Copyright (C) 2009-2011 Daniel Beer <dlbeer@gmail.com>
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

Trying to open interface 1 on 002
Initializing FET...
FET protocol version is 30394216
Configured for Spy-Bi-Wire
Set Vcc: 3000 mV
fet: FET returned error code 4 (Could not find device (or device not supported))
fet: command C_IDENT1 failed
fet: identify failed
Trying again...
Initializing FET...
FET protocol version is 30394216
Configured for Spy-Bi-Wire
Sending reset...
Set Vcc: 3000 mV
Device ID: 0x2553
Device: MSP430G2553
Code memory starts at 0xc000
Number of breakpoints: 1
Erasing...
Programming...
Writing  170 bytes to c000...
Writing   32 bytes to ffe0...
```

Make sure your MSP430 is plugged in to your laptop, otherwise you cannot
install your software.

## Source code

Source code is in the `src` directory.  Your project will be compiled in to
the `build` directory.

[1]: http://code.google.com/p/osx-launchpad/downloads/list
