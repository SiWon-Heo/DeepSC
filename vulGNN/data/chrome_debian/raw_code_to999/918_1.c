char * cluster_conn_opts ( ClusterInfo * cluster ) {
 static char conn_opts [ MAXPGPATH + NAMEDATALEN + 100 ] ;
 if ( cluster -> sockdir ) snprintf ( conn_opts , sizeof ( conn_opts ) , "--host \"%s\" --port %d --username \"%s\"" , cluster -> sockdir , cluster -> port , os_info . user ) ;
 else snprintf ( conn_opts , sizeof ( conn_opts ) , "--port %d --username \"%s\"" , cluster -> port , os_info . user ) ;
 return conn_opts ;
 }