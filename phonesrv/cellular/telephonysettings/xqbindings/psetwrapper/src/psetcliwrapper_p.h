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


#ifndef PSETCLIWRAPPER_P_H_
#define PSETCLIWRAPPER_P_H_

#include <mpsetcliobserver.h>

class PSetCliWrapper;

class PSetCliWrapperPrivate : public MPsetCliObserver
{
public:
    PSetCliWrapperPrivate( PSetCliWrapper &owner );
    ~PSetCliWrapperPrivate();
    
public: // From MPsetCliObserver
    void SetEngineContact( MPsetCli* aEngine );
    void HandleCliRequestingL( TBool aOngoing, 
            TBool aInterrupted );
    void CliInformationL( TPsuiCli aType );
    void HandleCliStatusL( TUint8 aBsc[KPSetNumberOfBsc], TPsuiCli aMode );
    void HandleCnapStatusL( TInt aStatus );
    void HandleCliErrorL( TInt aError );
private:
    PSetCliWrapper &m_Owner;
};

#endif
    
// End of File. 
