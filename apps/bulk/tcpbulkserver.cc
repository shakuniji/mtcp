/*
 * tcpbulkserver.{cc,hh} -- a bulk transfer server over TCP
 * Rafael Laufer
 *
 * Copyright (c) 2015 Bell Labs, Alcatel-Lucent
 *
 */


#include <click/config.h>
#include <click/args.hh>
#include <click/error.hh>
#include <click/router.hh>
#include <click/routervisitor.hh>
#include <click/standard/scheduleinfo.hh>
#include <clicknet/ip.h>
#include <clicknet/tcp.h>
#include "tcpbulkserver.hh"
#include "util.hh"
CLICK_DECLS

TCPBulkServer::TCPBulkServer()
	: _task(this), _length(0), _buflen(0), _verbose(false)
{
}

int
TCPBulkServer::configure(Vector<String> &conf, ErrorHandler *errh)
{
	String buflen = "64K";
	if (Args(conf, this, errh)
		.read_mp("ADDRESS", _addr)
		.read_mp("PORT", _port)
		.read("BUFLEN", buflen)
		.read("VERBOSE", _verbose)
		.complete() < 0)
		return -1;

	int b_shift = get_shift(buflen);

	if (!IntArg().parse(buflen, _buflen) || _buflen == 0)
		return errh->error("BUFLEN must be a positive integer");

	_buflen <<= b_shift;

	return 0;
}

int
TCPBulkServer::initialize(ErrorHandler *errh)
{
	int r = TCPApplication::initialize(errh);
	if (r < 0)
		return r;

	ScheduleInfo::initialize_task(this, &_task, errh);

	return 0;
}



void
TCPBulkServer::push(int, Packet *p)
{
	output(0).push(p);
}

bool
TCPBulkServer::run_task(Task *)
{
	int err = 0, fd;
	uint16_t  port;
	IPAddress addr;
	char *msg = new char [_buflen];
	assert(msg);

	fd = click_socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		perror("socket");
		return fd;
	}
	if (_verbose)
		click_chatter("%s: got sockfd %d", class_name(), fd);

	// Bind
	err = click_bind(fd, _addr, _port);
	if (err) {
		perror("bind");
		return err;
	}
	if (_verbose)
		click_chatter("%s: bounded to %s, port %d", \
		                          class_name(), _addr.unparse().c_str(), _port);

	// Listen
	err = click_listen(fd, 1);
	if (err) {
		perror("listen");
		return false;
	}
	if (_verbose)
		click_chatter("%s: listening", class_name());

	// Accept
	int sockfd = click_accept(fd, addr, port);
	if (sockfd == -1) {
		perror("accept");
		return false;
	}
	if (_verbose)
		click_chatter("%s: accepted, sockfd = %d", class_name(), sockfd);

	Timestamp begin = Timestamp::now_steady();
	bool first = true;
	while (err != -1) {
		if (_verbose)
			click_chatter("%s: preparing to receive", class_name());

//		int size = click_recv(sockfd, msg, _buflen);
		Packet *p = click_pull(sockfd);
		if (!p) {
			perror("recv");
			break;
		}

		int size = p->length();
		if (size == 0)
			break;
//		if (size == -1) {
//			perror("recv");
//			break;
//		}

		_length += size;

		if (first) {
			first = false;
			begin = Timestamp::now_steady();
		}

		if (_verbose)
			click_chatter("%s: received %d bytes", class_name(), size);

		if (noutputs() > 1) {
//			Packet *p = Packet::make(0, msg, _buflen, 0);
			output(1).push(p);
		}
		else
			p->kill();
	}
	Timestamp end = Timestamp::now_steady();

	if (_verbose)
		click_chatter("%s: closing sockfd %d", class_name(), sockfd);
	click_close(sockfd);

	if (_verbose)
		click_chatter("%s: closing sockfd %d", class_name(), fd);
	click_close(fd);

	float rate_mbps = float(_length << 3)/(end - begin).usecval();

	if (rate_mbps < 1000)
		click_chatter("%s: TX rate %.3f Mbps", class_name(), rate_mbps);
	else
		click_chatter("%s: TX rate %.3f Gbps", class_name(), rate_mbps/1000);

	return false;
}

CLICK_ENDDECLS
EXPORT_ELEMENT(TCPBulkServer)
ELEMENT_REQUIRES(Util TCPApplication)
