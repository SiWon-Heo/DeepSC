static inline struct isoent * isoent_clone ( struct isoent * src ) {
 return ( isoent_new ( src -> file ) ) ;
 }