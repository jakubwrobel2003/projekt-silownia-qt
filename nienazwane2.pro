QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AI/aiworkoutgenerator.cpp \
    AI/geminiservice.cpp \
    Database/databasemanager.cpp \
    Model/cardioexercise.cpp \
    Model/exercise.cpp \
    Model/exerciseresult.cpp \
    Model/strengthexercise.cpp \
    Model/trainingmanager.cpp \
    Model/usermodel.cpp \
    Model/workout.cpp \
    Model/workoutsession.cpp \
    Repository/exercisedefinitionrepository.cpp \
    Repository/exercisehistoryrepository.cpp \
    Repository/exerciserepository.cpp \
    Repository/exerciseresultrepository.cpp \
    Repository/historyrepository.cpp \
    Repository/userrepository.cpp \
    Repository/workoutrepository.cpp \
    Repository/workoutsessionrepository.cpp \
    Utils/passwordhasher.cpp \
    dashboardview.cpp \
    historyview.cpp \
    main.cpp \
    mainwindow.cpp \
    profileview.cpp \
    workoutsview.cpp

HEADERS += \
    AI/aiworkoutgenerator.h \
    AI/aiworkoutplan.h \
    AI/geminiservice.h \
    Database/databasemanager.h \
    Model/ExerciseHistoryItem.h \
    Model/cardioexercise.h \
    Model/exercise.h \
    Model/exerciseresult.h \
    Model/strengthexercise.h \
    Model/trainingmanager.h \
    Model/usermodel.h \
    Model/workout.h \
    Model/workoutsession.h \
    Repository/exercisedefinitionrepository.h \
    Repository/exercisehistoryrepository.h \
    Repository/exerciserepository.h \
    Repository/exerciseresultrepository.h \
    Repository/historyrepository.h \
    Repository/userrepository.h \
    Repository/workoutrepository.h \
    Repository/workoutsessionrepository.h \
    Utils/passwordhasher.h \
    dashboardview.h \
    historyview.h \
    mainwindow.h \
    profileview.h \
    workoutsview.h

FORMS += \
    dashboardview.ui \
    historyview.ui \
    mainwindow.ui \
    profileview.ui \
    workoutsview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
