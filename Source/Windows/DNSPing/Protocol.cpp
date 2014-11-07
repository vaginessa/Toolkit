﻿// This code is part of DNSPing(Windows)
// DNSPing, Ping with DNS requesting.
// Copyright (C) 2014 Chengr28
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


#include "DNSPing.h"

extern bool EDNS0, DNSSEC;

//Minimum supported system of Windows Version Helpers is Windows Vista.
#ifdef _WIN64
#else //x86
//Check operation system which higher than Windows 7.
bool __fastcall IsLowerThanWin8(void)
{
	OSVERSIONINFOEX OSVI = {0};
	OSVI.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	BOOL bOsVersionInfoEx = GetVersionExW((OSVERSIONINFO *)&OSVI);

	if (bOsVersionInfoEx && OSVI.dwPlatformId == VER_PLATFORM_WIN32_NT && 
		(OSVI.dwMajorVersion < 6U || OSVI.dwMajorVersion == 6U && OSVI.dwMinorVersion < 2U))
			return true;

	return false;
}
#endif

//Check empty buffer
bool __fastcall CheckEmptyBuffer(const void *Buffer, const size_t Length)
{
	if (Buffer == nullptr)
		return true;

	for (size_t Index = 0;Index < Length;Index++)
	{
		if (((uint8_t *)Buffer)[Index] != NULL)
			return false;
	}

	return true;
}

//Convert address strings to binary.
size_t __fastcall AddressStringToBinary(const PSTR AddrString, void *pAddr, const uint16_t Protocol, SSIZE_T &ErrorCode)
{
	SSIZE_T Result = 0;

//inet_ntop() and inet_pton() was only support in Windows Vista and newer system. [Roy Tam]
#ifdef _WIN64
#else //x86
	sockaddr_storage SockAddr = {0};
	int SockLength = 0;
#endif

//IPv6
	if (Protocol == AF_INET6)
	{
	//Check IPv6 addresses
		for (Result = 0;Result < (SSIZE_T)strlen(AddrString);Result++)
		{
			if (AddrString[Result] < 48 || AddrString[Result] > 58 && AddrString[Result] < 65 || AddrString[Result] > 70 && AddrString[Result] < 97 || AddrString[Result] > 102)
				break;
		}

		std::string sAddrString(AddrString);
	//Check abbreviation format.
		if (sAddrString.find(58) == std::string::npos)
		{
			sAddrString.clear();
			sAddrString.append("::");
			sAddrString.append(AddrString);
		}
		else if (sAddrString.find(58) == sAddrString.rfind(58))
		{
			sAddrString.replace(sAddrString.find(58), 1U, ("::"));
		}

	//Convert to binary.
	#ifdef _WIN64
		Result = inet_pton(AF_INET6, sAddrString.c_str(), pAddr);
		if (Result == SOCKET_ERROR || Result == FALSE)
	#else //x86
		SockLength = sizeof(sockaddr_in6);
		if (WSAStringToAddressA((PSTR)sAddrString.c_str(), AF_INET6, NULL, (LPSOCKADDR)&SockAddr, &SockLength) == SOCKET_ERROR)
	#endif
		{
			ErrorCode = WSAGetLastError();
			return EXIT_FAILURE;
		}
	#ifdef _WIN64
	#else //x86
		memcpy(pAddr, &((PSOCKADDR_IN6)&SockAddr)->sin6_addr, sizeof(in6_addr));
	#endif
	}
//IPv4
	else {
		size_t CommaNum = 0;
		for (Result = 0;Result < (SSIZE_T)strlen(AddrString);Result++)
		{
			if (AddrString[Result] != 46 && AddrString[Result] < 48 || AddrString[Result] > 57)
				return EXIT_FAILURE;
			else if (AddrString[Result] == 46)
				CommaNum++;
		}

		std::string sAddrString(AddrString);
	//Delete zero(s) before whole data.
		while (sAddrString.length() > 1U && sAddrString[0] == 48 && sAddrString[1U] != 46)
			sAddrString.erase(0, 1U);

	//Check abbreviation format.
		if (CommaNum == 0)
		{
			sAddrString.clear();
			sAddrString.append("0.0.0.");
			sAddrString.append(AddrString);
		}
		else if (CommaNum == 1U)
		{
			sAddrString.replace(sAddrString.find(46), 1U, (".0.0."));
		}
		else if (CommaNum == 2U)
		{
			sAddrString.replace(sAddrString.find(46), 1U, (".0."));
		}

	//Delete zero(s) before data.
		while (sAddrString.find(".00") != std::string::npos)
			sAddrString.replace(sAddrString.find(".00"), 3U, ("."));
		while (sAddrString.find(".0") != std::string::npos)
			sAddrString.replace(sAddrString.find(".0"), 2U, ("."));
		while (sAddrString.find("..") != std::string::npos)
			sAddrString.replace(sAddrString.find(".."), 2U, (".0."));
		if (sAddrString[sAddrString.length() - 1U] == 46)
			sAddrString.append("0");

	//Convert to binary.
	#ifdef _WIN64
		Result = inet_pton(AF_INET, sAddrString.c_str(), pAddr);
		if (Result == SOCKET_ERROR || Result == FALSE)
	#else //x86
		SockLength = sizeof(sockaddr_in);
		if (WSAStringToAddressA((PSTR)sAddrString.c_str(), AF_INET, NULL, (LPSOCKADDR)&SockAddr, &SockLength) == SOCKET_ERROR)
	#endif
		{
			ErrorCode = WSAGetLastError();
			return EXIT_FAILURE;
		}
	#ifdef _WIN64
	#else //x86
		memcpy(pAddr, &((PSOCKADDR_IN)&SockAddr)->sin_addr, sizeof(in_addr));
	#endif
	}

	return EXIT_SUCCESS;
}

//Convert data from char(s) to DNS query
size_t __fastcall CharToDNSQuery(const PSTR FName, PSTR TName)
{
	int Index[] = {(int)strlen(FName) - 1, 0, 0};
	Index[2U] = Index[0] + 1;
	TName[Index[0] + 2] = 0;

	for (;Index[0] >= 0;Index[0]--,Index[2U]--)
	{
		if (FName[Index[0]] == 46)
		{
			TName[Index[2U]] = (char)Index[1U];
			Index[1U] = 0;
		}
		else
		{
			TName[Index[2U]] = FName[Index[0]];
			Index[1U]++;
		}
	}
	TName[Index[2U]] = (char)Index[1U];

	return strlen(TName) + 1U;
}

//Validate packets
bool __fastcall ValidatePacket(const PSTR Buffer, const size_t Length, const uint16_t DNS_ID)
{
	auto pdns_hdr = (dns_hdr *)Buffer;

//DNS ID and Questions check
	if (pdns_hdr->ID != DNS_ID || pdns_hdr->Questions == 0)
		return false;

//EDNS0 Lable check
	if (EDNS0)
	{
		if (pdns_hdr->Additional == 0)
		{
			return false;
		}
		else if (pdns_hdr->Additional == 1U)
		{
			if (Length > sizeof(dns_edns0_label))
			{
				auto pEDNS0 = (dns_edns0_label *)(Buffer + Length - sizeof(dns_edns0_label));

			//UDP Payload Size and Z Field of DNSSEC check
				if (pEDNS0->UDPPayloadSize == 0 || DNSSEC && pEDNS0->Z_Field == 0)
					return false;
			}
			else {
				return false;
			}
		}
	}

	return true;
}