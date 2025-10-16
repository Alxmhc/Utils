//crypt32.lib

#include <string>

#include <windows.h>

namespace cert
{
	class CertS
	{
		HCERTSTORE store;
		void Close()
		{
			if(store != nullptr)
			{
				CertCloseStore(store, CERT_CLOSE_STORE_FORCE_FLAG);
			}
		}
	public:
		CertS() : store(nullptr) {}

		~CertS()
		{
			Close();
		}

		bool Open_System_Storage_User(const char* path)
		{
			Close();
			store = CertOpenStore(CERT_STORE_PROV_SYSTEM_A, 0, 0, CERT_STORE_OPEN_EXISTING_FLAG | CERT_SYSTEM_STORE_CURRENT_USER, path);
			return store != nullptr;
		}

		bool Open_File_PEM(const char* fl)
		{
			Close();
			store = CertOpenStore(CERT_STORE_PROV_FILENAME_A, X509_ASN_ENCODING, 0, 0, fl);
			return store != nullptr;
		}

		PCCERT_CONTEXT read(PCCERT_CONTEXT ct) const
		{
			if(store == nullptr)
				return nullptr;
			return CertEnumCertificatesInStore(store, ct);
		}
	};

	static bool Get_String(PCCERT_CONTEXT ct, DWORD type, DWORD fl, std::string &res)
	{
		const DWORD sz = CertGetNameStringA(ct, type, fl, nullptr, nullptr, 0);
		if(sz != 0)
		{
			res.resize(sz);
			if(CertGetNameStringA(ct, type, fl, nullptr, &res[0], sz) == sz)
			{
				res.resize(sz-1);
				return true;
			}
		}
		res.clear();
		return false;
	}
}
