use lazy_static::lazy_static;
use spin::Mutex;
use uart_16550::SerialPort;

macro_rules! serial_mutex {
	($port_num:literal) => {{
		let mut port = unsafe { SerialPort::new($port_num) };
		port.init();
		Mutex::new(port)
	}};
}

lazy_static! {
	pub static ref SER_1: Mutex<SerialPort> = serial_mutex!(0x3F8);
}

#[macro_export]
macro_rules! ser_vprintf {
	($ser:ident, $($arg:tt)*) => {
		$crate::io::printf($ser.lock(), format_args!($($arg)*))
	};
}
