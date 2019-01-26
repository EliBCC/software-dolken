# README

## >>> Submodules <<<

IMPORTANT: After cloning this repository run

    git submodule update --init --recursive

so that modm is downloaded from its separate repository.

## Prerequisites

To build and upload code, and generate documentation, you will need some tools installed on your system. On Windows, there is an installation script that will obtain everything through the Scoop package manager, and on Linux, you should rely on your system's package manager or other usual means of installing software.

### Windows

Scoop is a Windows package manager that makes it easy to install most dependencies. There is a Powershell script which will download and install Scoop, and all the required packages listed above.

If you have never used PowerShell scripts before, you will need to open up a new PowerShell terminal and copy and paste in the following command to enable scripts, which will ask you for a confirmation:

    Set-ExecutionPolicy RemoteSigned -scope CurrentUser

Next, just run the `windows_install.ps1` script in this repository and everything will be installed, if not already detected on your computer. If you want to know what the script is doing, open it in a text editor and look at the comments. If your antivirus complains, turn it off temporarily, as it doesn't like the idea of scripts downloading and running code from the internet.

### Linux

You should be able to get everything through your system package manager and pip. Just watch out for arm-none-eabi-gcc being too old, in which case you can download binaries from [ARM](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads), extract, and add to your path.

Don't forget to add the Python scripts directory to your path so you have access to the `lbuild` command.

Things needed:

 * Python 3
 * CMake
 * make
 * arm-none-eabi-gcc, version 7 or later
 * OpenOCD
 * Doxygen
 * Graphviz (binaries)

As well as these Python packages through pip:

 * lbuild
 * graphviz (Python interface)

## Drivers

In most cases, driver installation for the STLink is not needed. However, if you run into any trouble:

### Windows

First, try different USB ports. Some USB3 ports seem to not work properly. If you are still having trouble the correct drivers might not be selected. Download and run (Zadig)[https://zadig.akeo.ie/], select the STLink device, and change its driver to libusb-win32.

### Linux

If you have libusb installed, the STLink should work. However, you may need to add udev rules to allow your user account access to the device. Some rules can be found [here](https://github.com/texane/stlink/tree/master/etc/udev/rules.d), and you will need to figure out where they go depending on your distribution.

## Building

Before building, you need to make sure the `lbuild` command can be run from a terminal. If the command cannot be found, you will have to add your Python Scripts folder to the PATH variable.

[Instructions for Windows](https://projects.raspberrypi.org/en/projects/using-pip-on-windows/5) (It is written for the pip command, but the process is exactly the same for lbuild.)

### Visual Studio Code

If you do not already have a preferred text editor, Visual Studio code works well and provides CMake build support, C++ autocompletion, and debugging with a few extensions you should install:

 * The [Microsoft C/C++ extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) provides autocompletion and error detection.

 * The [CMake Tools extension](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug) provides an interface to build CMake projects.

 * The [Cortex-Debug extension](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug) provides graphical debugging support for ARM microcontrollers.

It is important to open up the top-level directory of this repository so that settings get loaded correctly. The first time, CMake Tools will ask if you want to configure the project. Say yes, and it will ask to choose a compiler, but for that select [Unspecified], as the correct one should later be selected automatically.

After the first time CMake configuration runs, you can select which board to build for. In the bottom toolbar is some buttons. Clicking *⚙ Build* will build code, and just to the right is where you can select which target to build. Selecting an *upload-<boardname>* target will build and flash code to the microcontroller, while selecting a target without *upload-* in the name will only build it. Clicking *CMake: ___: Ready* will allow you to choose between debug or release settings.

After CMake runs, there will also be debug launch configurations automatically generated. Click on the debugger icon in the left side of the screen, choose which file to debug, and hit the ▶️ button to start debugging. (Make sure to build and upload the correct firmware first! If you try to debug code that is different from what is on the microcontroller, you will end up very confused.)

### Command Line

If you know how to use CMake already, then you are good to go. The default target builds all boards, and *upload-<boardname>* will build code for a specific board and upload it.

Alternatively, there is a makefile wrapper around the CMake command syntax. Run `make help` in the terminal to see the available options.

Debugging can be done through Visual Studio Code in the same way while building from the command line.
