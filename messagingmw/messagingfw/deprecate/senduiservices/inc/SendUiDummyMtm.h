/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  SendUiDummyMtm  declaration
*
*/



#ifndef CSENDUIDUMMYMTM_H
#define CSENDUIDUMMYMTM_H

// INCLUDES
#include <mtclbase.h>
#include <eikfutil.h>

// ==========================================================
// CSendUiDummyMtm
// ==========================================================

class CSendUiDummyMtm : public CBaseMtm
    {
    public:
        CSendUiDummyMtm(
            CRegisteredMtmDll& aRegisteredMtmDll,
            CMsvSession& aSession );

    public:
        inline CCharFormatLayer*& CharFormat()
            {
            return iCharFormatLayer;
            }

        inline CParaFormatLayer*& ParaFormat()
            {
            return iParaFormatLayer;
            }

        inline void SetCharFormatL(const CCharFormatLayer& aCharFormatLayer)
            {
            delete iCharFormatLayer;
            iCharFormatLayer = NULL;
            iCharFormatLayer = aCharFormatLayer.CloneL();
            }

        inline void SetParaFormatL(const CParaFormatLayer& aParaFormatLayer)
            {
            delete iParaFormatLayer;
            iParaFormatLayer = NULL;
            iParaFormatLayer = aParaFormatLayer.CloneL();
            }
    };

#endif // CSENDUIDUMMYMTM_H

// End of file
