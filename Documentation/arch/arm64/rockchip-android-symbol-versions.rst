Android 17 Rockchip exported-symbol versions
============================================

This builds on dvab-sarma's Android RK3588/RK3588S kernel support.
Android 17 GKI uses DWARF-based export CRC generation. Enable
CONFIG_GENDWARFKSYMS and disable the legacy GENKSYMS choice when building
with the matching Android Clang toolchain and debug information.
The host needs the libdw/libelf development headers (libdw-dev and
libelf-dev on Debian/Ubuntu).

This avoids zero/missing CRCs reported by legacy genksyms for exports such
as cpufreq_frequency_table_verify, inet_rcv_saddr_equal,
inet_sk_get_local_port_range and inet_csk_get_port. The change applies to
the shared Rockchip configuration, independent of CM5 pin wiring.
