#![feature(asm)]
#![no_main]
#![no_std]
#![feature(custom_test_frameworks)]
#![test_runner(test::test_runner)]
#![reexport_test_harness_main = "test_main"]

mod io;
#[cfg(not(test))]
mod panic;
#[cfg(test)]
mod test;

#[no_mangle]
pub extern "C" fn _start() -> ! {
	#[cfg(test)]
	test_main();

	// TODO: follow arch_init procedure

	#[allow(clippy::empty_loop)]
	loop {}
}
