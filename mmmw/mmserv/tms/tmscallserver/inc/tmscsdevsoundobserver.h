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
 * Description: Telephony Multimedia Service
 *
 */

#ifndef TMSCSDEVSOUNDOBSERVER_H
#define TMSCSDEVSOUNDOBSERVER_H

namespace TMS {

/**
 *  TMSCSDevSoundObserver
  * An observer interface providing TMSCSDevSound event notifications.
 */
class TMSCSDevSoundObserver
    {
public:
    virtual void DownlinkInitCompleted(gint status) = 0;
    virtual void UplinkInitCompleted(gint status) = 0;
    virtual void UplinkActivationCompleted(gint status) = 0;
    virtual void DownlinkActivationCompleted(gint status) = 0;
    };

} //namespace TMS

#endif // TMSCSDEVSOUNDOBSERVER_H
