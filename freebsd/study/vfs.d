#pragma D option quiet
#pragma D option defaultargs
#pragma D option switchrate=123hz
#pragma D option bufsize=8m
#pragma D option dynvarsize=32m

syscall::write:entry
/pid != $pid && execname == "a.out"/
{
        printf("entry %d 0x%p %d\n",timestamp,arg1, arg2);
}
syscall::write:return
/pid != $pid && execname == "a.out"/
{
        printf("return %d %d\n", timestamp, arg1);
}
vfs::vop_write:entry
/pid != $pid && execname == "a.out"/
{
        self->bytes[stackdepth] = args[1]->a_uio->uio_resid;
        printf("entry %d\n", timestamp);
}
vfs::vop_write:return
/pid != $pid && execname == "a.out"/
{
        l = self->bytes[stackdepth] - args[1]->a_uio->uio_resid;
        printf("return %d %d\n", timestamp, l);
        self->bytes[stackdepth] = 0;
}

fbt::bufwrite:entry
/pid != $pid && execname == "a.out"/
{
        printf("entry %d %d 0x%08X\n", timestamp, args[0]->b_bcount, args[0]->b_flags);
}
fbt::bufwrite:return
/pid != $pid && execname == "a.out"/
{
        printf("return %d %d\n", timestamp, arg1);
}

fbt::bdwrite:entry
/pid != $pid && execname == "a.out"/
{
        printf("entry %d %d\n", timestamp, args[0]->b_bcount);
}
fbt::bdwrite:return
/pid != $pid && execname == "a.out"/
{
        printf("return %d\n", timestamp);
}

fbt:kernel:ufs_strategy:entry,
vfs:vop:vop_strategy:entry,
fbt:kernel:bufstrategy:entry
/pid != $pid && execname == "a.out"/
{
        printf("entry %d\n", timestamp);
}
fbt:kernel:ufs_strategy:return,
vfs:vop:vop_strategy:return,
fbt:kernel:bufstrategy:return
/pid != $pid && execname == "a.out"/
{
        printf("return %d\n", timestamp);
}
fbt:kernel:cluster_write:entry,
fbt:kernel:ffs_truncate:entry,
fbt:kernel:ffs_update:entry,
fbt:kernel:ffs_balloc_ufs2:entry,
fbt:kernel:vfs_busy_pages:entry,
fbt:kernel:bwait:entry
/pid != $pid && execname == "a.out"/
{
        printf("entry %d\n", timestamp);
}
fbt:kernel:cluster_write:return,
fbt:kernel:ffs_truncate:return,
fbt:kernel:ffs_update:return,
fbt:kernel:ffs_balloc_ufs2:return,
fbt:kernel:vfs_busy_pages:return,
fbt:kernel:bwait:return
/pid != $pid && execname == "a.out"/
{
        printf("return %d\n", timestamp);
}