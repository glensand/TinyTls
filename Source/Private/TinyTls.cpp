#include "TinyTls.h"
#include "Foundation.h"

IMPLEMENT_MODULE( FTinyTlsModule, TinyTls );
DEFINE_LOG_CATEGORY(LogTinyTls);

void FTinyTlsModule::StartupModule()
{
	FDefaultModuleImpl::StartupModule();

	WSADATA WsaData;
	(void)WSAStartup(MAKEWORD(2, 2), &WsaData);

	SSL_library_init();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();
}

void FTinyTlsModule::ShutdownModule()
{
	FDefaultModuleImpl::ShutdownModule();

	ERR_free_strings();
	EVP_cleanup();

	WSACleanup();
}
