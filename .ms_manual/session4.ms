cd /home/jyniemit/projects/mnshell-worktrees/main/.ms_manual
pwd
./lslink -1 | /usr/bin/head -n 3
/bin/echo -e '#!/bin/sh\necho READY' > s2.sh
chmod +x s2.sh
./s2.sh
echo rc:$?
exit
