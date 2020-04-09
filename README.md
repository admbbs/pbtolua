# pbtolua

A protobuf code generator for tolua++ .pkg files.

Built and tested with protobuf-2.6.1.

## Prerequisites

protobuf-2.6.1 built and installed under third_party/protobuf as follows.

```
third_party
├── README
└── protobuf
    ├── bin
    ├── include
    └── lib
```

## Build

linux

```
$ mkdir build
$ cd build
$ cmake ..
$ make
```

windows

```
$ mkdir build
$ cd build
$ cmake -G"Visual Studio XX XXXX" ..
$ start pblua.sln
```

## Run

Run it as a standalone compiler

```
$ bin/protoc_tolua --proto_path=PATH_TO_TARGET_PROTO_DIR --tolua_out=. TARGET_PROTO_FILE.proto
```

or a plugin

```
$ protoc --proto_path=PATH_TO_TARGET_PROTO_DIR --tolua_out=. --plugin=bin/proto_gen_tolua TARGET_PROTO_FILE.proto
```

## TODO

- [ ] Proto 2
  - [x] Primitives
  - [x] Enumeration
  - [x] Package
  - [x] Message
  - [x] Oneof
  - [ ] Map
  - [ ] Option

- [ ] Proto 3

## No plan for support

- Service
- Extension
