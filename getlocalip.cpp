#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/if_link.h>
#include <iostream>

std::string getLocalIP()
{
    struct ifaddrs *ifaddr, *ifa;
    int family, s;
    char host[NI_MAXHOST];

    if ( getifaddrs(&ifaddr) == -1 ) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    /* Walk through linked list, maintaining head pointer so we
       can free list later */

    for ( ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next ) {
        if ( ifa->ifa_addr == NULL )
            continue;

        family = ifa->ifa_addr->sa_family;

        /* Display interface name and family (including symbolic
           form of the latter for the common families) */
/*
        printf("%-8s %s (%d)\n",
                ifa->ifa_name,
                (family == AF_PACKET) ? "AF_PACKET" :
                (family == AF_INET) ? "AF_INET" :
                (family == AF_INET6) ? "AF_INET6" : "???",
                family);
*/
        /* For an AF_INET* interface address, display the address */

//        if ( family == AF_INET || family == AF_INET6 ) {
        if ( family == AF_INET ) {
            s = getnameinfo(ifa->ifa_addr,
                    (family == AF_INET) ? sizeof(struct sockaddr_in) :
                    sizeof(struct sockaddr_in6),
                    host, NI_MAXHOST,
                    NULL, 0, NI_NUMERICHOST);
            if ( s != 0 ) {
                printf("getnameinfo() failed: %s\n", gai_strerror(s));
                exit(EXIT_FAILURE);
            }

            //printf("\t\taddress: <%s>\n", host);
            std::string ip_addr(host);
            if ( ip_addr!= "127.0.0.1" ) {
                freeifaddrs(ifaddr);
                return ip_addr;
            }
        }
        /*
        else if (family == AF_PACKET && ifa->ifa_data != NULL) {
            struct rtnl_link_stats *stats = (struct rtnl_link_stats*)ifa->ifa_data;

            printf("\t\ttx_packets = %10u; rx_packets = %10u\n"
                    "\t\ttx_bytes   = %10u; rx_bytes   = %10u\n",
                    stats->tx_packets, stats->rx_packets,
                    stats->tx_bytes, stats->rx_bytes);
        }*/
    }

    freeifaddrs(ifaddr);
    return "";
}
int main(int argc, char *argv[])
{
    std::string ip_addr = getLocalIP();
    std::cout << ip_addr << std::endl;
    return 0;
}
