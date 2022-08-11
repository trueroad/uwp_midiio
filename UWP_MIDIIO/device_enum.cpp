//
// UWP MIDIIO Library (DLL) that enables using BLE MIDI devices for Sekaiju
// https://github.com/trueroad/uwp_midiio
//
// device_enum.cpp:
//   Device enumeration class `device_enum`
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

#include "device_enum.h"

#include "debug_message.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Devices::Enumeration;
using namespace Windows::Devices::Midi;

namespace uwp_midiio
{
	std::vector<device_enum::port> device_enum::in_ports_;
	std::vector<device_enum::port> device_enum::out_ports_;

	const std::wregex device_enum::hex_id_pattern_
		{ std::wregex(L"#MIDII_([0-9A-F]{8})\\..+#") };

	std::mutex device_enum::mtx_in_;
	std::mutex device_enum::mtx_out_;

	void device_enum::refresh_in_ports()
	{
		DEBUG_MESSAGE_W(L"enter\n");

		auto ports{ list_ports(MidiInPort::GetDeviceSelector()) };
		sort_display_name(ports);

		{
			std::lock_guard<std::mutex> lock(mtx_in_);

			in_ports_ = std::move(ports);
		}
	}

	void device_enum::refresh_out_ports()
	{
		DEBUG_MESSAGE_W(L"enter\n");

		auto in_ports{ list_ports(MidiInPort::GetDeviceSelector()) };
		auto out_ports{ list_ports(MidiOutPort::GetDeviceSelector()) };

		sort_display_name(in_ports);
		fix_display_name(in_ports, out_ports);
		sort_display_name(out_ports);

		{
			std::scoped_lock lock{ mtx_in_, mtx_out_ };

			in_ports_ = std::move(in_ports);
			out_ports_ = std::move(out_ports);
		}
	}

	std::vector<device_enum::port>
		device_enum::list_ports(winrt::hstring device_selector)
	{
		DEBUG_MESSAGE_W(L"enter\n");

		const auto& devs
			{ DeviceInformation::FindAllAsync(device_selector).get() };

		std::vector<port> retval;
		for (const auto& d : devs)
		{
			port p;
			p.name = d.Name();
			p.id = d.Id();
			p.display_name = d.Name();

			std::wsmatch m;
			if (std::regex_search(p.id, m, hex_id_pattern_))
			{
				p.hex_id = m[1];

				std::wostringstream ss;
				ss << p.name
					<< L" [ "
					<< p.hex_id
					<< L" ]";
				p.display_name = ss.str();
			}

			retval.push_back(p);
		}

		return retval;
	}

	void device_enum::fix_display_name(const std::vector<port>& in_ports,
		std::vector<port>& out_ports)
	{
		DEBUG_MESSAGE_W(L"enter\n");

		for (auto& outp : out_ports)
		{
			if (outp.hex_id.empty() ||
				std::wstring_view{ outp.name }.substr(0, 4) != L"MIDI")
				continue;

			for (const auto& inp : in_ports)
			{
				if (outp.hex_id == inp.hex_id)
				{
					DEBUG_MESSAGE_W(L"\"" << outp.display_name <<
						L"\" -> \"" << inp.display_name << "\"\n");

					outp.display_name = inp.display_name;
					break;
				}
			}
		}
	}

	void device_enum::sort_display_name(std::vector<port>& ports)
	{
		DEBUG_MESSAGE_W(L"enter\n");

		std::sort(ports.begin(), ports.end(),
			[](const auto& lhs, const auto& rhs)
		{
			return lhs.display_name < rhs.display_name;
		});
	}

	std::wstring device_enum::find_id_from_display_name(
		std::wstring_view display_name, const std::vector<port>& ports)
	{
		DEBUG_MESSAGE_W(L"enter \"" << display_name << "\"\n");

		auto len{ display_name.size() };
		if (len == 0)
			return L"";

		for (const auto& p : ports)
		{
			DEBUG_MESSAGE_W(L"  p: "
				<< p.display_name
				<< L", "
				<< p.id
				<< L"\n");
			DEBUG_MESSAGE_W(L"    compare: \""
				<< std::wstring_view{ p.display_name }.substr(0, len)
				<< L"\", \""
				<< display_name
				<< L"\"\n");

			if (std::wstring_view{ p.display_name }.substr(0, len) ==
				display_name)
			{
				DEBUG_MESSAGE_W(L"returns \"" << p.id << L"\"\n");
				return p.id;
			}
		}

		DEBUG_MESSAGE_W(L"returns empty\n");
		return L"";
	}

	std::wstring device_enum::find_in_id_from_display_name(
		std::wstring_view display_name)
	{
		DEBUG_MESSAGE_W(L"enter \"" << display_name << "\"\n");

		if (get_in_ports_size() == 0)
			refresh_in_ports();

		{
			std::lock_guard<std::mutex> lock{ mtx_in_ };

			return find_id_from_display_name(display_name, in_ports_);
		}
	}

	std::wstring device_enum::find_out_id_from_display_name(
		std::wstring_view display_name)
	{
		DEBUG_MESSAGE_W(L"enter \"" << display_name << "\"\n");

		if (get_out_ports_size() == 0)
			refresh_out_ports();

		{
			std::lock_guard<std::mutex> lock{ mtx_out_ };

			return find_id_from_display_name(display_name, out_ports_);
		}
	}
}
