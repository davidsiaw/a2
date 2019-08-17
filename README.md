# a2

Spiritual successor to Aldebaran

# Getting the code

```
git clone https://github.com/davidsiaw/a2 --recursive
```

# Building

## MSVC

Windows being Windows is a little less user-friendly when it comes to development, but where there is a will there is a way.

### Requirements

1. You will need to install Ruby and Bison on your PC before building.
2. ** note ** Do not use the Bison installer. Download the zip file and unzip it into a folder with no spaces in its path, and then add its bin folder to the PATH.
3. For building the x86 configuration you need to download nasm at https://www.nasm.us/pub/nasm/releasebuilds.
4. Then add the install location to PATH.

### Build it

Open `build/msvc/a2.sln` with Visual Studio 2015 and build (Ctrl+Shift+B).

mruby is built in the pre-link section of a2

## Cygwin

TODO

## Linux

### Requirements

```
rvm install 2.6

```

### Build

```
make run
```

## Android

TODO

## XCode OS X

TODO

## XCode iOS

TODO
