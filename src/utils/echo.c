#include <abi.h>
#include <fudge.h>

void main()
{

    unsigned char buffer[FUDGE_BSIZE];
    unsigned int rcount, roff, wcount, woff;

    call_open(CALL_PO);
    call_open(CALL_P0);

    for (roff = 0; (rcount = call_read(CALL_P0, roff, 1, FUDGE_BSIZE, buffer)); roff += rcount)
        for (woff = roff; (wcount = call_write(CALL_PO, woff, 1, roff + rcount - woff, buffer)); woff += wcount);

    call_close(CALL_P0);
    call_close(CALL_PO);

}
