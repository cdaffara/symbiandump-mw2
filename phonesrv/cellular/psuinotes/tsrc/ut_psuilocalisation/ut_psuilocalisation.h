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
* Description:
*
*/

#ifndef UT_PSUILOCALISATION_H
#define UT_PSUILOCALISATION_H

#include <QtTest/QtTest>
#include <mockservice.h>

class PsUiLocalisation; 

class UT_psuilocalisation : public QObject, MockService
{
    Q_OBJECT

public:    
    UT_psuilocalisation();
    ~UT_psuilocalisation();
    
private slots:
    void init();
    void cleanup();
    void t_memleak();
    void t_installTranslator();
    void t_removeTranslators(); 
 
private: 
    QScopedPointer<PsUiLocalisation> m_psuiLocalisation; 
    
};

#endif
