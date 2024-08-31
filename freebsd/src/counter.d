#pragma D option quiet
#pragma D option defaultargs
#pragma D option switchrate=10hz
#pragma D option bufsize=8m

syscall::read:entry,
fbt:counter:counter_read:entry,
fbt:kernel:devfs_read_f:entry,
fbt:kernel:dofileread:entry,
fbt:kernel:fget_read:entry
/pid != $pid && execname == "cat"/
{
        printf("%s entry %d\n", probefunc, timestamp);
}

syscall::read:return,
fbt:counter:counter_read:return,
fbt:kernel:devfs_read_f:return,
fbt:kernel:dofileread:return,
fbt:kernel:fget_read:return
/pid != $pid && execname == "cat"/
{
        printf("%s return %d %d\n", probefunc, timestamp, arg1);
}
