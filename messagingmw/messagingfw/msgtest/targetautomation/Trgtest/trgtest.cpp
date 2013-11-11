// Copyright (c) 1994-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// e32utils\trgtest\trgtesth.cpp
// 
//

#include <e32err.h>
#include <windows.h>
#include <stdio.h>

const TUint8 SOH=0x01;
const TUint8 STX=0x02;
const TUint8 EOT=0x04;
const TUint8 ACK=0x06;
const TUint8 NAK=0x15;
const TUint8 CAN=0x18;
const TUint8 SUB=0x1A;
const TUint8 BIGC=0x43;
const TUint8 BIGG=0x47;

#define READ_BUF_SIZE	32768
#define WRITE_BUF_SIZE	32768
#define MAX_LINE		32768
#define PACKET_SIZE		1024

#define MIN(a,b)		((a)<(b)?(a):(b))
#define OFFSET(p,off)	((void*)((char*)p+off))

#define RESET_COMM()	PurgeComm(Comm, PURGE_RXCLEAR|PURGE_TXCLEAR)


FILE* LogFile;
HANDLE Comm;

char ImgFileName[256];
TInt ImgFileSize;
char LineBuf[MAX_LINE+1];
void* ImgFileChunkBase;

#define TT_ASSERT(x)	__ASSERT_ALWAYS(x, __Panic(__LINE__))

void __Panic(TInt aLine)
	{
	fprintf(stderr, "Assertion failed at line %d\n", aLine);
	exit(999);
	}

void WriteLog(const void* aBuf, TInt aLength)
	{
	fwrite(aBuf, 1, aLength, LogFile);
	}

void WriteLogS(const char* aString)
	{
	WriteLog( aString, strlen(aString) );
	}

/*
void TraceLog(const char* aFmt, ...)
	{
	char buf[512];
	va_list list;
	va_start(list, aFmt);
	sprintf(buf, "TRGTEST: ");
// timestamp
	vsprintf(buf, aFmt, list);
	strcat(buf, "\n");
	WriteLogS(buf);
	}
*/

/*
YModem packet structure:
Byte 0 = STX
Byte 1 = sequence number (first user data packet is 1)
Byte 2 = complement of sequence number
Bytes 3-1026 = data (1K per packet)
Bytes 1027, 1028 = 16-bit CRC (big-endian)

A herald packet is sent first, with sequence number 0
The data field consists of the null-terminated file name
followed by the null-terminated file size in ASCII decimal
digits.
*/
struct SPacket
	{
	TUint8 iPTI;
	TUint8 iSeq;
	TUint8 iSeqBar;
	TUint8 iData[PACKET_SIZE];
	TUint8 iCRC1;
	TUint8 iCRC0;
	};

static const TUint16 crcTab[256] =
    {
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,0x8108,0x9129,0xa14a,
	0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,
	0x72f7,0x62d6,0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,0x2462,
	0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,0xa56a,0xb54b,0x8528,0x9509,
	0xe5ee,0xf5cf,0xc5ac,0xd58d,0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,
	0x46b4,0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,0x48c4,0x58e5,
	0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,
	0x9969,0xa90a,0xb92b,0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
	0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,0x6ca6,0x7c87,0x4ce4,
	0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,
	0x8d68,0x9d49,0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,0xff9f,
	0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,0x9188,0x81a9,0xb1ca,0xa1eb,
	0xd10c,0xc12d,0xf14e,0xe16f,0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,
	0x6067,0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,0x02b1,0x1290,
	0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,
	0xe54f,0xd52c,0xc50d,0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
	0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,0x26d3,0x36f2,0x0691,
	0x16b0,0x6657,0x7676,0x4615,0x5634,0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,
	0xb98a,0xa9ab,0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,0xcb7d,
	0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,0x4a75,0x5a54,0x6a37,0x7a16,
	0x0af1,0x1ad0,0x2ab3,0x3a92,0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,
	0x8dc9,0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,0xef1f,0xff3e,
	0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,
	0x3eb2,0x0ed1,0x1ef0
    };

void UpdateCrc(const void* aPtr, TInt aLength, TUint16& aCrc)
//
// Perform a CCITT CRC checksum.
//
	{

	register const TUint8* pB = (const TUint8*)aPtr;
	register TUint16 crc=aCrc;
    while (aLength--)
		crc=TUint16((crc<<8)^crcTab[(crc>>8)^*pB++]);
	aCrc=crc;
	}

void ClearCommError()
	{
	DWORD err;
	COMSTAT s;
	TT_ASSERT(ClearCommError(Comm,&err,&s));
	}

void SetupCommPort(TInt aPort)
	{
	char buf[10];
	DCB dcb;

	sprintf(buf, "COM%d", aPort);
	Comm = CreateFileA(buf, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (Comm == INVALID_HANDLE_VALUE)
		{
		fprintf(stderr, "Can't open COM%d : Error %d\n", aPort, GetLastError());
		exit(999);
		}
	dcb.DCBlength = sizeof(dcb);
	TT_ASSERT(GetCommState(Comm, &dcb));
	TT_ASSERT(SetCommMask(Comm, EV_ERR|EV_RXCHAR));
	TT_ASSERT(SetupComm(Comm, READ_BUF_SIZE, WRITE_BUF_SIZE));
	ClearCommError();

	dcb.fAbortOnError = TRUE;
	dcb.BaudRate=115200;
	dcb.Parity=NOPARITY;
	dcb.fParity = FALSE;
	dcb.fErrorChar = FALSE;
	dcb.ByteSize=8;
	dcb.StopBits=ONESTOPBIT;
	dcb.fInX  = FALSE;
	dcb.fOutX = FALSE;
	dcb.XonChar = 0;
	dcb.XoffChar = 0;
	dcb.ErrorChar = 0;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fDsrSensitivity =  FALSE;
	dcb.fRtsControl = RTS_CONTROL_DISABLE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	TT_ASSERT(SetCommState(Comm, &dcb));
	EscapeCommFunction(Comm, SETDTR);
	EscapeCommFunction(Comm, SETRTS);
	}

void SetupTimeout(TInt aInt, TInt aTot)
	{
	COMMTIMEOUTS ct;
	GetCommTimeouts(Comm, &ct);
	ct.ReadIntervalTimeout = aInt;	//ms
	ct.ReadTotalTimeoutMultiplier = 0;
	ct.ReadTotalTimeoutConstant = aTot;	// ms
	ct.WriteTotalTimeoutConstant = 0;
	ct.WriteTotalTimeoutMultiplier = 0;
	SetCommTimeouts(Comm, &ct);
	}

TInt CommRead1(TInt aTimeout)
	{
	unsigned char c;
	DWORD n=0;
	BOOL ok;

	SetupTimeout(0,aTimeout);
	ok = ReadFile(Comm, &c, 1, &n, NULL);
	if (!ok)
		ClearCommError();
	if (ok && n)
		return c;
	return KErrTimedOut;
	}

TInt CommRead(void* aBuf, TInt aMax, TInt aTimeout)
	{
	DWORD n=0;
	BOOL ok;

	SetupTimeout(100, aTimeout);
	ok = ReadFile(Comm, aBuf, aMax, &n, NULL);
	if (!ok)
		ClearCommError();
	if (n==0)
		return KErrTimedOut;
	return n;
	}

void CommWrite(const void* aBuf, TInt aLen)
	{
	DWORD n = 0;
	BOOL ok = WriteFile(Comm, aBuf, aLen, &n, NULL);
	if (!ok)
		ClearCommError();
	}

void CommWriteC(TUint aChar)
	{
	unsigned char c = (unsigned char)aChar;
	CommWrite(&c, 1);
	}

void CommWriteS(const char* aString)
	{
	CommWrite( aString, strlen(aString) );
	}

TInt PreparePacket(SPacket& pkt, TInt aSeq)
	{
	TInt r = KErrNone;
	TUint16 crc = 0;

	pkt.iPTI = STX;
	pkt.iSeq = (TUint8)(aSeq>=0 ? aSeq : 0);
	pkt.iSeqBar = (TUint8)~pkt.iSeq;
	if (aSeq>0)
		{
		TInt l;
		TInt fpos = (aSeq-1)*PACKET_SIZE;	// file position of packet
		if ( fpos >= ImgFileSize )
			return KErrEof;
		l = MIN(PACKET_SIZE, ImgFileSize-fpos);
		memcpy(pkt.iData, OFFSET(ImgFileChunkBase,fpos), l);
		if (l<PACKET_SIZE)
			memset(pkt.iData+l, 0, PACKET_SIZE-l);
		}
	else
		{
		memset(pkt.iData, 0, PACKET_SIZE);
		if (aSeq==0)
			{
			const char* p = ImgFileName;
			const char* q = p + strlen(p);
			while (--q>=p && *q!='\\') {}
			sprintf( (char*)pkt.iData, "%s%c%d", q+1, 0, ImgFileSize);
			}
		}
	UpdateCrc(pkt.iData, PACKET_SIZE, crc);
	pkt.iCRC1 = (TUint8)(crc>>8);
	pkt.iCRC0 = (TUint8)crc;
	return r;
	}

TInt SendPacket(TInt& aSeq, TBool aStream)
	{
	TInt c;
	SPacket pkt;
	TInt retries = 10;
	TInt tmout = (aSeq>=0) ? 2000 : 500;
	TInt r = PreparePacket(pkt, aSeq);

//	if ( (aSeq & 7) == 0 )
//		printf("SendPacket %d\n", aSeq);
	if ( (aSeq & 0x0F) == 0 )
		printf(".");

	if (r!=KErrNone)
		return r;
	for(;;)
		{
		RESET_COMM();
		CommWrite(&pkt, sizeof(pkt));
		if (aStream)
			break;
		c = CommRead1(tmout);
		if (c==KErrTimedOut && aSeq<0)
			return KErrNone;
		if (c>=0)
			{
			if (c==ACK)
				break;
			}
		if (--retries==0)
			return KErrTimedOut;
		}
	if (aSeq==0)
		{
		c = CommRead1(100);
		if (c==KErrTimedOut)
			{
			++aSeq;
			return KErrNone;
			}
		if (aStream && c!=BIGG)
			return KErrTimedOut;
		else if (!aStream && c!=BIGC)
			return KErrTimedOut;
		}
	++aSeq;
	return r;
	}

TInt SendImageFile()
	{
	TInt r = 0;
	TInt b1;
	TUint8 b2[256];
	TBool stream;
	TInt seq = 0;

	printf("Sending image file: %s\n",ImgFileName);
//	TraceLog("Sending image file: %s",ImgFileName);

	RESET_COMM();
	r = CommRead(b2, 256, 20000);
	if (r<0)
		return r;
//	TraceLog(b2);
	if (b2[r-1]!=BIGG && b2[r-1]!=BIGC)
		return KErrTimedOut;
	stream = (b2[r-1]==BIGG);
	seq = 0;
	r = KErrNone;
	while (r==KErrNone)
		{
		r = SendPacket(seq, stream);
		if (r!=KErrNone && r!=KErrEof)
			printf("<SendPacket %d -> %d\n", seq, r);
//		TraceLog(_L8("<SendPacket seq=%d r=%d"), seq, r);
		}
	if (r!=KErrEof)
		return r;
	r=KErrNone;
	RESET_COMM();
	CommWriteC(EOT);
	b1 = CommRead1(500);
	if (b1==KErrTimedOut)
		return KErrNone;
	if (b1!=ACK)
		return KErrNone;
	b1 = CommRead1(1000);
	if (b1==KErrTimedOut)
		return KErrNone;
	if (stream && b1!=BIGG)
		return KErrNone;
	else if (!stream && b1!=BIGC)
		return KErrNone;
	seq = -1;
	r = SendPacket(seq, stream);
	return r;
	}

void GetResponse()
	{
	TInt r;
	do	{
		r = CommRead(LineBuf, MAX_LINE, 1000);
		if (r>=0)
			LineBuf[r]=0, WriteLog(LineBuf, r);
		} while (r>=0);
	}

void IssueCommandAndGetResponse(const char* aCmd)
	{
	CommWriteS(aCmd);
	GetResponse();
	}

int token_length(const char* s)
	{
	const char* p = s;
	for (; *p && !isspace(*p); ++p) {}
	return p - s;
	}

const char* skip_space(const char* s)
	{
	for (; *s && isspace(*s); ++s) {}
	return s;
	}

void GetMonitorInfo()
	{
	BOOL ok = FALSE;
	int l;
	int type;
	RESET_COMM();
	IssueCommandAndGetResponse("replacement\xd");
	IssueCommandAndGetResponse("f\xd");
	const char* p = LineBuf;
	const char* q = LineBuf + strlen(LineBuf);
	while (p<q)
		{
		p = skip_space(p);
		if (p+16>q || memcmp(p, "Fault Category: ", 16))
			{
			p += token_length(p);
			continue;
			}
		p = skip_space(p+16);
		l = token_length(p);
		if (l!=4 || memcmp(p, "KERN", 4))
			{
			p += l;
			continue;
			}
		p = skip_space(p+4);
		if (p+14>q || memcmp(p, "Fault Reason: ", 14))
			{
			p += token_length(p);
			continue;
			}
		p = skip_space(p+14);
		if (p+8<=q && memcmp(p, "00000050", 8)==0)
			{
			ok = TRUE;
			break;
			}
		p+=8;
		continue;
		}
	if (ok)
		return;

	IssueCommandAndGetResponse("i\xd");
	for (type=0; type<14; ++type)
		{
		char b[8];
		sprintf(b, "c%x\xd", type);
		IssueCommandAndGetResponse(b);
		}
	}

TInt ReceiveTestLog()
	{
	const TInt KMaxNoActivityTimeout = 60 * 60 * 1000;	// 1 hour
	const TInt KSerialPollInterval = 10;
	TInt bytes = 0;
	TInt error = KErrNone;
	TInt noActivityTimeMilliSeconds = 0;

	printf("\nLogging to file started.\n");
	
	for (;;)
		{
		bytes =	CommRead(LineBuf, 100, KSerialPollInterval);
		if (bytes > 0)
			{
			if (bytes >= 23 && memcmp(LineBuf+bytes-23, "!TERMINATE_MSG_LOGGING!", 23)==0)
				{
				printf("SUCCESS: Terminated from remote.\n");
				error = KErrNone;
				break;
				}
			LineBuf[bytes]=0;
			WriteLog(LineBuf, bytes);
//			printf(LineBuf);
			noActivityTimeMilliSeconds=0;
			}
		else if (bytes == KErrTimedOut)
			{
			noActivityTimeMilliSeconds += KSerialPollInterval;
			if (noActivityTimeMilliSeconds >= KMaxNoActivityTimeout)
				{
				error = KErrTimedOut;
				printf("FAILED: Timed out.\n");
				break;
				}
			}
		else
			{
			error = bytes;
			printf("FAILED: Error detected (%d).\n",error);
			break;
			}
		}

	CommRead(LineBuf, 100, 100);
	return error;
	}

/*
void ReceiveTestLog()
	{
	TInt r=0;
	TInt r2;
	TInt tmc=0;

	for (;;)
		{
		r =	CommRead(LineBuf, 100, 100);
		if (r>0)
			{
			LineBuf[r]=0;
			WriteLog(LineBuf, r);
			printf(LineBuf);
			}
//		printf("r=%d\n",r);
		if (r>=23 && memcmp(LineBuf+r-23, "!TERMINATE_MSG_LOGGING!", 23)==0)
			break;
		check = TRUE;
		}

	for(;;)
		{
		TBool check = FALSE;
		r = CommRead(LineBuf, MAX_LINE, 2000);
		if (r==KErrTimedOut)
			{
			++tmc;
			if (tmc<500)
				continue;
			check = TRUE;
			}
		else if (r>=0)
			tmc=0;
		if (r>=10 && memcmp(LineBuf+r-10, "Password: ", 10)==0)
			check = TRUE;
		if (r>0)
			WriteLog(LineBuf, r);
		if (check || r==KErrTimedOut)
			{
			// Send CR to see if it elicits monitor prompt
			CommWriteC(0x0d);
			r2 = CommRead(LineBuf, 12, 1000);
			if (r2>0)
				WriteLog(LineBuf, r2);
			if (r2==12 && memcmp(LineBuf, "\xd\xaPassword: ", 12)==0)
				break;
			}
		if (r<0)
			break;
		}

	TT_ASSERT(r>0 || r==KErrTimedOut);
	if (r>0)
		{
		// yes, extract info
		GetMonitorInfo();
		CommWriteS("X\xd");		// this should make the target do a cold reboot
		}
	else
		{
		// locked up somehow
		TraceLog("Target hung");
		}
	}
*/

int main(int argc, char** argv)
	{
	TInt r;
	TInt port;
	const char* logfilename;
	FILE* img;
	int n;

	if (argc != 4)
		{
		fprintf(stderr, "trgtest <port#> <imagefilename> <logfilename>\n");
		exit(999);
		}
	port = atoi(argv[1]);
	strcpy(ImgFileName, argv[2]);
	logfilename = argv[3];
	LogFile = fopen(logfilename, "wb");
	if (!LogFile)
		{
		fprintf(stderr, "Can't open %s for write\n", logfilename);
		exit(999);
		}

//	TraceLog("Port  = %d\n", port);
//	TraceLog("Image = %s\n", ImgFileName);
//	TraceLog("Log   = %s\n", logfilename);

	img = fopen(ImgFileName, "rb");
	if (!img)
		{
		fprintf(stderr, "Can't open %s for read\n", ImgFileName);
		exit(999);
		}
	fseek(img,0,SEEK_END);
	ImgFileSize=ftell(img);
	fseek(img,0,SEEK_SET);
//	TraceLog("ImgFileSize = %d\n", ImgFileSize);
	ImgFileChunkBase = malloc(ImgFileSize);
	TT_ASSERT(ImgFileChunkBase != NULL);
	n = fread(ImgFileChunkBase, 1, ImgFileSize, img);
	TT_ASSERT(n == ImgFileSize);
	fclose(img);

	SetupCommPort(port);

	r = SendImageFile();
	free(ImgFileChunkBase);
	if (r==KErrNone)
		{
//		TraceLog("Send image file OK");
		ReceiveTestLog();
		}
//	else
//		TraceLog("SendImageFile -> %d",r);

	WriteLogS("\n\n");
//	TraceLog("DONE");

	EscapeCommFunction(Comm, CLRRTS);
	EscapeCommFunction(Comm, CLRDTR);
	CloseHandle(Comm);
	fclose(LogFile);
	return 0;
	}
