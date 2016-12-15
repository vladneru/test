#ifndef __CZMQ_PRELUDE_H_INCLUDED__
#define __CZMQ_PRELUDE_H_INCLUDED__

//- Establish the compiler and computer system ------------------------------
/*
 *  Defines zero or more of these symbols, for use in any non-portable
 *  code:
 *
 *  __WINDOWS__         Microsoft C/C++ with Windows calls
 *  __MSDOS__           System is MS-DOS (set if __WINDOWS__ set)
 *  __VMS__             System is VAX/VMS or Alpha/OpenVMS
 *  __UNIX__            System is UNIX
 *  __OS2__             System is OS/2
 *
 *  __IS_32BIT__        OS/compiler is 32 bits
 *  __IS_64BIT__        OS/compiler is 64 bits
 *
 *  When __UNIX__ is defined, we also define exactly one of these:
 *
 *  __UTYPE_AUX         Apple AUX
 *  __UTYPE_BEOS        BeOS
 *  __UTYPE_BSDOS       BSD/OS
 *  __UTYPE_DECALPHA    Digital UNIX (Alpha)
 *  __UTYPE_IBMAIX      IBM RS/6000 AIX
 *  __UTYPE_FREEBSD     FreeBSD
 *  __UTYPE_HPUX        HP/UX
 *  __UTYPE_ANDROID     Android
 *  __UTYPE_LINUX       Linux
 *  __UTYPE_GNU         GNU/Hurd
 *  __UTYPE_MIPS        MIPS (BSD 4.3/System V mixture)
 *  __UTYPE_NETBSD      NetBSD
 *  __UTYPE_NEXT        NeXT
 *  __UTYPE_OPENBSD     OpenBSD
 *  __UTYPE_OSX         Apple Macintosh OS X
 *  __UTYPE_IOS         Apple iOS
 *  __UTYPE_QNX         QNX
 *  __UTYPE_IRIX        Silicon Graphics IRIX
 *  __UTYPE_SINIX       SINIX-N (Siemens-Nixdorf Unix)
 *  __UTYPE_SUNOS       SunOS
 *  __UTYPE_SUNSOLARIS  Sun Solaris
 *  __UTYPE_UNIXWARE    SCO UnixWare
 *                      ... these are the ones I know about so far.
 *  __UTYPE_GENERIC     Any other UNIX
 *
 *  When __VMS__ is defined, we may define one or more of these:
 *
 *  __VMS_XOPEN         Supports XOPEN functions
 */

#if (defined (__64BIT__) || defined (__x86_64__))
#    define __IS_64BIT__                //  May have 64-bit OS/compiler
#else
#    define __IS_32BIT__                //  Else assume 32-bit OS/compiler
#endif

#if (defined WIN32 || defined _WIN32)
#   undef __WINDOWS__
#   define __WINDOWS__
#   undef __MSDOS__
#   define __MSDOS__
#endif

#if (defined WINDOWS || defined _WINDOWS || defined __WINDOWS__)
#   undef __WINDOWS__
#   define __WINDOWS__
#   undef __MSDOS__
#   define __MSDOS__
//  Stop cheeky warnings about "deprecated" functions like fopen
#   if _MSC_VER >= 1500
#       undef  _CRT_SECURE_NO_DEPRECATE
#       define _CRT_SECURE_NO_DEPRECATE
#       pragma warning(disable: 4996)
#   endif
#endif

//  MSDOS               Microsoft C
//  _MSC_VER            Microsoft C
#if (defined (MSDOS) || defined (_MSC_VER))
#   undef __MSDOS__
#   define __MSDOS__
#   if (defined (_DEBUG) && !defined (DEBUG))
#       define DEBUG
#   endif
#endif

#if (defined (__EMX__) && defined (__i386__))
#   undef __OS2__
#   define __OS2__
#endif

//  VMS                 VAX C (VAX/VMS)
//  __VMS               Dec C (Alpha/OpenVMS)
//  __vax__             gcc
#if (defined (VMS) || defined (__VMS) || defined (__vax__))
#   undef __VMS__
#   define __VMS__
#   if (__VMS_VER >= 70000000)
#       define __VMS_XOPEN
#   endif
#endif

//  Try to define a __UTYPE_xxx symbol...
//  unix                SunOS at least
//  __unix__            gcc
//  _POSIX_SOURCE is various UNIX systems, maybe also VAX/VMS
#if (defined (unix) || defined (__unix__) || defined (_POSIX_SOURCE))
#   if (!defined (__VMS__))
#       undef __UNIX__
#       define __UNIX__
#       if (defined (__alpha))          //  Digital UNIX is 64-bit
#           undef  __IS_32BIT__
#           define __IS_64BIT__
#           define __UTYPE_DECALPHA
#       endif
#   endif
#endif

#if (defined (_AUX))
#   define __UTYPE_AUX
#   define __UNIX__
#elif (defined (__BEOS__))
#   define __UTYPE_BEOS
#   define __UNIX__
#elif (defined (__hpux))
#   define __UTYPE_HPUX
#   define __UNIX__
#   define _INCLUDE_HPUX_SOURCE
#   define _INCLUDE_XOPEN_SOURCE
#   define _INCLUDE_POSIX_SOURCE
#elif (defined (_AIX) || defined (AIX))
#   define __UTYPE_IBMAIX
#   define __UNIX__
#elif (defined (BSD) || defined (bsd))
#   define __UTYPE_BSDOS
#   define __UNIX__
#elif (defined (__ANDROID__))
#   define __UTYPE_ANDROID
#   define __UNIX__
#elif (defined (LINUX) || defined (linux) || defined (__linux__))
#   define __UTYPE_LINUX
#   define __UNIX__
#   ifndef __NO_CTYPE
#   define __NO_CTYPE                   //  Suppress warnings on tolower()
#   endif
#   ifndef _DEFAULT_SOURCE
#   define _DEFAULT_SOURCE                  //  Include stuff from 4.3 BSD Unix
#   endif
#elif (defined (__GNU__))
#   define __UTYPE_GNU
#   define __UNIX__
#elif (defined (Mips))
#   define __UTYPE_MIPS
#   define __UNIX__
#elif (defined (FreeBSD) || defined (__FreeBSD__))
#   define __UTYPE_FREEBSD
#   define __UNIX__
#elif (defined (NetBSD) || defined (__NetBSD__))
#   define __UTYPE_NETBSD
#   define __UNIX__
#elif (defined (OpenBSD) || defined (__OpenBSD__))
#   define __UTYPE_OPENBSD
#   define __UNIX__
#elif (defined (APPLE) || defined (__APPLE__))
#   include <TargetConditionals.h>
#   define __UNIX__
#   if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#      define __UTYPE_IOS
#   else
#      define __UTYPE_OSX
#   endif
#elif (defined (NeXT))
#   define __UTYPE_NEXT
#   define __UNIX__
#elif (defined (__QNX__))
#   define __UTYPE_QNX
#   define __UNIX__
#elif (defined (sgi))
#   define __UTYPE_IRIX
#   define __UNIX__
#elif (defined (sinix))
#   define __UTYPE_SINIX
#   define __UNIX__
#elif (defined (SOLARIS) || defined (__SVR4)) || defined (SVR4)
#   define __UTYPE_SUNSOLARIS
#   define __UNIX__
#elif (defined (SUNOS) || defined (SUN) || defined (sun))
#   define __UTYPE_SUNOS
#   define __UNIX__
#elif (defined (__USLC__) || defined (UnixWare))
#   define __UTYPE_UNIXWARE
#   define __UNIX__
#elif (defined (__CYGWIN__))
#   define __UTYPE_CYGWIN
#   define __UNIX__
#elif (defined (__UNIX__))
#   define __UTYPE_GENERIC
#endif


#if (defined (__MSDOS__))
#   if (defined (__WINDOWS__))
#       if (_WIN32_WINNT < 0x0600)
#           undef _WIN32_WINNT
#           define _WIN32_WINNT 0x0600
#       endif
#       if (!defined (FD_SETSIZE))
#           define FD_SETSIZE 1024      //  Max. filehandles/sockets
#       endif
#       include <direct.h>
#       include <winsock2.h>
#       include <windows.h>
#       include <process.h>
#       include <ws2tcpip.h>            //  For getnameinfo ()
#       include <iphlpapi.h>            //  For GetAdaptersAddresses ()
#   endif
#   include <malloc.h>
#   include <dos.h>
#   include <io.h>
#   include <fcntl.h>
#   include <sys/types.h>
#   include <sys/stat.h>
#   include <sys/utime.h>
#   include <share.h>
#endif

#include <stdio.h>   
#include <openssl/rsa.h>
#include <curl/curl.h>
#include <openssl/x509v3.h>
#include <openssl/x509_vfy.h>
#include <openssl/crypto.h>
#include <openssl/lhash.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/pkcs12.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <cstdio> 
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <algorithm>
#include <fcntl.h>
//#include <sys\stat.h>
#include <boost/filesystem.hpp>
#include <algorithm>
#include <functional>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#define BUFSIZE 1024








struct Data
{
	char * buffer;
	unsigned long long position;
	unsigned long long size;
};

auto stream(char * ptr, size_t item_size, size_t item_count, void * stream)->size_t
{
	auto in_stream = (std::istream *)stream;
	auto read_bytes = static_cast<unsigned long long>(item_size * item_count);
	auto position = static_cast<unsigned long long>(in_stream->tellg());
	in_stream->seekg(0, std::ios::end);
	auto size = static_cast<unsigned long long>(in_stream->tellg());
	in_stream->seekg(position, std::ios::beg);
	auto rest_bytes = size - position;
	read_bytes = std::min<unsigned long long>(read_bytes, rest_bytes);
	in_stream->read(ptr, read_bytes);
	return read_bytes;
}

auto buffer(char * ptr, size_t item_size, size_t item_count, void * buffer)->size_t
{
	auto data = (Data*)buffer;
	auto append_size = item_size * item_count;
	auto new_buffer_size = data->size + append_size;
	auto new_buffer = new char[new_buffer_size];
	if (data->size != 0) memcpy(new_buffer, data->buffer, data->size);
	memcpy(new_buffer + data->size, ptr, append_size);
	delete[] data->buffer;
	data->buffer = new_buffer;
	data->size = new_buffer_size;
	return append_size;
}




   

class Client {
public:
	Client(std::string str);
	auto Entry()->void;
	bool FindFile(const boost::filesystem::path& directory);
	std::string aes_encrypt(int i, std::string name);
private:
	std::string server, login, password, path, client_file;
	std::vector<std::string> file_names_dir, direct, encrypt_files_path;
};

Client::Client(std::string str) : client_file(str) {
	std::ifstream file(client_file);
	getline(file, server);
	getline(file, login);
	getline(file, password);
	getline(file, path);
	file.close();
	Entry();
}


std::string Client::aes_encrypt(int i, std::string name)
{
	int outlen, inlen;
	std::string name_out = "C:\\" + path + "\\" + "encrypt_" + file_names_dir[i];
	encrypt_files_path.push_back(name_out);
	FILE *in = fopen(name.c_str(), "rb"), *out = fopen(name_out.c_str(), "wb");
	unsigned char inbuf[BUFSIZE], outbuf[BUFSIZE];
	unsigned char key[32]; /* 256- битный ключ */
	unsigned char iv[8]; /* вектор инициализации */
	const EVP_CIPHER * cipher;
	EVP_CIPHER_CTX ctx;
	/* обнуляем структуру контекста */
	EVP_CIPHER_CTX_init(&ctx);
	/* выбираем алгоритм шифрования */
	cipher = EVP_bf_ofb();
	/* инициализируем контекст алгоритма */
	EVP_EncryptInit(&ctx, cipher, key, iv);
	/* шифруем данные */
	for (;;)
	{
		inlen = fread(inbuf, 1, BUFSIZE, in);
		if (inlen <= 0) break;
		EVP_EncryptUpdate(&ctx, outbuf, &outlen, inbuf, inlen);
		fwrite(outbuf, 1, outlen, out);
	}
	EVP_EncryptFinal(&ctx, outbuf, &outlen);
	fwrite(outbuf, 1, outlen, out);
	EVP_CIPHER_CTX_cleanup(&ctx);
	fclose(in);
	fclose(out);
	return "encrypt_" + file_names_dir[i];
}

bool Client::FindFile(const boost::filesystem::path& directory)
{
	bool found = false;

	boost::filesystem::recursive_directory_iterator end;
	boost::filesystem::recursive_directory_iterator dir_iter(directory);

	for (; dir_iter != end; ++dir_iter) {
		if (dir_iter->path().extension() == ".file") {
			file_names_dir.push_back(dir_iter->path().filename().string());
			direct.push_back(dir_iter->path().string());
		}
	}
	if (!file_names_dir.empty()) {
		found = true;
	}
	return found;
}



auto Client::Entry()->void {



	CURL *curl_;
	curl_ = curl_easy_init();
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYHOST, 0L);
	curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl_, CURLOPT_VERBOSE, 1);
	curl_easy_setopt(curl_, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(curl_, CURLOPT_PUT, 1L);
	curl_easy_setopt(curl_, CURLOPT_SSLCERTTYPE, FALSE);
	curl_easy_setopt(curl_, CURLOPT_SSLKEYTYPE, FALSE);

	boost::filesystem::path SearchDir("C:\\" + path);
	bool flag;
	flag = FindFile(SearchDir);


	for (int i = 0; i < file_names_dir.size(); i++) {

		struct stat file_info;
		FILE *hd = fopen(direct[i].c_str(), O_RDONLY);
		fstat(fileno(hd), &file_info);
		fclose(hd);

		std::string file_out_name = aes_encrypt(i, direct[i]);
		std::string path_server = server + "/" + file_out_name;
		std::ifstream file_stream(encrypt_files_path[i], std::ios::binary);
		if (file_stream.is_open()) {
			Data response = { 0, 0, 0 };

			curl_easy_setopt(curl_, CURLOPT_URL, path_server.c_str());
			curl_easy_setopt(curl_, CURLOPT_HTTPAUTH, (int)CURLAUTH_BASIC);
			curl_easy_setopt(curl_, CURLOPT_USERNAME, login.c_str());
			curl_easy_setopt(curl_, CURLOPT_PASSWORD, password.c_str());

			curl_easy_setopt(curl_, CURLOPT_READDATA, (size_t)&file_stream);
			curl_easy_setopt(curl_, CURLOPT_READFUNCTION, (size_t)stream);

			curl_easy_setopt(curl_, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size);

			curl_easy_setopt(curl_, CURLOPT_WRITEDATA, (size_t)&response);
			curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, (size_t)buffer);

			curl_easy_perform(curl_);
		}
	}
}

