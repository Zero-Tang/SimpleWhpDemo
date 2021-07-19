#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <WinHvPlatform.h>
#include "vmdef.h"

HRESULT SwCheckSystemHypervisor()
{
	UINT32 ReturnLength;
	HRESULT hr = WHvGetCapability(WHvCapabilityCodeHypervisorPresent, &HypervisorPresence, sizeof(BOOL), &ReturnLength);
	if (hr == S_OK && HypervisorPresence)
	{
		WHvGetCapability(WHvCapabilityCodeFeatures, &CapFeat, sizeof(CapFeat), &ReturnLength);
		WHvGetCapability(WHvCapabilityCodeExtendedVmExits, &ExtExitFeat, sizeof(ExtExitFeat), &ReturnLength);
		WHvGetCapability(WHvCapabilityCodeProcessorFeatures, &ProcFeat, sizeof(ProcFeat), &ReturnLength);
		WHvGetCapability(WHvCapabilityCodeProcessorXsaveFeatures, &XsaveFeat, sizeof(XsaveFeat), &ReturnLength);
		puts("Hypervisor is Present to run SimpleWhpDemo!");
	}
	else
	{
		printf("Failed to run SimpleWhpDemo! Hypervisor is %s! Code=0x%X!\n", HypervisorPresence ? "Present" : "Not Present", hr);
		if (!HypervisorPresence)hr = S_FALSE;
	}
	return hr;
}

void SwTerminateVirtualMachine()
{
	VirtualFree(VirtualMemory, 0, MEM_RELEASE);
	WHvDeleteVirtualProcessor(hPart, 0);
	WHvDeletePartition(hPart);
}

HRESULT SwInitializeVirtualMachine()
{
	BOOL PartitionCreated = FALSE;
	BOOL VcpuCreated = FALSE;
	BOOL MemoryAllocated = FALSE;
	// Create a virtual machine.
	HRESULT hr = WHvCreatePartition(&hPart);
	if (hr == S_OK)
		PartitionCreated = TRUE;
	else
		goto Cleanup;
	// Setup Partition Properties.
	hr = WHvSetPartitionProperty(hPart, WHvPartitionPropertyCodeProcessorCount, &SwProcessorCount, sizeof(SwProcessorCount));
	if (hr != S_OK)
	{
		printf("Failed to setup Processor Count! HRESULT=0x%X\n", hr);
		goto Cleanup;
	}
	// Setup Partition
	hr = WHvSetupPartition(hPart);
	if (hr != S_OK)
	{
		printf("Failed to setup Virtual Machine! HRESULT=0x%X\n", hr);
		goto Cleanup;
	}
	// Create Virtual Memory.
	VirtualMemory = VirtualAlloc(NULL, GuestMemorySize, MEM_COMMIT, PAGE_READWRITE);
	if (VirtualMemory)
		MemoryAllocated = TRUE;
	else
		goto Cleanup;
	RtlZeroMemory(VirtualMemory, GuestMemorySize);
	hr = WHvMapGpaRange(hPart, VirtualMemory, 0, GuestMemorySize, WHvMapGpaRangeFlagRead | WHvMapGpaRangeFlagWrite | WHvMapGpaRangeFlagExecute);
	if (hr != S_OK)goto Cleanup;
	// Create Virtual Processors.
	hr = WHvCreateVirtualProcessor(hPart, 0, 0);
	if (hr == S_OK)
		VcpuCreated = TRUE;
	else
		goto Cleanup;
	// Initialize Virtual Processor State
	hr = WHvSetVirtualProcessorRegisters(hPart, 0, SwInitGprNameGroup, 0x12, SwInitGprValueGroup);
	if (hr != S_OK)
	{
		printf("Failed to initialize General Purpose Registers! HRESULT=0x%X\n", hr);
		goto Cleanup;
	}
	hr = WHvSetVirtualProcessorRegisters(hPart, 0, SwInitSrNameGroup, 8, (WHV_REGISTER_VALUE*)SwInitSrValueGroup);
	if (hr != S_OK)
	{
		printf("Failed to initialize Segment Registers! HRESULT=0x%X\n", hr);
		goto Cleanup;
	}
	hr = WHvSetVirtualProcessorRegisters(hPart, 0, SwInitDescriptorNameGroup, 2, (WHV_REGISTER_VALUE*)SwInitDescriptorValueGroup);
	if (hr != S_OK)
	{
		printf("Failed to initialize Descriptor Tables! HRESULT=0x%X\n", hr);
		goto Cleanup;
	}
	hr = WHvSetVirtualProcessorRegisters(hPart, 0, SwInitCrNameGroup, 4, SwInitCrValueGroup);
	if (hr != S_OK)
	{
		printf("Failed to initialize Control Registers! HRESULT=0x%X\n", hr);
		goto Cleanup;
	}
	hr = WHvSetVirtualProcessorRegisters(hPart, 0, SwInitDrNameGroup, 6, SwInitDrValueGroup);
	if (hr != S_OK)
	{
		printf("Failed to initialize Debug Registers! HRESULT=0x%X\n", hr);
		goto Cleanup;
	}
	hr = WHvSetVirtualProcessorRegisters(hPart, 0, SwInitXcrNameGroup, 1, SwInitXcrValueGroup);
	if (hr != S_OK)
	{
		printf("Failed to initialize Extended Control Registers! HRESULT=0x%X\n", hr);
		goto Cleanup;
	}
	hr = WHvSetVirtualProcessorRegisters(hPart, 0, &SwInitFpcsName, 1, (WHV_REGISTER_VALUE*)&SwInitFpcsValue);
	if (hr != S_OK)
	{
		printf("Failed to initialize x87 Floating Point Control Status! HRESULT=0x%X\n", hr);
		goto Cleanup;
	}
	return S_OK;
Cleanup:
	if (MemoryAllocated)VirtualFree(VirtualMemory, 0, MEM_RELEASE);
	if (VcpuCreated)WHvDeleteVirtualProcessor(hPart, 0);
	if (PartitionCreated)WHvDeletePartition(hPart);
	return S_FALSE;
}

HRESULT SwDumpVirtualProcessorGprState()
{
	WHV_REGISTER_VALUE RegVal[0x12];
	HRESULT hr = WHvGetVirtualProcessorRegisters(hPart, 0, SwInitGprNameGroup, 0x12, RegVal);
	if (hr == S_OK)
	{
		puts("============ Dumping General-Purpose Registers ============");
		puts("Name\t Value");
		for (UINT32 i = 0; i < 0x12; i++)
			printf("%s\t 0x%016llX\n", SwGprNameGroup[i], RegVal[i].Reg64);
		return hr;
	}
	printf("Failed to dump General-Purpose Registers! HRESULT=0x%X\n", hr);
	return hr;
}

HRESULT SwDumpVirtualProcessorSegmentState()
{
	WHV_REGISTER_VALUE RegVal[8];
	HRESULT hr = WHvGetVirtualProcessorRegisters(hPart, 0, SwInitSrNameGroup, 8, RegVal);
	if (hr == S_OK)
	{
		puts("============ Dumping Segment Registers ============");
		puts("Name\t Selector\t Attributes\t Limit\t\t Base");
		for (UINT32 i = 0; i < 8; i++)
			printf("%s\t 0x%04X\t\t 0x%04X\t\t 0x%08X\t 0x%016llX\n", SwSrNameGroup[i], RegVal[i].Segment.Selector, RegVal[i].Segment.Attributes, RegVal[i].Segment.Limit, RegVal[i].Segment.Base);
		return hr;
	}
	printf("Failed to dump General-Purpose Registers! HRESULT=0x%X\n", hr);
	return hr;
}

BOOL LoadVirtualMachineProgram(IN PSTR FileName, IN ULONG Offset)
{
	BOOL Result = FALSE;
	HANDLE hFile = CreateFileA(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD FileSize = GetFileSize(hFile, NULL);
		if (FileSize != INVALID_FILE_SIZE)
		{
			DWORD dwSize = 0;
			PVOID ProgramAddress = (PVOID)((ULONG_PTR)VirtualMemory + Offset);
			Result = ReadFile(hFile, ProgramAddress, FileSize, &dwSize, NULL);
		}
		CloseHandle(hFile);
	}
	else
		printf("Failed to open file! Error Code=%d\n", GetLastError());
	return Result;
}

UINT32 SwDosStringLength(IN PSTR String, IN UINT32 MaximumLength)
{
	for (UINT32 i = 0; i < MaximumLength; i++)
		if (String[i] == '\0' || String[i] == '$')
			return i;
	return MaximumLength;
}

HRESULT SwExecuteProgram()
{
	WHV_RUN_VP_EXIT_CONTEXT ExitContext = { 0 };
	BOOL ContinueExecution = TRUE;
	HRESULT hr = S_FALSE;
	ULONG64 PrintAddr = 0;
	while (ContinueExecution)
	{
		hr = WHvRunVirtualProcessor(hPart, 0, &ExitContext, sizeof(ExitContext));
		if (hr == S_OK)
		{
			WHV_REGISTER_NAME RipName = WHvX64RegisterRip;
			WHV_REGISTER_VALUE Rip = { ExitContext.VpContext.Rip };
			switch (ExitContext.ExitReason)
			{
			case WHvRunVpExitReasonMemoryAccess:
			{
				PSTR AccessType[4] = { "Read","Write","Execute","Unknown"};
				puts("Memory Access Violation occured!");
				printf("Access Context: GVA=0x%llXX GPA=0x%0llXX\n", ExitContext.MemoryAccess.Gva, ExitContext.MemoryAccess.Gpa);
				printf("Behavior: %s\t", AccessType[ExitContext.MemoryAccess.AccessInfo.AccessType]);
				printf("GVA is %s \t", ExitContext.MemoryAccess.AccessInfo.GvaValid ? "Valid" : "Invalid");
				printf("GPA is %s \n", ExitContext.MemoryAccess.AccessInfo.GpaUnmapped ? "Mapped" : "Unmapped");
				printf("Number of Instruction Bytes: %d\n Instruction Bytes: ", ExitContext.MemoryAccess.InstructionByteCount);
				for (UINT8 i = 0; i < ExitContext.MemoryAccess.InstructionByteCount; i++)
					printf("%02X ", ExitContext.MemoryAccess.InstructionBytes[i]);
				ContinueExecution = FALSE;
				break;
			}
			case WHvRunVpExitReasonX64IoPortAccess:
			{
				WHV_REGISTER_NAME RevGprName[4] = { WHvX64RegisterRax,WHvX64RegisterRcx,WHvX64RegisterRsi,WHvX64RegisterRdi };
				WHV_REGISTER_VALUE RevGprValue[4];
				RevGprValue[0].Reg64 = ExitContext.IoPortAccess.Rax;
				RevGprValue[1].Reg64 = ExitContext.IoPortAccess.Rcx;
				RevGprValue[2].Reg64 = ExitContext.IoPortAccess.Rsi;
				RevGprValue[3].Reg64 = ExitContext.IoPortAccess.Rdi;
				if (ExitContext.IoPortAccess.PortNumber == IO_PORT_STRING_PRINT)
				{
					if (ExitContext.IoPortAccess.AccessInfo.IsWrite)
					{
						INT32 Direction = _bittest64(&ExitContext.VpContext.Rflags, 10) ? -1 : 1;
						INT32 Increment = ExitContext.IoPortAccess.AccessInfo.AccessSize * Direction;
						if (ExitContext.IoPortAccess.AccessInfo.StringOp)
						{
							UINT64 Gpa = ((UINT64)ExitContext.IoPortAccess.Ds.Selector << 4) + ExitContext.IoPortAccess.Rsi;
							PSTR StringAddress = (PSTR)((ULONG_PTR)VirtualMemory + Gpa);
							if (ExitContext.IoPortAccess.AccessInfo.RepPrefix)
							{
								printf("%.*s", SwDosStringLength(StringAddress, 1000), StringAddress);
								RevGprValue[1].Reg64 = 0;
							}
							else
							{
								putc(*StringAddress, stdout);
							}
						}
						else
						{
							putc((UINT8)ExitContext.IoPortAccess.Rax, stdout);
						}
					}
				}
				WHvSetVirtualProcessorRegisters(hPart, 0, RevGprName, 4, RevGprValue);
				break;
			}
			case WHvRunVpExitReasonUnrecoverableException:
				puts("The processor went into shutdown state due to unrecoverable exception!");
				ContinueExecution = FALSE;
				break;
			case WHvRunVpExitReasonInvalidVpRegisterValue:
				puts("The specified processor state is invalid!");
				ContinueExecution = FALSE;
				break;
			case WHvRunVpExitReasonX64Halt:
				ContinueExecution = _bittest64(&ExitContext.VpContext.Rflags, 9);
				break;
			case WHvRunVpExitReasonException:
				break;
			default:
				printf("Unknown VM-Exit Code=0x%X!\n", ExitContext.ExitReason);
				ContinueExecution = FALSE;
				break;
			}
			Rip.Reg64 += ExitContext.VpContext.InstructionLength;
			hr = WHvSetVirtualProcessorRegisters(hPart, 0, &RipName, 1, &Rip);
		}
		else
		{
			printf("Failed to run virtual processor! HRESULT=0x%X\n", hr);
			ContinueExecution = FALSE;
		}
	}
	return hr;
}

int main(int argc, char* argv[], char* envp[])
{
	PSTR ProgramFileName = argc >= 3 ? argv[2] : "hello.com";
	PSTR IvtFileName = argc >= 3 ? argv[1] : "ivt.fw";
	SwCheckSystemHypervisor();
	if (ExtExitFeat.X64CpuidExit && ExtExitFeat.X64MsrExit)
	{
		HRESULT hr = SwInitializeVirtualMachine();
		if (hr == S_OK)
		{
			BOOL LoadProgramResult = LoadVirtualMachineProgram(ProgramFileName, 0x10100);
			BOOL LoadIvtFwResult = LoadVirtualMachineProgram(IvtFileName, 0);
			puts("Virtual Machine is initialized successfully!");
			if (LoadProgramResult && LoadIvtFwResult)
			{
				puts("Program is loaded successfully!");
				puts("============ Program Start ============");
				SwExecuteProgram();
				puts("============= Program End =============");
			}
			SwTerminateVirtualMachine();
		}
	}
	return 0;
}