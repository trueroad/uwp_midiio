//
// UWP MIDIIO Library (DLL) that enables using BLE MIDI devices for Sekaiju
// https://github.com/trueroad/uwp_midiio
//
// midi_port_in.cpp:
//   MIDI IN port class `uwp_midiio_port_in`
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

#include "midi_port_in.h"

#include "debug_message.h"
#include "device_enum.h"

using namespace std::chrono_literals;

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Devices::Midi;
using namespace Windows::Storage::Streams;
using namespace Windows::Security::Cryptography;

namespace uwp_midiio
{
	std::wstring uwp_midiio_port_in::find_id_from_display_name(
		std::wstring_view display_name)
	{
		return device_enum::find_in_id_from_display_name(display_name);
	}

	void uwp_midiio_port_in::open_from_id(std::wstring_view id)
	{
		DEBUG_MESSAGE_W(L"enter\n");

		uwp_midiio_port::open_from_id(id);

		if (!port())
		{
			WARNING_MESSAGE_W(L"port is nullptr\n");
			return;
		}

		DEBUG_MESSAGE_W(L"  trying MessageReceived\n");
		try
		{
			before_token_ = port().MessageReceived(
				{ this, &uwp_midiio_port_in::midi_in_callback });
			DEBUG_MESSAGE_W(L"  before_token is "
				<< static_cast<bool>(before_token_)
				<< L" (value = "
				<< before_token_.value
				<< L")\n");
		}
		catch (winrt::hresult_error const& ex)
		{
			WARNING_MESSAGE_W(L"exception 0x"
				<< std::hex << ex.code()
				<< L", "
				<< static_cast<std::wstring_view>(ex.message())
				<< L"\n");

			return;
		}

		DEBUG_MESSAGE_W(L"returns\n");
	}

	void uwp_midiio_port_in::midi_in_callback(const MidiInPort&,
		const MidiMessageReceivedEventArgs& e)
	{
		DEBUG_MESSAGE_W(L"enter\n");

		{
			std::lock_guard<std::mutex> lock(mtx_);

			message_queue_.push_back(e.Message());

			while (message_queue_.size() > MAX_MIDI_IN_QUEUE_SIZE)
			{
				WARNING_MESSAGE_W(L"queue overflow\n");
				message_queue_.pop_front();
			}
		}

		DEBUG_MESSAGE_W(L"returns\n");
	}

	size_t uwp_midiio_port_in::pop_message(unsigned char* buff,
		size_t capacity)
	{
		// TRACE_MESSAGE_W(L"enter\n");

		IMidiMessage message;

		{
			std::lock_guard<std::mutex> lock(mtx_);

			if (message_queue_.empty())
			{
				// TRACE_MESSAGE_W(L"returns 0, message queue is empty\n");
				return 0;
			}

			TRACE_MESSAGE_W(L"received message exists\n");

			message = message_queue_.front();
			message_queue_.pop_front();
		}

		if (!message)
		{
			WARNING_MESSAGE_W(L"message is nullptr\n");
			return 0;
		}
		TRACE_MESSAGE_W(L"  trying RawData\n");
		const auto raw_data{ message.RawData() };
		TRACE_MESSAGE_W(L"  trying Length\n");
		size_t len{ raw_data.Length() };
		if (capacity < len)
		{
			WARNING_MESSAGE_W(L"Destination buffer capacity ("
				<< capacity
				<< L") is smaller than received message length ("
				<< len
				<< L"). Truncated.");
			len = capacity;
		}
		TRACE_MESSAGE_W(L"  trying std::memcpy\n");
		std::memcpy(buff, raw_data.data(), len);

		TRACE_MESSAGE_W(L"returns " << len << "\n");
		return len;
	}
}
