websvr
======
基于异步网络编程实现Web server，支持http

# Install tips

## lib configure

### evlib
```bash
tar zxf libevent-1.4.14b-stable.tar.gz
cd libevent-1.4.14b-stable/
./configure --prefix=~/websvr/src/evlib --disable-shared
make && make install

# add internal struct
cp libevent-1.4.14b-stable/http-internal.h ~/websvr/src/evlib/include
```

### cfglib
```bash
tar zxf libconfig-1.4.9.tar.gz
 cd libconfig-1.4.9/
 ./configure --prefix=~/websvr/src/cfglib --disable-shared
 make && make install
```
