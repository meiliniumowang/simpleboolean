TEMPLATE = lib

CONFIG += release
DEFINES += NDEBUG

INCLUDEPATH += ./
INCLUDEPATH += thirdparty/moller97

HEADERS += simpleboolean/meshdatatype.h

SOURCES += simpleboolean/axisalignedboundingbox.cpp
HEADERS += simpleboolean/axisalignedboundingbox.h

SOURCES += simpleboolean/retriangulation.cpp
HEADERS += simpleboolean/retriangulation.h

SOURCES += simpleboolean/edgeloop.cpp
HEADERS += simpleboolean/edgeloop.h

SOURCES += simpleboolean/subsurface.cpp
HEADERS += simpleboolean/subsurface.h

SOURCES += simpleboolean/subblock.cpp
HEADERS += simpleboolean/subblock.h

SOURCES += simpleboolean/distinguish.cpp
HEADERS += simpleboolean/distinguish.h

SOURCES += simpleboolean/meshcombiner.cpp
HEADERS += simpleboolean/meshcombiner.h

SOURCES += thirdparty/moller97/tritri_isectline.cpp
HEADERS += thirdparty/moller97/tritri_isectline.h

QMAKE_CXXFLAGS += -std=c++11

target.path = ./
INSTALLS += target