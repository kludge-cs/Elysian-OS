use crate::io::vga::{PANIC_FMT, VGA_TERM};
use crate::vprintf;
use core::panic::PanicInfo;

#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
	VGA_TERM.lock().set_fmt(*PANIC_FMT);

	#[cfg(target_arch = "x86")]
	unsafe {
		asm!("cli");
	}

	vprintf!("\n==============================");
	vprintf!("\n-------- KERNEL PANIC --------");
	vprintf!("\n==============================");
	vprintf!("\n\n");
	vprintf!("{}", info);
	vprintf!("\n\nSystem halted.");
	loop {}
}
