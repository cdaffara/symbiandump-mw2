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

#include "ut_sssettingswrapper.h"
#include "qtestmains60.h"
#define private public
#include "sssettingswrapper.h"
#include "sssettingswrapper_p.h"

/*!
  UT_SsSettingsWrapper::UT_SsSettingsWrapper
 */
UT_SsSettingsWrapper::UT_SsSettingsWrapper() 
    : mWrapper(NULL)
{
}

/*!
  UT_SsSettingsWrapper::~UT_SsSettingsWrapper
 */
UT_SsSettingsWrapper::~UT_SsSettingsWrapper()
{
    delete mWrapper;
}

/*!
  UT_SsSettingsWrapper::init
 */
void UT_SsSettingsWrapper::init()
{
    initialize();

    mWrapper = new SsSettingsWrapper();
}

/*!
  UT_SsSettingsWrapper::cleanup
 */
void UT_SsSettingsWrapper::cleanup()
{
    reset();
    
    delete mWrapper;
    mWrapper = NULL;
}

/*!
  UT_SsSettingsWrapper::t_memleak
 */
void UT_SsSettingsWrapper::t_memleak()
{
    
}

/*!
  UT_SsSettingsWrapper::t_get
 */
void UT_SsSettingsWrapper::t_get()
{
    int value = 0;
    expect("RSSSettings::Get").with(ESSSettingsCug, 0);
    QVERIFY( !mWrapper->get(Cug, value) );
    expect("RSSSettings::Get").with(ESSSettingsClir, 0);
    QVERIFY( !mWrapper->get(Clir, value) );
    expect("RSSSettings::Get").with(ESSSettingsAls, 0);
    QVERIFY( !mWrapper->get(Als, value) );
    expect("RSSSettings::Get").with(ESSSettingsAlsBlocking, 0);
    QVERIFY( !mWrapper->get(AlsBlocking, value) );
    expect("RSSSettings::Get").with(ESSSettingsDefaultCug, 0);
    QVERIFY( !mWrapper->get(DefaultCug, value) );
    QVERIFY(true == verify());
}

/*!
  UT_SsSettingsWrapper::t_set
 */
void UT_SsSettingsWrapper::t_set()
{
    int value = 0;
    expect("RSSSettings::Set").with(ESSSettingsCug, 0);
    QVERIFY( !mWrapper->set(Cug, value) );
    expect("RSSSettings::Set").with(ESSSettingsClir, 0);
    QVERIFY( !mWrapper->set(Clir, value) );
    expect("RSSSettings::Set").with(ESSSettingsAls, 0);
    QVERIFY( !mWrapper->set(Als, value) );
    expect("RSSSettings::Set").with(ESSSettingsAlsBlocking, 0);
    QVERIFY( !mWrapper->set(AlsBlocking, value) );
    expect("RSSSettings::Set").with(ESSSettingsDefaultCug, 0);
    QVERIFY( !mWrapper->set(DefaultCug, value) );
    QVERIFY(true == verify());
}

/*!
  UT_SsSettingsWrapper::t_PhoneSettingChanged
 */
Q_DECLARE_METATYPE(SsSettingsWrapperSettings)
void UT_SsSettingsWrapper::t_PhoneSettingChanged()
{
    qRegisterMetaType<SsSettingsWrapperSettings> ("SsSettingsWrapperSettings");
    QSignalSpy spy(mWrapper, 
            SIGNAL(phoneSettingChanged(SsSettingsWrapperSettings, int )));
    
    mWrapper->m_Priv->PhoneSettingChanged(ESSSettingsCug, 0);
    
    QCOMPARE(spy.count(), 1);
    const QList<QVariant> &arguments = spy.at(0);
    QVERIFY(Cug == arguments.at(0).value<SsSettingsWrapperSettings>());
    QVERIFY(0 == arguments.at(1).value<int>());
}

QTEST_MAIN_S60(UT_SsSettingsWrapper)
