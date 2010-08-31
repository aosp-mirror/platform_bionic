#ifndef _SYS_EVENTFD_H_
#define _SYS_EVENTFD_H_

#include <sys/cdefs.h>
#include <asm/unistd.h>
#include <asm/termbits.h>

__BEGIN_DECLS

extern int eventfd(int count, int flags);

__END_DECLS

#endif /*  _SYS_EVENTFD_H_ */
