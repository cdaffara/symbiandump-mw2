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
 * Description: Header file for Custom docml loader
 *
 */


#ifndef CUSTOMDOCUMENTLOADER_H
#define CUSTOMDOCUMENTLOADER_H
#include <hbdocumentloader.h>

//Forward Declarations

class CustomDocumentLoader : public HbDocumentLoader
{

public:

    /**
     * Constructor.
     */
    CustomDocumentLoader();

    /**
     * Destructor.   
     */
    ~CustomDocumentLoader();

public:

    /**
     * from base class
     */
    QObject *createObject(const QString& type, const QString &name);

};

#endif /* CUSTOMDOCUMENTLOADER_H */
