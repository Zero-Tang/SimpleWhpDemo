#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <WinHvPlatform.h>

// Hypervisor Capability.
BOOL HypervisorPresence;
WHV_CAPABILITY_FEATURES CapFeat;
WHV_EXTENDED_VM_EXITS ExtExitFeat;
WHV_PROCESSOR_FEATURES ProcFeat;
WHV_PROCESSOR_XSAVE_FEATURES XsaveFeat;

// Virtual Machine Definitions.
WHV_PARTITION_HANDLE hPart;
PVOID VirtualMemory = NULL;

#define GuestMemorySize		0x10000

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

HRESULT SwInitializeVirtualMachine()
{
	HRESULT hr = WHvCreatePartition(&hPart);
	if (hr == S_OK)
	{
		hr = WHvCreateVirtualProcessor(hPart, 0, 0);
		if (hr == S_OK)
		{
			VirtualMemory = VirtualAlloc(NULL, GuestMemorySize, MEM_COMMIT, PAGE_READWRITE);
			if (VirtualMemory)
			{
				RtlZeroMemory(VirtualMemory, GuestMemorySize);
				hr = WHvMapGpaRange(hPart, VirtualMemory, 0, GuestMemorySize, WHvMapGpaRangeFlagRead | WHvMapGpaRangeFlagWrite | WHvMapGpaRangeFlagExecute);
			}
			else
			{
				WHvDeleteVirtualProcessor(hPart, 0);
				hr = S_FALSE;
			}
		}
		else
			WHvDeletePartition(hPart);
	}
	return hr;
}

int main(int argc, char* argv[], char* envp[])
{
	SwCheckSystemHypervisor();
	if (ExtExitFeat.X64CpuidExit && ExtExitFeat.X64MsrExit)
	{
	}
	return 0;
}