#include <Windows.h>
#include <iostream>


int R2I( void *a1 )
{
	signed int v1; // esi@1
	int v2; // edx@1
	int result; // eax@2
	int v4; // eax@5
	char v5; // cl@5

	v1 = *( signed int*)a1;
	v2 = ( ( unsigned int ) *( signed int* ) a1 >> 23 ) & 0xFF;
	if ( ( unsigned int ) v2 >= 0x7F )
	{
		if ( ( unsigned int ) v2 < 0x9E )
		{
			v4 = v1 & 0x7FFFFF | 0x800000;
			v5 = -106 - ( ( unsigned int ) *( signed int* ) a1 >> 23 );
			if ( 150 - v2 < 0 )
				result = ( ( v4 << -v5 ) ^ ( v1 >> 31 ) ) - ( v1 >> 31 );
			else
				result = ( ( ( unsigned int ) v4 >> v5 ) ^ ( v1 >> 31 ) ) - ( v1 >> 31 );
		}
		else
		{
			result = 2147483648 - ( ( unsigned int ) ~v1 >> 31 );
		}
	}
	else
	{
		result = 0;
	}
	return result;
}



int main( )
{
	float flt1 = 999.60;

	int int1 = R2I( &flt1 );

	std::cout << int1 << std::endl;

	system( "pause" );
	return 0;
}