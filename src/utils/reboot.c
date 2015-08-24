#include <abi.h>
#include <fudge.h>

void main(void)
{

    if (!call_walk(CALL_L0, CALL_PR, 12, "system/reset"))
        return;

    call_open(CALL_PO);
    call_write(CALL_PO, 23, "System is rebooting...\n");
    call_close(CALL_PO);
    call_open(CALL_L0);
    call_write(CALL_L0, 1, "1");
    call_close(CALL_L0);

}

