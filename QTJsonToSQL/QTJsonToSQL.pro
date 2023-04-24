 
QT += sql core

SOURCES   = SensorsClass.cpp \
            main.cpp
HEADERS   = SensorsClass.h

# install
target.path = ./build/
INSTALLS += target
