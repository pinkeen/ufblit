ufblit
======

An exercise to create an ultra-fast blitter without asm or any x86/x64 extensions.

Initially intended for dingoo A320 but it was slower than SDL on dingoo and faster than SDL on the PC (despite the fact that SDL used SIMD).

Well, now it's slower than SDL on my rig. Code uses the clever trick of cramming the whole color into a single 32bit variable (RGB565).
