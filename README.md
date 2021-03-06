libkernaux
==========

[![Build Status](https://travis-ci.org/kernelmq/libkernaux.svg?branch=master)](https://travis-ci.org/kernelmq/libkernaux)

Auxiliary library for kernel development.



Table of contents
-----------------

* [Overview](#libkernaux)
* [Table of contents](#table-of-contents)
* [API](#api)
* [Tips](#tips)
* [Portability](#portability)
* [Discussion](#discussion)



API
---

* [Simple command line parser](/include/kernaux/cmdline.h) *(work in progress)*
* [Multiboot 2 (GRUB 2) information parser](/include/kernaux/multiboot2.h)
* [Serial console](/include/kernaux/console.h)
* [Page Frame Allocator](/include/kernaux/pfa.h) *(work in progress)*
* ELF utils *(planned)*
* [Architecture-specific helpers](/include/kernaux/arch/)
* [printf replacement](/include/kernaux/printf.h) *(work in progress)*
* [stdlib replacement](/include/kernaux/stdlib.h):
  * `memset`
  * `strlen`
  * `strncpy`
  * `itoa` *(work in progress)*



Tips
----

Create configuration script with `./autogen.sh`.

Configure with cross-compiler in `$PATH` to make without it in `$PATH`:

```
./configure \
  --host='i386-elfkernelmq' \
  AR="$(which i386-elfkernelmq-ar)" \
  CC="$(which i386-elfkernelmq-gcc)" \
  RANLIB="$(which i386-elfkernelmq-ranlib)" \
  CFLAGS='-ffreestanding -nostdlib -fno-builtin -fno-stack-protector'
```

When configuring with cross-compiler you can see the following messages. It's
[a bug](https://savannah.gnu.org/support/index.php?110393) in **autoconf**, just
ignore it.

```
checking for _Bool... no
checking stdarg.h usability... no
checking stdarg.h presence... yes
configure: WARNING: stdarg.h: present but cannot be compiled
configure: WARNING: stdarg.h:     check for missing prerequisite headers?
configure: WARNING: stdarg.h: see the Autoconf documentation
configure: WARNING: stdarg.h:     section "Present But Cannot Be Compiled"
configure: WARNING: stdarg.h: proceeding with the compiler's result
configure: WARNING:     ## ------------------------------------------------------------ ##
configure: WARNING:     ## Report this to https://github.com/kernelmq/libkernaux/issues ##
configure: WARNING:     ## ------------------------------------------------------------ ##
checking for stdarg.h... no
checking stddef.h usability... no
checking stddef.h presence... yes
configure: WARNING: stddef.h: present but cannot be compiled
configure: WARNING: stddef.h:     check for missing prerequisite headers?
configure: WARNING: stddef.h: see the Autoconf documentation
configure: WARNING: stddef.h:     section "Present But Cannot Be Compiled"
configure: WARNING: stddef.h: proceeding with the compiler's result
configure: WARNING:     ## ------------------------------------------------------------ ##
configure: WARNING:     ## Report this to https://github.com/kernelmq/libkernaux/issues ##
configure: WARNING:     ## ------------------------------------------------------------ ##
checking for stddef.h... no
```

To install into specific directory use full path:
`DESTDIR="$(pwd)/dest" make install` instead of `DESTDIR=dest make install`.

When configured with cross-compiler, library can't be build and installed with
just `make && sudo make install`. Instead use the following commands:

* `make libkernaux.a`
* `sudo make install-exec`
* `sudo make install-data`

Check if compilation targets i386: `objdump -d src/arch/i386.o`. It should
output something like this:

```
src/arch/i386.o:     file format elf32-i386


Disassembly of section .text:

00000000 <kernaux_arch_i386_hang>:
   0:   fa                    cli
   1:   f4                    hlt
   2:   eb fc                 jmp    0 <kernaux_arch_i386_hang>

00000004 <kernaux_arch_i386_read_cr0>:
   4:   0f 20 c0              mov    %cr0,%eax
   7:   c3                    ret

00000008 <kernaux_arch_i386_read_cr4>:
   8:   0f 20 e0              mov    %cr4,%eax
   b:   c3                    ret

0000000c <kernaux_arch_i386_write_cr0>:
   c:   8b 44 24 04           mov    0x4(%esp),%eax
  10:   0f 22 c0              mov    %eax,%cr0
  13:   c3                    ret

00000014 <kernaux_arch_i386_write_cr3>:
  14:   8b 44 24 04           mov    0x4(%esp),%eax
  18:   0f 22 d8              mov    %eax,%cr3
  1b:   c3                    ret

0000001c <kernaux_arch_i386_write_cr4>:
  1c:   8b 44 24 04           mov    0x4(%esp),%eax
  20:   0f 22 e0              mov    %eax,%cr4
  23:   c3                    ret
```



Portability
-----------

Except GNU/Linux, the library is periodically successfully built (starting with
`./autogen.sh`) and tested with **autoconf**, **automake**, **binutils** and
**gcc**/**clang** (depending on what is present) on the following operating
systems:

* FreeBSD 12.2
* OpenBSD 6.8
* Minix 3.3.0
* NetBSD 9.1



Discussion
----------

* [Topic on OSDev.org forum](https://forum.osdev.org/viewtopic.php?f=1&t=37958)
* [Thread on r/osdev](https://www.reddit.com/r/osdev/comments/k3ueeu/libkernaux_auxiliary_library_for_kernel/)
