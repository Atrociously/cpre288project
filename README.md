# cpre288project

## How to download
1. Open file explorer on U drive
2. Right click and select `Git Bash Here`
3. In git bash window run `mkdir -p /u/.ssh/`
4. In same window run `ssh-keygen` and just hit enter until done
5. Now run `cat /u/.ssh/id_rsa.pub` and copy the output from that command
6. Then go your github settings, in SSH and GPG keys
7. Select new ssh key name it whatever you want and paste the output into key
8. Now you can run `git clone git@github.com:Atrociously/cpre288project.git`
9. You now have access to the repo
