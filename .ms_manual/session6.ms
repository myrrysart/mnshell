cd /home/jyniemit/projects/mnshell-worktrees/main/.ms_manual
/bin/echo -- verify contents
echo START; /bin/cat -n d1/foo; /bin/ls -l d1/foo
/bin/echo -- abs exec
d1/foo
/home/jyniemit/projects/mnshell-worktrees/main/.ms_manual/d1/foo
/bin/sh -c '/home/jyniemit/projects/mnshell-worktrees/main/.ms_manual/d1/foo'
/bin/echo -- via PATH
export PATH=/home/jyniemit/projects/mnshell-worktrees/main/.ms_manual/d1:/bin:/usr/bin
foo
exit
