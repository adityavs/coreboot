/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 	
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */
/* This code taken from LinuxBIOS v2, which was taken from Linux */

#ifndef ARCH_X86_CPU_H
#define ARCH_X86_CPU_H

#include <types.h>

/*
 * EFLAGS bits
 */
#define X86_EFLAGS_CF	0x00000001 /* Carry Flag */
#define X86_EFLAGS_PF	0x00000004 /* Parity Flag */
#define X86_EFLAGS_AF	0x00000010 /* Auxillary carry Flag */
#define X86_EFLAGS_ZF	0x00000040 /* Zero Flag */
#define X86_EFLAGS_SF	0x00000080 /* Sign Flag */
#define X86_EFLAGS_TF	0x00000100 /* Trap Flag */
#define X86_EFLAGS_IF	0x00000200 /* Interrupt Flag */
#define X86_EFLAGS_DF	0x00000400 /* Direction Flag */
#define X86_EFLAGS_OF	0x00000800 /* Overflow Flag */
#define X86_EFLAGS_IOPL	0x00003000 /* IOPL mask */
#define X86_EFLAGS_NT	0x00004000 /* Nested Task */
#define X86_EFLAGS_RF	0x00010000 /* Resume Flag */
#define X86_EFLAGS_VM	0x00020000 /* Virtual Mode */
#define X86_EFLAGS_AC	0x00040000 /* Alignment Check */
#define X86_EFLAGS_VIF	0x00080000 /* Virtual Interrupt Flag */
#define X86_EFLAGS_VIP	0x00100000 /* Virtual Interrupt Pending */
#define X86_EFLAGS_ID	0x00200000 /* CPUID detection flag */

struct cpuid_result {
	u32 eax;
	u32 ebx;
	u32 ecx;
	u32 edx;
};
/*
 * Generic CPUID function
 */
static inline struct cpuid_result cpuid(int op)
{
	struct cpuid_result result;
	asm volatile(
		"cpuid"
		: "=a" (result.eax),
		  "=b" (result.ebx),
		  "=c" (result.ecx),
		  "=d" (result.edx)
		: "0" (op));
	return result;
}


/*
 * CPUID functions returning a single datum
 */
static inline unsigned int cpuid_eax(unsigned int op)
{
	unsigned int eax;

	__asm__("cpuid"
		: "=a" (eax)
		: "0" (op)
		: "ebx", "ecx", "edx");
	return eax;
}
static inline unsigned int cpuid_ebx(unsigned int op)
{
	unsigned int eax, ebx;

	__asm__("cpuid"
		: "=a" (eax), "=b" (ebx)
		: "0" (op)
		: "ecx", "edx" );
	return ebx;
}
static inline unsigned int cpuid_ecx(unsigned int op)
{
	unsigned int eax, ecx;

	__asm__("cpuid"
		: "=a" (eax), "=c" (ecx)
		: "0" (op)
		: "ebx", "edx" );
	return ecx;
}
static inline unsigned int cpuid_edx(unsigned int op)
{
	unsigned int eax, edx;

	__asm__("cpuid"
		: "=a" (eax), "=d" (edx)
		: "0" (op)
		: "ebx", "ecx");
	return edx;
}



#define X86_VENDOR_INVALID    0
#define X86_VENDOR_INTEL      1
#define X86_VENDOR_CYRIX      2
#define X86_VENDOR_AMD        3
#define X86_VENDOR_UMC        4
#define X86_VENDOR_NEXGEN     5
#define X86_VENDOR_CENTAUR    6
#define X86_VENDOR_RISE       7
#define X86_VENDOR_TRANSMETA  8
#define X86_VENDOR_NSC        9
#define X86_VENDOR_SIS       10 
#define X86_VENDOR_UNKNOWN 0xff

#include <device/device.h>

struct cpu_info {
	struct device cpu;
	unsigned long index;
};

struct cpuinfo_x86 {
        u8    x86;            /* CPU family */
        u8    x86_vendor;     /* CPU vendor */
        u8    x86_model;
        u8    x86_mask;
};
/**
  * Get the u32 cpuinfo information into a struct cpuinfo. 
  */
static void inline get_fms(struct cpuinfo_x86 *c, u32  tfms)
{
        c->x86 = (tfms >> 8) & 0xf;
        c->x86_model = (tfms >> 4) & 0xf;
        c->x86_mask = tfms & 0xf;
        if (c->x86 == 0xf)
                c->x86 += (tfms >> 20) & 0xff;
        if (c->x86 >= 0x6)
                c->x86_model += ((tfms >> 16) & 0xF) << 4;

}

static inline unsigned long read_cr0(void)
{
	unsigned long cr0;
	asm volatile ("movl %%cr0, %0" : "=r" (cr0));
	return cr0;
}

static inline void write_cr0(unsigned long cr0)
{
	asm volatile ("movl %0, %%cr0" : : "r" (cr0));
}

static inline void invd(void)
{
	asm volatile("invd" ::: "memory");
}
static inline void wbinvd(void)
{
	asm volatile ("wbinvd");
}

static inline void enable_cache(void)
{
	unsigned long cr0;
	cr0 = read_cr0();
	cr0 &= 0x9fffffff;
	write_cr0(cr0);
}

static inline void disable_cache(void)
{
	/* Disable and write back the cache */
	unsigned long cr0;
	cr0 = read_cr0();
	cr0 |= 0x40000000;
	wbinvd();
	write_cr0(cr0);
	wbinvd();
}


#endif /* ARCH_X86_CPU_H */
