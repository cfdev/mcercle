# -------------------------------------------------
# mcercle - Logiciel de gestion libre
# -------------------------------------------------

QT += sql
TARGET = mcercle
TEMPLATE = app
CONFIG += ordered
INCLUDEPATH += src

# Fichiers sources
#SOURCES += src/*.cpp
#HEADERS += src/*.h
#FORMS += src/*.ui

# Liste des fichiers qui seront incorpores au binaire en tant que ressources
RESOURCES += ressources.qrc

# Description de l'installation
target.path = /usr/bin/

copyright.path     = /usr/share/doc/mcercle/
copyright.files    = License Changelog

desktop.path       = /usr/share/applications/
desktop.files      = art/mcercle.desktop art/logo/mcercle-logo-64.png

# Icon
win32 {
    ICON = art/logo/mcercle.ico
}
macx {
    ICON = art/logo/mcercle.icns
}

# L installation comprend la copie du binaire, du fichier LICENSE
INSTALLS += target desktop copyright 

OTHER_FILES += \
    Todo.txt \
    Changelog.txt

HEADERS += \
    src/tax.h \
    src/table.h \
    src/settings.h \
    src/services_commons.h \
    src/services.h \
    src/provider.h \
    src/proposal.h \
    src/productView.h \
    src/productcategory.h \
    src/product.h \
    src/printc.h \
    src/mainwindow.h \
    src/invoice.h \
    src/dialogwaiting.h \
    src/dialogtax.h \
    src/dialogsettings.h \
    src/dialogservicesedit.h \
    src/dialogservices.h \
    src/dialogprovidersedit.h \
    src/dialogproviders.h \
    src/dialogproduct.h \
    src/dialogprintchoice.h \
    src/dialoginvoicelist.h \
    src/dialoginvoice.h \
    src/dialogcustomer.h \
    src/dialogcategory.h \
    src/dbase.h \
    src/customerView.h \
    src/customer.h \
    src/board.h \
    src/about.h

SOURCES += \
    src/tax.cpp \
    src/table.cpp \
    src/settings.cpp \
    src/services_commons.cpp \
    src/services.cpp \
    src/provider.cpp \
    src/proposal.cpp \
    src/productView.cpp \
    src/productcategory.cpp \
    src/product.cpp \
    src/printc.cpp \
    src/mainwindow.cpp \
    src/main.cpp \
    src/invoice.cpp \
    src/dialogwaiting.cpp \
    src/dialogtax.cpp \
    src/dialogsettings.cpp \
    src/dialogservicesedit.cpp \
    src/dialogservices.cpp \
    src/dialogprovidersedit.cpp \
    src/dialogproviders.cpp \
    src/dialogproduct.cpp \
    src/dialogprintchoice.cpp \
    src/dialoginvoicelist.cpp \
    src/dialoginvoice.cpp \
    src/dialogcustomer.cpp \
    src/dialogcategory.cpp \
    src/dbase.cpp \
    src/customerView.cpp \
    src/customer.cpp \
    src/board.cpp \
    src/about.cpp

FORMS += \
    src/productView.ui \
    src/mainwindow.ui \
    src/dialogwaiting.ui \
    src/dialogtax.ui \
    src/dialogsettings.ui \
    src/dialogservicesedit.ui \
    src/dialogservices.ui \
    src/dialogprovidersedit.ui \
    src/dialogproviders.ui \
    src/dialogproduct.ui \
    src/dialogprintchoice.ui \
    src/dialoginvoicelist.ui \
    src/dialoginvoice.ui \
    src/dialogcustomer.ui \
    src/dialogcategory.ui \
    src/customerView.ui \
    src/board.ui \
    src/about.ui
