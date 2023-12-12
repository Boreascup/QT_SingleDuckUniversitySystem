QT       += core gui sql

QT       += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminpage.cpp \
    courseChoose.cpp \
    edit.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    scores.cpp \
    stuInfo.cpp \
    studentpage.cpp \
    teacherchangepwd.cpp \
    teachermain.cpp \
    teacherspage.cpp

HEADERS += \
    adminpage.h \
    courseChoose.h \
    edit.h \
    login.h \
    mainwindow.h \
    scores.h \
    stuInfo.h \
    studentpage.h \
    teacherchangepwd.h \
    teachermain.h \
    teacherspage.h

FORMS += \
    adminpage.ui \
    courseChoose.ui \
    edit.ui \
    login.ui \
    mainwindow.ui \
    scores.ui \
    stuInfo.ui \
    studentpage.ui \
    teacherchangepwd.ui \
    teachermain.ui \
    teacherspage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc
