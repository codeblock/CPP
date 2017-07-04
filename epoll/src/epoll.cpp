#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
//#include <sys/epoll.h> // move to process.h
#include <errno.h>
#include <signal.h> // add
#include <thread>
#include <iostream>

#include "Pool.h" // add
#include "query.h" // add

#define MAXEVENTS 64
#define MAXTHREADS 4

int sfd, s;
int efd;
struct epoll_event event;
struct epoll_event *events;

void sighandler(int signo);
void* eventLoop(struct epoll_event *evts, int idx);

int processCallBack(processArgs args)
{
	int rtn;
	
	if (
			args.end // Connection Closed
		 || ((args.evt->events & EPOLLERR) || (args.evt->events & EPOLLHUP) || (!(args.evt->events & EPOLLIN))) // epoll error
	)
	{
		//server_ips_index[0] = args.evt->data.fd;
		// if (args.evt->data.fd == serverip index) { init index }
		int i;
		for (i = 0; i < 3; i++)
		{
			if (server_ips_index[i] == args.evt)
			{
				switch (i)
				{
				case 0:
					myUTIL::strCopy(server_ips.m_chat, "");
					Logger::log(Logger::DEBUG, "clear chat ip");
					break;
				case 1:
					myUTIL::strCopy(server_ips.m_game, "");
					Logger::log(Logger::DEBUG, "clear game ip");
					break;
				case 2:
					myUTIL::strCopy(server_ips.m_lobby, "");
					Logger::log(Logger::DEBUG, "clear lobby ip");
					break;
				}
				server_ips_index[i] = NULL;
				break;
			}
		}
		
		return 0;
	}
	
	rtn = send(args.evt->data.fd, args.buf, *args.cnt, 0);
	if (rtn == -1)
	{
		perror ("write");
		abort ();
	}
	else
	{
		args.evt->events = EPOLLIN | EPOLLET | EPOLLONESHOT;
		epoll_ctl(efd, EPOLL_CTL_MOD, args.evt->data.fd, args.evt);
	}
	
	return rtn;
}

static int make_socket_non_blocking(int sfd)
{
	int flags, s;

	flags = fcntl (sfd, F_GETFL, 0);
	if (flags == -1)
	{
		perror ("fcntl");
		return -1;
	}

	flags |= O_NONBLOCK;
	s = fcntl (sfd, F_SETFL, flags);
	if (s == -1)
	{
		perror ("fcntl");
		return -1;
	}

	return 0;
}

static int create_and_bind (char *port)
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int s, sfd;

	memset (&hints, 0, sizeof (struct addrinfo));
	hints.ai_family = AF_UNSPEC; /* Return IPv4 and IPv6 choices */
	hints.ai_socktype = SOCK_STREAM; /* We want a TCP socket */
	hints.ai_flags = AI_PASSIVE; /* All interfaces */

	s = getaddrinfo (NULL, port, &hints, &result);
	if (s != 0)
	{
		fprintf (stderr, "getaddrinfo: %s\n", gai_strerror (s));
		return -1;
	}

	for (rp = result; rp != NULL; rp = rp->ai_next)
	{
		sfd = socket (rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (sfd == -1)
			continue;

		s = bind (sfd, rp->ai_addr, rp->ai_addrlen);
		if (s == 0)
		{
			/* We managed to bind successfully! */
			break;
		}

		close(sfd);
	}

	if (rp == NULL)
	{
		fprintf (stderr, "Could not bind\n");
		return -1;
	}

	freeaddrinfo(result);

	return sfd;
}

int main (int argc, char *argv[])
{

	if (argc != 2)
	{
		fprintf (stderr, "Usage: %s [port]\n", argv[0]);
		exit (EXIT_FAILURE);
	}
	
	// add
	struct sigaction act;
	act.sa_handler = sighandler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaction(SIGTERM, &act, 0);
	// add

	sfd = create_and_bind (argv[1]);
	if (sfd == -1)
	abort ();
	Logger::log(Logger::INFO, "Server socket created.");
	Logger::log(Logger::INFO, "Server bind completed.");

	s = make_socket_non_blocking (sfd);
	if (s == -1)
	abort ();

	s = listen (sfd, SOMAXCONN);
	if (s == -1)
	{
		perror ("listen");
		abort ();
	}
	Logger::log(Logger::INFO, "Server listen completed.");
	Logger::log(Logger::INFO, "Server started.");

	efd = epoll_create1 (0);
	if (efd == -1)
	{
		perror ("epoll_create");
		abort ();
	}

	event.data.fd = sfd;
	event.events = EPOLLIN | EPOLLET;
	s = epoll_ctl (efd, EPOLL_CTL_ADD, sfd, &event);
	if (s == -1)
	{
		perror ("epoll_ctl");
		abort ();
	}

	/* Buffer where events are returned */
	events = (struct epoll_event *)calloc (MAXEVENTS, sizeof event);
	Logger::log(Logger::INFO, "%d Event Objects created.", MAXEVENTS);
	
	int i;
	
	std::thread thrd[MAXTHREADS];
	for (i = 0; i < MAXTHREADS; i++)
	{
		thrd[i] = std::thread(eventLoop, events + i * (MAXEVENTS / MAXTHREADS), i);
	}
	Logger::log(Logger::INFO, "%d Threads created.", i);
	for (i = 0; i < MAXTHREADS; i++)
	{
		thrd[i].join();
	}
	
	//Query::release(); // add
	free (events);
	close (sfd);

	return EXIT_SUCCESS;
}

void sighandler(int signo)
{
	//Query::release(); // add
	Pool::clear();
	free (events);
	Logger::log(Logger::INFO, "Event Objects released.");
	close (sfd);
	Logger::log(Logger::INFO, "Server shutdown.");
	exit(0);
}

void* eventLoop(struct epoll_event *evts, int idx)
{
	/* The event loop */
	while (1)
	{
		int n, i;

		n = epoll_wait (efd, evts, MAXEVENTS / MAXTHREADS, -1);
		for (i = 0; i < n; i++)
		{
			if ((evts[i].events & EPOLLERR) ||
				(evts[i].events & EPOLLHUP) ||
				(!(evts[i].events & EPOLLIN)))
			{
				processArgs args = {&evts[i], NULL, (ssize_t *)0, 0};
				processCallBack(args);
				/* An error has occured on this fd, or the socket is not
					 ready for reading (why were we notified then?) */
				fprintf (stderr, "epoll error\n");
				close (evts[i].data.fd);
				continue;
			}

			else if (sfd == evts[i].data.fd)
			{
				/* We have a notification on the listening socket, which
				 means one or more incoming connections. */
				while (1)
				{
					struct sockaddr in_addr;
					socklen_t in_len;
					int infd;
					char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

					in_len = sizeof in_addr;
					infd = accept (sfd, &in_addr, &in_len);
					if (infd == -1)
					{
						if ((errno == EAGAIN) ||
							(errno == EWOULDBLOCK))
						{
							/* We have processed all incoming
							 connections. */
							break;
						}
						else
						{
							perror ("accept");
							break;
						}
					}

					s = getnameinfo (&in_addr, in_len,
									hbuf, sizeof hbuf,
									sbuf, sizeof sbuf,
									NI_NUMERICHOST | NI_NUMERICSERV);
					if (s == 0)
					{
						printf("Accepted connection on descriptor %d "
							"(host=%s, port=%s)\n", infd, hbuf, sbuf);
					}

					/* Make the incoming socket non-blocking and add it to the
					 list of fds to monitor. */
					s = make_socket_non_blocking (infd);
					if (s == -1)
					abort ();

					event.data.fd = infd;
					event.events = EPOLLIN | EPOLLET;
					s = epoll_ctl (efd, EPOLL_CTL_ADD, infd, &event);
					if (s == -1)
					{
						perror ("epoll_ctl");
						abort ();
					}
				}
				continue;
			}
			else
			{
				/* We have data on the fd waiting to be read. Read and
				 display it. We must read whatever data is available
				 completely, as we are running in edge-triggered mode
				 and won't get a notification again for the same
				 data. */
				int done = 0;

				while (1)
				{
					ssize_t count;
					unsigned char buf[512] = {0}; // [ = {0};] add
					
					//count = read (evts[i].data.fd, buf, sizeof buf);
					count = recv(evts[i].data.fd, buf, sizeof buf, 0); // add
					if (count == -1)
					{
						/* If errno == EAGAIN, that means we have read all
						 data. So go back to the main loop. */
						if (errno != EAGAIN)
						{
							perror ("read");
							done = 1;
						}
						//break;
					}
					else if (count == 0)
					{
						/* End of file. The remote has closed the
						 connection. */
						done = 1;
						//break;
					}

					/* Write the buffer to standard output */
					//s = write (1, buf, count);
					processArgs args = {&evts[i], buf, &count, done};
					if (done)
					{
						processCallBack(args);
						break;
					}
					if (count > 0)
					{
						process(args);
					}
					/*
					process(buf, &count);
					s = send(evts[i].data.fd, buf, count, 0); // add
					if (s == -1)
					{
						perror ("write");
						abort ();
					}
					else
					{
						//std::cout << "/" << args << " : " << std::this_thread::get_id() << "/" << std::endl;
						evts[i].events = EPOLLIN | EPOLLET | EPOLLONESHOT;
						epoll_ctl(efd, EPOLL_CTL_MOD, evts[i].data.fd, &evts[i]);
						//puts(buf + 2);
						//printf("%s", buf + 2);
					}
					*/
				}

				if (done)
				{
					printf ("Closed connection on descriptor %d\n",
							evts[i].data.fd);

					/* Closing the descriptor will make epoll remove it
					 from the set of descriptors which are monitored. */
					close (evts[i].data.fd);
				}
			}
		}
	}
	
	return (void *)NULL;
}