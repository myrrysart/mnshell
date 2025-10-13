echo -- START
pwd
cd /home/jyniemit/projects/mnshell-worktrees/main/.ms_manual
pwd
/bin/ls -1
cd sub/dir
/bin/pwd
cd ..
pwd
cd .
echo dot-ok

echo -- PATH tests
unset PATH
ls
echo rc:$?
export PATH=/bin:/usr/bin
/bin/ls -1 | /usr/bin/head -n 3

echo -- REDIRS
/bin/echo one > t1
/bin/echo two >> t1
cat t1

/bin/echo -- PIPES
/bin/echo alpha | /usr/bin/grep alpha | wc -l

/bin/echo -- STATUS CODES
/bin/sh -c 'exit 7'
echo rc:$?
/bin/false
echo rc:$?
/bin/true
echo rc:$?

/bin/echo -- QUOTES
echo "$USER"
echo "\$USER"

/bin/echo -- RELPATH
./lslink -1 | /usr/bin/head -n 1

exit
