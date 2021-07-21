#![allow(dead_code)]

use core::fmt;
use lazy_static::lazy_static;
use spin::Mutex;
use x86_64::instructions::port::Port;

const BUF_HEIGHT: usize = 25;
const BUF_WIDTH: usize = 80;
const BUF_START: usize = 0xB8000;
// const BUF_START: usize = 0xC00B8000;

// NOTE: all x offsets must be 2x because 1 char = 2 bytes
const fn idx(x: usize, y: usize) -> usize {
	(y * 160) + (x * 2)
}

const fn buf_ptr_at_idx(idx: usize) -> *const u16 {
	(BUF_START + idx) as *const u16
}

const fn buf_mut_ptr_at_idx(idx: usize) -> *mut u16 {
	(BUF_START + idx) as *mut u16
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u8)]
pub enum Colour {
	Black = 0x0,
	Blue = 0x1,
	Green = 0x2,
	Cyan = 0x3,
	Red = 0x4,
	Magenta = 0x5,
	Brown = 0x6,
	LGray = 0x7,
}

/*
 * ARCHITECTURE OF VGA CHARACTER:
 * +==============+
 * |  P-437 Code  | 0
 * +--------------+
 * |  Bright Bit  | 8
 * +--------------+
 * |   FGColour   | 9
 * +--------------+
 * |   BGColour   | 12
 * +--------------+
 * |   Blinking   | 15
 * +==============+
 */

pub fn fmt_byte_from(
	fg_colour: Colour,
	bright: bool,
	bg_colour: Colour,
	blink: bool,
) -> u8 {
	(fg_colour as u8)
		| ((bright as u8) << 3)
		| ((bg_colour as u8) << 4)
		| ((blink as u8) << 7)
}

pub fn char_from(p437_char: u8, fmt: u8) -> u16 {
	((fmt as u16) << 8) | p437_char as u16
}

pub fn split_char(chr: u16) -> [u8; 2] {
	chr.to_le_bytes()
}

pub struct VGATextDriver {
	col: usize,
	row: usize,
	fmt: u8,
}

impl VGATextDriver {
	fn raw_putch_at(&mut self, pos: [usize; 2], chr: u8, fmt: Option<u8>) {
		let [x, y] = pos;

		if x >= BUF_WIDTH || y >= BUF_HEIGHT {
			panic!("BUF_OVERFLOW: Attempting raw_putch_at '{}' ({}, {}) overshoots VGA memory.", chr, x, y);
		}

		unsafe {
			*buf_mut_ptr_at_idx(idx(x, y)) =
				char_from(chr, fmt.unwrap_or(self.fmt));
		}
	}

	fn raw_putch(&mut self, chr: u8, fmt: Option<u8>) {
		self.raw_putch_at([self.col, self.row], chr, fmt.or(Some(self.fmt)));
	}

	fn raw_getch_at(&mut self, pos: [usize; 2]) -> u16 {
		let [x, y] = pos;

		if x >= BUF_WIDTH || y >= BUF_HEIGHT {
			panic!("BUF_OVERFLOW: Attempting raw_getch_at ({}, {}) overshoots VGA memory.", x, y);
		}

		unsafe { *buf_ptr_at_idx(idx(x, y)) }
	}

	pub fn putch(&mut self, byte: u8) {
		match byte {
			b'\n' => self.new_line(),
			b'\r' => self.col = 0,
			byte => {
				if self.col >= BUF_WIDTH {
					self.new_line();
				}

				self.raw_putch(byte, None);
				self.col += 1;
			}
		}
	}

	pub fn puts(&mut self, s: &str) {
		for byte in s.bytes() {
			match byte {
				0x20..=0x7e | b'\n' | b'\r' => self.putch(byte),
				_ => self.putch(0xFE),
			}
		}
	}

	fn new_line(&mut self) {
		self.col = 0;
		if self.row == BUF_HEIGHT - 1 {
			self.shift_up();
			self.clear_line(self.row);
			return;
		}
		return self.row += 1;
	}

	fn shift_up(&mut self) {
		self.clear_line(0);
		let start = idx(0, 1);
		unsafe {
			core::ptr::copy(
				buf_ptr_at_idx(start),
				buf_mut_ptr_at_idx(0),
				idx(BUF_WIDTH, self.row) - start,
			)
		};
	}

	pub fn clear_line(&mut self, row: usize) {
		for col in 0..BUF_WIDTH {
			self.raw_putch_at([col, row], b' ', Some(*DEFAULT_FMT))
		}
	}

	pub fn set_fmt(&mut self, fmt: u8) {
		self.fmt = fmt;
	}

	pub fn clear_screen(&mut self) {
		for row in 0..BUF_HEIGHT {
			self.clear_line(row);
		}
		self.col = 0;
		self.row = 0;
		self.update_curs();
	}

	pub fn update_curs(&self) {
		let pos_idx = idx(self.col, self.row) as u32;
		let mut cmd_reg = Port::new(0x3D4);
		let mut data_reg = Port::new(0x3D5);
		unsafe {
			// TODO: debug - cursor is left untouched by these writes

			cmd_reg.write(0x0Eu32); // cursor high register
			data_reg.write(pos_idx >> 8);
			cmd_reg.write(0x0Fu32); // cursor low register
			data_reg.write(pos_idx);
		}
	}
}

impl fmt::Write for VGATextDriver {
	fn write_str(&mut self, s: &str) -> fmt::Result {
		Ok(self.puts(s))
	}

	fn write_char(&mut self, c: char) -> fmt::Result {
		Ok(self.putch(c as u8))
	}
}

lazy_static! {
	pub static ref PANIC_FMT: u8 =
		fmt_byte_from(Colour::Red, false, Colour::Black, false);
	pub static ref DEFAULT_FMT: u8 =
		fmt_byte_from(Colour::LGray, false, Colour::Black, false);
	pub static ref VGA_TERM: Mutex<VGATextDriver> = Mutex::new(VGATextDriver {
		col: 0,
		row: 0,
		fmt: *DEFAULT_FMT,
	});
}

#[macro_export]
macro_rules! vprintf {
	($term:ident, $($arg:tt)*) => ($crate::io::printf($term.lock(), format_args!($($arg)*)));
	($($arg:tt)*) => ($crate::io::printf($crate::io::vga::VGA_TERM.lock(), format_args!($($arg)*)));
}

fn luce_stellarum() {
	// Original testing function for Elysian's VGA driver. Do not touch.

	let mut term = VGA_TERM.lock();

	term.putch(b'I');
	term.puts(" love you");
	term.puts(" Keisha! ");
	term.set_fmt(fmt_byte_from(Colour::Magenta, true, Colour::Black, false));
	term.puts("<3");
}
