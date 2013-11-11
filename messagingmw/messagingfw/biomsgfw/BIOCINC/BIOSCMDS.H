// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif
#if !defined (__BIOSCMDS_H__)
#define __BIOSCMDS_H__

#if !defined (__MSVSTD_HRH__)
#include <msvstd.hrh>
#endif

/** BIO message MTM commands for CBIOClientMtm::InvokeAsyncFunctionL().

The behaviour resulting from the command is specific to the BIO message type. 
Typically, parsing causes an interpretation of the raw message body, and storage 
of the results. Processing causes some system action to take place according 
to the instructions in the message. */
enum TBiosCmds {
	/** Parse the BIO message. */
	KBiosMtmParse = KMtmFirstFreeMtmFunctionId,
	/** Parse then process the BIO message. */
	KBiosMtmParseThenProcess,
	/** Process the BIO message. */
	KBiosMtmProcess
};

//
// progress class (just report a state and an error code)
// 
class TBioProgress
/** Progress information for BIO server MTM.

Progress information can be requested using CMsvOperation::ProgressL(). 
@publishedPartner
@released
*/
	{
public:
	/** BIO server MTM state. */
	enum TBioState 
		{
		/** Waiting for a command. */
		EBiosWaiting,
		/** Creating a parser. */
		EBiosCreating,
		/** Parsing a message. */
		EBiosParsing,
		/** Processing a message. */
		EBiosProcessing
		};

	/** BIO server MTM state. */
	TBioState			iBioState;
	/** Error code. */
	TInt				iErrorCode; // client side needs to be informed of an error
	/** Percentage of operation done. */
	TInt				iPercentDone;
	};

#endif
