from posix.stdio cimport open_memstream
from libc.stdio cimport FILE, fclose
from libc.stdint cimport uint8_t
from libc.stdlib cimport free

cdef extern void rawshark_init()
cdef extern void rawshark_clean()
cdef extern void rawshark_process_packet(uint8_t *data, int len, FILE *file)


def load():
    rawshark_init()

def unload():
    rawshark_clean()

def dissect(data):
    cdef char *buf = NULL;
    cdef size_t buflen = 0;
    cdef FILE *out = open_memstream(&buf, &buflen)

    rawshark_process_packet(data, len(data), out)
    fclose(out)

    if buflen > 4:
        json = buf[4:buflen]

    free(buf)
    return json.decode()
