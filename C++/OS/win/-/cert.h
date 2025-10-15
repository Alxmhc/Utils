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

		bool Read_System_Storage_User(const char* path)
		{
			Close();
			store = CertOpenStore(CERT_STORE_PROV_SYSTEM_REGISTRY_A, 0, NULL, CERT_STORE_OPEN_EXISTING_FLAG | CERT_STORE_READONLY_FLAG | CERT_SYSTEM_STORE_CURRENT_USER, path);
			return store != nullptr;
		}

		bool nxt(PCCERT_CONTEXT &ct)
		{
			if(store == nullptr)
				return false;
			ct = CertEnumCertificatesInStore(store, ct);
			return ct != nullptr;
		}
	};

	static std::string Get_String(PCCERT_CONTEXT ct, DWORD type, DWORD fl)
	{
		std::string res;
		DWORD sz = CertGetNameStringA(ct, type, fl, NULL, NULL, 0);
		if(sz > 1)
		{
			res.resize(sz);
			CertGetNameStringA(ct, type, fl, NULL, &res[0], sz);
		}
		return res;
	}
}
