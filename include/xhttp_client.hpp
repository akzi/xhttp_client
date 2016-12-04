#pragma once
#include "detail/detail.hpp"
namespace xhttp_client
{
	class client
	{
	public:
		using response_callback = std::function<void(response &)>;
		client(xnet::connection &&conn)
			:conn_(std::move(conn)),
			response_(conn_)
		{

		}
		~client()
		{
			close();
		}
		client &append_entry(const std::string &name, const std::string &value)
		{
			if (reload_)
			{
				reload_ = false;
				http_builder_.reset();
				files_.clear();
				url_.clear();
				body_.clear();
				forms_.clear();
			}
			http_builder_.append_entry(name, value);
			return *this;
		}
		client &append_body(const std::string &data)
		{
			return *this;
		}
		client &append_form(const std::string &name, const std::string &value)
		{
			forms_.emplace_back(name, value);
			return *this;
		}
		client &append_file(const std::string &filepath)
		{
			files_.push_back(filepath);
			return *this;
		}
		bool do_get(const std::string &url, response_callback handle)
		{
			bool result = true;
			method_ = "GET";
			if (!check()) return false;
			conn_.regist_send_callback([&](std::size_t len) {
				if (len == 0)
				{
					close();
					return;
				}
			});
			conn_.async_send(http_builder_.
				set_method(method_).
				set_url(url).
				build_req());
			conn_.async_recv_some();
			xcoroutine::yield(response_.parser_done_callback_);
			handle(response_);
			return true;
		}
		bool do_post(const std::string &url, response_callback handle)
		{
			conn_.regist_send_callback([&](std::size_t len) {
				if (len == 0)
				{
					close();
					return;
				}
			});
			if (!check())
				return false;
			return true;
		}
	private:
		bool check()
		{
			if (method_ == "GET")
			{
				if (body_.size() ||
					files_.size() ||
					forms_.size())
					return false;
				return true;
			}
			if (body_.size())
			{
				if (files_.size() || forms_.size())
					return false;
			}
			return true;
		}
		void close()
		{

		}
		std::string gen_boundary()
		{
			return "----XHttpClientFormBoundaryuwYcfA2AIgxqIxA0";
		}
		bool reload_;
		std::string method_;
		xnet::connection conn_;
		response response_;
		xhttper::http_builder http_builder_;
		std::string url_;
		std::string body_;
		std::list<std::string> files_;
		std::list<std::pair<std::string, std::string>> forms_;
	};
}