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
* Description:  Header of StreamControlEvent types.
*
*/



#ifndef STREAMCONTROLEVENTS_H
#define STREAMCONTROLEVENTS_H

const TUid KStreamControlEventStateChangedStopped    = {0x10207B31};
const TUid KStreamControlEventStateChangedPrimed     = {0x10207B32};
const TUid KStreamControlEventStateChangedPlaying    = {0x10207B33};
const TUid KStreamControlEventStateChangedPaused     = {0x10207B34};
const TUid KStreamControlEventStateChangedAutoPaused = {0x10207B35};
const TUid KStreamControlEventDurationChanged        = {0x10207B38};
const TUid KStreamControlEventSeekingSupportChanged  = {0x10207B39};
const TUid KStreamControlEventRandomSeekingSupportChanged = {0x10207B3A};


#endif // STREAMCONTROLEVENTS_H

// End of file
