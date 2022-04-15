static inline int rawv6_rcv_skb ( struct sock * sk , struct sk_buff * skb ) {
 if ( ( raw6_sk ( sk ) -> checksum || sk -> sk_filter ) && skb_checksum_complete ( skb ) ) {
 atomic_inc ( & sk -> sk_drops ) ;
 kfree_skb ( skb ) ;
 return NET_RX_DROP ;
 }
 if ( sock_queue_rcv_skb ( sk , skb ) < 0 ) {
 kfree_skb ( skb ) ;
 return NET_RX_DROP ;
 }
 return 0 ;
 }