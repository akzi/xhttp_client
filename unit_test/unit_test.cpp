#pragma once
#include "../include/client_maker.hpp"
#include "../include/downloader.hpp"
#include "../../xtest/include/xtest.hpp"
#include <iostream>

xtest_run;

XTEST_SUITE(client)
{
	void do_get(xhttp_client::client &&client)
	{
		client.do_get("/", [](xhttp_client::response &resp) 
		{
			std::cout << resp.get_body() << std::endl;
		});
	}
	void do_post(xhttp_client::client &&client)
	{
		client.append_entry("Connection", "keep-alive");
		client.append_file("name1","index.html");
		client.do_post("/", [&](xhttp_client::response &resp) {

			std::cout << resp.get_body() << std::endl;
		});
	}
	void do_download_file(xhttp_client::client &&client)
	{
		client.do_get("/", [&](xhttp_client::response &resp)
		{
			std::cout << resp.get_status() << std::endl;
			std::cout << resp.get_status_str() << std::endl;

			std::vector<std::string> files;
			xhttp_client::downloader downloader(resp);
			downloader.set_path("D:/");
			if (!downloader.do_download(files))
			{
				std::cout << "download file error" << std::endl;
				return;
			}
			for (auto &itr : files)
				std::cout << itr << std::endl;
		});
	}
	XUNIT_TEST(do_test)
	{
		xnet::proactor pro;
		xhttp_client::client_maker maker(pro);
		maker.get_client("127.0.0.1", 9001, 
			[](const std::string & error_code, xhttp_client::client &&client)
		{
			if (error_code.size())
			{
				std::cout << "error£º" << error_code << std::endl;
				return;
			}
			do_post(std::move(client));
		});
		pro.run();
	}
}