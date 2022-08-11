//
// UWP MIDIIO Library (DLL) that enables using BLE MIDI devices for Sekaiju
// https://github.com/trueroad/uwp_midiio
//
// upw_midiio.h:
//   UWP MIDIIO API
//
// Copyright (C) 2022 Masamichi Hosoda.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.
//

#pragma once

#ifdef UWP_MIDIIO_EXPORTS
#define UWP_MIDIIO_DECLSPEC __declspec(dllexport)
#else
#define UWP_MIDIIO_DECLSPEC __declspec(dllimport)
#endif

#define UWP_MIDIIO_API __stdcall

//
// MIDI struct based on MIDIIO.h
//
// See
// https://openmidiproject.osdn.jp/MIDIIOLibrary.html
//
typedef struct tagMIDI {
	void* m_pDeviceHandle;
	void* m_pDeviceName;
	long  m_lMode;
	void* m_pSysxHeader;
	long  m_bStarting;
	unsigned char* m_pBuf;
	long  m_lBufSize;
	long  m_lReadPosition;
	long  m_lWritePosition;
	long  m_bBufLocked;
	unsigned char m_byRunningStatus;
} MIDIIO_Port;

#ifdef __cplusplus

class MIDIOut : public MIDIIO_Port
{
public:
	MIDIOut() = default;
	~MIDIOut() = default;
	MIDIOut(const MIDIOut&) = delete;
	MIDIOut& operator=(const MIDIOut&) = delete;
	MIDIOut(MIDIOut&&) = delete;
	MIDIOut& operator=(MIDIOut&&) = delete;
};

class MIDIIn : public MIDIIO_Port
{
public:
	MIDIIn() = default;
	~MIDIIn() = default;
	MIDIIn(const MIDIIn&) = delete;
	MIDIIn& operator=(const MIDIIn&) = delete;
	MIDIIn(MIDIIn&&) = delete;
	MIDIIn& operator=(MIDIIn&&) = delete;
};

#else  // __cplusplus

typedef MIDIIO_Port MIDIOut;
typedef MIDIIO_Port MIDIIn;

#endif  // __cplusplus

//
// MIDIIO.dll APIs
//
// See
// https://openmidiproject.osdn.jp/MIDIIOLibrary.html
//

#ifdef __cplusplus
extern "C"
{
#endif

UWP_MIDIIO_DECLSPEC long UWP_MIDIIO_API MIDIOut_GetDeviceNum();
UWP_MIDIIO_DECLSPEC long UWP_MIDIIO_API MIDIOut_GetDeviceNameW(long lID,
	wchar_t* pszDeviceName, long lLen);

UWP_MIDIIO_DECLSPEC MIDIOut* UWP_MIDIIO_API MIDIOut_OpenW(
	const wchar_t* pszDeviceName);
UWP_MIDIIO_DECLSPEC long UWP_MIDIIO_API MIDIOut_Close(MIDIOut* pMIDIDevice);
UWP_MIDIIO_DECLSPEC MIDIOut* UWP_MIDIIO_API MIDIOut_ReopenW(
	MIDIOut* pMIDIOut, const wchar_t* pszDeviceName);

UWP_MIDIIO_DECLSPEC long UWP_MIDIIO_API MIDIOut_PutMIDIMessage(
	MIDIOut* pMIDI, unsigned char* pMessage, long lLen);

UWP_MIDIIO_DECLSPEC long UWP_MIDIIO_API MIDIIn_GetDeviceNum();
UWP_MIDIIO_DECLSPEC long UWP_MIDIIO_API MIDIIn_GetDeviceNameW(long lID,
	wchar_t* pszDeviceName, long lLen);

UWP_MIDIIO_DECLSPEC MIDIIn* UWP_MIDIIO_API MIDIIn_OpenW(
	const wchar_t* pszDeviceName);
UWP_MIDIIO_DECLSPEC long UWP_MIDIIO_API MIDIIn_Close(MIDIIn* pMIDIDevice);
UWP_MIDIIO_DECLSPEC MIDIIn* UWP_MIDIIO_API MIDIIn_ReopenW(
	MIDIIn* pMIDIIn, const wchar_t* pszDeviceName);

UWP_MIDIIO_DECLSPEC long UWP_MIDIIO_API MIDIIn_GetMIDIMessage(
	MIDIIn* pMIDIIn, unsigned char* pMessage, long lLen);

#ifdef __cplusplus
}
#endif
