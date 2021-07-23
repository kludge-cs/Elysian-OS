pub mod general;

/*
 * This is a proof of concept currently. Eventually, this will enable the kernel
 * to dynamically compile in architecture exclusive initialization for the
 * target and re-export it to the rest of the kernel through a general interface
 * similar to the linker hackery used in the original Elysian microkernel.
 */

/*
#[cfg(target_arch = "x86")]
pub mod x86;
#[cfg(target_arch = "x86")]
pub use x86::arch::Architecture as UseArch;

#[cfg(target_arch = "x86_64")]
pub mod x86_64;
#[cfg(target_arch = "x86_64")]
pub use x86_64::arch::Architecture as UseArch;
*/
