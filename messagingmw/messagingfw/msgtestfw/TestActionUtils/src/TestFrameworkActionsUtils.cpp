// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
*/

#include "TestFrameworkActionsUtils.h"
#include "watcherdef.h"

#include <es_sock.h>
#include <smsuaddr.h>
#include <etelmm.h>
#include <etel.h>
#include <etelmm.h>

const TInt KDefBufferSize = 1024;

TBool TestFrameworkActionsUtils::CheckIfWatchersAlreadyRunningL()
	{
	TFindProcess find(_L("Watcher*"));
	TFileName name;	// not used
	return (find.Next(name) == KErrNone);
	}
	
	
TBool TestFrameworkActionsUtils::CheckIfSmsWatcherAlreadyRunningL()
	{
	RSocketServ socketServer;
	RSocket socket;
	
	User::LeaveIfError(socketServer.Connect());
	CleanupClosePushL(socketServer);

	TProtocolDesc protoInfo;
	TProtocolName protocolname;
	protocolname.Copy(KSmsDatagram);
	User::LeaveIfError(socketServer.FindProtocol(protocolname,protoInfo));
	User::LeaveIfError(socket.Open(socketServer,
										protoInfo.iAddrFamily,
										protoInfo.iSockType, 
										protoInfo.iProtocol));

	CleanupClosePushL(socket);
	
	TSmsAddr addr;
	addr.SetSmsAddrFamily(ESmsAddrRecvAny);
	TInt err = socket.Bind(addr);
	
	socket.CancelAll();
	CleanupStack::PopAndDestroy(2); //socketServer, socket
		
	return (err == KErrAlreadyExists);
	}
/**
Compares two input files (ASCII & Unicode).
@return the error code
@param aFileOne	- First file
@param aFileTwo	- Second file
@param aIsUnicode - Whether unicode or ascii
@param aIgnoreCharList - Ignore characters list
@param aDiffFlag - Out parameter ETrue if the files are different
@leave KErrNoMemory
 */
TInt TestFrameworkActionsUtils::CompareFilesL(TPtrC aFileOne, TPtrC aFileTwo, TBool aIsUnicode,
									  CArrayFixFlat<TUint16>* aIgnoreCharList, TBool& aDiffFlag)
	{
	// Retern Code
	TInt retCode = KErrNone;

	// Streams for the file1 and file2 to compare
	RFileReadStream fileReadStream1;
	RFileReadStream fileReadStream2;

	// Files difference flag
	aDiffFlag = EFalse;

	RFs iFs;
	User::LeaveIfError(iFs.Connect());

	// Open file one to compare
	retCode = fileReadStream1.Open( iFs, aFileOne, EFileShareReadersOnly);//EFileRead);
	if(retCode == KErrNone)
		{
		retCode = fileReadStream2.Open( iFs, aFileTwo, EFileShareReadersOnly);//EFileRead);
		if(retCode == KErrNone)
			{
			// To hold the line content of file one 
			HBufC* lineOneBuffer = HBufC::NewLC(KDefBufferSize);
			TPtr ptrLineOne = lineOneBuffer->Des();
			
			// To hold the line content of file two
			HBufC* lineTwoBuffer = HBufC::NewLC(KDefBufferSize);
			TPtr ptrLineTwo = lineTwoBuffer->Des();

			// Eof indicators
			TBool	eofOne = EFalse;
			TBool	eofTwo = EFalse; 
			
			// Read the file one and file two data
			do   
				{
				// Read file one data
				eofOne = ReadDataL(fileReadStream1, ptrLineOne, aIsUnicode, aIgnoreCharList);
				// Read file two data
				eofTwo = ReadDataL(fileReadStream2, ptrLineTwo, aIsUnicode, aIgnoreCharList);
				// Check EOF state of the files.
				// Either both the files will be in EOF state or 
				// none of the files will be in EOF state			  
				if((!eofOne && !eofTwo)||(eofOne && eofTwo))
	            	{			
					// Compare the read lines from file one and the file two
					if(ptrLineOne.Compare(ptrLineTwo) != 0)
						{
						// Different data content
						aDiffFlag = ETrue;
						}
					}
				else
					{
					//Different EOF
					aDiffFlag = ETrue;
					}
				} while(!eofOne && !eofTwo && !aDiffFlag);				
			
			// Delete line buffers
			CleanupStack::PopAndDestroy(2, lineOneBuffer);				
			}
		}
	
	// Close the open streams
	fileReadStream1.Close();
	fileReadStream2.Close();	

	iFs.Close();
	// Return the difference flag
	return retCode;
	}

/**
Reads data chunk from the file stream.
@return ETrue if the EOF found.
@param aFileReadStream - reference to file stream 
@param aPtrLineBuffer - reference to the buffer data read
@param aIsUnicode - flag to check is it Unicode or ASCII file
@param aIgnoreCharList - pointer to ingnore char list
@leave KErrNotFound
 */
TBool TestFrameworkActionsUtils::ReadDataL(RFileReadStream& aFileReadStream,
	  TPtr& aPtrLineBuffer, TBool aIsUnicode, CArrayFixFlat<TUint16>* aIgnoreCharList)
 	{
	TUint16			element = 0;
	TKeyArrayFix	key(0, ECmpTUint16);
	TInt			charPosition = 0;
	TBool			eof = EFalse;
	TInt			errorCode = KErrNone;
	TUint8			aCharASCII = 0;

	aPtrLineBuffer.FillZ();
	aPtrLineBuffer.SetLength(0);	
	
	// Validate the input ignore char list
	if (!aIgnoreCharList)
		{
		// Invalid parameter passed to ReadDataL: No ignore char list passed
		User::Leave(KErrNotFound);
		}

	// Read data from file and store it in lineBuffer 
	do   
		{
		if(aIsUnicode)
			{
			TRAP(errorCode, (element = aFileReadStream.ReadUint16L()));
			}
		else
			{
			TRAP(errorCode, (aCharASCII = aFileReadStream.ReadUint8L()));
			element = (TUint16) aCharASCII;
			}
			
		if (errorCode!=KErrEof)
			{		
			// Check the availability of ignore char in the array							
			if ( aIgnoreCharList->Find(element, key, charPosition) != KErrNone )
				{
				// Append the char to the buffer if the read char is not ignore char
				aPtrLineBuffer.Append(element);
				}
			}								
			else
				{
				eof = ETrue;
				break;
				}
				
			// Stop at the end of line or no more data 
			} while(aPtrLineBuffer.Length()<KDefBufferSize);
	
	// Return end of file flag		
	return eof;	
	}



/**
Get Byte from Unicode Hex Text character
@return 
@param Hex character
@leave None
 */
TUint8 TestFrameworkActionsUtils::GetByteFromUnicodeHexTextChar(const TUint16 *input)
	{
	TUint8 result = (TUint8)*input;

	if(result >='0' && result <='9')
		result = (TUint8)(result - '0');
	else if(result >='a' && result <='f')
		result = (TUint8)(result - 'a' + 10);
	else if(result >='A' && result <='F')
		result = (TUint8)(result - 'A' + 10);
	else
		result = 0;

	return result;
	}
