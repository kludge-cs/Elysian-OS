# Elysian OS (Microkernel)

This is a completely Rustic rewrite of an unfinished microkernel,
originally authored by [Joshua Pritsker]. At the moment, the
intention of this project is to continue its development and
eventually to incorporate it as part of a wider operating system.

I am doing this to continue the work of an old friend, better learn
Rust, and get into systems development.

Instructions for usage:
1. Switch to Nightly Rust
2. Use `cargo install bootimage` to acquire the boot image builder
3. `cargo build`
4. Use `bochs` or `qemu` to boot the generated image (always `qemu` if you are
   actively working on Elysian and intend to open a PR, since our test suite
   is designed to run on `qemu`).

## License

The 2021-onwards full rewrite of Elysian's microkernel is licensed
under the terms specified at [`LICENSE`] in the root of this
repository. The license for the original unfinished microkernel is
preserved at [`LICENSE-ORIGINAL`] - credit to Joshua Pritsker, an old
friend of mine and a brilliant scientist.

[LICENSE]: LICENSE
[LICENSE-ORIGINAL]: LICENSE-ORIGINAL
[Joshua Pritsker]: https://github.com/the-sushi
