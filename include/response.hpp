#pragma once
namespace xhttp_client
{
	class response
	{
	public:
		response(xnet::connection & conn)
			:conn_(conn)
		{
			conn_.regist_recv_callback([&](char *data, std::size_t len) {
				if (len == 0)
				{
					close();
					return;
				}
				http_parser_.append(data, len);
				if (http_parser_.parse_rsp())
				{
					parser_done_callback_();
					return;
				}
				conn_.async_recv_some();
			});
		}
		int get_status()
		{
			return std::strtol(http_parser_.get_status().c_str(), nullptr, 10);
		}
		std::string get_status_str()
		{
			return http_parser_.get_status_str();
		}
		std::string get_entry(const std::string &name)
		{
			using xutil::functional::strncasecmper;

			return http_parser_.get_header<strncasecmper>(name.c_str());
		}
		std::size_t get_content_length()
		{
			if (content_length_ == (size_t)-1)
			{
				auto buf = get_entry("Content-Length");
				if (buf.empty())
					content_length_ = 0;
				content_length_ = std::strtoul(buf.c_str(), 0, 10);
			}
			return content_length_;
		}
		std::string get_body()
		{
			std::string buffer = http_parser_.get_string();
			if (buffer.size() == get_content_length())
				return std::move(buffer);

			conn_.regist_recv_callback([&](char *data, std::size_t len) {
				if (len == 0)
				{
					close();
					return;
				}
				buffer.append(data, len);
				if (buffer.size() == get_content_length())
				{
					parser_done_callback_();
					return;
				}
			});
			conn_.async_recv(get_content_length() - buffer.size());
			xcoroutine::yield(parser_done_callback_);
			return std::move(buffer);
		}
	private:
		void reset()
		{
			http_parser_.reset();
			content_length_ = (size_t)-1;
		}
		void close()
		{
		}
		friend class client;
		std::size_t content_length_ = (size_t)-1;
		std::function<void()> parser_done_callback_;
		xhttper::http_parser http_parser_;
		xnet::connection &conn_;
	};
}