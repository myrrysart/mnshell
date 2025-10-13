# Session 1: basic navigation and commands
pwd
cd /home/jyniemit/projects/mnshell-worktrees/main/.ms_manual
pwd
/bin/ls
cd sub/dir && /bin/pwd
cd .. && pwd
cd . && echo dot-ok
# try cd - (may be unsupported)
cd -
echo after-cdminus:$PWD
# PATH tests
unset PATH
ls
echo pathless:$?
export PATH=/bin:/usr/bin
ls > out.txt
cat < out.txt | grep inner | wc -l
# quoting and variables
echo hello "world"
echo "$USER"
echo "\$USER"
# status code
/bin/sh -c 'exit 42'
echo exitcode:$?
# pipes
/bin/echo alpha | /usr/bin/grep alpha | wc -l
cat file.txt | grep beta | wc -l
# redirections append
/bin/echo one > tmp.txt
/bin/echo two >> tmp.txt
cat tmp.txt
# relative path exec
./lslink -1
exit
