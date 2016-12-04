#pragma once
#include "../include/xhttp_client.hpp"
#include "../../xtest/include/xtest.hpp"

xtest_run;

XTEST_SUITE(client)
{
	void do_req(xnet::connection &conn)
	{
		xhttp_client::client client(std::move(conn));
		client.do_get("/", [](xhttp_client::response &resp) 
		{

		});
	}
	XUNIT_TEST(do_test)
	{
		xnet::proactor pro;
		auto connector = pro.get_connector();
		connector.async_connect("127.0.0.1", 9001);

		connector.bind_fail_callback([](std::string) {
		
		}).bind_success_callback([](xnet::connection &&conn) {
			do_req(std::move(conn));
		});
		pro.run();
	}
}