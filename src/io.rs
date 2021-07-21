use core::fmt;
use core::fmt::Write;
use spin::MutexGuard;

pub mod serial;
pub mod vga;

#[doc(hidden)]
pub fn printf(mut device: MutexGuard<impl Write>, args: fmt::Arguments) {
	device.write_fmt(args).expect("Write-out failed [UNKNOWN DEVICE]");
}
