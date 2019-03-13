TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXX = g++

SOURCES += \
    src/main.cpp \
    src/states/IGameState.cpp \
    src/CEngine.cpp \
    src/shared.cpp \
    src/window.cpp \
    src/keyboard.cpp \
    src/mouse.cpp \
    src/spaceship.cpp \
    src/mixer.cpp \
    src/level.cpp \
    src/menu.cpp \
    src/path.cpp \
    src/dir.cpp \
    src/render.cpp \
    src/tiles.cpp \
    src/config.cpp \
    src/editor.cpp \
    src/thread.cpp \
    src/text.cpp \
    src/particle.cpp \
    src/vector2.cpp \
    src/emitter.cpp \
    src/states/GameStateManager.cpp \
    src/gamestates/CMainGameState.cpp \
    src/gamestates/CMenuState.cpp \
    src/states/NonCopyable.cpp \
    src/states/NonMovable.cpp \
    src/Utils/CRandom.cpp



HEADERS += \
    include/window.h \
    include/states/IGameState.h \
    include/CEngine.h \
    include/shared.h \
    include/keyboard.h \
    include/mouse.h \
    include/spaceship.h \
    include/mixer.h \
    include/level.h \
    include/menu.h \
    include/path.h \
    include/dir.h \
    include/render.h \
    include/main.h \
    include/tiles.h \
    include/config.h \
    include/editor.h \
    include/thread.h \
    include/text.h \
    include/particle.h \
    include/vector2.h \
    include/emitter.h \
    include/states/GameStateManager.h \
    include/gamestates/CMainGameState.h \
    include/gamestates/CMenuState.h \
    include/states/NonCopyable.h \
    include/states/NonMovable.h \
    include/Utils/CRandom.h

CONFIG += c++14
QMAKE_CXXFLAGS += -std=c++14
QMAKE_CXXFLAGS_DEBUG += -std=c++14 -Wall -Wextra -fpermissive -pedantic -O0
QMAKE_CXXFLAGS_RELEASE += -s -std=c++14 -Wall -Wextra -fpermissive -pedantic -Os

debug : {
        DEFINES += DEBUG_MODE
}

win32: {

    INCLUDEPATH += include/
    DEPENDPATH  += include/



    DEFINES += ALLEGRO_STATICLINK
    LIBS += -L$$PWD/../../libs/Allegro524_mingw/lib/ -lallegro_monolith-debug-static  -static -ljpeg -ldumb -lFLAC  -ltheora -lvorbisfile -lvorbis -logg -lphysfs -lfreetype -lpng16 -lzlibstatic -ldsound -lgdiplus -luuid -lkernel32 -lwinmm -lpsapi -lopengl32 -lglu32 -luser32 -lcomdlg32 -lgdi32 -lshell32 -lole32 -ladvapi32 -lws2_32 -lshlwapi -static-libstdc++ -static-libgcc
    INCLUDEPATH += $$PWD/../../libs/Allegro524_mingw/include
    DEPENDPATH += $$PWD/../../libs/Allegro524_mingw/include

    LIBS += -L$$PWD/../libs/Allegro524_mingw/lib/ -lallegro_monolith-debug-static -static -ljpeg -ldumb -lFLAC  -ltheora -lvorbisfile -lvorbis -logg -lphysfs -lfreetype -lpng16 -lzlibstatic -ldsound -lgdiplus -luuid -lkernel32 -lwinmm -lpsapi -lopengl32 -lglu32 -luser32 -lcomdlg32 -lgdi32 -lshell32 -lole32 -ladvapi32 -lws2_32 -lshlwapi -static-libstdc++ -static-libgcc
    INCLUDEPATH += $$PWD/../libs/Allegro524_mingw/include
    DEPENDPATH += $$PWD/../libs/Allegro524_mingw/include


}

unix: {
    INCLUDEPATH += -I/usr/local/include
    DEPENDPATH  += /usr/local/include
    LIBS += -L/usr/local/lib -lallegro -lallegro_main -lallegro_image -lallegro_font -lallegro_ttf -lallegro_dialog -lallegro_audio -lallegro_acodec -lallegro_primitives
    #INCLUDEPATH += /usr/include
    #DEPENDPATH  += /usr/include
}

macx: {

    INCLUDEPATH += include/
    DEPENDPATH  += include/

    LIBS += -L/usr/local/lib -lallegro -lallegro_main -lallegro_image -lallegro_font -lallegro_ttf -lallegro_dialog -lallegro_audio -lallegro_acodec -lallegro_primitives
    INCLUDEPATH += /usr/local/include
    DEPENDPATH  += /usr/local/include
}

