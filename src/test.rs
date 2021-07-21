use crate::io::serial::SER_1;
use crate::ser_vprintf;
use core::panic::PanicInfo;
use x86_64::instructions::port::Port;

#[repr(u8)]
pub enum ExitCode {
	Pass = 0b10,
	Fail = 0b11,
}

pub fn test_runner(tests: &[&dyn Test]) {
	ser_vprintf!(SER_1, "Running {} tests...\n\n", tests.len());
	for test in tests {
		test.run();
	}
	shutdown(ExitCode::Pass);
}

pub fn shutdown(exit: ExitCode) {
	unsafe {
		Port::new(0xF4).write(exit as u32);
	}
}

#[panic_handler]
pub fn panic(info: &PanicInfo) -> ! {
	ser_vprintf!(SER_1, "\n\n:: TEST FAILED - KERNEL PANIC ::");
	ser_vprintf!(SER_1, "\n{}\n", info);
	shutdown(ExitCode::Fail);
	loop {}
}

pub trait Test {
	fn run(&self) -> ();
}

impl<T> Test for T
where
	T: Fn(),
{
	fn run(&self) {
		ser_vprintf!(SER_1, "{}...\t", core::any::type_name::<T>());
		self();
		ser_vprintf!(SER_1, "[PASS]\n");
	}
}
