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
//#pragma comment(lib, "curllib_static.lib")
#pragma comment(lib, "wldap32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "ssleay32.lib")
//#pragma comment(lib, "openldap.lib")
#pragma comment(lib, "libeay32.lib")
#include <io.h>
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
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
		int hd = _open(direct[i].c_str(), O_RDONLY);
		fstat(hd, &file_info);
		_close(hd);

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



void main() {
	Client A("config");

	system("pause");

};
