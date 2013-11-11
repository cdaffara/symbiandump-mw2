/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of "Eclipse Public License v1.0"
 * which accompanies this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 *
 * Initial Contributors:
 * Nokia Corporation - initial contribution.
 *
 * Contributors:
 *
 * Description: Telephony Multimedia Service - TestApp
 *
 */

#include <QtGui>
#include <QApplication>
#include "qttmstestapp.h"

int main(int argc, char *argv[])
    {
    QApplication app(argc, argv);
    QApplication::setOrganizationName("Nokia");
    QApplication::setOrganizationDomain("nokia.com");
    QApplication::setApplicationName("QTTMStestapp");

    TmsTestApp tmsapp;
    tmsapp.showMaximized();
    tmsapp.show();
    return app.exec();
    }
