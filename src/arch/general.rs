// infix elysian typedefs. these will likely be removed from general.
pub type schr = i8;
pub type uchr = u8;
pub type sshort = i16;
pub type ushort = u16;
pub type sint = i32;
pub type uint = u32;
pub type slong = i64;
pub type ulong = u64;

pub trait Arch {
	fn init();
}

pub struct Paging {}

pub struct VMem {}
