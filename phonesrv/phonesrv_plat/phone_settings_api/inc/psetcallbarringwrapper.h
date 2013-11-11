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

#ifndef PSETCALLBARRINGGWRAPPER_H
#define PSETCALLBARRINGGWRAPPER_H

#include <QObject>
#include <psetwrappertypes.h>

// Forward declarations 
class CPsetContainer;
class PSetCallBarringWrapperPrivate;

#ifdef BUILD_PSETWRAPPER
#define PSETWRAPPER_EXPORT Q_DECL_EXPORT
#else
#define PSETWRAPPER_EXPORT Q_DECL_IMPORT
#endif

class PSETWRAPPER_EXPORT PSetCallBarringWrapper : public QObject
{
    Q_OBJECT

public:
    
    /** barring types */
    enum BarringType
        {
        BarringTypeAllBarrings                              = 0x0001,
        BarringTypeAllOutgoing                              = 0x0002,
        BarringTypeOutgoingInternational                    = 0x0004,
        BarringTypeOutgoingInternationalExceptToHomeCountry = 0x0008,
        BarringTypeAllIncoming                              = 0x0010,
        BarringTypeIncomingWhenRoaming                      = 0x0020,
        BarringTypeAllServices,
        BarringTypeAllOutgoingServices,
        BarringTypeAllIncomingServices
        };
    
    /** barring status */
    enum BarringStatus
        {
        BarringStatusUnknown,
        BarringStatusActive,
        BarringStatusInactive,
        BarringStatusNotProvisioned,
        BarringStatusUnavailable
        };
    
    /** barring error */
    enum BarringError
    {
        BarringErrorNone
    };
    
public:
    
    explicit PSetCallBarringWrapper( 
        CPsetContainer &psetContainer, 
        QObject *parent = NULL);
    
    ~PSetCallBarringWrapper();
    
    /**
     * Checks the barring status from network. Result is signaled with 
     * barringStatusRequestCompleted.
     * 
     * @param   serviceGroup  Service group concerned.
     * @param   barringType   Barring type.
     */
    void barringStatus(
        PsServiceGroup serviceGroup, 
        BarringType barringType);

    /**
     * Enables specified barring. Result is signaled with 
     * enableBarringRequestCompleted.
     * 
     * @param   basicServiceGroup   Basic service group concerned.
     * @param   serviceGroup    Service group concerned.
     * @param   barringType     Barring type.
     * @param   barringPassword Barring password.
     */
    void enableBarring(
        int basicServiceGroup,
        PsServiceGroup serviceGroup,
        BarringType barringType,
        QString barringPassword);
    
    /**
     * Disables specified barring. Result is signaled with 
     * disableBarringRequestCompleted.
     * 
     * @param   basicServiceGroup   Basic service group concerned.
     * @param   serviceGroup    Service group concerned.
     * @param   barringType     Barring type.
     * @param   barringPassword Barring password.
     */
    void disableBarring(
        int basicServiceGroup,
        PsServiceGroup serviceGroup,
        BarringType barringType,
        QString barringPassword);
    
    /**
     * Changes barring password. Maximum password length is 10.
     * 
     * @param   oldPassword         Old barring password.
     * @param   newPassword         New barring password.
     * @param   verifiedPassword    New password verified.
     */
    void changeBarringPassword(
        const QString &oldPassword, 
        const QString &newPassword,
        const QString &verifiedPassword);
    
signals:
    
    /**
     * This signal is emitted when barring status query request is completed.
     * 
     * Basic service group identifier list contains items only if status is
     * queried for several services at once and barrings are active for some
     * of those services.
     *
     * @param   result                  0 if request was completed successfully 
     * or error code defined either in gsmerror.h or exterror.h.
     * @param   basicServiceGroupIds    Service group identifiers.
     * @param   status                  Barring status.
     */
    void barringStatusRequestCompleted(
        int result,
        const QList<unsigned char> & basicServiceGroupIds,
        PSetCallBarringWrapper::BarringStatus status);
    
    /**
     * This signal is emitted when barring enabling is completed.
     * 
     * @param   result                  0 if request was completed successfully 
     * or error code defined either in gsmerror.h or exterror.h.
     * @param   barringType             Service group identifiers.
     * @param   barringStatus           Barring status.
     * @param   plural                  Plurality.
     */
    void enableBarringRequestCompleted(
        int result,
        PSetCallBarringWrapper::BarringType barringType,
        PSetCallBarringWrapper::BarringStatus barringStatus, 
        bool plural);

    /**
     * This signal is emitted when barring disabling is completed.
     * 
     * @param   result                  0 if request was completed successfully 
     * or error code defined either in gsmerror.h or exterror.h.
     * @param   barringType             Service group identifiers.
     * @param   barringStatus           Barring status.
     * @param   plural                  Plurality.
     */
    void disableBarringRequestCompleted(
        int result,
        PSetCallBarringWrapper::BarringType barringType,
        PSetCallBarringWrapper::BarringStatus barringStatus, 
        bool plural);
    
    /**
     * This signal is emitted when barring password change request is completed.
     * 
     * @param   result                  0 if request was completed successfully 
     * or error code defined either in gsmerror.h or exterror.h.
     */
    void barringPasswordChangeRequestCompleted(int result);
    
private: // data 
    
    /** Own. Private implementation. */
    QScopedPointer<PSetCallBarringWrapperPrivate> m_privateImpl;
    friend class PSetCallBarringWrapperPrivate;
};

#endif // PSETCALLBARRINGGWRAPPER_H
