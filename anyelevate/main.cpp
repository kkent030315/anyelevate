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

#include <windows.h>
#include <iostream>

#include "libanycall.h"
#include "anyelevate.hpp"

#pragma comment( lib, "libanycall64" )

int main( const int argc, const char** argv, const char** envp )
{
    SetConsoleTitle( TEXT( "libanyelevate by Kento Oki at www.godeye.club" ) );

    printf( "\n[=] libanyelevate by Kento Oki at www.godeye.club\n\n" );

    const bool use_default = argc < 2;

    if ( !libanycall::init( "ntdll.dll", "NtTraceControl" ) )
    {
        printf( "[!] failed to init libanycall\n" );
        return EXIT_FAILURE;
    }

    if ( !anyelevate::init() )
    {
        printf( "[!] failed to init anyelevate\n" );
        return EXIT_FAILURE;
    }

    printf( "[+] press any key to elevate\n" );
    std::cin.ignore();

    if ( !anyelevate::elevate( 
            use_default ? GetCurrentProcessId() : atoi( argv[1] ) ) )
    {
        printf( "[!] failed to elevate process privilege\n" );
        return EXIT_FAILURE;
    }

    std::cin.ignore();
    return EXIT_SUCCESS;
}