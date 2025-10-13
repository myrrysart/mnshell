# Session 3: more checks
cd /home/jyniemit/projects/mnshell-worktrees/main/.ms_manual
pwd
# relative exec of script
./script1.sh
# wildcard expansion in current dir
echo *.ms | /usr/bin/tr ' ' '\n' | /usr/bin/sort | /usr/bin/head -n 3
# PATH precedence check
mkdir -p d1 d2
/bin/echo -e '#!/bin/sh\necho from-d1' > d1/foo && chmod +x d1/foo
/bin/echo -e '#!/bin/sh\necho from-d2' > d2/foo && chmod +x d2/foo
export PATH=/home/jyniemit/projects/mnshell-worktrees/main/.ms_manual/d1:/home/jyniemit/projects/mnshell-worktrees/main/.ms_manual/d2:/bin:/usr/bin
foo
echo rc:$?
export PATH=/home/jyniemit/projects/mnshell-worktrees/main/.ms_manual/d2:/home/jyniemit/projects/mnshell-worktrees/main/.ms_manual/d1:/bin:/usr/bin
foo
# heredoc test
cat << EOF
hello
world
EOF
# && and ||
/bin/true && echo should-not-be-parsed
/bin/false || echo also-not-parsed
# quoting
/bin/echo SQUOTE:'$USER'
/bin/echo DQ-ESC:"\$USER"
exit
