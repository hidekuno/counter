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

fbt::bufwrite:entry,
fbt::bdwrite:entry
/pid != $pid && execname == "a.out"/
{
        printf("entry %d %d %d 0x%08X %d\n", timestamp, args[0]->b_bufsize, args[0]->b_bcount, args[0]->b_flags, args[0]->b_vp->v_type & 0x000000ff);
}
fbt::bufwrite:return,
fbt::bdwrite:return
/pid != $pid && execname == "a.out"/
{
        printf("return %d\n", timestamp);
}

fbt:kernel:ffs_truncate:entry
/pid != $pid && execname == "a.out"/
{
        printf("entry %d %d\n",timestamp,arg1);
}
fbt:kernel:uiomove_fromphys:return,
fbt:kernel:vm_fault:return
/pid != $pid && execname == "a.out"/
{
        printf("return %d %d\n", timestamp, arg1);
}
fbt:kernel:pmap_enter:entry,
fbt:kernel:vm_pager_get_pages:entry,
fbt:kernel:vn_io_fault1:entry,
fbt:kernel:ufs_strategy:entry,
vfs:vop:vop_strategy:entry,
fbt:kernel:bufstrategy:entry,
fbt:kernel:uiomove_fromphys:entry,
fbt:kernel:vm_fault:entry,
fbt:kernel:ffs_update:entry,
fbt:kernel:ffs_balloc_ufs2:entry,
fbt:kernel:bwait:entry
/pid != $pid && execname == "a.out"/
{
        printf("entry %d\n", timestamp);
}
fbt:kernel:pmap_enter:return,
fbt:kernel:vm_pager_get_pages:return,
fbt:kernel:vn_io_fault1:return,
fbt:kernel:ufs_strategy:return,
vfs:vop:vop_strategy:return,
fbt:kernel:bufstrategy:return,
fbt:kernel:ffs_truncate:return,
fbt:kernel:ffs_update:return,
fbt:kernel:ffs_balloc_ufs2:return,
fbt:kernel:bwait:return
/pid != $pid && execname == "a.out"/
{
        printf("return %d\n", timestamp);
}

/***
fbt:kernel:vfs_bio_clrbuf:entry,
fbt:kernel:vfs_bio_clrbuf:return,
fbt:kernel:pmap_copy_pages:entry,
fbt:kernel:pmap_copy_pages:return,
fbt:kernel:vm_pager_allocate:entry,
fbt:kernel:vm_pager_allocate:return,
fbt:kernel:vm_page_alloc:entry,
fbt:kernel:vm_page_alloc:return,
***/
