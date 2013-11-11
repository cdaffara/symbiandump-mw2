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

#ifndef SSSETTINGSWRAPPER_H
#define SSSETTINGSWRAPPER_H


#include <QObject>
#include <sssettingswrappertypes.h>

class RSSSettings;
class SsSettingsWrapperPrivate;

#ifdef BUILD_SSSETTINGSWRAPPER
#define SSSETTINGSWRAPPER_EXPORT Q_DECL_EXPORT
#else
#define SSSETTINGSWRAPPER_EXPORT Q_DECL_IMPORT
#endif

class SSSETTINGSWRAPPER_EXPORT SsSettingsWrapper: public QObject
{
    Q_OBJECT

public:
    explicit SsSettingsWrapper(QObject *parent = NULL);
    virtual ~SsSettingsWrapper();
    
public: // Functions:  
    /**
     Getter for supplementary service setting values.
     @param     setting      Enumeration of which setting is get.
     @param     value        Setting value.
     @return    Zero if no error occurred.
     */
    int get(SsSettingsWrapperSettings setting, int &value);

    /**
     Setter for supplementary service setting values.
     @param     setting      Enumeration of which setting is set.
     @param     value        Setting value.
     @return    Zero if no error occurred.
     */
    int set(SsSettingsWrapperSettings setting, int value );

signals: 
    /**
     Signals when supplementary setting value changes.
     @param     setting      Enumeration of which setting is changed.
     @param     value        Setting value.
     */
    void phoneSettingChanged( 
        SsSettingsWrapperSettings setting,
        int newValue );

private: // Data: 
    // Own
    RSSSettings *m_ssSettings;
    
    // Wrapper
    // Own
    SsSettingsWrapperPrivate* m_Priv;
    friend class SsSettingsWrapperPrivate;
    
};
#endif // SSSETTINGSWRAPPER

