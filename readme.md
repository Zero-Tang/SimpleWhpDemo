# SimpleWhpDemo
This project is a simple demo of running a real-mode DOS program in Windows 10 x64 by using Windows Hypervisor Platform (WHP).

## Showcase
With a minimal analog firmware, SimpleWhpDemo is running a hello world program which is also capable to be running in a real DOS system. (The screenshot demonstrates the comparison to DOSBox)
![Hello DOS](HelloDOS.png)

## System Requirement
This project requires Windows 10 x64 1803 (Build 17134) or newer. \
Since this project relies on Microsoft Hypervisor, you will have to Install WHP in Control Panels.

![WHP in Control Panel](Capture.PNG)

Please note that even if Device Guard is running, this program cannot run without WHP.

## Build
To build this project, you are required to install [VS2019](https://visualstudio.microsoft.com/) and [Windows SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-10-sdk/).

To build test cases, you are required to install [NASM](https://nasm.us/). \
Run the following command to build a test case:
```bat
nasm (source assembly file name) -o (output executable file name)
```

## Firmware
A legacy x86 computer system would load firmware data from its NVRAM (Non-Volatile RAM). The firmware would provide some functions to the bootloaders to invoke. \
This demo project has implemented a minimal firmware with an IVT (Interrupt Vector Table) and certain interrupt handlers to output string and terminate the program. \
To build the firmware, go to the test cases directory and execute:
```bat
nasm ivt.asm -o ivt.fw
```
Place the firmware file in the same directory with the hypervisor program to run .

## Personal Comments
The [official reference to WHP APIs](https://docs.microsoft.com/en-us/virtualization/api/hypervisor-platform/hypervisor-platform) provided by Microsoft is not well documented. Also, the functionality of WHP, at least from my experience on Windows 10 x64 LTSC 2019 (version 1809, Build 17763), is so scarce that I have to use I/O instructions for hypercalls. What's even worse is that WHP in LTSC does not even support exception interceptions! This means I can't debug the guest in hypervisor level. I thereby conjecture such lack of functionalities is why VMware Workstation requires Windows 10 x64 2004 to run alongside with Hyper-V.

Why am I implementing this demo? I am familiarizing myself with the design logic of WHP. \
Please follow the [Project NoirVisor](https://github.com/Zero-Tang/NoirVisor). In this project, I am aiming to implement a hypervisor as a competitor to WHP.

## License
This project is licensed under the MSPL License.