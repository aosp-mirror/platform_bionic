#include <sys/eventfd.h>
#include <unistd.h>

/* We duplicate the GLibc error semantics, which are poorly defined
 * if the read() or write() does not return the proper number of bytes.
 */
int eventfd_read(int fd, eventfd_t *counter)
{
    int ret = read(fd, counter, sizeof(*counter));

    if (ret == sizeof(*counter))
        return 0;

    return -1;
}

int eventfd_write(int fd, eventfd_t counter)
{
    int ret = write(fd, &counter, sizeof(counter));

    if (ret == sizeof(counter))
        return 0;

    return -1;
}
