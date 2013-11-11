/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  class for information for a location based trigger
*
*/


#ifndef C_LBT_CONTAINER_TRIGGER_INFO_H
#define C_LBT_CONTAINER_TRIGGER_INFO_H

#include <e32base.h>
#include <lbtcommon.h>

/**
 *  This class contains extended trigger information that is used
 *  by the Strategy component.
 *  The information is calculated and stored in the container by the 
 *  server logic component.
 *
 *  @lib lbtcontainer.lib
 *  @since S60 v4.0
 */
NONSHARABLE_CLASS( CLbtExtendedTriggerInfo ) : public CBase
    {
public:    
/** Structure used to store trigger information calculated by the Server Logic
*/
    struct TLbtTriggerRectArea
        {
        TReal iTrigAreaSouthLat;
        TReal iTrigAreaNorthLat;
        TReal iTrigAreaEastLong;
        TReal iTrigAreaWestLong;
        };

public:
    IMPORT_C static CLbtExtendedTriggerInfo* NewL();
    
    IMPORT_C static CLbtExtendedTriggerInfo* NewL(
        TLbtTriggerRectArea aArea,
        TReal aHystRadius,
        TBool aFired,
        const TDesC8& aStrategyOpaqueData,
        TSecureId aOwnerSid,
        TBool aFireOnCreation =  ETrue);

    IMPORT_C virtual ~CLbtExtendedTriggerInfo();
    
    IMPORT_C const TLbtTriggerRectArea TriggerReactangleArea() const;
    IMPORT_C void SetTriggerRectangleArea(const TLbtTriggerRectArea& aArea);    

    IMPORT_C const TReal& HysteresisRadius() const;
    IMPORT_C void SetHysteresisRadius(const TReal& aHystRadius);
    
    IMPORT_C TBool IsTriggerFired() const;
    IMPORT_C void SetTriggerFiredState(TBool aFired);

    IMPORT_C const TDesC8& StategyData() const;
    IMPORT_C void SetStrategyDataL(const TDesC8& aStrategyData);

    IMPORT_C const TSecureId& OwnerSid() const;
    IMPORT_C void SetOwnerSid(const TSecureId& aOwnerSid);
    
    IMPORT_C TLbtTriggerFireInfo GetFiredInfo() const;
    IMPORT_C void SetFiredInfo(TLbtTriggerFireInfo aFiredInfo);
    
    IMPORT_C TBool IsTriggerFireOnCreation() const;
    IMPORT_C void SetTriggerFireOnCreation(TBool aFireOnCreation);
    

private:
    CLbtExtendedTriggerInfo();
    
    void ConstructL();
    
    void ConstructL(
        TLbtTriggerRectArea aArea,
        TReal aHystRadius,
        TBool aFired,
        const TDesC8& aStrategyOpaqueData,
        TSecureId aOwnerSid,
        TBool aFireOnCreation);
        
private: // data members
    TLbtTriggerRectArea iArea;
    TReal iHystRadius;
    TBool iFired;
    TLbtTriggerFireInfo iFiredInfo;
    HBufC8* iStrategyData;
    TSecureId iOwnerSid;
    TBool iFireOnCreation;    
    };
       	
#endif // C_LBT_CONTAINER_TRIGGER_INFO_H
