<a href="https://www.buymeacoffee.com/st1vms"><img src="https://img.buymeacoffee.com/button-api/?text=1 Pizza Margherita&emoji=🍕&slug=st1vms&button_colour=0fa913&font_colour=ffffff&font_family=Bree&outline_colour=ffffff&coffee_colour=FFDD00" /></a>
# dynamic-bits

## Table of content

- [Overview](#overview)
- [Linux Installation](#linux-installation)
    - [Building on Linux (Cmake)](#building-on-linux-cmake)
- [Android Installation](#manual-android-installation)
- [Uninstalling](#uninstalling)
- [Usage](#usage)
- [Notes](#notes)


# Overview

This is a serialization library that supports **variable length integers/doubles**, **UTF8 strings** and serialization of **boolean bits**.
This library is designed to be **strong**, **lightweight** and **fast**, using (one-malloc) allocation strategy and minimizing the amount of resources during serialization/deserialization.

This library uses variable-length integers, trying to serialize the minimum amount of bits required for each data type, prepending a bit header representing the bit size of the data. It follows IEEE 754 for 64-bit double precision, is UTF8 string compatible, and allows boolean serialization using only one bit.

Each "packet" is defined in a packet table, a fixed number of fields is assigned to every packet, along with an unsigned 8-bit integer representing the packet-id.

Programmers must register their packets in the table before any de/serialization activity, this way thread safety can be achieved.


# Linux-Installation

## Requirements

- [cmake](https://cmake.org/install/) is required in order to build this library.
- Download this repository, either by zip or git using:
    ```
    git clone https://github.com/st1vms/dynamic-bits
    ```

## Before building

Ensure to set optimal values for `PACKET_TABLE_SIZE` and `MAX_PACKET_FIELDS` macros. They are located in the first lines of [include/dpacket.h](include/dpacket.h)

***PACKET_TABLE_SIZE** -> Max number of packets that can be registered.*

***MAX_PACKET_FIELDS** -> Max number of fields a packet can hold.*

It is drastically important to tweak these two values in order to optimize space required for building the static packet table.

## Building-On-Linux-CMake

After configuration is done, open a terminal inside the repository directory, make a build folder and generate build files using cmake.
```
cd ./dynamic-bits
mkdir build
cd build
cmake .. && cmake --build .
```

Inside the build folder you will find the generated shared library.

To install the library on Linux run these two commands inside build folder...

```
sudo make install
sudo ldconfig /usr/local/lib
```


# Manual-Android-Installation

- Install [Android Studio](https://cmake.org/install/) if not already installed.
- Open the JDBits bind project inside Android Studio

## Before Building ( Android )

Ensure to set optimal values for `PACKET_TABLE_SIZE` and `MAX_PACKET_FIELDS` macros. They are located in the first lines of [jdbits/src/main/cpp/include/dpacket.h](binds/Android/JDBits/jdbits/src/main/cpp/include/dpacket.h)

***PACKET_TABLE_SIZE** -> Max number of packets that can be registered.*

***MAX_PACKET_FIELDS** -> Max number of fields a packet can hold.*

It is drastically important to tweak these two values in order to optimize space required for building the static packet table.


## Building AAR library in Android Studio

- After syncing gradle files, open a terminal and execute this command:
    ```
    JAVA_HOME=$HOME/android-studio/jbr ./gradlew jdbits:assemble
    ```

You will find the generated **jdbits-<build_version>.aar** file inside the `build/outputs/aar` folder

Copy the AAR file into your Android project **`libs`** folder, and import is as an **`implementation`** dependency, using the Android Studio dependency manager in your Project Settings.


## Usage

Please refer to the examples provided in the [examples](examples/) folder, for the specific language/platform you're using...

- [C-Example](examples/c-example/)
- [Android-Java-Example](examples/android-example/app/src/main/java/com/example/dbitsandroidexample/MainActivity.java)


## Notes

This library doesn't still support complex structures, such as arrays, unions, dictonaries, etc...

I'm planning to add them very soon along with bindings for Python and Java.


## Uninstalling

Run these two commands inside project folder...
```
chmod u+x uninstall.sh
sudo ./uninstall.sh
```
