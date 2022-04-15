static void xhci_kick_ep ( XHCIState * xhci , unsigned int slotid , unsigned int epid , unsigned int streamid ) {
 XHCIEPContext * epctx ;
 assert ( slotid >= 1 && slotid <= xhci -> numslots ) ;
 assert ( epid >= 1 && epid <= 31 ) ;
 if ( ! xhci -> slots [ slotid - 1 ] . enabled ) {
 DPRINTF ( "xhci: xhci_kick_ep for disabled slot %d\n" , slotid ) ;
 return ;
 }
 epctx = xhci -> slots [ slotid - 1 ] . eps [ epid - 1 ] ;
 if ( ! epctx ) {
 DPRINTF ( "xhci: xhci_kick_ep for disabled endpoint %d,%d\n" , epid , slotid ) ;
 return ;
 }
 xhci_kick_epctx ( epctx , streamid ) ;
 }