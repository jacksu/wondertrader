#pragma once
#include "../Share/IParserApi.h"
#include "../Share/BoostDefine.h"

#include <queue>

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/asio/io_service.hpp>

USING_NS_OTP;
using namespace boost::asio;

class ParserUDP : public IParserApi
{
public:
	ParserUDP();
	~ParserUDP();

	//IQuoteParser �ӿ�
public:
	virtual bool init(WTSParams* config) override;

	virtual void release() override;

	virtual bool connect() override;

	virtual bool disconnect() override;

	virtual bool isConnected() override;

	virtual void subscribe(const CodeSet &vecSymbols) override;
	virtual void unsubscribe(const CodeSet &vecSymbols) override;

	virtual void registerListener(IParserApiListener* listener) override;


private:
	void	handle_read(const boost::system::error_code& e, std::size_t bytes_transferred, bool isBroad);
	void	handle_write(const boost::system::error_code& e);

	bool	reconnect();

	void	subscribe();

	void	extract_buffer(uint32_t length, bool isBroad);

private:
	void	doOnConnected();
	void	doOnDisconnected();

	void	do_send();

private:
	std::string	_hots;
	int			_bport;
	int			_sport;

	ip::udp::endpoint	_broad_ep;
	ip::udp::endpoint	_server_ep;
	io_service			_io_service;

	io_service::strand	_strand;

	ip::udp::socket*	_b_socket;
	ip::udp::socket*	_s_socket;

	boost::array<char, 1024> _b_buffer;
	boost::array<char, 1024> _s_buffer;

	IParserApiListener*		_sink;
	bool					_stopped;
	bool					_connecting;

	CodeSet					_set_subs;

	BoostThreadPtr			_thrd_parser;

	std::queue<std::string>	_send_queue;
};

