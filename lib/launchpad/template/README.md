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
```

To compile and install your project, run `rake install`.  You should see
something like this:

```
```

Make sure your MSP430 is plugged in to your laptop, otherwise you cannot
install your software.

## Source code

Source code is in the `src` directory.  Your project will be compiled in to
the `build` directory.

[1]: http://code.google.com/p/osx-launchpad/downloads/list
