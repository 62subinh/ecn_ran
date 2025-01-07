Copied from OAI [document](https://gitlab.eurecom.fr/oai/openairinterface5g/-/tree/develop/ci-scripts/yaml_files/5g_rfsimulator)

**Table of Contents**

[[_TOC_]]

# 1. Pull images from Docker-Hub #

```bash
$ docker pull mysql:8.0
$ docker pull oaisoftwarealliance/oai-amf:v2.0.0
$ docker pull oaisoftwarealliance/oai-smf:v2.0.0
$ docker pull oaisoftwarealliance/oai-upf:v2.0.0
$ docker pull oaisoftwarealliance/trf-gen-cn5g:focal

$ docker pull oaisoftwarealliance/oai-gnb:develop
$ docker pull oaisoftwarealliance/oai-nr-ue:develop
```

# 2. Deploy OAI 5G Core Network #

## 2.1. HOW-TO ##

**CAUTION: DO NOT USE `docker-compose`!**

```bash
$ cd ci-scripts/yaml_files/5g_rfsimulator
$ docker compose up -d mysql oai-amf oai-smf oai-upf oai-ext-dn
```

## 2.2. Check ##

```bash
$ docker compose ps -a
       Name                     Command                  State                  Ports
-------------------------------------------------------------------------------------------------
rfsim5g-mysql        docker-entrypoint.sh mysqld      Up (healthy)   3306/tcp, 33060/tcp
rfsim5g-oai-amf      /bin/bash /openair-amf/bin ...   Up (healthy)   38412/sctp, 80/tcp, 9090/tcp
rfsim5g-oai-ext-dn   /bin/bash -c  apt update;  ...   Up (healthy)
rfsim5g-oai-smf      /bin/bash -c /openair-smf/ ...   Up (healthy)   80/tcp, 8805/udp, 9090/tcp
rfsim5g-oai-upf      /bin/bash -c /openair-upf/ ...   Up (healthy)   2152/udp, 8805/udp
```

At this point, you can prepare a capture on the newly-created public docker bridges:

```bash
$ ifconfig
...
rfsim5g-public: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.71.129  netmask 255.255.255.192  broadcast 192.168.71.191
        inet6 fe80::42:c4ff:fe2b:3d38  prefixlen 64  scopeid 0x20<link>
        ether 02:42:c4:2b:3d:38  txqueuelen 0  (Ethernet)
        RX packets 4  bytes 112 (112.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 7  bytes 626 (626.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

rfsim5g-traffic: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.72.129  netmask 255.255.255.192  broadcast 192.168.72.191
        inet6 fe80::42:b5ff:fed3:e732  prefixlen 64  scopeid 0x20<link>
        ether 02:42:b5:d3:e7:32  txqueuelen 0  (Ethernet)
        RX packets 2652  bytes 142335 (142.3 KB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 3999  bytes 23367972 (23.3 MB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
...
```

# 3. Deploy OAI gNB (RF Simulator, Standalone) #

## 3.1 HOW-TO ##

**CAUTION: To execute this 2nd step, the whole `CN5G` SHALL be in `healthy` state (especially the `mysql` container).**

The gNB can be deployed either in monolithic mode, or in CU/DU split mode.
- For a deployment with a monolithic gNB:
```bash
$ docker compose up -d oai-gnb
```
- For a deployment with the gNB split in CU and DU components:
```bash
$ docker compose up -d oai-cu   # Deployment of the CU
$ docker compose up -d oai-du   # Deployment of the DU
```

## 3.2. Check ##

```bash
$ docker compose ps -a
       Name                     Command                  State                  Ports
-------------------------------------------------------------------------------------------------
rfsim5g-mysql        docker-entrypoint.sh mysqld      Up (healthy)   3306/tcp, 33060/tcp
rfsim5g-oai-amf      /bin/bash /openair-amf/bin ...   Up (healthy)   38412/sctp, 80/tcp, 9090/tcp
rfsim5g-oai-ext-dn   /bin/bash -c  apt update;  ...   Up (healthy)
rfsim5g-oai-gnb      /opt/oai-gnb/bin/entrypoin ...   Up (healthy)
rfsim5g-oai-smf      /bin/bash -c /openair-smf/ ...   Up (healthy)   80/tcp, 8805/udp, 9090/tcp
rfsim5g-oai-upf      /bin/bash -c /openair-upf/ ...   Up (healthy)   2152/udp, 8805/udp
```

You can verify that the `gNB` is connected with the `AMF`:

```bagh
$ docker logs rfsim5g-oai-amf
...
[AMF] [amf_app] [info ] |----------------------------------------------------gNBs' information-------------------------------------------|
[AMF] [amf_app] [info ] |    Index    |      Status      |       Global ID       |       gNB Name       |               PLMN             |
[AMF] [amf_app] [info ] |      1      |    Connected     |         0x0       |         gnb-rfsim        |            208, 99             |
[AMF] [amf_app] [info ] |----------------------------------------------------------------------------------------------------------------|
...
```

# 4. Deploy OAI NR-UE (RF Simulator, Standalone) #

## 4.1. HOW-TO ##

```bash
$ docker compose up -d oai-nr-ue
```

## 4.2. Check ##

```bash
$ docker compose ps -a
       Name                     Command                  State                  Ports
-------------------------------------------------------------------------------------------------
rfsim5g-mysql        docker-entrypoint.sh mysqld      Up (healthy)   3306/tcp, 33060/tcp
rfsim5g-oai-amf      /bin/bash /openair-amf/bin ...   Up (healthy)   38412/sctp, 80/tcp, 9090/tcp
rfsim5g-oai-ext-dn   /bin/bash -c  apt update;  ...   Up (healthy)
rfsim5g-oai-gnb      /opt/oai-gnb/bin/entrypoin ...   Up (healthy)
rfsim5g-oai-nr-ue    /opt/oai-nr-ue/bin/entrypo ...   Up (healthy)
rfsim5g-oai-smf      /bin/bash -c /openair-smf/ ...   Up (healthy)   80/tcp, 8805/udp, 9090/tcp
rfsim5g-oai-upf      /bin/bash -c /openair-upf/ ...   Up (healthy)   2152/udp, 8805/udp
```

Making sure the OAI UE is connected:

```bash
$ docker exec -it rfsim5g-oai-nr-ue /bin/bash
root@bb4d400a832d:/opt/oai-nr-ue# ifconfig
eth0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.71.150  netmask 255.255.255.192  broadcast 192.168.71.191
        ether 02:42:c0:a8:47:89  txqueuelen 0  (Ethernet)
        RX packets 224259  bytes 5821372018 (5.8 GB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 235916  bytes 7848786376 (7.8 GB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        loop  txqueuelen 1000  (Local Loopback)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 0  bytes 0 (0.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

oaitun_ue1: flags=4305<UP,POINTOPOINT,RUNNING,NOARP,MULTICAST>  mtu 1500
        inet 12.1.1.2  netmask 255.255.255.0  destination 12.1.1.2
        unspec 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00  txqueuelen 500  (UNSPEC)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 0  bytes 0 (0.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
```

# 5. Deploy Second OAI NR-UE (RF Simulator, Standalone) #

## 5.1. HOW-TO ##

1. **(NOTE: `oai_db.sql` and `docker-compose.yaml` are prepared for 10 UEs at max)**
```bash
$ docker compose up -d oai-nr-ue2
```

Do the same as above for container state check and connectivity check! 

```bash
$ docker compose ps -a
```

```bash
$ docker exec -it rfsim5g-oai-nr-ue2 /bin/bash
root@bb4d400a832d:/opt/oai-nr-ue# ifconfig
```

# 6. Check traffic #

## 6.1. Check your Internet connectivity ##

```bash
$ docker exec -it rfsim5g-oai-nr-ue /bin/bash
root@bb4d400a832d:/opt/oai-nr-ue# ping -I oaitun_ue1 -c 10 www.lemonde.fr
PING s2.shared.global.fastly.net (151.101.122.217) from 12.1.1.2 oaitun_ue1: 56(84) bytes of data.
64 bytes from 151.101.122.217 (151.101.122.217): icmp_seq=1 ttl=53 time=64.5 ms
64 bytes from 151.101.122.217 (151.101.122.217): icmp_seq=2 ttl=53 time=37.0 ms
64 bytes from 151.101.122.217 (151.101.122.217): icmp_seq=3 ttl=53 time=43.2 ms
64 bytes from 151.101.122.217 (151.101.122.217): icmp_seq=4 ttl=53 time=43.2 ms
64 bytes from 151.101.122.217 (151.101.122.217): icmp_seq=5 ttl=53 time=54.3 ms
64 bytes from 151.101.122.217 (151.101.122.217): icmp_seq=6 ttl=53 time=24.0 ms
64 bytes from 151.101.122.217 (151.101.122.217): icmp_seq=7 ttl=53 time=32.5 ms
64 bytes from 151.101.122.217 (151.101.122.217): icmp_seq=8 ttl=53 time=37.0 ms
64 bytes from 151.101.122.217 (151.101.122.217): icmp_seq=9 ttl=53 time=41.2 ms
64 bytes from 151.101.122.217 (151.101.122.217): icmp_seq=10 ttl=53 time=50.3 ms

--- s2.shared.global.fastly.net ping statistics ---
10 packets transmitted, 10 received, 0% packet loss, time 9011ms
rtt min/avg/max/mdev = 24.035/42.765/64.557/10.904 ms
```

If it does not work, certainly you need to modify the DNS values in the docker-compose.

But you can also check with the `ext-dn` container (IP address is `192.168.72.135` in docker-compose)

```bash
$ docker exec -it rfsim5g-oai-nr-ue /bin/bash
root@bb4d400a832d# ping -I oaitun_ue1 -c 2 192.168.72.135
PING 192.168.72.135 (192.168.72.135) from 12.1.1.2 oaitun_ue1: 56(84) bytes of data.
64 bytes from 192.168.72.135: icmp_seq=1 ttl=63 time=10.9 ms
64 bytes from 192.168.72.135: icmp_seq=2 ttl=63 time=16.5 ms

--- 192.168.72.135 ping statistics ---
2 packets transmitted, 2 received, 0% packet loss, time 1001ms
rtt min/avg/max/mdev = 10.939/13.747/16.556/2.810 ms
```

Let now try to check UDP traffic in Downlink.

You will need 2 terminals: one in the NR-UE container, one in the ext-dn container.

Note:
Similarly, Second OAI UE Internet connectivity can be checked.

## 6.2. Start the `iperf` server inside the NR-UE container ##

```bash
$ docker exec -it rfsim5g-oai-nr-ue /bin/bash
#Inside container:
$ iperf -B 12.1.1.2 -u -i 1 -s
```

## 6.3. Start the `iperf` client inside the ext-dn container ##

```bash
$ docker exec -it rfsim5g-oai-ext-dn /bin/bash
# Inside container:
$ iperf -c 12.1.1.2 -u -i 1 -t 20 -b 500K
```

Back on your NR-UE terminal you shall see the log.

# 7. Un-deployment #

```bash
$ docker compose down
```

# 8. Running with local changes #

You can run the testcase with local changes by substituting the binaries in
execution images. `local-override.yaml` file provides a way to substitute the
gNB and nrUE executables as well as librfsimulator.so. Refer to the `-volumes`
section in the file for details. This includes an image build service as well as
code compilation service. This is necessary as the executable has to be linked
against the same libraries that are present in the executing image. This might
take a while the first time but other that that is very fast. Here is a list of
commands (wait between each command). Tested with `docker compose` v2.27.0.

This command deploys OAI 5G Core Network
```bash
docker compose -f docker-compose.yaml -f local-override.yaml up -d mysql oai-amf oai-smf oai-upf oai-ext-dn
```
This command builds base images locally, builds local gNB & nrUE executable and
runs the gnb service with modified gNB executable.
```bash
docker compose -f docker-compose.yaml -f local-override.yaml up -d oai-gnb
```
This command rebuilds both the gNB & nrUE and runs the oai-nr-ue container with 
modified nrUE executable.
```bash
docker compose -f docker-compose.yaml -f local-override.yaml up -d oai-nr-ue
```

## 6.1 Running nrUE in gdb
`local-override-ue-gdb.yaml` is an additional override file which can be used
to run the UE executable in gdb. Replace the last command above with the
following:

```bash
docker compose -f docker-compose.yaml -f local-override.yaml -f local-override-ue-gdb.yaml run oai-nr-ue
```
