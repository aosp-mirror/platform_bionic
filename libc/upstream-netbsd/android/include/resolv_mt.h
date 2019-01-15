#pragma once

// Android never enabled the per-thread code, so this was always static like glibc.
static char inet_nsap_ntoa_tmpbuf[255*3];
