/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: This implements the custom dialog loader for posverification dialogs
 *
 */

#include "customdocumentloader.h"
#include "posverificationcustomdialog.h"

// ---------------------------------------------------------------------------
// CustomDocumentLoader::CustomDocumentLoader()
// ---------------------------------------------------------------------------
//
CustomDocumentLoader::CustomDocumentLoader()
    {
    }

// ---------------------------------------------------------------------------
// CustomDocumentLoader::~CustomDocumentLoader()
// ---------------------------------------------------------------------------
//
CustomDocumentLoader::~CustomDocumentLoader()
    {
    }

// ---------------------------------------------------------------------------
// CustomDocumentLoader::createObject
// ---------------------------------------------------------------------------
//
QObject *CustomDocumentLoader::createObject(const QString& type,
        const QString &name)
    {
    //for CustomDialog
    if (type == PosVerificationCustomDialog::staticMetaObject.className())
        {
        QObject *object = new PosVerificationCustomDialog;
        object->setObjectName(name);
        return object;
        }
    //default case
    return HbDocumentLoader::createObject(type, name);
    }
