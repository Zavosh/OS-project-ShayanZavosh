# ~/.profile: executed by the command interpreter for login shells.
# This file is not read by bash(1), if ~/.bash_profile or ~/.bash_login
# exists.
# see /usr/share/doc/bash/examples/startup-files for examples.
# the files are located in the bash-doc package.

# the default umask is set in /etc/profile; for setting the umask
# for ssh logins, install and configure the libpam-umask package.
#umask 022

# if running bash
if [ -n "$BASH_VERSION" ]; then
    # include .bashrc if it exists
    if [ -f "$HOME/.bashrc" ]; then
        . "$HOME/.bashrc"
    fi
fi

# set PATH so it includes user's private bin directories
PATH="$HOME/bin:$HOME/.local/bin:$PATH"

if [ -d "$HOME/bin" ]; then
        PATH="$HOME/bin:$PATH"
fi

if [ -d "/media/os96971/SAMSUNG/os-project/phase2/step3/Android/Sdk/platform-tools" ] ; then
        PATH="/media/os96971/SAMSUNG/os-project/phase2/step3/Android/Sdk/platform-tools:$PATH"
fi

if [ -d "/media/os96971/SAMSUNG/os-project/phase2/step3/android/lineage" ] ; then
        PATH="/media/os96971/SAMSUNG/os-project/phase2/step3/android/lineage:$PATH"
fi
