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

#include <QList>
#include <PsetContainer.h>
#include "ut_psetwrapper.h"
#include "psetwrapper.h"
#include "testutilities.h"

void SimulateLeaveL()
{
    User::Leave(KErrGeneral);
}

/*!
  UT_PSetWrapper::UT_PSetWrapper
 */
UT_PSetWrapper::UT_PSetWrapper()
    :
    m_setWrapper(NULL),
    m_psetContainerMock(NULL)
{
}

/*!
  UT_PSetWrapper::~UT_PSetWrapper
 */
UT_PSetWrapper::~UT_PSetWrapper()
{
    delete m_setWrapper;
    delete m_psetContainerMock;
}

/*!
  UT_PSetWrapper::init
 */
void UT_PSetWrapper::init()
{
    initialize();
    
    m_setWrapper = new PSetWrapper();
    m_psetContainerMock = new CPsetContainer();
}

/*!
  UT_PSetWrapper::cleanup
 */
void UT_PSetWrapper::cleanup()
{
    reset();
    
    delete m_setWrapper;
    m_setWrapper = NULL;
    delete m_psetContainerMock;
    m_psetContainerMock = NULL;
}

/*!
  UT_PSetWrapper::t_construction
 */
void UT_PSetWrapper::t_construction()
{
    if (qstrcmp(QTest::currentTestFunction(), "t_exceptionSafety") != 0) {
        expect("CPsetContainer::NewL").
            willOnce(invokeWithoutArguments(SimulateLeaveL));
        
        PSetWrapper *wrapper = NULL;
        EXPECT_EXCEPTION(wrapper = new PSetWrapper(NULL);
        )
        delete wrapper;
        
        QVERIFY(verify());
    }
}

/*!
  UT_PSetWrapper::t_cliWrapper
 */
void UT_PSetWrapper::t_cliWrapper()
{
    PSetCliWrapper *wrapper = NULL;
    wrapper = &m_setWrapper->cliWrapper();
    QVERIFY(wrapper == &m_setWrapper->cliWrapper());
}

/*!
  UT_PSetWrapper::t_callWaitingWrapper
 */
void UT_PSetWrapper::t_callWaitingWrapper()
{
    PSetCallWaitingWrapper *wrapper = NULL;
    wrapper = &m_setWrapper->callWaitingWrapper();
    QVERIFY(wrapper == &m_setWrapper->callWaitingWrapper());
}

/*!
  UT_PSetWrapper::t_callDivertingWrapper
 */
void UT_PSetWrapper::t_callDivertingWrapper()
{
    PSetCallDivertingWrapper *wrapper = NULL;
    wrapper = &m_setWrapper->callDivertingWrapper();
    QVERIFY(wrapper == &m_setWrapper->callDivertingWrapper());
}

/*!
  UT_PSetWrapper::t_networkWrapper
 */
void UT_PSetWrapper::t_networkWrapper()
{
    PSetNetworkWrapper *wrapper = NULL;
    wrapper = &m_setWrapper->networkWrapper();
    QVERIFY(wrapper == &m_setWrapper->networkWrapper());
}

/*!
  UT_PSetWrapper::t_callBarringWrapper
 */
void UT_PSetWrapper::t_callBarringWrapper()
{
    PSetCallBarringWrapper *wrapper = NULL;
    wrapper = &m_setWrapper->callBarringWrapper();
    QVERIFY(wrapper == &m_setWrapper->callBarringWrapper());
}

/*!
  UT_PSetWrapper::t_exceptionSafety
 */
void UT_PSetWrapper::t_exceptionSafety()
{
    cleanup();
    
    OomTestExecuter::runAllTests(*this, "t_exceptionSafety");
}

QTEST_MAIN_S60(UT_PSetWrapper)
