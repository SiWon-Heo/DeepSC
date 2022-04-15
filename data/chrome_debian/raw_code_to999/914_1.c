static void _reconnectToDB ( ArchiveHandle * AH , const char * dbname ) {
 if ( RestoringToDB ( AH ) ) ReconnectToServer ( AH , dbname , NULL ) ;
 else {
 PQExpBuffer qry = createPQExpBuffer ( ) ;
 appendPQExpBuffer ( qry , "\\connect %s\n\n" , dbname ? fmtId ( dbname ) : "-" ) ;
 ahprintf ( AH , "%s" , qry -> data ) ;
 destroyPQExpBuffer ( qry ) ;
 }
 if ( AH -> currUser ) free ( AH -> currUser ) ;
 AH -> currUser = NULL ;
 if ( AH -> currSchema ) free ( AH -> currSchema ) ;
 AH -> currSchema = NULL ;
 if ( AH -> currTablespace ) free ( AH -> currTablespace ) ;
 AH -> currTablespace = NULL ;
 AH -> currWithOids = - 1 ;
 _doSetFixedOutputState ( AH ) ;
 }