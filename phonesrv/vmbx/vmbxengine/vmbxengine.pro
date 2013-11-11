#
# Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
# Description:
#
#
#

TEMPLATE = lib
TARGET = vmbxengine
DEPENDPATH += .
HEADERS += $$PUBLIC_HEADERS

CONFIG += hb
MOC_DIR = moc

symbian: {
    TARGET.CAPABILITY = CAP_GENERAL_DLL
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.UID3=0x1000590F
    INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE \
                   $$MOC_DIR \
                   ../../inc \
                   ./inc
        
    LIBS += -lcentralrepository \
            -lcenrepnotifhandler \
            -lfeatmgr \
            -lphoneclient \
            -lsssettings \
            -letelmm \
            -letel \
            -lsatclient \
            -lserviceprovidersettings \
            -lefsrv \
            -lFlogger

    HEADERS += inc/mvmbxcenrephandler.h \
               inc/mvmbxcenrepobserver.h \
               inc/mvmbxchangeobserver.h \
               inc/mvmbxresourceprovider.h \
               inc/mvmbxsatrefreshobserver.h \
               inc/mvmbxservicenotify.h \
               inc/mvmbxsimstoreobserver.h \
               inc/mvmbxuihandler.h \
               inc/mvmbxuiutilities.h \
               inc/vmbxcenrephandler.h \
               inc/vmbxcenrepobserver.h \
               inc/vmbxcsvideoengine.h \
               inc/vmbxcsvoiceengine.h \
               inc/vmbxemergencycall.h \
               inc/vmbxenginebase.h \
               inc/vmbxenginefactory.h \
               inc/vmbxetelconnection.h \
               inc/vmbxlogger.h \
               inc/vmbxobserver.h \
               inc/vmbxpbkstore.h \
               inc/vmbxqtuihandler.h \
               inc/vmbxsatrefreshobserver.h \
               inc/vmbxsimhandler.h \
               inc/vmbxsimstoreobserver.h \
               inc/vmbxuihandler.h \
               inc/vmbxuiutilities.h \
               inc/vmbxutilities.h \
               inc/vmbxvoipengine.h \
               inc/vmsettingsuiiconfileprovider.h \
               inc/vmspshandler.h \
               inc/voicemailboxdefsinternal.h \
               inc/voicemailboximpl.h \
               inc/voicemailboxprivatecrkeys.h \
               inc/dialogwaiter.h

    SOURCES += src/vmbxcenrepobserver.cpp \
               src/vmbxcsvoiceengine.cpp \
               src/vmbxcsvideoengine.cpp \
               src/vmbxcenrephandler.cpp \
               src/cvoicemailboxentry.cpp \
               src/cvoicemailbox.cpp \
               src/voicemailboximpl.cpp \
               src/vmspshandler.cpp \
               src/vmsettingsuiiconfileprovider.cpp \
               src/vmbxvoipengine.cpp \
               src/vmbxutilities.cpp \
               src/vmbxuiutilities.cpp \
               src/vmbxuihandler.cpp \
               src/vmbxsimstoreobserver.cpp \
               src/vmbxsimhandler.cpp \
               src/vmbxsatrefreshobserver.cpp \
               src/vmbxqtuihandler.cpp \
               src/vmbxpbkstore.cpp \
               src/vmbxobserver.cpp \
               src/vmbxetelconnection.cpp \
               src/vmbxenginefactory.cpp \
               src/vmbxenginebase.cpp \
               src/vmbxemergencycall.cpp \
               src/dialogwaiter.cpp
               
     defFiles = \
        "$${LITERAL_HASH}ifdef WINS" \
            "DEFFILE bwins/vmbxengine.def" \
        "$${LITERAL_HASH}else" \
            "DEFFILE eabi/vmbxengine.def" \
        "$${LITERAL_HASH}endif"
    
    MMP_RULES += defFiles

    BLD_INF_RULES.prj_exports += \
        "rom/backup_registration.xml /epoc32/data/z/private/100058F5/backup_registration.xml" \
        "conf/voicemailbox.confml         MW_LAYER_CONFML(voicemailbox.confml)" \
        "conf/voicemailbox_101F874F.crml  MW_LAYER_CRML(voicemailbox_101F874F.crml)" \
        "rom/backup_registration.xml   /epoc32/release/winscw/udeb/z/private/100058F5/backup_registration.xml" \
        "rom/backup_registration.xml   /epoc32/release/winscw/urel/z/private/100058F5/backup_registration.xml" \
        "rom/vmbxengine_stub.sis /epoc32/data/z/system/install/vmbxengine_stub.sis" \
        "rom/vmbxengine.iby CORE_MW_LAYER_IBY_EXPORT_PATH(vmbxengine.iby)" \
        "rom/vmbxresources.iby       LANGUAGE_MW_LAYER_IBY_EXPORT_PATH(vmbxresources.iby)"

    TRANSLATIONS = vmbx.ts

}
