/** RaspOS
 * Copyright (C) 2022  Benny Henning
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Contact: benny.henning@web.de
 */

.section ".text.boot"
 
// Make _start global.
.globl _start

// AArch64 mode
    .org 0x80000
// Entry point for the kernel. Registers:
// x0 -> 32 bit pointer to DTB in memory (primary core only) / 0 (secondary cores)
// x1 -> 0
// x2 -> 0
// x3 -> 0
// x4 -> 32 bit kernel entry point, _start location
_start:
	// Shut off extra cores
	mrs x5, MPIDR_EL1
	and x5, x5, #3
	cmp x5, #0
	bne halt

    // set stack before our code
    ldr     x5, =_start
    mov     sp, x5
 
    // clear bss
    ldr     x5, =__bss_start
    ldr     w6, =__bss_size
1:  cbz     w6, 2f
    str     xzr, [x5], #8
    sub     w6, w6, #1
    cbnz    w6, 1b
 
    // jump to C code, should not return
2:  bl      kernel_main
   // for failsafe, halt this core too
halt:
    wfe
    b halt
