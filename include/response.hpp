#pragma once
namespace xhttp_client
{
	class response
	{
	public:
		response(xnet::connection & conn)
			:conn_(conn)
		{

		}
	private:
		xnet::connection &conn_;
	};
}