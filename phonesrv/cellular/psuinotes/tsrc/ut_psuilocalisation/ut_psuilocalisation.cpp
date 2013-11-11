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

#include "ut_psuilocalisation.h"
#include "qtestmains60.h"
#include <smcmockclassincludes.h>
#define private public
#include "psuilocalisation.h"

void QCoreApplication::installTranslator(QTranslator * messageFile)
{
    SMC_MOCK_METHOD1( void, QTranslator *, messageFile)
}

/*!
  UT_psuilocalisation::UT_psuilocalisation
 */
UT_psuilocalisation::UT_psuilocalisation() 
{
    
}

/*!
  UT_psuilocalisation::~UT_psuilocalisation
 */
UT_psuilocalisation::~UT_psuilocalisation()
{
    
}

/*!
  UT_psuilocalisation::init
 */
void UT_psuilocalisation::init()
{
    initialize();
    
    m_psuiLocalisation.reset(new PsUiLocalisation); 
}

/*!
  UT_psuilocalisation::cleanup
 */
void UT_psuilocalisation::cleanup()
{
    reset();
    delete m_psuiLocalisation.take(); 
}

/*!
  UT_psuilocalisation::t_memleak
 */
void UT_psuilocalisation::t_memleak()
{
    
}

/*!
  UT_psuilocalisation::t_installTranslator
 */
void UT_psuilocalisation::t_installTranslator()
{
    
    // Test: successfull load  
    expect("QTranslator::load").
        returns(true);    
    expect("QCoreApplication::installTranslator").times(1);
    
    m_psuiLocalisation->installTranslator(
            PsUiLocalisation::TranslationFileCommon);
    QVERIFY(m_psuiLocalisation->m_translators.count() == 1);
    QVERIFY(verify());
    
    // Test: failing load  
    expect("QTranslator::load").
        returns(false);
    expect("QCoreApplication::installTranslator").times(0);
    m_psuiLocalisation->installTranslator(
        PsUiLocalisation::TranslationFileCommon);
    QVERIFY(m_psuiLocalisation->m_translators.count() == 1);
    
    // Test: Unknown enum value
    QVERIFY(verify());
    expect("QCoreApplication::installTranslator").times(0);
    m_psuiLocalisation->installTranslator(
            (PsUiLocalisation::TranslationFileId)2);
    QVERIFY(m_psuiLocalisation->m_translators.count() == 1);
    QVERIFY(verify());
    
    // Test: telephone_cp translator load 
    expect("QTranslator::load").
        returns(false);
    expect("QCoreApplication::installTranslator").times(0);
    m_psuiLocalisation->installTranslator(
        PsUiLocalisation::TranslationFileTelephoneCp);
    QVERIFY(verify());
 
}

/*!
  UT_psuilocalisation::t_removeTranslators
 */
void UT_psuilocalisation::t_removeTranslators()
{
    expect("QTranslator::load").
        returns(true);
    
    m_psuiLocalisation->installTranslator(
        PsUiLocalisation::TranslationFileCommon); 
    m_psuiLocalisation->installTranslator(
        PsUiLocalisation::TranslationFileTelephoneCp);
    
    m_psuiLocalisation->removeTranslators(); 
    QVERIFY(m_psuiLocalisation->m_translators.count() == 0);
    QVERIFY(verify());
}

QTEST_MAIN_S60(UT_psuilocalisation)
