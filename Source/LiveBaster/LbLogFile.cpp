// Project LiveBaster                                                         //
// Copyright © 1993-2023 LiveBaster. All Rights Reserved.                     //
//                                                                            //
//	FILE:       LbLogFile.cpp                                                 //
//	AUTHOR:     Victor Artyukhov                                              //
//	SUPPORT:    www.livebaster.ru; support@livebaster.ru                      //
//                                                                            //

#include "LbLogFile.h"

void LIVEBASTERSHARED_EXPORT AddLog( char* fileName, const char* message )
{
	
	if( strlen( fileName ) != 0 ) {
		FILE* out = 0;
		if( fopen_s( &out, fileName, "at" ) == 0 ) {
			fprintf( out, "%s\n", message );
			fflush( out );
			fclose( out );
		}
	}
	
}
