//
// UWP MIDIIO Library (DLL) that enables using BLE MIDI devices for Sekaiju
// https://github.com/trueroad/uwp_midiio
//
// midi_port_out.cpp:
//   MIDI OUT port class `uwp_midiio_port_out`
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

// See
// https://gist.github.com/trueroad/9c5317af5f212b2de7c7012e76b9e66b

#include "pch.h"
#include "config.h"

#include "midi_port_out.h"

#include "debug_message.h"
#include "device_enum.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Devices::Midi;
using namespace Windows::Storage::Streams;
using namespace Windows::Security::Cryptography;

namespace uwp_midiio
{
	std::wstring uwp_midiio_port_out::find_id_from_display_name(
		std::wstring_view display_name)
	{
		return device_enum::find_out_id_from_display_name(display_name);
	}

	bool uwp_midiio_port_out::send_buffer(const unsigned char* buff,
		size_t len)
	{
		TRACE_MESSAGE_W(L"enter\n");

		if (!port())
		{
			WARNING_MESSAGE_W(L"port is nullptr\n");

			return false;
		}

		TRACE_MESSAGE_W(L"  trying CreateFromByteArray\n");
		try
		{
			auto b{ CryptographicBuffer::CreateFromByteArray
				(array_view(buff, buff + len)) };
			TRACE_MESSAGE_W(L"  trying SendBuffer\n");
			port().SendBuffer(b);
		}
		catch (hresult_error const& ex)
		{
			WARNING_MESSAGE_W(L"exception 0x"
				<< std::hex << ex.code()
				<< L", "
				<< static_cast<std::wstring_view>(ex.message())
				<< L"\n");

			return false;
		}

		TRACE_MESSAGE_W(L"returns true\n");
		return true;
	}
}
