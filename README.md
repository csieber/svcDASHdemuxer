# svcDASHdemuxer
A quality level demuxer for raw scalable h264 Annex.G (SVC) bitstreams in preparation for DASH streaming.

## Requirements

 - premake4
 - g++
 - libboost-system-dev 
 - libboost-program-options-dev
 
## Compile

Use premake4 to create the make files:

```bash
git clone git@github.com:csieber/svcDASHdemuxer.git
cd svcDASHdemuxer/
premake4 gmake
```

Build the application:

```bash
cd build/
make
```

## Usage

```
Options:
  --help                               Help
  -i [ --input ] arg                   Input SVC stream
  -o [ --output ] arg (=segments)      Output directory
  -n [ --idr-per-segment ] arg (=1)    IDR Periods per segment. IDR periods are
                                       counted by NAL units with type
                                       NAL_T_SEQ_PARAMETER_SET_RBSP(7).
  -p [ --filename-prefix ] arg         Filename suffix of the output segment
                                       files
  -s [ --filename-suffix ] arg (=.264) Filename prefix of the output segment
                                       files
  -f [ --ifps ] arg (=24)              Intended frames per segment.
```

Check the vagrant [here](vagrant/README.md) for an example how to use the demuxer.

## Citing

Please cite the following publication if you use this application:

```
@inproceedings{sieber2013implementation,
  title={Implementation and User-centric Comparison of a Novel Adaptation Logic for DASH with SVC},
  author={Sieber, Christian and Ho{\ss}feld, Tobias and Zinner, Thomas and Tran-Gia, Phuoc and Timmerer, Christian},
  booktitle={2013 IFIP/IEEE International Symposium on Integrated Network Management (IM 2013)},
  pages={1318--1323},
  year={2013},
  organization={IEEE}
}
```