# Vagrant Example

## Requirements

 - Vagrant
 - VirtualBox
 
## Setup

Clone the repository, spin up the VM and ssh into the machine:

```bash
git clone git@github.com:csieber/svcDASHdemuxer.git
cd svcDASHdemuxer/vagrant/

vagrant up
vagrant shh
```

Inside the machine, download an example video:

```bash
sudo apt-get install unzip

cd /svcDASHdemuxer/bin

wget http://sourceforge.net/projects/opensvcdecoder/files/Video%20Streams/video_5.zip/download -O video_5.zip

unzip video_5.zip
```

Finally demux the example bitstream:

```bash
mkdir demuxed
./svcDASHdemuxer_d -i video_5/video_5.264 -o demuxed/ --filename-prefix video5_
```

The demuxer will output the segments for each of the two quality levels. 00 indicates the base layer segments, 01 the enhancement layer segments.

```bash
....
-rw-rw-r-- 1 vagrant vagrant  18K Jan 22 15:22 video5_00_0014.264
-rw-rw-r-- 1 vagrant vagrant  56K Jan 22 15:22 video5_00_0015.264
-rw-rw-r-- 1 vagrant vagrant  48K Jan 22 15:22 video5_00_0016.264
-rw-rw-r-- 1 vagrant vagrant  31K Jan 22 15:22 video5_00_0017.264
-rw-rw-r-- 1 vagrant vagrant 7.0K Jan 22 15:22 video5_01_0002.264
-rw-rw-r-- 1 vagrant vagrant  20K Jan 22 15:22 video5_01_0003.264
-rw-rw-r-- 1 vagrant vagrant  17K Jan 22 15:22 video5_01_0004.264
-rw-rw-r-- 1 vagrant vagrant  11K Jan 22 15:22 video5_01_0005.264
...
```

