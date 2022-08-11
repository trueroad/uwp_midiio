//
// UWP MIDIIO Library (DLL) that enables using BLE MIDI devices for Sekaiju
// https://github.com/trueroad/uwp_midiio
//
// midi_ports.cpp:
//   MIDI ports open/close class `uwp_midiio_ports`
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

#include "midi_ports.h"

#include "debug_message.h"
#include "midi_port_in.h"
#include "midi_port_out.h"
#include "uwp_midiio.h"

namespace uwp_midiio
{
	std::vector<std::unique_ptr<uwp_midiio_port_in>>
		uwp_midiio_ports::ports_in_;
	std::vector<std::unique_ptr<uwp_midiio_port_out>>
		uwp_midiio_ports::ports_out_;

	std::mutex uwp_midiio_ports::mtx_in_;
	std::mutex uwp_midiio_ports::mtx_out_;

	template
	MIDIIn* uwp_midiio_ports::open<uwp_midiio_port_in, MIDIIn>(
		std::wstring_view display_name,
		std::vector<std::unique_ptr<uwp_midiio_port_in>>& ports,
		std::mutex& mtx);
	template
	MIDIOut* uwp_midiio_ports::open<uwp_midiio_port_out, MIDIOut>(
		std::wstring_view display_name,
		std::vector<std::unique_ptr<uwp_midiio_port_out>>& ports,
		std::mutex& mtx);

	template <class uwp_midiio_port_T, class MidiIO_T>
	static MidiIO_T* uwp_midiio_ports::open(std::wstring_view display_name,
		std::vector<std::unique_ptr<uwp_midiio_port_T>>& ports,
		std::mutex& mtx)
	{
		DEBUG_MESSAGE_W(L"enter \"" << display_name << L"\"\n");

		auto p{ std::make_unique<uwp_midiio_port_T>() };
		p->open_from_display_name(display_name);

		auto ptr{ p->get_ptr() };
		if (p->port())
		{
			p->set_display_name(display_name);
			{
				std::lock_guard<std::mutex> lock(mtx);

				ports.push_back(std::move(p));
			}

			DEBUG_MESSAGE_W(L"returns 0x"
				<< static_cast<void*>(ptr)
				<< L", "
				<< ports_in_.size()
				<< L" port(s) open\n");
			return ptr;
		}

		DEBUG_MESSAGE_W(L"returns nullptr, "
			<< ports_in_.size()
			<< L" port(s) open\n");
		return nullptr;
	}

	template
	bool uwp_midiio_ports::close(MIDIIn* ptr,
		std::vector<std::unique_ptr<uwp_midiio_port_in>>& ports,
		std::mutex& mtx);
	template
	bool uwp_midiio_ports::close(MIDIOut* ptr,
		std::vector<std::unique_ptr<uwp_midiio_port_out>>& ports,
		std::mutex& mtx);

	template <class uwp_midiio_port_T, class MidiIO_T>
	static bool uwp_midiio_ports::close(MidiIO_T* ptr,
		std::vector<std::unique_ptr<uwp_midiio_port_T>>& ports,
		std::mutex& mtx)
	{
		DEBUG_MESSAGE_W(L"enter 0x" << static_cast<void*>(ptr) << L"\n");
		std::lock_guard<std::mutex> lock(mtx);

		auto it{ std::find_if(ports.begin(), ports.end(),
			[ptr](std::unique_ptr<uwp_midiio_port_T>& port)
			{return port->get_ptr() == ptr; }) };
		if (it == ports.end())
		{
			DEBUG_MESSAGE_W(L"returns false, "
				<< ports.size()
				<< L" port(s) open\n");
			return false;
		}
		ports.erase(it);

		DEBUG_MESSAGE_W(L"returns true, "
			<< ports.size()
			<< L" port(s) open\n");
		return true;
	}
}
