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

Make sure you install Ruby and have some sort of sound system installed (I tested with PulseAudio and ALSA).

```
rvm install 2.6

```

### Build

```
make run
```

```
make run V=1 # To see the verbose output
```

## Android

TODO

## Mac OS X (On the console)

You will need openssl

```
brew install openssl
```

Make sure you install Ruby and have some sort of sound system installed (I tested with PulseAudio and ALSA).

```
rvm install 2.6
```

### Build

```
make run
```

```
make run V=1 # To see the verbose output
```

## Mac OS X (Using XCode)

TODO

## iOS

You can only build this using XCode.

TODO
