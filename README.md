# lunchbox

* http://github/com/tenderlove/lunchbox

## DESCRIPTION:

A gem to help you get started with development on the TI Launchpad.  This gem
contains a set of rake tasks to help you compile and install code on to your
launchpad, and also contains libraries to help make development a bit easier.

## FEATURES/PROBLEMS:

* Contains rake tasks to simplify compilation and installation of msp430
  launchpad programs.

* Provides header files to give *some* compatibility with Arduino code.

* Includes libraries for writing UARTS (both hardware and software)

* Includes libraries for interfacing with RHT03 temperature sensors.

## SYNOPSIS:

First install the [dev tools and usb driver for the
launchpad](http://code.google.com/p/osx-launchpad/downloads/list).  Then
install this gem:

    $ gem install lunchbox

Generate a new project and install the sample program:

    $ lunchbox my_project
    $ cd my_project
    $ rake install

You should see the LEDs on your launchpad oscillate between red and green.

## REQUIREMENTS:

* Ruby
* [Launchpad dev kit](http://code.google.com/p/osx-launchpad/downloads/list)

## INSTALL:

* gem install lunchbox
* Install the [launchpad dev kit](http://code.google.com/p/osx-launchpad/downloads/list)

## LICENSE:

(The MIT License)

Copyright (c) 2012 Aaron Patterson

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
