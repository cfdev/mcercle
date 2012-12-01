# -------------------------------------------------
# mcercle - Logiciel de gestion libre
# -------------------------------------------------
#QT += sql

# Test la version de Qt
lessThan(QT_VERSION, 4.6) {
    error("mcercle requires Qt 4.6 or greater")
}

TARGET = mcercle
TEMPLATE = app
CONFIG += ordered

# Compilation en fonction du OS
unix {
    DEFINES += IBPP_LINUX
}
win32 {
    DEFINES += IBPP_WINDOWS
}
macx {
    DEFINES += IBPP_LINUX
}

DEFINES -= UNICODE
LIBS += -lfbclient

# Fichiers sources
SOURCES += src/mainwindow.cpp \
    src/main.cpp \
    src/board.cpp \
    src/customerView.cpp \
    src/productView.cpp \
    src/about.cpp \
    src/dialogcustomer.cpp \
    src/customer.cpp \
    src/invoice.cpp \
    src/product.cpp \
    src/dialogproduct.cpp \
    src/provider.cpp \
    src/dialogproviders.cpp \
    src/dialogwaiting.cpp \
    src/settings.cpp \
    src/dialogsettings.cpp \
    src/dbase.cpp \
    src/bdd/all_in_one.cpp \
    src/dialogcategory.cpp \
    src/productcategory.cpp \
    src/services.cpp \
    src/dialogservices.cpp \
    src/dialogprovidersedit.cpp \
    src/dialogservicesedit.cpp \
    src/proposal.cpp \
    src/dialoginvoice.cpp \
    src/services_commons.cpp \
    src/dialogprintchoice.cpp


HEADERS += src/mainwindow.h \
    src/board.h \
    src/customerView.h \
    src/productView.h \
    src/about.h \
    src/dialogcustomer.h \
    src/customer.h \
    src/invoice.h \
    src/product.h \
    src/dialogproduct.h \
    src/provider.h \
    src/dialogproviders.h \
    src/dialogwaiting.h \
    src/settings.h \
    src/dialogsettings.h \
    src/dbase.h \
    src/bdd/ibpp.h \
    src/bdd/iberror.h \
    src/bdd/ibase.h \
    src/bdd/_ibpp.h \
    src/dialogcategory.h \
    src/productcategory.h \
    src/services.h \
    src/dialogservices.h \
    src/dialogprovidersedit.h \
    src/dialogservicesedit.h \ 
    src/proposal.h \
    src/dialoginvoice.h \
    src/services_commons.h \
    src/dialogprintchoice.h


FORMS += src/mainwindow.ui \
    src/board.ui \
    src/customerView.ui \
    src/productView.ui \
    src/about.ui \
    src/dialogcustomer.ui \
    src/dialogproduct.ui \
    src/dialogproviders.ui \
    src/dialogwaiting.ui \
    src/dialogsettings.ui \
    src/dialogcategory.ui \
    src/dialogservices.ui \
    src/dialogprovidersedit.ui \
    src/dialogservicesedit.ui \
    src/dialoginvoice.ui \
    src/dialogprintchoice.ui

UI_SOURCES_DIR = src/
UI_HEADERS_DIR = src/

# Liste des fichiers qui seront incorpores au binaire en tant que ressources
RESOURCES += ressources.qrc

# Destination des fichiers objets
MOC_DIR = out/
OBJECTS_DIR = out/
DESTDIR = out/

# Description de l'installation
target.path = /usr/bin/

copyright.path     = /usr/share/doc/mcercle/
copyright.files    = License Changelog

desktop.path       = /usr/share/applications/
desktop.files      = art/mcercle.desktop art/logo/mcercle-logo-64.png

# L'installation comprend la copie du binaire, des fichiers de langue et du fichier LICENSE
INSTALLS += target desktop copyright 
