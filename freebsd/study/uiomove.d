#pragma D option quiet
#pragma D option defaultargs
#pragma D option switchrate=10hz
#pragma D option bufsize=8m

fbt:kernel:vn_io_fault_uiomove:entry,
fbt:kernel:uiomove_fromphys:entry,
fbt:kernel:vn_io_fault_pgmove:entry
/pid != $pid && execname == "a.out"/
{
        printf("%s entry %d\n", probefunc, timestamp);
}
fbt:kernel:vn_io_fault_uiomove:return,
fbt:kernel:uiomove_fromphys:return,
fbt:kernel:vn_io_fault_pgmove:return
/pid != $pid && execname == "a.out"/
{
        printf("%s return %d %d\n", probefunc, timestamp, arg1);
}
