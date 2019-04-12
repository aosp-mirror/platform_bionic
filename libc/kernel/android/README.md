The files under the uapi directory are android kernel uapi header files that
exist in android kernels, but have not been upstreamed into the regular
kernel.

None of these files are updated automatically, and are frozen at their
current value.

The files under the scsi directory are frozen copies of kernel scsi headers.
Linux's scsi headers are a mix of userspace-facing and kernel-facing
declarations that can't be directly used by userspace. The glibc
maintainers manually copy-and-pasted these definitions into their own
scsi headers and haven't substantially updated them in 15 years. The
musl libc project has a similar set of definitions in its scsi headers.

These files are actually maintained in `external/kernel-headers/modified/scsi/`.
Any modification should first be made there then copied into the scsi
directory.
