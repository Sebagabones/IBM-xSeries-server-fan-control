there are sooo many bugs in this - don't use this yet
~~~
So! I had some free time and made a new version of my sketchy attempt at making my x3550 M4 be a bit less loud (although this current code is actually for an x3650 M4 - the process is pretty much the same - you may need to tweak this a little for other servers (I also may make custom modes - will see)

anyway - this is much more user friendly - only two(!) files this time!

You'll need gcc, ipmitools, and maybe a few other dependencies (as with last time - in the interest of not blowing up peoples servers - I'm not going to make this super easy to follow - if you can follow this you can probably use this lol - but please don't use this in any mission critical applications)

anyway, clone the repo, compile the tempChecker.c with gcc -o, and then add the path to the tempChecker executable to the systemd service, (and move the systemd file to `/etc/systemd/system/` then enable it and you *should* be good!

there is a verbose mode and a dryrun mode if you really want (and a fan noise testing - see -h) - and this will work with different cpu's in theory

good luck!
~~~
