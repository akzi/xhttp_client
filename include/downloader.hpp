#pragma once
#include "xhttp_client.hpp"
namespace xhttp_client
{
	class downloader
	{
	public:
		downloader(client &_client)
			:client_(_client)
		{

		}
		~downloader()
		{

		}
		bool do_download(std::vector<std::string> &files)
		{

		}
	private:
		client &client_;
	};
}
