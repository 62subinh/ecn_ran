# Prerequisites

Environment setup: Ubuntu 22.04.5 LTS (GNU/Linux 6.8.0-48-generic x86_64)

## LIBBPF

```bash
# Optional: create symbolic link if "asm" is not found 
sudo ln -s /usr/src/linux-headers-`uname -r`/include/asm-generic /usr/src/linux-headers-`uname -r`/include/asm
```

## ZeroMQ

[ZeroMQ](https://github.com/zeromq/libzmq)

```bash
apt-get install libzmq3-dev
```

## XDP (Do I need this?)

Follow [XDP-Tutorial](https://github.com/xdp-project/xdp-tutorial/tree/main?tab=readme-ov-file) if necessary.

### Dependencies

To install main dependencies ``libxdp``, ``libbpf``, ``llvm``, ``clang`` and ``libelf``:
```bash
sudo apt install clang llvm libelf-dev libpcap-dev build-essential libc6-dev-i386
```
To install linux headers:
```bash
sudo apt install linux-headers-$(uname -r)
```
To install iperf and extra tools:
```bash
sudo apt install linux-perf
sudo apt install linux-tools-common linux-tools-generic
sudo apt install tcpdump
```
