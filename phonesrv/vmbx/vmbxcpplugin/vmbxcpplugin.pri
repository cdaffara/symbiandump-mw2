#
# Copyright (c) 2009 - 2010 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description: vmbxcpplugin source files
#


CONFIG += debug_and_release 


CONFIG(debug, debug|release) {
    SUBDIRPART = debug
} else {
    SUBDIRPART = release
}


# Add the output dirs to the link path too
LIBS += -L$$DESTDIR

MOC_DIR = moc
OBJECT_DIR = obj
RCC_DIR = rcc

#For some reason the default include path doesn't include MOC_DIR on symbian
symbian {
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
    INCLUDEPATH += $$MOC_DIR
    TARGET.CAPABILITY = CAP_GENERAL_DLL
    TARGET.EPOCALLOWDLLDATA = 1
}


CONFIG += hb plugin
LIBS += -lcpframework\
        -lvmbxengine

symbian: plugin { # copy qtstub and manifest
    PLUGIN_STUB_PATH = /resource/qt/plugins/controlpanel

    deploy.path = C:
    pluginstub.sources = $${TARGET}.dll
    pluginstub.path = $$PLUGIN_STUB_PATH
    DEPLOYMENT += pluginstub

    qtplugins.path = $$PLUGIN_STUB_PATH
    qtplugins.sources += qmakepluginstubs/$${TARGET}.qtplugin

    for(qtplugin, qtplugins.sources):BLD_INF_RULES.prj_exports += "./$$qtplugin  $$deploy.path$$qtplugins.path/$$basename(qtplugin)" \
    "rom/vmbxcpplugin_stub.sis ZSYSTEM/install/vmbxcpplugin_stub.sis"
}

HEADERS += inc/vmbxcpplugin.h \
           inc/vmbxcpgroup.h \
           inc/vmbxuiengine.h \
           inc/loggerutil.h \
           inc/actioncustomitem.h \
           inc/customedit.h

SOURCES += src/vmbxcpplugin.cpp \
           src/vmbxcpgroup.cpp \
           src/vmbxuiengine.cpp \
           src/actioncustomitem.cpp \
           src/customedit.cpp