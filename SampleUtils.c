/*
	File:		SampleUtils.c
	
	Contains:	Common utility routines

	Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc.
				("Apple") in consideration of your agreement to the following terms, and your
				use, installation, modification or redistribution of this Apple software
				constitutes acceptance of these terms.  If you do not agree with these terms,
				please do not use, install, modify or redistribute this Apple software.

				In consideration of your agreement to abide by the following terms, and subject
				to these terms, Apple grants you a personal, non-exclusive license, under Apple�s
				copyrights in this original Apple software (the "Apple Software"), to use,
				reproduce, modify and redistribute the Apple Software, with or without
				modifications, in source and/or binary forms; provided that if you redistribute
				the Apple Software in its entirety and without modifications, you must retain
				this notice and the following text and disclaimers in all such redistributions of
				the Apple Software.  Neither the name, trademarks, service marks or logos of
				Apple Computer, Inc. may be used to endorse or promote products derived from the
				Apple Software without specific prior written permission from Apple.  Except as
				expressly stated in this notice, no other rights or licenses, express or implied,
				are granted by Apple herein, including but not limited to any patent rights that
				may be infringed by your derivative works or by other works in which the Apple
				Software may be incorporated.

				The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
				WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
				WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
				PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
				COMBINATION WITH YOUR PRODUCTS.

				IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
				CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
				GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
				ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION
				OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT
				(INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN
				ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Copyright � 2003 Apple Computer, Inc., All Rights Reserved
*/


#include <Carbon/Carbon.h>
#include "WindowFun.h"




OSStatus	GetControlBySigAndID( WindowRef window, OSType  signature, SInt32 id, ControlRef *control )
{
	ControlID	controlID;
	
	controlID.id		= id;
	controlID.signature	= signature;

	return( GetControlByID( window, &controlID, control ) );
}


CFStringRef	GetControlCFString( WindowRef window, OSType signature, SInt32 id )
{
	ControlRef		control;
	SInt32			dataSize;
	OSStatus		err;
	CFStringRef		cfString		= NULL;
	
	GetControlBySigAndID( window, signature, id, &control );
	if ( control == NULL ) goto Bail;
	
	err	= GetControlData( control, 0, kControlStaticTextCFStringTag, sizeof(CFStringRef), (Ptr)&cfString, &dataSize );
	if ( err != noErr ) goto Bail;
	
	return( cfString );
	
Bail:
	if ( cfString != NULL ) CFRelease( cfString );
	return( NULL );
}

void	SetControlCFString( WindowRef window, OSType signature, SInt32 id, CFStringRef cfString )
{
	ControlRef		control;
	
	GetControlBySigAndID( window, signature, id, &control );
	if ( control == NULL ) goto Bail;
	
	(void) SetControlData( control, 0, kControlStaticTextCFStringTag, sizeof(CFStringRef), &cfString );
	Draw1Control( control );
	
Bail:
	return;
}

char	*GetControlCString( WindowRef window, OSType signature, SInt32 id, char *cString )
{
	ControlRef		control;
	SInt32			dataSize;

	cString[0]			= 0;	
	GetControlBySigAndID( window, signature, id, &control );
	if ( control == NULL ) goto Bail;
	
	(void)GetControlData( control, 0, kControlStaticTextTextTag, 255, cString, &dataSize );
	cString[dataSize]	= 0;
Bail:
	return( cString );
}

void	SetControlCString( WindowRef window, OSType signature, SInt32 id, char *cString )
{
	ControlRef		control;

	GetControlBySigAndID( window, signature, id, &control );
	if ( control == NULL ) goto Bail;
	
	(void) SetControlData( control, 0, kControlStaticTextTextTag, strlen(cString), cString );

	Draw1Control( control );
	
Bail:
	return;
}


void	GetControlPString( WindowRef window, OSType signature, SInt32 id, StringPtr pString )
{
	ControlRef		control;
	SInt32			dataSize;

	pString[0]			= 0;	
	GetControlBySigAndID( window, signature, id, &control );
	if ( control == NULL ) goto Bail;
	
	(void)GetControlData( control, 0, kControlStaticTextTextTag, 255, pString+1, &dataSize );
	pString[0]	= dataSize;
Bail:
	return;
}

void	SetControlPString( WindowRef window, OSType signature, SInt32 id, StringPtr pString )
{
	ControlRef		control;

	GetControlBySigAndID( window, signature, id, &control );
	if ( control == NULL ) goto Bail;
	
	(void) SetControlData( control, 0, kControlStaticTextTextTag, pString[0], pString+1 );
	Draw1Control( control );
	
Bail:
	return;
}



SInt16	GetControlValueByID( WindowRef window, OSType signature, SInt32 id )
{
	ControlRef		control;

	GetControlBySigAndID( window, signature, id, &control );
	if ( control == NULL ) return( 0 );

	return( GetControlValue(control) );
}


void	SetControlValueByID( WindowRef window, OSType signature, SInt32 id, SInt32 value )
{
	ControlRef		control;

	GetControlBySigAndID( window, signature, id, &control );
	if ( control == NULL ) goto Bail;

	SetControlValue( control, value );
	
	Draw1Control( control );
Bail:
	return;
}



SInt32	MIN( SInt32 value1, SInt32 value2 )
{
	return( value1<value2 ? value1 : value2 );
}

SInt32	MAX( SInt32 value1, SInt32 value2 )
{
	return( value1>value2 ? value1 : value2 );
}


void	CFStringToStr255( CFStringRef input, StringPtr output )
{
	 output[0] = CFStringGetBytes( input, CFRangeMake(0,  MIN(CFStringGetLength(input), 255)), kTextEncodingMacRoman, '^', false, &(output[1]), 255, NULL );
}


void	SendWindowCloseEvent( WindowRef window )
{
	EventRef				event;
	
	(void) CreateEvent( NULL,  kEventClassWindow, kEventWindowClose, GetCurrentEventTime(), kEventAttributeUserEvent, &event );
	(void) SetEventParameter( event, kEventParamDirectObject, typeWindowRef, sizeof(window), &window );
	(void) SendEventToWindow( event, window );
	(void) ReleaseEvent( event );
}



void	SendCommandProcessEvent( UInt32 commandID )
{
	HICommand				command;
	EventRef				event;

	BlockZero( &command, sizeof(command) );
	command.commandID	= commandID;
	
	(void) CreateEvent( NULL,  kEventClassCommand, kEventCommandProcess, GetCurrentEventTime(), kEventAttributeUserEvent, &event );
	(void) SetEventParameter( event, kEventParamDirectObject, typeHICommand, sizeof(command), &command );
	(void) SendEventToApplication( event );
	(void) ReleaseEvent( event );
}


void	LocalToGlobalRect( Rect *rect )
{
	LocalToGlobal( (Point*) &(rect->top) );
	LocalToGlobal( (Point*) &(rect->bottom) );
}

void	GlobalToLocalRect( Rect *rect )
{
	GlobalToLocal( (Point*) &(rect->top) );
	GlobalToLocal( (Point*) &(rect->bottom) );
}
