
int open_socket_in(const char *ip, int port)
{
    struct sockaddr_in sock;
    struct hostent *he;
    int res;
    int one = 1;

    if (port < 1 || port > 65535) {
        /* htons() will truncate, not check */
        rs_log_error("port number out of range: %d", port);
        return -1;
    }

    if((he = gethostbyname(ip)) == NULL) {
        rs_log_error("%s is not a valid host", ip);
        return -1;
    }

    memset((char *) &sock, 0, sizeof(sock));
    sock.sin_port = htons(port);
    sock.sin_family = PF_INET;

    memcpy(&sock.sin_addr.s_addr, he->h_addr, he->h_length);

    res = socket(PF_INET, SOCK_STREAM, 0);
    if (res == -1) {
        rs_log_error("socket creation failed: %s", strerror(errno));
        return -1;
    }

    setsockopt(res, SOL_SOCKET, SO_REUSEADDR, (char *) &one, sizeof(one));

    /* now we've got a socket - we need to bind it */
    if (bind(res, (struct sockaddr *) &sock, sizeof(sock)) == -1) {
	rs_log_error("bind failed on port %d: %s", port, strerror(errno));
	close(res);
	return -1;
    }

    if (listen(res, 10)) {
        rs_log_error("listen failed: %s", strerror(errno));
        return -1;
    }

    // call accept(2) on this filedes to return a new socket when a connection is made
    return res;
}

