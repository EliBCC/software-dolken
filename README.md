# README

## >>> Submodules <<<

IMPORTANT: After cloning this repository run

    git submodule update --init --recursive

so that modm is downloaded from its separate repository.

## Prerequisites

To build and upload code, and generate documentation, you will need the following packages and tools installed on your system:

 * Python 3
 * CMake
 * ninja
 * make
 * touch
 * arm-none-eabi-gcc, version 7 or later
 * OpenOCD
 * Doxygen
 * Graphviz (binaries)

As well as these Python packages through pip:

 * lbuild
 * graphviz (Python interface)

### Linux

You should be able to get everything through your system package manager and pip. Just watch out for arm-none-eabi-gcc being too old, in which case you can download binaries from [ARM](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads), extract, and add to your path.

Don't forget to add the Python scripts directory to your path so you have access to the `lbuild` command.

### Windows

Scoop is a Windows package manager that makes it easy to install most dependencies. There is a Powershell script which will download and install Scoop, and all the required packages listed above.

If you have never used PowerShell scripts before, you will need to open up a new PowerShell terminal and copy and paste in the following command, which will ask you for a confirmation:

    Set-ExecutionPolicy RemoteSigned -scope CurrentUser

Next, just right click on the `windows_install.ps1` script in this repository and everything will be installed, if not already detected on your computer. If you want to know what the script is doing, open it in a text editor and look at the comments.

If the above does not run, try to install Scoop manually by writing in the PowerShell command prompt:

    iex (new-object net.webclient).downloadstring('https://get.scoop.sh')
    
Then run `windows_install.ps1` again.

## Building

From a terminal in the root of this repository, run `make build-mainboard-debug` and if all the tools are installed and available, the modm libraries will be generated, and code compiled.

To upload a debug binary run `make upload-mainboard-debug` and OpenOCD will be used to upload binaries to the device, but first the binaries will be rebuilt if necessary.

To see other make subcommands for this project run `make help`.

## Editing and Debugging

If you do not already have a preferred text editor, Visual Studio code works well and provides debugging and C++ autocompletion with a couple of extensions:

 * The [Microsoft C/C++ extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) provides autocompletion and error detection.

 * The [Cortex-Debug extension](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug) provides graphical debugging support for ARM microcontrollers.

This repository already has a debug launch configuration, so just open the repository folder in VS code, go to the debug tab and press the play button to start debugging. (Make sure to build and upload the debug firmware using `make upload-debug` first!)
