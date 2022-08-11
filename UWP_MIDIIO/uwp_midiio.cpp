//
// UWP MIDIIO Library (DLL) that enables using BLE MIDI devices for Sekaiju
// https://github.com/trueroad/uwp_midiio
//
// upw_midiio.cpp:
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

#include "pch.h"
#include "config.h"

#include "uwp_midiio.h"

#include "debug_message.h"
#include "device_enum.h"
#include "midi_port_in.h"
#include "midi_port_out.h"
#include "midi_ports.h"

UWP_MIDIIO_DECLSPEC long UWP_MIDIIO_API MIDIOut_GetDeviceNum()
{
	DEBUG_MESSAGE_W(L"enter\n");

	uwp_midiio::device_enum::refresh_out_ports();

	auto retval
		{ static_cast<long>(uwp_midiio::device_enum::get_out_ports_size()) };

	DEBUG_MESSAGE_W(L"returns " << retval << L"\n");
	return retval;
}

UWP_MIDIIO_DECLSPEC long UWP_MIDIIO_API MIDIOut_GetDeviceNameW(long lID,
	wchar_t* pszDeviceName, long lLen)
{
	DEBUG_MESSAGE_W(L"enter\n");

	const auto display_name
		{ uwp_midiio::device_enum::get_out_port_display_name(lID) };
	auto len{ display_name.size() };
	if (len > (static_cast<size_t>(lLen) - 1))
	{
		WARNING_MESSAGE_W(L"Destination buffer capacity "
			<< lLen
			<< L") is smaller than device name length ("
			<< len
			<< L") + null-terminator. Truncated.\n");
		len = (static_cast<size_t>(lLen) - 1);
	}
	DEBUG_MESSAGE_W(L"	trying wcsncpy_s\n");
	wcsncpy_s(pszDeviceName, lLen, display_name.data(), len);

	DEBUG_MESSAGE_W(L"returns " << len << L", \"" <<
		display_name << L"\"\n");
	return static_cast<long>(len);
}

UWP_MIDIIO_DECLSPEC MIDIOut* UWP_MIDIIO_API MIDIOut_OpenW(
	const wchar_t* pszDeviceName)
{
	DEBUG_MESSAGE_W(L"enter \"" << pszDeviceName << L"\"\n");

	if (!pszDeviceName)
	{
		DEBUG_MESSAGE_W(L"returns nullptr\n");
		return nullptr;
	}
	std::wstring_view display_name{ pszDeviceName };
	if (display_name.size() == 0)
	{
		DEBUG_MESSAGE_W(L"returns nullptr\n");
		return nullptr;
	}

	auto retval{ uwp_midiio::uwp_midiio_ports::open_out(display_name) };

	DEBUG_MESSAGE_W(L"returns 0x" << static_cast<void*>(retval) << L"\n");
	return retval;
}

UWP_MIDIIO_DECLSPEC long UWP_MIDIIO_API MIDIOut_Close(MIDIOut* pMIDIDevice)
{
	DEBUG_MESSAGE_W(L"enter 0x" << static_cast<void*>(pMIDIDevice) << L"\n");

	auto retval{ uwp_midiio::uwp_midiio_ports::close_out(pMIDIDevice) };

	DEBUG_MESSAGE_W(L"returns " << retval << L"\n");
	return retval;
}

UWP_MIDIIO_DECLSPEC MIDIOut* UWP_MIDIIO_API MIDIOut_ReopenW(
	MIDIOut* pMIDIOut, const wchar_t* pszDeviceName)
{
	DEBUG_MESSAGE_W(L"enter\n");

	if (pMIDIOut)
	{
		if (!MIDIOut_Close(pMIDIOut))
			return nullptr;
	}
	return MIDIOut_OpenW(pszDeviceName);
}

UWP_MIDIIO_DECLSPEC long UWP_MIDIIO_API MIDIOut_PutMIDIMessage(
	MIDIOut* pMIDI, unsigned char* pMessage, long lLen)
{
	TRACE_MESSAGE_W(L"enter\n");

	auto port_ptr{ uwp_midiio::uwp_midiio_port_out::get_class(pMIDI) };
	if (port_ptr)
	{
		TRACE_MESSAGE_W(L"	trying send_buffer\n");

		if (port_ptr->send_buffer(pMessage, lLen))
		{
			TRACE_MESSAGE_W(L"returns " << lLen << L"\n");
			return lLen;
		}
	}

	TRACE_MESSAGE_W(L"returns 0\n");
	return 0;
}

UWP_MIDIIO_DECLSPEC long UWP_MIDIIO_API MIDIIn_GetDeviceNum()
{
	DEBUG_MESSAGE_W(L"enter\n");

	uwp_midiio::device_enum::refresh_in_ports();

	auto retval
		{ static_cast<long>(uwp_midiio::device_enum::get_in_ports_size()) };

	DEBUG_MESSAGE_W(L"returns " << retval << L"\n");
	return retval;
}

UWP_MIDIIO_DECLSPEC long UWP_MIDIIO_API MIDIIn_GetDeviceNameW(long lID,
	wchar_t* pszDeviceName, long lLen)
{
	DEBUG_MESSAGE_W(L"enter\n");

	const auto display_name
		{ uwp_midiio::device_enum::get_in_port_display_name(lID) };
	auto len{ display_name.size() };
	if (len > (static_cast<size_t>(lLen) - 1))
	{
		WARNING_MESSAGE_W(L"Destination buffer capacity "
			<< lLen
			<< L") is smaller than device name length ("
			<< len
			<< L") + null-terminator. Truncated.\n");
		len = (static_cast<size_t>(lLen) - 1);
	}
	DEBUG_MESSAGE_W(L"	trying wcsncpy_s\n");
	wcsncpy_s(pszDeviceName, lLen, display_name.data(), len);

	DEBUG_MESSAGE_W(L"returns " << len << L", \"" <<
		display_name << L"\"\n");
	return static_cast<long>(len);
}

UWP_MIDIIO_DECLSPEC MIDIIn* UWP_MIDIIO_API MIDIIn_OpenW(
	const wchar_t* pszDeviceName)
{
	DEBUG_MESSAGE_W(L"enter \"" << pszDeviceName << L"\"\n");

	if (!pszDeviceName)
	{
		DEBUG_MESSAGE_W(L"returns nullptr");
		return nullptr;
	}
	std::wstring_view display_name{ pszDeviceName };
	if (display_name.size() == 0)
	{
		DEBUG_MESSAGE_W(L"returns nullptr");
		return nullptr;
	}

	auto retval{ uwp_midiio::uwp_midiio_ports::open_in(display_name) };

	DEBUG_MESSAGE_W(L"returns 0x" << static_cast<void*>(retval) << L"\n");
	return retval;
}

UWP_MIDIIO_DECLSPEC long UWP_MIDIIO_API MIDIIn_Close(MIDIIn* pMIDIDevice)
{
	DEBUG_MESSAGE_W(L"enter 0x" << static_cast<void*>(pMIDIDevice) << L"\n");

	auto retval{ uwp_midiio::uwp_midiio_ports::close_in(pMIDIDevice) };

	DEBUG_MESSAGE_W(L"returns " << retval << L"\n");
	return retval;
}

UWP_MIDIIO_DECLSPEC MIDIIn* UWP_MIDIIO_API MIDIIn_ReopenW(
	MIDIIn* pMIDIIn, const wchar_t* pszDeviceName)
{
	DEBUG_MESSAGE_W(L"enter\n");

	if (pMIDIIn)
	{
		if (!MIDIIn_Close(pMIDIIn))
			return nullptr;
	}
	return MIDIIn_OpenW(pszDeviceName);
}

UWP_MIDIIO_DECLSPEC long UWP_MIDIIO_API MIDIIn_GetMIDIMessage(
	MIDIIn* pMIDIIn, unsigned char* pMessage, long lLen)
{
	// TRACE_MESSAGE_W(L"enter\n");

	auto port_ptr{ uwp_midiio::uwp_midiio_port_in::get_class(pMIDIIn) };
	if (port_ptr)
	{
		// TRACE_MESSAGE_W(L"  trying pop_message\n");

		auto retval
			{ static_cast<long>(port_ptr->pop_message(pMessage, lLen)) };

		// TRACE_MESSAGE_W(L"returns " << retval << L"\n");
		return retval;
	}

	WARNING_MESSAGE_W(L"port_prt is nullptr\n");
	return 0;
}
