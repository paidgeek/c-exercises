#include "server.h"

#define MAXLINE 1024

int Socket(int family, int type, int protocol)
{
	int n;
	
	if ((n = socket(family, type, protocol)) < 0) {
		PERR("socket")
	}

	if(setsockopt(n, SOL_SOCKET, SO_REUSEADDR, &(int) {1}, sizeof(int)) <0 ) {
		PERR("setsockopt")
	}

	return n;
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if(bind(fd, sa, salen) < 0) {
		PERR("bind")
	}
}

void Listen(int fd, int backlog)
{
	char *ptr;

	if(listen(fd, backlog) < 0) {
		PERR("listen")
	}
}

void Connect(int socket, const struct sockaddr *sa, socklen_t salen)
{
	if(connect(socket, sa, salen) < 0) {
		PERR("connect")
	}
}

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
	int n;

	again:
	if((n = accept(fd, sa, salenptr)) < 0) {
		if(errno == ECONNABORTED) {
			goto again;
		} else {
			PERR("accept")
		}
	}

	return n;
}

void Close(int fd)
{
	if(close(fd) < 0) {
		PERR("close")
	}
}

ssize_t readn(int fd, void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nread;
	char *ptr;
	
	ptr = vptr;
	nleft = n;
	
	while (nleft > 0) {
		if ( (nread = read(fd, ptr, nleft)) < 0) {
			if (errno == EINTR) {
				nread = 0; /* read se kliče ponovno */
			} else {
				return -1;
			}
		} else if (nread == 0) {
			break; /* EOF */
		}
			
		nleft -= nread;
		ptr += nread;
	}
	
	return n - nleft; /* return >= 0 */
}

ssize_t writen(int fd, const void *vptr, size_t n)
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;

	ptr = vptr;
	nleft = n;

	while (nleft > 0) {
		if ((nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && errno == EINTR) {
				nwritten = 0; /* write se kliče ponovno */
			} else {
				return -1; /* napaka */
			}
		}
		
		nleft -= nwritten;
		ptr += nwritten;
	}
	
	return n;
}

static ssize_t my_read(int fd, char *ptr)
{
	static int read_cnt = 0;
	static char *read_ptr;
	static char read_buf[MAXLINE];
	
	if (read_cnt <= 0) {
		again:
		if ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
			if (errno == EINTR) {
				goto again;
			}
			
			return -1;
		} else if (read_cnt == 0) {
			return 0;
		}
		
		read_ptr = read_buf;
	}
	
	read_cnt--;
	*ptr = *read_ptr++;
	
	return 1;
}

ssize_t readline(int fd, void *vptr, size_t maxlen)
{
	ssize_t n, rc;
	char c, *ptr;
	ptr = vptr;
	
	for (n = 1; n < maxlen; n++) {
		if ( (rc = my_read(fd, &c)) == 1) {
			*ptr++ = c;
			
			if (c == '\n') {
				break; /* newline se shrani, kot pri fgets */
			}
		} else if (rc == 0) {
			*ptr = 0;
			
			return n - 1; /* EOF, n - 1 bajtov prebranih */
		} else {
			return -1; /* napaka, read postavi errno */
		}
	}
	
	*ptr = 0; /* ničti terminator, kot pri fgets */
	
	return n;
}
