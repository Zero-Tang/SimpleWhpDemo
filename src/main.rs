use std::{ffi::c_void, ptr::null_mut, slice, sync::atomic::{AtomicPtr, Ordering}};

use windows::{core::*, Win32::{Foundation::*, System::{Hypervisor::*,Memory::*},Storage::FileSystem::*}};
use aligned::*;

static GLOBAL_EMULATOR_HANDLE:AtomicPtr<c_void>=AtomicPtr::new(null_mut());
static GLOBAL_EMULATOR_CALLBACKS:WHV_EMULATOR_CALLBACKS=WHV_EMULATOR_CALLBACKS
{
	Size:size_of::<WHV_EMULATOR_CALLBACKS>() as u32,
	Reserved:0,
	WHvEmulatorIoPortCallback:Some(emu_io_port_callback),
	WHvEmulatorMemoryCallback:Some(emu_memory_callback),
	WHvEmulatorGetVirtualProcessorRegisters:Some(emu_get_vcpu_reg_callback),
	WHvEmulatorSetVirtualProcessorRegisters:Some(emu_set_vcpu_reg_callback),
	WHvEmulatorTranslateGvaPage:Some(emu_translate_gva_callback)
};

const INITIAL_VCPU_COUNT:usize=40;
const INITIAL_VCPU_REGISTER_NAMES:[WHV_REGISTER_NAME;INITIAL_VCPU_COUNT]=
[
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
	WHvX64RegisterRflags,
	WHvX64RegisterEs,
	WHvX64RegisterCs,
	WHvX64RegisterSs,
	WHvX64RegisterDs,
	WHvX64RegisterFs,
	WHvX64RegisterGs,
	WHvX64RegisterLdtr,
	WHvX64RegisterTr,
	WHvX64RegisterIdtr,
	WHvX64RegisterGdtr,
	WHvX64RegisterCr0,
	WHvX64RegisterCr2,
	WHvX64RegisterCr3,
	WHvX64RegisterCr4,
	WHvX64RegisterDr0,
	WHvX64RegisterDr1,
	WHvX64RegisterDr2,
	WHvX64RegisterDr3,
	WHvX64RegisterDr6,
	WHvX64RegisterDr7,
	WHvX64RegisterXCr0,
	WHvX64RegisterFpControlStatus
];
// Note: WHV_REGISTER_VALUE should be aligned on 16-byte boundary. However, the definition of it isn't aligned to 16-byte boundary.
const INITIAL_VCPU_REGISTER_VALUES:Aligned<A16,[WHV_REGISTER_VALUE;INITIAL_VCPU_COUNT]>=Aligned
(
	[
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0xFFF0},
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0x100},
		WHV_REGISTER_VALUE{Reg64:0x2},
		WHV_REGISTER_VALUE{Segment:WHV_X64_SEGMENT_REGISTER{Base:0x10000,Limit:0xFFFFFFFF,Selector:0x1000,Anonymous:WHV_X64_SEGMENT_REGISTER_0{Attributes:0x93}}},
		WHV_REGISTER_VALUE{Segment:WHV_X64_SEGMENT_REGISTER{Base:0x10000,Limit:0xFFFFFFFF,Selector:0x1000,Anonymous:WHV_X64_SEGMENT_REGISTER_0{Attributes:0x9B}}},
		WHV_REGISTER_VALUE{Segment:WHV_X64_SEGMENT_REGISTER{Base:0x10000,Limit:0xFFFFFFFF,Selector:0x1000,Anonymous:WHV_X64_SEGMENT_REGISTER_0{Attributes:0x93}}},
		WHV_REGISTER_VALUE{Segment:WHV_X64_SEGMENT_REGISTER{Base:0x10000,Limit:0xFFFFFFFF,Selector:0x1000,Anonymous:WHV_X64_SEGMENT_REGISTER_0{Attributes:0x93}}},
		WHV_REGISTER_VALUE{Segment:WHV_X64_SEGMENT_REGISTER{Base:0x10000,Limit:0xFFFFFFFF,Selector:0x1000,Anonymous:WHV_X64_SEGMENT_REGISTER_0{Attributes:0x93}}},
		WHV_REGISTER_VALUE{Segment:WHV_X64_SEGMENT_REGISTER{Base:0x10000,Limit:0xFFFFFFFF,Selector:0x1000,Anonymous:WHV_X64_SEGMENT_REGISTER_0{Attributes:0x93}}},
		WHV_REGISTER_VALUE{Segment:WHV_X64_SEGMENT_REGISTER{Base:0,Limit:0xFFFF,Selector:0,Anonymous:WHV_X64_SEGMENT_REGISTER_0{Attributes:0x82}}},
		WHV_REGISTER_VALUE{Segment:WHV_X64_SEGMENT_REGISTER{Base:0,Limit:0xFFFF,Selector:0,Anonymous:WHV_X64_SEGMENT_REGISTER_0{Attributes:0x83}}},
		WHV_REGISTER_VALUE{Table:WHV_X64_TABLE_REGISTER{Base:0,Pad:[0;3],Limit:0xFFFF}},
		WHV_REGISTER_VALUE{Table:WHV_X64_TABLE_REGISTER{Base:0,Pad:[0;3],Limit:0xFFFF}},
		WHV_REGISTER_VALUE{Reg64:0x10},
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0},
		WHV_REGISTER_VALUE{Reg64:0xFFFF0FF0},
		WHV_REGISTER_VALUE{Reg64:0x400},
		WHV_REGISTER_VALUE{Reg64:1},
		WHV_REGISTER_VALUE{FpControlStatus:WHV_X64_FP_CONTROL_STATUS_REGISTER{Anonymous:WHV_X64_FP_CONTROL_STATUS_REGISTER_0{FpControl:0x40,FpStatus:0,FpTag:0x55,Reserved:0,LastFpOp:0,Anonymous:WHV_X64_FP_CONTROL_STATUS_REGISTER_0_0{LastFpRip:0}}}},
	]
);

#[repr(C)] struct SimpleVirtualMachine
{
	handle:WHV_PARTITION_HANDLE,
	vmem:*mut c_void,
	size:usize
}

impl SimpleVirtualMachine
{
	fn new(memory_size:usize)->Result<Self>
	{
		match unsafe{WHvCreatePartition()}
		{
			Ok(h)=>
			{
				let vcpu_count_prop=WHV_PARTITION_PROPERTY{ProcessorCount:1};
				if let Err(e)=unsafe{WHvSetPartitionProperty(h,WHvPartitionPropertyCodeProcessorCount,(&raw const vcpu_count_prop).cast(),size_of::<WHV_PARTITION_PROPERTY>() as u32)}
				{
					panic!("Failed to setup vCPU Count! Reason: {e}");
				}
				if let Err(e)=unsafe{WHvSetupPartition(h)}
				{
					panic!("Failed to setup partition! Reason: {e}");
				}
				if let Err(e)=unsafe{WHvCreateVirtualProcessor(h,0,0)}
				{
					panic!("Failed to create vCPU! Reason: {e}");
				}
				else
				{
					if let Err(e)=unsafe{WHvSetVirtualProcessorRegisters(h,0,INITIAL_VCPU_REGISTER_NAMES.as_ptr(),INITIAL_VCPU_COUNT as u32,INITIAL_VCPU_REGISTER_VALUES.as_ptr())}
					{
						panic!("Failed to initialize vCPU registers! Reason: {e}");
					}
				}
				let p=unsafe{VirtualAlloc(None,memory_size,MEM_COMMIT,PAGE_READWRITE)};
				if p.is_null()
				{
					panic!("Failed to allocate virtual memory! Reason: {}",unsafe{GetLastError()}.to_hresult().message());
				}
				if let Err(e)=unsafe{WHvMapGpaRange(h,p,0,memory_size as u64,WHvMapGpaRangeFlagRead|WHvMapGpaRangeFlagWrite|WHvMapGpaRangeFlagExecute)}
				{
					panic!("Failed to map guest memory! Reason: {e}");
				}
				Ok
				(
					Self
					{
						handle:h,
						vmem:p,
						size:memory_size
					}
				)
			}
			Err(e)=>Err(e)
		}
	}

	fn load_program(&self,file_name:&str,offset:usize)->Result<()>
	{
		let path=file_name.encode_utf16();
		let v:Vec<u16>=path.collect();
		match unsafe{CreateFileW(PCWSTR(v.as_ptr()),GENERIC_READ.0,FILE_SHARE_READ,None,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,None)}
		{
			Ok(h)=>
			{
				let mut file_size=0;
				let r=match unsafe{GetFileSizeEx(h,&raw mut file_size)}
				{
					Ok(_)=>
					{
						if offset+file_size as usize>self.size
						{
							panic!("Overflow happens while loading {file_name}!");
						}
						let mut size=0;
						let addr:*mut u8=unsafe{self.vmem.byte_add(offset).cast()};
						let buffer:&mut [u8]=unsafe{slice::from_raw_parts_mut(addr,file_size as usize)};
						unsafe{ReadFile(h,Some(buffer),Some(&raw mut size),None)}
					}
					Err(e)=>Err(e)
				};
				let _=unsafe{CloseHandle(h)};
				r
			}
			Err(e)=>Err(e)
		}
	}

	fn run(&self)
	{
		let mut exit_ctxt:WHV_RUN_VP_EXIT_CONTEXT=WHV_RUN_VP_EXIT_CONTEXT::default();
		let mut cont_exec=true;
		while cont_exec
		{
			if let Err(e)=unsafe{WHvRunVirtualProcessor(self.handle,0,(&raw mut exit_ctxt).cast(),size_of::<WHV_RUN_VP_EXIT_CONTEXT>() as u32)}
			{
				println!("Failed to run vCPU! Reason: {e}");
				cont_exec=false;
			}
			else
			{
				#[allow(non_upper_case_globals)]
				match exit_ctxt.ExitReason
				{
					WHvRunVpExitReasonX64IoPortAccess=>
					{
						match self.try_emulate_io(&raw const exit_ctxt.VpContext,unsafe{&raw const exit_ctxt.Anonymous.IoPortAccess})
						{
							Ok(st)=>
							{
								let s=unsafe{st.AsUINT32};
								if s!=1
								{
									println!("Failed to emulate I/O instruction! Status=0x{:08X}",s)
								}
							}
							Err(e)=>
							{
								println!("Failed to emulate I/O instruction! Reason: {e}");
								cont_exec=false;
							}
						}
					}
					WHvRunVpExitReasonX64Halt=>
					{
						cont_exec=false;
					}
					_=>
					{
						println!("Unknown Exit Reason: 0x{:X}!",exit_ctxt.ExitReason.0);
						cont_exec=false;
					}
				}
			}
		}
	}

	fn try_emulate_io(&self,vcpu_ctxt:*const WHV_VP_EXIT_CONTEXT,io_ctxt:*const WHV_X64_IO_PORT_ACCESS_CONTEXT)->std::result::Result<WHV_EMULATOR_STATUS, Error>
	{
		unsafe
		{
			WHvEmulatorTryIoEmulation(GLOBAL_EMULATOR_HANDLE.load(Ordering::Relaxed),(self as *const Self).cast(),vcpu_ctxt,io_ctxt)
		}
	}
}

impl Drop for SimpleVirtualMachine
{
	fn drop(&mut self)
	{
		if let Err(e)=unsafe{WHvDeletePartition(self.handle)}
		{
			panic!("Failed to delete virtual machine! Reason: {e}");
		}
		if let Err(e)=unsafe{VirtualFree(self.vmem,0,MEM_RELEASE)}
		{
			panic!("Failed to release virtual memory! Reason: {e}");
		}
		println!("Successfully deleted virtual machine!");
	}
}

unsafe extern "system" fn emu_io_port_callback(_context:*const c_void,io_access:*mut WHV_EMULATOR_IO_ACCESS_INFO)->HRESULT
{
	unsafe
	{
		if (*io_access).AccessSize!=1
		{
			println!("Only size of 1 operand is allowed! This access size is {} bytes!",(*io_access).AccessSize);
			E_NOTIMPL
		}
		else if (*io_access).Direction==0
		{
			println!("Input is not implemented!");
			E_NOTIMPL
		}
		else if (*io_access).Port!=0
		{
			println!("Unknown I/O Port (0x{:04X}) is accessed!",(*io_access).Port);
			E_NOTIMPL
		}
		else
		{
			print!("{}",(*io_access).Data as u8 as char);
			S_OK
		}
	}
}

unsafe extern "system" fn emu_memory_callback(context:*const c_void,memory_access:*mut WHV_EMULATOR_MEMORY_ACCESS_INFO)->HRESULT
{
	unsafe
	{
		let ctxt:&SimpleVirtualMachine=&(*context.cast());
		let len:usize=(*memory_access).AccessSize as usize;
		let gpa_start:u64=(*memory_access).GpaAddress;
		let gpa_end:u64=gpa_start+len as u64-1;
		if (0..ctxt.size as u64).contains(&gpa_start) && (0..ctxt.size as u64).contains(&gpa_end)
		{
			let hva:*mut u8=ctxt.vmem.byte_add((*memory_access).GpaAddress as usize).cast();
			// println!("Memory GPA=0x{:X} has HVA {hva:p}!",(*memory_access).GpaAddress);
			if (*memory_access).Direction!=0
			{
				let vmem:&mut [u8]=slice::from_raw_parts_mut(hva,len);
				vmem.copy_from_slice(&(*memory_access).Data[0..len]);
			}
			else
			{
				let vmem:&[u8]=slice::from_raw_parts(hva,len);
				(*memory_access).Data[0..len].copy_from_slice(vmem);
			}
			S_OK
		}
		else
		{
			println!("Memory-Access Overflow is detected! GPA=0x{:016X}, Access-Size={} bytes!",gpa_start,(*memory_access).AccessSize);
			E_FAIL
		}
	}
}

unsafe extern "system" fn emu_get_vcpu_reg_callback(context:*const c_void,reg_names:*const WHV_REGISTER_NAME,reg_count:u32,reg_values:*mut WHV_REGISTER_VALUE)->HRESULT
{
	unsafe
	{
		let ctxt:&SimpleVirtualMachine=&(*context.cast());
		match WHvGetVirtualProcessorRegisters(ctxt.handle,0,reg_names,reg_count,reg_values)
		{
			Ok(_)=>S_OK,
			Err(e)=>e.code()
		}
	}
}

unsafe extern "system" fn emu_set_vcpu_reg_callback(context:*const c_void,reg_names:*const WHV_REGISTER_NAME,reg_count:u32,reg_values:*const WHV_REGISTER_VALUE)->HRESULT
{
	unsafe
	{
		let ctxt:&SimpleVirtualMachine=&(*context.cast());
		match WHvSetVirtualProcessorRegisters(ctxt.handle,0,reg_names,reg_count,reg_values)
		{
			Ok(_)=>S_OK,
			Err(e)=>e.code()
		}
	}
}

unsafe extern "system" fn emu_translate_gva_callback(context:*const c_void,gva_page:u64,translate_flags:WHV_TRANSLATE_GVA_FLAGS,translation_result:*mut WHV_TRANSLATE_GVA_RESULT_CODE,gpa_page:*mut u64)->HRESULT
{
	unsafe
	{
		let ctxt:&SimpleVirtualMachine=&(*context.cast());
		let mut r:WHV_TRANSLATE_GVA_RESULT=WHV_TRANSLATE_GVA_RESULT::default();
		match WHvTranslateGva(ctxt.handle,0,gva_page,translate_flags,&raw mut r,gpa_page)
		{
			Ok(_)=>
			{
				*translation_result=r.ResultCode;
				S_OK
			}
			Err(e)=>e.code()
		}
	}
}

fn init_whpx()->HRESULT
{
	let mut hv_present:WHV_CAPABILITY=WHV_CAPABILITY::default();
	match unsafe{WHvGetCapability(WHvCapabilityCodeHypervisorPresent,(&raw mut hv_present).cast(),size_of::<WHV_CAPABILITY>() as u32,None)}
	{
		Ok(_)=>
		{
			if unsafe{hv_present.HypervisorPresent}.as_bool()
			{
				let mut eh:*mut c_void=null_mut();
				match unsafe{WHvEmulatorCreateEmulator(&raw const GLOBAL_EMULATOR_CALLBACKS,&raw mut eh)}
				{
					Ok(_)=>
					{
						GLOBAL_EMULATOR_HANDLE.store(eh,Ordering::Relaxed);
						S_OK
					}
					Err(e)=>e.code()
				}
			}
			else
			{
				S_FALSE
			}
		}
		Err(e)=>
		{
			println!("Failed to query Windows Hypervisor Platform presence! Reason: {e}");
			e.code()
		}
	}
}

fn main()
{
	if init_whpx()==S_OK
	{
		println!("WHPX is present and initalized!");
		if let Ok(vm)=SimpleVirtualMachine::new(0x100000)
		{
			println!("Successfully created virtual machine!");
			if let Err(e)=vm.load_program("ivt.fw\0",0)
			{
				panic!("Failed to load firmware! Reason: {e}");
			}
			if let Err(e)=vm.load_program("hello.com\0",0x10100)
			{
				panic!("Failed to load program! Reason: {e}");
			}
			println!("============ Program Start ============");
			vm.run();
			println!("============= Program End =============");
		}
		let _=unsafe{WHvEmulatorDestroyEmulator(GLOBAL_EMULATOR_HANDLE.load(Ordering::Relaxed))};
	}
}
