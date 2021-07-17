#pragma once

// Give the guest 2MiB of memory.
#define GuestMemorySize		0x200000

#define IO_PORT_STRING_PRINT	0x0000

// Hypervisor Capability.
BOOL HypervisorPresence;
WHV_CAPABILITY_FEATURES CapFeat;
WHV_EXTENDED_VM_EXITS ExtExitFeat;
WHV_PROCESSOR_FEATURES ProcFeat;
WHV_PROCESSOR_XSAVE_FEATURES XsaveFeat;

// Virtual Machine Definitions.
WHV_PARTITION_HANDLE hPart;
PVOID VirtualMemory = NULL;

// Virtual Processor Initial State.
WHV_REGISTER_NAME SwInitGprNameGroup[0x12] =
{
	WHvX64RegisterRax,
	WHvX64RegisterRcx,
	WHvX64RegisterRdx,
	WHvX64RegisterRbx,
	WHvX64RegisterRsp,
	WHvX64RegisterRbp,
	WHvX64RegisterRsi,
	WHvX64RegisterRdi,
	WHvX64RegisterR8,
	WHvX64RegisterR9,
	WHvX64RegisterR10,
	WHvX64RegisterR11,
	WHvX64RegisterR12,
	WHvX64RegisterR13,
	WHvX64RegisterR14,
	WHvX64RegisterR15,
	WHvX64RegisterRip,
	WHvX64RegisterRflags
};

WHV_REGISTER_VALUE SwInitGprValueGroup[0x12] =
{
	{0},{0},{0},{0},{0xFFFC},{0},{0},{0},
	{0},{0},{0},{0},{0},{0},{0},{0},
	{0x100},{0x2}
};

WHV_REGISTER_NAME SwInitSrNameGroup[8] =
{
	WHvX64RegisterEs,
	WHvX64RegisterCs,
	WHvX64RegisterSs,
	WHvX64RegisterDs,
	WHvX64RegisterFs,
	WHvX64RegisterGs,
	WHvX64RegisterLdtr,
	WHvX64RegisterTr
};

WHV_X64_SEGMENT_REGISTER SwInitSrValueGroup[8] =
{
	{0,0xFFFF,0x1920,{3,1,0,1,0,1,0,0,0}},
	{0,0xFFFF,0x1920,{11,1,0,1,0,1,0,0,0}},
	{0,0xFFFF,0x1920,{3,1,0,1,0,1,0,0,0}},
	{0,0xFFFF,0x1920,{3,1,0,1,0,1,0,0,0}},
	{0,0xFFFF,0x1920,{3,1,0,1,0,1,0,0,0}},
	{0,0xFFFF,0x1920,{3,1,0,1,0,1,0,0,0}},
	{0,0xFFFF,0,{2,0,0,1,0,1,0,0,0}},
	{0,0xFFFF,0,{3,0,0,1,0,1,0,0,0}}
};

WHV_REGISTER_NAME SwInitDescriptorNameGroup[2] =
{
	WHvX64RegisterIdtr,
	WHvX64RegisterGdtr
};

WHV_X64_TABLE_REGISTER SwInitDescriptorValueGroup[2] =
{
	{{0,0,0},0xFFFF,0},
	{{0,0,0},0xFFFF,0}
};

WHV_REGISTER_NAME SwInitCrNameGroup[4] =
{
	WHvX64RegisterCr0,
	WHvX64RegisterCr2,
	WHvX64RegisterCr3,
	WHvX64RegisterCr4
};

WHV_REGISTER_VALUE SwInitCrValueGroup[4] =
{
	{0x60000010},
	{0},{0},{0}
};

WHV_REGISTER_NAME SwInitDrNameGroup[6] =
{
	WHvX64RegisterDr0,
	WHvX64RegisterDr1,
	WHvX64RegisterDr2,
	WHvX64RegisterDr3,
	WHvX64RegisterDr6,
	WHvX64RegisterDr7,
};

WHV_REGISTER_VALUE SwInitDrValueGroup[6] =
{
	{0},{0},{0},{0},
	{0xFFFF0FF0},{0x400}
};

WHV_REGISTER_NAME SwInitXcrNameGroup[1] =
{
	WHvX64RegisterXCr0
};

WHV_REGISTER_VALUE SwInitXcrValueGroup[1] =
{
	{1}
};

WHV_REGISTER_NAME SwInitFpcsName = WHvX64RegisterFpControlStatus;
WHV_X64_FP_CONTROL_STATUS_REGISTER SwInitFpcsValue =
{
	0x40,0x0,0x5555,0,0,{0}
};

// Virtual Machine Property
UINT32 SwProcessorCount = 1;
WHV_EXTENDED_VM_EXITS SwExtendedVmExits = { 0 };