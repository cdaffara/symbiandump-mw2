/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Audio Output Control Utility
*
*/


#ifndef AudioOutputControlUtilityProxyImpl_H
#define AudioOutputControlUtilityProxyImpl_H

#include <e32base.h>
#include "AudioOutputControlUtilityImpl.h"
#include "AudioOutputControlUtilityProxy.h"
#include <RestrictedAudioOutput.h>
#include <ConfigurationComponentsFactory.h>
#include <AudioOutput.h>
#include "DRMConfigTypes.h"

class CAudioOutputControlUtilityImpl;

class CAudioOutputControlUtilityProxyImpl : public CAudioOutputControlUtilityImpl
										//public CBase,	[shouldnt inherit from multiple C-based classes]
    {
public:
   /**
     * Constructs, and returns a pointer to, a new CBody object.
     * Leaves on failure..
     * @return CBody* A pointer to newly created object.
     */
	static CAudioOutputControlUtilityProxyImpl* NewL();


    /**
     * Destructor
     *
     */
	~CAudioOutputControlUtilityProxyImpl();

	TInt Configure(CMMFDevSound& aDevSound);
	TInt SetDRMConfigIntfc(CDRMConfigIntfc* aDRMConfigIntfc);
	TInt SetDataSource(MDataSource* aDataSource);
	TInt SetDataSource(CMultimediaDataSource* aDataSource);
private:
    void ConstructL();
    CAudioOutputControlUtilityProxyImpl();
    //CRestrictedAudioOutput::TAllowedOutputPreference CalcPref(TDRMAllowedOutputDevice aOutputDevice);
    TInt CalcPref(TDRMAllowedOutputDevice aOutputDevice, CRestrictedAudioOutput::TAllowedOutputPreference& aAllowedOutput);
    //TDRMAllowedOutputDevice CalcDrmAllowedOutputDevice(CRestrictedAudioOutput::TAllowedOutputPreference aPref);
    TInt ConfigureRestrictedAudioOutputL(CMMFDevSound& aDevSound);
    TInt SetCalculatedPreference(const TInt& aValue);

private:
    //Data:
    CConfigurationComponentsFactory* iFactory;
    CRestrictedAudioOutput* iRestrictedAudioOutput;
    CDRMConfigIntfc* iDRMConfigIntfc;
    MDataSource* iDataSource;
    CMultimediaDataSource* iMMDataSource;
    CAudioOutput* iAudioOutput;
    };

#endif // AudioOutputControlUtilityProxyImpl_H

// End of file
