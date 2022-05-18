#include "regfile.h"

RegFile::RegFile()
{
    // In a straightforward implementation, reading or writing any field in
    // sstatus is equivalent to reading or writing the homonymous field in
    // mstatus.
    s_csr_regs_[CSR_S::sstatus] = CSR({
        // WIRI since extensions sre disabled for our emulator
        { "SD", { 0b10000000000000000000000000000000, CSR::Spec::WIRI, 31 } },
        // The MXR (Make eXecutable Readable) bit modifies the privilege
        // with which loads access virtual memory. When MXR=0, only loads
        // from pages marked readable (R=1 in Figure 4.18) will succeed.
        // When MXR=1, loads from pages marked either readable or
        // executable (R=1 or X=1) will succeed. MXR has no effect when
        // page-based virtual memory is not in effect.
        { "MXR", { 0b00000000000010000000000000000000, CSR::Spec::NONE, 19 } },
        // The SUM (permit Supervisor User Memory access) bit modifies the
        // privilege with which S-mode loads and stores access virtual
        // memory. When SUM=0, S-mode memory accesses to pages that are
        // accessible by U-mode (U=1 in Figure 4.18) will fault. When
        // SUM=1, these accesses are permitted. SUM has no effect when
        // page-based virtual memory is not in effect, nor when executing
        // in U-mode. Note that S-mode can never execute instructions from
        // user pages, regardless of the state of SUM. SUM is read-only 0
        // if satp.MODE is read-only 0.
        { "SUM", { 0b00000000000001000000000000000000, CSR::Spec::NONE, 18 } },
        // WIRI since extensions sre disabled for our emulator
        { "XS", { 0b00000000000000011000000000000000, CSR::Spec::WIRI, 15 } },
        // WIRI since extensions sre disabled for our emulator
        { "FS", { 0b00000000000000000110000000000000, CSR::Spec::WIRI, 13 } },
        // WIRI since extensions sre disabled for our emulator
        { "VS", { 0b00000000000000000000011000000000, CSR::Spec::WIRI, 9 } },
        // The SPP bit indicates the privilege level at which a hart was
        // executing before entering supervisor mode. When a trap is taken,
        // SPP is set to 0 if the trap originated from user mode, or 1
        // otherwise. When an SRET instruction (see Section 3.3.2) is
        // executed to return from the trap handler, the privilege level is
        // set to user mode if the SPP bit is 0, or supervisor mode if the
        // SPP bit is 1; SPP is then set to 0.
        { "SPP", { 0b00000000000000000000000100000000, CSR::Spec::WARL, 8 } },
        // The UBE bit is a WARL field that controls the endianness of
        // explicit memory accesses made from U-mode, which may differ from
        // the endianness of memory accesses in S-mode. An implementation
        // may make UBE be a read-only field that always specifies the same
        // endianness as for S-mode. UBE controls whether explicit load and
        // store memory accesses made from U-mode are little-endian (UBE=0)
        // or big-endian (UBE=1). UBE has no effect on instruction fetches,
        // which are implicit memory accesses that are always
        // little-endian. For implicit accesses to supervisor-level memory
        // management data structures, such as page tables, S-mode
        // endianness always applies and UBE is ignored.
        { "UBE", { 0b00000000000000000000000001000000, CSR::Spec::WARL, 6 } },
        // The SPIE bit indicates whether supervisor interrupts were
        // enabled prior to trapping into supervisor mode. When a trap is
        // taken into supervisor mode, SPIE is set to SIE, and SIE is set
        // to 0. When an SRET instruction is executed, SIE is set to SPIE,
        // then SPIE is set to 1.
        { "SPIE", { 0b00000000000000000000000000100000, CSR::Spec::NONE, 5 } },
        // The SIE bit enables or disables all interrupts in supervisor
        // mode. When SIE is clear, interrupts are not taken while in
        // supervisor mode. When the hart is running in user-mode, the
        // value in SIE is ignored, and supervisor-level interrupts are
        // enabled. The supervisor can disable individual interrupt sources
        // using the sie CSR.
        { "SIE", { 0b00000000000000000000000000000010, CSR::Spec::NONE, 1 } },
    });

    m_csr_regs_[CSR_M::mstatus] = CSR({
        // WIRI since extensions sre disabled for our emulator
        { "SD", { 0b10000000000000000000000000000000, CSR::Spec::WIRI, 31 } },
        { "TSR", { 0b00000000010000000000000000000000, CSR::Spec::NONE, 22 } },
        { "TW", { 0b00000000001000000000000000000000, CSR::Spec::NONE, 21 } },
        // If S-mode is not supported, SBE is read-only 0. Otherwise, SBE
        // controls whether explicit load and store memory accesses made from
        // S-mode are little-endian (SBE=0) or big-endian (SBE=1).
        { "TVM", { 0b00000000000100000000000000000000, CSR::Spec::NONE, 20 } },
        // The MXR (Make eXecutable Readable) bit modifies the privilege
        // with which loads access virtual memory. When MXR=0, only loads
        // from pages marked readable (R=1 in Figure 4.18) will succeed.
        // When MXR=1, loads from pages marked either readable or
        // executable (R=1 or X=1) will succeed. MXR has no effect when
        // page-based virtual memory is not in effect.
        { "MXR", { 0b00000000000010000000000000000000, CSR::Spec::NONE, 19 } },
        // The SUM (permit Supervisor User Memory access) bit modifies the
        // privilege with which S-mode loads and stores access virtual
        // memory. When SUM=0, S-mode memory accesses to pages that are
        // accessible by U-mode (U=1 in Figure 4.18) will fault. When
        // SUM=1, these accesses are permitted. SUM has no effect when
        // page-based virtual memory is not in effect, nor when executing
        // in U-mode. Note that S-mode can never execute instructions from
        // user pages, regardless of the state of SUM. SUM is read-only 0
        // if satp.MODE is read-only 0.
        { "SUM", { 0b00000000000001000000000000000000, CSR::Spec::NONE, 18 } },
        // The MPRV (Modify PRiVilege) bit modifies the effective privilege
        // mode, i.e., the privilege level at which loads and stores execute.
        // When MPRV=0, loads and stores behave as normal, using the
        // translation and protection mechanisms of the current privilege mode.
        // When MPRV=1, load and store memory addresses are translated and
        // protected, and endianness is applied, as though the current
        // privilege mode were set to MPP. Instruction address-translation and
        // protection are unaffected by the setting of MPRV. MPRV is read-only
        // 0 if U-mode is not supported. An MRET or SRET instruction that
        // changes the privilege mode to a mode less privileged than M also
        // sets MPRV=0
        { "MPRV",
          { 0b00000000000000100000000000000000, CSR::Spec::NONE, 17 } },
        // WIRI since extensions sre disabled for our emulator
        { "XS", { 0b00000000000000011000000000000000, CSR::Spec::WIRI, 15 } },
        // WIRI since extensions sre disabled for our emulator
        { "FS", { 0b00000000000000000110000000000000, CSR::Spec::WIRI, 13 } },
        { "MPP", { 0b00000000000000000001100000000000, CSR::Spec::WARL, 11 } },
        // WIRI since extensions sre disabled for our emulator
        { "VS", { 0b00000000000000000000011000000000, CSR::Spec::WIRI, 9 } },
        // The SPP bit indicates the privilege level at which a hart was
        // executing before entering supervisor mode. When a trap is taken,
        // SPP is set to 0 if the trap originated from user mode, or 1
        // otherwise. When an SRET instruction (see Section 3.3.2) is
        // executed to return from the trap handler, the privilege level is
        // set to user mode if the SPP bit is 0, or supervisor mode if the
        // SPP bit is 1; SPP is then set to 0.
        { "SPP", { 0b00000000000000000000000100000000, CSR::Spec::WARL, 8 } },
        // The UBE bit is a WARL field that controls the endianness of
        // explicit memory accesses made from U-mode, which may differ from
        // the endianness of memory accesses in S-mode. An implementation
        // may make UBE be a read-only field that always specifies the same
        // endianness as for S-mode. UBE controls whether explicit load and
        // store memory accesses made from U-mode are little-endian (UBE=0)
        // or big-endian (UBE=1). UBE has no effect on instruction fetches,
        // which are implicit memory accesses that are always
        // little-endian. For implicit accesses to supervisor-level memory
        // management data structures, such as page tables, S-mode
        // endianness always applies and UBE is ignored.
        { "UBE", { 0b00000000000000000000000001000000, CSR::Spec::WARL, 6 } },
        // The SPIE bit indicates whether supervisor interrupts were
        // enabled prior to trapping into supervisor mode. When a trap is
        // taken into supervisor mode, SPIE is set to SIE, and SIE is set
        // to 0. When an SRET instruction is executed, SIE is set to SPIE,
        // then SPIE is set to 1.
        { "SPIE", { 0b00000000000000000000000000100000, CSR::Spec::NONE, 5 } },
        // Global interrupt-enable bits, MIE and SIE, are provided for
        // M-mode and S-mode respectively. These bits are primarily used to
        // guarantee atomicity with respect to interrupt handlers in the
        // current privilege mode.
        { "MIE", { 0b00000000000000000000000000001000, CSR::Spec::NONE, 3 } },
        // The SIE bit enables or disables all interrupts in supervisor
        // mode. When SIE is clear, interrupts are not taken while in
        // supervisor mode. When the hart is running in user-mode, the
        // value in SIE is ignored, and supervisor-level interrupts are
        // enabled. The supervisor can disable individual interrupt sources
        // using the sie CSR.
        { "SIE", { 0b00000000000000000000000000000010, CSR::Spec::NONE, 1 } },
    });

    m_csr_regs_[CSR_M::mstatush] = CSR({
        // MBE controls whether non-instruction-fetch memory accesses made from
        // M-mode (assuming
        // mstatus.MPRV=0) are little-endian (MBE=0) or big-endian (MBE=1).
        { "MBE", { 0b00000000000000000000000000000000, CSR::Spec::NONE, 31 } },
        // If S-mode is not supported, SBE is read-only 0. Otherwise, SBE
        // controls whether explicit load and
        // store memory accesses made from S-mode are little-endian (SBE=0) or
        // big-endian (SBE=1).
        { "SBE", { 0b00000000000000000000000000000000, CSR::Spec::NONE, 31 } },
    });
}