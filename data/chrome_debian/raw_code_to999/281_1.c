int udpv6_queue_rcv_skb ( struct sock * sk , struct sk_buff * skb ) {
 struct udp_sock * up = udp_sk ( sk ) ;
 int rc ;
 int is_udplite = IS_UDPLITE ( sk ) ;
 if ( ! xfrm6_policy_check ( sk , XFRM_POLICY_IN , skb ) ) goto drop ;
 if ( ( is_udplite & UDPLITE_RECV_CC ) && UDP_SKB_CB ( skb ) -> partial_cov ) {
 if ( up -> pcrlen == 0 ) {
 LIMIT_NETDEBUG ( KERN_WARNING "UDPLITE6: partial coverage" " %d while full coverage %d requested\n" , UDP_SKB_CB ( skb ) -> cscov , skb -> len ) ;
 goto drop ;
 }
 if ( UDP_SKB_CB ( skb ) -> cscov < up -> pcrlen ) {
 LIMIT_NETDEBUG ( KERN_WARNING "UDPLITE6: coverage %d " "too small, need min %d\n" , UDP_SKB_CB ( skb ) -> cscov , up -> pcrlen ) ;
 goto drop ;
 }
 }
 if ( sk -> sk_filter ) {
 if ( udp_lib_checksum_complete ( skb ) ) goto drop ;
 }
 if ( ( rc = sock_queue_rcv_skb ( sk , skb ) ) < 0 ) {
 if ( rc == - ENOMEM ) UDP6_INC_STATS_BH ( sock_net ( sk ) , UDP_MIB_RCVBUFERRORS , is_udplite ) ;
 goto drop_no_sk_drops_inc ;
 }
 return 0 ;
 drop : atomic_inc ( & sk -> sk_drops ) ;
 drop_no_sk_drops_inc : UDP6_INC_STATS_BH ( sock_net ( sk ) , UDP_MIB_INERRORS , is_udplite ) ;
 kfree_skb ( skb ) ;
 return - 1 ;
 }