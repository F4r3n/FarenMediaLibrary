//
//  Portable File Dialogs
//
//  Copyright © 2018—2019 Sam Hocevar <sam@hocevar.net>
//
//  This library is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

#if _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN 1
#endif
#include <windows.h>
#include <shlobj.h>

#include <future>
#else
#ifndef _POSIX_C_SOURCE
#   define _POSIX_C_SOURCE 2 // for popen()
#endif
#include <cstdio>   // for popen()
#include <cstdlib>  // for std::getenv()
#include <fcntl.h>  // for fcntl()
#include <unistd.h> // for read()
#endif

#include <string>
#include <map>


namespace pfd
{

	enum class button
	{
		cancel = -1,
		ok,
		yes,
		no,
		abort,
		retry,
		ignore,
	};

	enum class choice
	{
		ok = 0,
		ok_cancel,
		yes_no,
		yes_no_cancel,
		retry_cancel,
		abort_retry_ignore,
	};

	enum class icon
	{
		info = 0,
		warning,
		error,
		question,
	};

	// Process wait timeout, in milliseconds
	static int const default_wait_timeout = 20;

	// The settings class, only exposing to the user a way to set verbose mode
	// and to force a rescan of installed desktop helpers (zenity, kdialog…).
	class settings
	{
	public:
		static void verbose(bool value);
		static void rescan();


	protected:
		enum class flag
		{
			is_scanned = 0,
			is_verbose,

			has_zenity,
			has_matedialog,
			has_qarma,
			has_kdialog,
			is_vista,

			max_flag,
		};

		explicit settings(bool resync = false);

		inline bool is_osascript() const
		{
#if __APPLE__
			return true;
#else
			return false;
#endif
		}

		inline bool is_zenity() const
		{
			return flags(flag::has_zenity) ||
				flags(flag::has_matedialog) ||
				flags(flag::has_qarma);
		}

		inline bool is_kdialog() const
		{
			return flags(flag::has_kdialog);
		}

		// Static array of flags for internal state
		bool const &flags(flag in_flag) const;


		// Non-const getter for the static array of flags
		bool &flags(flag in_flag);

	};

	// Forward declarations for our API
	class notify;
	class message;

	// Internal classes, not to be used by client applications
	namespace internal
	{

		class executor
		{
			friend class dialog;

		public:
			// High level function to get the result of a command
			std::string result(int *exit_code = nullptr);
			
#if _WIN32
			void start(std::function<std::string(int *)> const &fun);
#endif
			void start(std::string const &command);
			~executor();


		protected:
			bool ready(int timeout = default_wait_timeout);
			void stop();
			

		private:
			bool m_running = false;
			std::string m_stdout;
			int m_exit_code = -1;
#if _WIN32
			std::future<std::string> m_future;
			PROCESS_INFORMATION m_pi;
#else
			FILE *m_stream = nullptr;
			int m_fd = -1;
#endif
		};

		class platform
		{
		protected:
#if _WIN32
			// Helper class around LoadLibraryA() and GetProcAddress() with some safety
			class dll
			{
			public:
				dll(std::string const &name);

				~dll();

				template<typename T> class proc
				{
				public:
					proc(dll const &lib, std::string const &sym)
						: m_proc(reinterpret_cast<T *>(::GetProcAddress(lib.handle, sym.c_str())))
					{}

					operator bool() const
					{
						return m_proc != nullptr;
					}

					operator T *() const
					{
						return m_proc;
					}

				private:
					T *m_proc;
				};

			private:
				HMODULE handle;
			};

			// Helper class around CreateActCtx() and ActivateActCtx()
			class new_style_context
			{
			public:
				new_style_context();
				

				~new_style_context();
				

			private:
				HANDLE create();

				ULONG_PTR m_cookie = 0;
			};
#endif
		};

		class dialog : protected settings, protected platform
		{
			friend class pfd::notify;
			friend class pfd::message;

		public:
			bool ready(int timeout = default_wait_timeout);
			

		protected:
			explicit dialog();
#if !_WIN32
			std::string desktop_helper() const;
#endif

			std::string buttons_to_name(choice choice) const;
			

			std::string get_icon_name(icon icon) const;
			
			// Properly quote a string for Powershell: replace ' or " with '' or ""
			// FIXME: we should probably get rid of newlines!
			// FIXME: the \" sequence seems unsafe, too!
			std::string powershell_quote(std::string const &str) const;
			
			// Properly quote a string for osascript: replace ' with '\'' and \ or " with \\ or \"
			std::string osascript_quote(std::string const &str) const;

			// Properly quote a string for the shell: just replace ' with '\''
			std::string shell_quote(std::string const &str) const;
			
			// Check whether a program is present using “which”
			bool check_program(std::string const &program);
			

		protected:
			// Keep handle to executing command
			std::shared_ptr<executor> m_async;
		};

		class file_dialog : public dialog
		{
		protected:
			enum type
			{
				open,
				save,
				folder,
			};

			file_dialog(type in_type,
				std::string const &title,
				std::string const &default_path = "",
				std::vector<std::string> filters = {},
				bool allow_multiselect = false,
				bool confirm_overwrite = true);

		protected:
			std::string string_result();
			

			std::vector<std::string> vector_result();

#if _WIN32			
			static int CALLBACK bffcallback(HWND hwnd, UINT uMsg, LPARAM, LPARAM pData);

			std::string select_folder_vista(IFileDialog *ifd);
			

			std::wstring m_wtitle;
			std::wstring m_wdefault_path;

			std::vector<std::string> m_vector_result;
#endif
		};

	} // namespace internal

	class notify : public internal::dialog
	{
	public:
		notify(std::string const &title,
			std::string const &message,
			icon icon = icon::info);
	};

	class message : public internal::dialog
	{
	public:
		message(std::string const &title,
			std::string const &text,
			choice choice = choice::ok_cancel,
			icon icon = icon::info);

		button result();

	private:
		// Some extra logic to map the exit code to button number
		std::map<int, button> m_mappings;
	};

	class open_file : public internal::file_dialog
	{
	public:
		open_file(std::string const &title,
			std::string const &default_path = "",
			std::vector<std::string> filters = { "All Files", "*" },
			bool allow_multiselect = false);
		


		std::vector<std::string> result();

	};

	class save_file : public internal::file_dialog
	{
	public:
		save_file(std::string const &title,
			std::string const &default_path = "",
			std::vector<std::string> filters = { "All Files", "*" },
			bool confirm_overwrite = true);
			


		std::string result();

	};

	class select_folder : public internal::file_dialog
	{
	public:
		select_folder(std::string const &title,
			std::string const &default_path = "");


		std::string result();

	};

} // namespace pfd
