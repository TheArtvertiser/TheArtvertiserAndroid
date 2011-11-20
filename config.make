# add custom variables to this file

# OF_ROOT allows to move projects outside apps/* just set this variable to the
# absoulte path to the OF root folder

OF_ROOT = ../../..


# USER_CFLAGS allows to pass custom flags to the compiler
# for example search paths like:
# USER_CFLAGS = -I src/objects

USER_CFLAGS = -Isrc/comm  -Isrc/app -Isrc/ofxHttpUtils/src 
USER_CFLAGS += -I../TheArtvertiserCommon/gui -I../TheArtvertiserCommon -I../TheArtvertiserCommon/ferns_demo-1.1 -I../TheArtvertiserCommon/FAST -I../TheArtvertiserCommon/persistance -I../TheArtvertiserCommon/cv -I../TheArtvertiserCommon/ofxGeoLocation -I../TheArtvertiserCommon/ofxMD5 -I../TheArtvertiserCommon/ofxMD5/libs/md5-1.3.0


# USER_LDFLAGS allows to pass custom flags to the linker
# for example libraries like:
# USER_LD_FLAGS = libs/libawesomelib.a

USER_LD_FLAGS =


# use this to add system libraries for example:
# USER_LIBS = -lpango

USER_LIBS =


# change this to add different compiler optimizations to your project

LINUX_COMPILER_OPTIMIZATION = -march=native -mtune=native -Os

ANDROID_COMPILER_OPTIMIZATION = -Os -g1


# you shouldn't need to change this for usual OF apps, it allows to include code from other directories
# useful if you need to share a folder with code between 2 apps. The makefile will search recursively
# you can only set 1 path here

USER_SOURCE_DIR = ../TheArtvertiserCommon

# you shouldn't need to change this for usual OF apps, it allows to exclude code from some directories
# useful if you have some code for reference in the project folder but don't want it to be compiled

EXCLUDE_FROM_SOURCE="bin,.xcodeproj,obj,src/ofxHttpUtils/example,.git"
