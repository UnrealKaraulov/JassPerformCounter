#include <Windows.h>
#include <stdio.h>
#include "PerfCounter.h"
#include <MinHook.h>
#pragma comment(lib,"libMinHook.x86.lib")

CPerformanceCounter ctr[512];

LONGLONG lDiff[ 512 ];
LONGLONG latestmicroseconds[ 512 ];
LONGLONG latestmilliseconds[ 512 ];
float latestseconds[ 512 ];

int GameDll = 0;
int StormDll = 0;

char * GetStringFromJassString( int straddr )
{
	int v1;
	char * result;

	v1 = *( int * ) ( straddr + 8 );
	if ( v1 )
		result = ( char * ) *( int * ) ( v1 + 28 );
	else
		result = "NULL";
	return result;
}

void WriteLogFile( int straddr )
{
	FILE* pFile = NULL;
	fopen_s( &pFile , ".\\logFile.txt" , "a" );
	fprintf( pFile , "%s\n" , GetStringFromJassString( straddr ) );
	fclose( pFile );
}

void WriteLogFileWoutEnd( char * straddr )
{
	FILE* pFile = NULL;
	fopen_s( &pFile , ".\\logFile.txt" , "a" );
	fprintf( pFile , "%s" , straddr );
	fclose( pFile );
}

void WriteLogFileWithEnd( char * straddr )
{
	FILE* pFile = NULL;
	fopen_s( &pFile , ".\\logFile.txt" , "a" );
	fprintf( pFile , "%s\n" , straddr );
	fclose( pFile );
}

void TextPrint( char *szText , float fDuration )
{
	UINT32 dwDuration = *( ( UINT32 * ) &fDuration );
	UINT32 GAME_GlobalClass = GameDll + 0xAB4F80;
	UINT32 GAME_PrintToScreen = GameDll + 0x2F8E40;
	__asm
	{
		PUSH	0xFFFFFFFF
			PUSH	dwDuration
			PUSH	szText
			PUSH	0x0
			PUSH	0x0
			MOV		ECX , [ GAME_GlobalClass ]
			MOV		ECX , [ ECX ]
			CALL	GAME_PrintToScreen
	}
}



int sub_6F3BAA20addr = 0;
_declspec( naked ) int __fastcall sub_6F3BAA20( char * a1, int unused )
{
	__asm
	{
		MOV EAX , sub_6F3BAA20addr
		JMP EAX
	}
}

void __cdecl Start( int id )
{
	ctr[ id ].Start( );
}


void __cdecl Stop( int id )
{
	ctr[id].Stop( );
	
	lDiff[ id ] = ctr[ id ].Difference( );

	latestmicroseconds[ id ] = ctr[ id ].AsMicroSeconds( lDiff[ id ] );
	latestmilliseconds[ id ] = ctr[ id ].AsMilliSeconds( lDiff[ id ] );
	latestseconds[ id ] = ( float ) ctr[ id ].AsSeconds( lDiff[ id ] );

}

void __cdecl PerfCounterPrintResult( int id )
{
	char buffer[ 256 ];
	sprintf_s( buffer , 256 , "Time [%i]: %I64d microseconds\n" , id, latestmicroseconds[ id ] );
	TextPrint( buffer , 10 );
	sprintf_s( buffer , 256 , "Time [%i]: %I64d milliseconds\n" , id , latestmilliseconds[ id ] );
	TextPrint( buffer , 10 );
	sprintf_s( buffer , 256 , "Time [%i]: %f seconds\n" , id , latestseconds[ id ] );
	TextPrint( buffer , 10 );
}

int __cdecl PerfCounterGetSec( int id )
{ 
	char msg[256];
	sprintf_s( msg , 256 , "%f" , latestseconds[ id ] );
	int retvalue = 0;
	try
	{
		retvalue = sub_6F3BAA20( msg , 0 );
	}
	catch (... )
	{
		WriteLogFileWithEnd( msg );
	}
	return retvalue;
}




int Game_sub_6F455110 = 0;
int __cdecl sub_6F455110( int a1 , const char * FuncName , const char *  Args )
{
	__asm
	{
		PUSH Args
		MOV EDX , FuncName
		MOV ECX , a1
		CALL Game_sub_6F455110
	}
}



typedef int( __cdecl *sub_6F3D4020 )( );
sub_6F3D4020 sub_6F3D4020org;
sub_6F3D4020 sub_6F3D4020ptr;


typedef LPVOID( __cdecl * Game_sub_6F453C00 )( );
Game_sub_6F453C00 sub_6F453C00;


int __cdecl xReadProcessMemory( int addr )
{
	if ( IsBadReadPtr( ( LPCVOID )addr, 4 ) )
	{
		MessageBox( 0, "Found Bad Memory #readmem", "Warning", 0 );
		return 0;
	}

	return *( int* )addr;
}

void __cdecl xWriteProcessMemory( int addr, int val )
{
	if ( IsBadWritePtr( ( LPVOID )addr, 4 ) )
	{
		MessageBox( 0, "Found Bad Memory #writemem", "Warning", 0 );
		return;
	}

	*( int* )addr = val;
}


int __cdecl sub_6F3D4020my( )
{
	sub_6F3D4020ptr( );
	sub_6F455110( ( int ) &xWriteProcessMemory, "WriteProcessMemory", "(II)V" );
	sub_6F455110( ( int ) &xReadProcessMemory, "ReadProcessMemory", "(I)I" );
	sub_6F455110( ( int ) &WriteLogFile , "PrintToLog" , "(S)V" );
	sub_6F455110( ( int ) &PerfCounterGetSec , "PerfCounterGetSec" , "(I)S" );
	sub_6F455110( ( int ) &Start , "StartPerfCounter" , "(I)V" );
	sub_6F455110( ( int ) &Stop , "StopPerfCounter" , "(I)V" );
	return sub_6F455110( ( int ) &PerfCounterPrintResult , "PerfCounterPrintResult" , "(I)V" );
}

void FirstInit( )
{
	for ( int i = 0; i < 512; i++ )
	{
		ctr[ i ] = CPerformanceCounter( );
		lDiff[ i ] = NULL;
		latestmicroseconds[ i ] = NULL;
		latestmilliseconds[ i ] = NULL;
		latestseconds[ i ] = 0.0f;
	}
	sub_6F455110( ( int )&xWriteProcessMemory, "WriteProcessMemory", "(II)V" );
	sub_6F455110( ( int )&xReadProcessMemory, "ReadProcessMemory", "(I)I" );
	sub_6F455110( ( int ) &WriteLogFile , "PrintToLog" , "(S)V" );
	sub_6F455110( ( int ) &PerfCounterGetSec , "PerfCounterGetSec" , "(I)S" );
	sub_6F455110( ( int ) &Start , "StartPerfCounter" , "(I)V" );
	sub_6F455110( ( int ) &Stop , "StopPerfCounter" , "(I)V" );
	sub_6F455110( ( int ) &PerfCounterPrintResult , "PerfCounterPrintResult" , "(I)V" );
}

BOOL WINAPI DllMain( HINSTANCE hDLL , UINT reason , LPVOID reserved )
{
	if ( reason == DLL_PROCESS_ATTACH )
	{
		MH_Initialize( );

		GameDll = ( int ) GetModuleHandle( "Game.dll" );
		StormDll = ( int ) GetModuleHandle( "Game.dll" );

		if ( !GameDll || !StormDll )
		{
			MessageBox( 0 , "Не обнаружены Game.dll или Storm.dll ок для выхода." , "Ошибка" , MB_OK );
			return FALSE;
		}

		sub_6F3BAA20addr = GameDll + 0x3BAA20; 
		int dword_6FADB154 = GameDll + 0xADB154;
		Game_sub_6F455110 = GameDll + 0x455110;
		sub_6F453C00 = ( Game_sub_6F453C00 ) ( GameDll + 0x453C00 );

		sub_6F3D4020org = ( sub_6F3D4020 ) ( GameDll + 0x3D4020 );
		MH_CreateHook( sub_6F3D4020org , &sub_6F3D4020my , reinterpret_cast< void** >( &sub_6F3D4020ptr ) );
		MH_EnableHook( sub_6F3D4020org );

		FirstInit( );
	}
	else if ( reason == DLL_PROCESS_DETACH )
	{

	}
	return TRUE;
}