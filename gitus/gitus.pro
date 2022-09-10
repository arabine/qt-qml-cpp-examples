HEADERS       = window.h \
    GitRepo.h
SOURCES       = main.cpp \
                GitRepo.cpp \
                window.cpp
RESOURCES     = gitus.qrc

VPATH += libgit2/include
INCLUDEPATH += libgit2/include

QT += widgets
requires(qtConfig(combobox))

unix:!macx: LIBS += -L$$PWD/libgit2/ -lgit2

INCLUDEPATH += $$PWD/libgit2
DEPENDPATH += $$PWD/libgit2

FORMS += \
    main.ui
