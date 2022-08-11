//
// UWP MIDIIO Library (DLL) that enables using BLE MIDI devices for Sekaiju
// https://github.com/trueroad/uwp_midiio
//
// device_enum.h:
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

#pragma once

#include "pch.h"

namespace uwp_midiio
{
	class device_enum final
	{
		struct port
		{
			std::wstring name;
			std::wstring id;
			std::wstring hex_id;
			std::wstring display_name;
		};

	public:
		device_enum() = delete;
		~device_enum() = delete;
		device_enum(const device_enum&) = delete;
		device_enum& operator=(const device_enum&) = delete;
		device_enum(device_enum&&) = delete;
		device_enum& operator=(device_enum&&) = delete;

		static void refresh_in_ports();
		static void refresh_out_ports();

		static size_t get_in_ports_size()
		{
			std::lock_guard<std::mutex> lock(mtx_in_);

			return in_ports_.size();
		}
		static size_t get_out_ports_size()
		{
			std::lock_guard<std::mutex> lock(mtx_out_);

			return out_ports_.size();
		}
		static std::wstring get_in_port_display_name(size_t index)
		{
			std::lock_guard<std::mutex> lock(mtx_in_);

			return in_ports_.at(index).display_name;
		}
		static std::wstring get_out_port_display_name(size_t index)
		{
			std::lock_guard<std::mutex> lock(mtx_out_);

			return out_ports_.at(index).display_name;
		}
		static std::wstring find_in_id_from_display_name(
			std::wstring_view display_name);
		static std::wstring find_out_id_from_display_name(
			std::wstring_view display_name);

	private:
		static std::vector<port> list_ports(winrt::hstring device_selector);
		static void fix_display_name(const std::vector<port>& in_ports,
			std::vector<port>& out_ports);
		static void sort_display_name(std::vector<port>& ports);
		static std::wstring find_id_from_display_name(
			std::wstring_view display_name, const std::vector<port>& ports);

		static std::vector<port> in_ports_;
		static std::vector<port> out_ports_;
		static const std::wregex hex_id_pattern_;

		static std::mutex mtx_in_;
		static std::mutex mtx_out_;
	};
}
