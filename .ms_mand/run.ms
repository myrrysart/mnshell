# Mandatory-only minishell checks
# NOTE: Avoid backslash, ';', &&, ||, wildcard tests (bonus/forbidden)

# Start in project root then move to sandbox
pwd
cd /home/jyniemit/projects/mnshell-worktrees/main/.ms_mand
pwd

# 1) Builtins: cd/pwd minimal and errors
cd sub
pwd
cd ..
cd DOES_NOT_EXIST
/bin/echo cd_rc:$?

# 2) PATH resolution and left-to-right order
unset PATH
ls
/bin/echo unset_rc:$?
/bin/ls -1 | /usr/bin/head -n 1
export PATH=/bin:/usr/bin
/bin/echo PATH_OK
export PATH=/home/jyniemit/projects/mnshell-worktrees/main/.ms_mand/d1:/home/jyniemit/projects/mnshell-worktrees/main/.ms_mand/d2:/bin:/usr/bin
foo
export PATH=/home/jyniemit/projects/mnshell-worktrees/main/.ms_mand/d2:/home/jyniemit/projects/mnshell-worktrees/main/.ms_mand/d1:/bin:/usr/bin
foo

# 3) Relative/absolute execution
./mys.sh
/home/jyniemit/projects/mnshell-worktrees/main/.ms_mand/mys.sh
./lslink -1 | /usr/bin/head -n 1

# 4) Redirections and append
/bin/echo one > t1
/bin/echo two >> t1
cat t1

# 5) Pipes
/bin/echo alpha | /usr/bin/grep alpha | wc -l
cat in.txt | /usr/bin/grep -i beta | wc -l

# 6) $? for commands and pipelines
/bin/sh -c 'exit 7'
echo rc:$?
/bin/false
echo rc:$?
/bin/true
echo rc:$?
true | false
echo pipe_rc:$?
false | true
echo pipe_rc:$?

# 7) Quoting per subject (no backslash expected)
VAR=hello
export VAR
# single quotes block expansion
echo '$VAR'
# double quotes expand only $
echo "$VAR" "$?"

# 8) Here-doc minimal behavior (no history needed)
cat << EOF
line1
line2
EOF

exit
