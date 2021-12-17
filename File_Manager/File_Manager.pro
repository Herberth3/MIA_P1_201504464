QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    sintactico.cpp \
    scanner.cpp \
    parametro.cpp \
    mkdisk.cpp \
    comando.cpp \
    rmdisk.cpp \
    fdisk.cpp \
    mount.cpp \
    unmount.cpp \
    mkfs.cpp \
    archivo.cpp \
    carpeta.cpp \
    recovery.cpp \
    rep.cpp

HEADERS += \
    sintactico.y \
    sintactico.h \
    scanner.h \
    parametro.h \
    mkdisk.h \
    lexico.l \
    comando.h \
    tipo_estructura.h \
    rmdisk.h \
    fdisk.h \
    mount.h \
    unmount.h \
    mkfs.h \
    archivo.h \
    carpeta.h \
    recovery.h \
    rep.h
