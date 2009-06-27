/*
 *  Server.c
 *  iX-Yoke-plugin
 *
 *  Created by Daniel Dickison on 5/9/09.
 *  Copyright 2009 Daniel_Dickison. All rights reserved.
 *
 */

#include "iX-Yoke-plugin.h"

#if APL || LIN
#include <arpa/inet.h>
#endif


/****** UDP Server ******/

#if IBM
void server_loop(LPVOID pParameter)
#else
void *server_loop(void *arg)
#endif
{
    int sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    struct sockaddr_in addr; 
    uint8_t buffer[kPacketSizeLimit];
    size_t recv_size;
    socklen_t addr_len = 0;
    
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(kServerPort);
    
    if (-1 == bind(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)))
    {
        server_msg = "Unable to bind server socket.";
        goto stop_server;
    }
    
    addr_len = sizeof(addr);
    getsockname(sock, (struct sockaddr *)&addr, &addr_len);
    server_ip = inet_ntoa(addr.sin_addr);
    
    server_msg = "Starting server loop.";
    
    for (;;) 
    {
        recv_size = recvfrom(sock, (char *)buffer, kPacketSizeLimit, 0, NULL, NULL);
        if (recv_size < 0)
        {
            server_msg = strerror(errno);
            goto stop_server;
        }
        
        int i = 0;
        while (i < recv_size)
        {
            uint8_t tag = ix_get_tag(buffer, &i);
            if (tag == kServerKillTag)
            {
                server_msg = "Server kill received";
                goto stop_server;
            }
            else if (tag == kProtocolVersion1Tag)
            {
                for (int axis = 0; axis < kNumAxes; axis++)
                {
                    get_axis((iXControlAxisID)axis)->value = ix_get_ratio(buffer, &i);
                }
            }
            // else ignore packet
        }
    }
    
stop_server:
    closesocket(sock);
#if APL || LIN
    return NULL;
#endif
}

