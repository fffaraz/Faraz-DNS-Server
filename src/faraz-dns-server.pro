#-------------------------------------------------
#
# Project created by QtCreator 2013-06-21T22:49:15
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TARGET = faraz-dns-server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app
DESTDIR = ../bin

SOURCES += main.cpp \
    dns.cpp \
    dnsserver.cpp

HEADERS += \
    dns.h \
    dns_packet.h \
    dnsserver.h
