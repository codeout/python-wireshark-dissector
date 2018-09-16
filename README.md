# python-wireshark-dissector

Sample implementation of python native extension of wireshark's dissector.


## How to try

### docker

```shell
$ git clone https://github.com/codeout/python-wireshark-dissector
$ cd python-wireshark-dissector
$ docker build . -t wireshark
$ docker run --rm -it -v `pwd`:/tmp/wireshark wireshark bash

root@1f7c9736e117:/# apt-get source libwireshark-dev
root@1f7c9736e117:/# vi setup.py   # Update WIRESHARK_VERSION
root@1f7c9736e117:/# cd test
root@1f7c9736e117:/# ./pywireshark.sh
```

### vagrant

```
$ vagrant up
$ vagrant ssh

vagrant@ubuntu-bionic:~$ cd /tmp/wireshark/
vagrant@ubuntu-bionic:/tmp/wireshark$ apt-get source libwireshark-dev
vagrant@ubuntu-bionic:/tmp/wireshark$ cd test
vagrant@ubuntu-bionic:/tmp/wireshark$ ./pywireshark.sh
```


## :bulb: TIPS

### wiresahrk-2.4.5

Disable lua as a workaround of https://www.wireshark.org/lists/wireshark-bugs/201602/msg00951.html

```diff
--- /etc/wireshark/init.lua.orig        2018-09-15 17:52:29.607764385 +0000
+++ /etc/wireshark/init.lua     2018-09-15 17:52:56.359764958 +0000
@@ -24,7 +24,7 @@
 -- Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 -- Set disable_lua to true to disable Lua support.
-disable_lua = true
+disable_lua = false

 if disable_lua then
     return
```


## Copyright and License

Copyright (c) 2018 Shintaro Kojima. Code released under the [GNU GENERAL PUBLIC LICENSE version 2](LICENSE.txt).
