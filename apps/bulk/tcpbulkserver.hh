/*
 * tcpbulkserver.{cc,hh} -- a bulk transfer server over TCP
 * Rafael Laufer
 *
 * Copyright (c) 2015 Bell Labs, Alcatel-Lucent
 *
 */

#ifndef CLICK_TCPBULKSERVER_HH
#define CLICK_TCPBULKSERVER_HH
#include <click/element.hh>
#include "tcpapplication.hh"
#include "usertask.hh"
CLICK_DECLS

class TCPBulkServer : public TCPApplication {  public:

	TCPBulkServer() CLICK_COLD;

	const char *class_name() const { return "TCPBulkServer"; }
	const char *port_count() const { return "1/1-2"; }
	const char *processing() const { return PUSH; }

	int configure(Vector<String> &, ErrorHandler *) CLICK_COLD;
	int initialize(ErrorHandler *) CLICK_COLD;

	void push(int, Packet *);
	bool run_task(Task *);

  private:

	UserTask _task;
	IPAddress _addr;
	uint16_t _port;
	uint64_t _length;
	uint64_t _buflen;
	bool _verbose;
};

CLICK_ENDDECLS
#endif

