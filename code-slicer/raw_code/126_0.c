int X509_REQ_get_attr_by_NID ( const X509_REQ * req , int nid , int lastpos ) {
 return X509at_get_attr_by_NID ( req -> req_info -> attributes , nid , lastpos ) ;
 }