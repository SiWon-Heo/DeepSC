static int restore_toc_entry ( ArchiveHandle * AH , TocEntry * te , bool is_parallel ) {
 RestoreOptions * ropt = AH -> public . ropt ;
 int status = WORKER_OK ;
 teReqs reqs ;
 bool defnDumped ;
 AH -> currentTE = te ;
 if ( _tocEntryIsACL ( te ) ) reqs = 0 ;
 else reqs = te -> reqs ;
 if ( ! ropt -> createDB && strcmp ( te -> desc , "DATABASE" ) == 0 ) reqs = 0 ;
 if ( ! ropt -> suppressDumpWarnings && strcmp ( te -> desc , "WARNING" ) == 0 ) {
 if ( ! ropt -> dataOnly && te -> defn != NULL && strlen ( te -> defn ) != 0 ) write_msg ( modulename , "warning from original dump file: %s\n" , te -> defn ) ;
 else if ( te -> copyStmt != NULL && strlen ( te -> copyStmt ) != 0 ) write_msg ( modulename , "warning from original dump file: %s\n" , te -> copyStmt ) ;
 }
 defnDumped = false ;
 if ( ( reqs & REQ_SCHEMA ) != 0 ) {
 if ( te -> namespace ) ahlog ( AH , 1 , "creating %s \"%s.%s\"\n" , te -> desc , te -> namespace , te -> tag ) ;
 else ahlog ( AH , 1 , "creating %s \"%s\"\n" , te -> desc , te -> tag ) ;
 _printTocEntry ( AH , te , false , false ) ;
 defnDumped = true ;
 if ( strcmp ( te -> desc , "TABLE" ) == 0 ) {
 if ( AH -> lastErrorTE == te ) {
 if ( ropt -> noDataForFailedTables ) {
 if ( is_parallel ) status = WORKER_INHIBIT_DATA ;
 else inhibit_data_for_failed_table ( AH , te ) ;
 }
 }
 else {
 if ( is_parallel ) status = WORKER_CREATE_DONE ;
 else mark_create_done ( AH , te ) ;
 }
 }
 if ( strcmp ( te -> desc , "DATABASE" ) == 0 ) {
 ahlog ( AH , 1 , "connecting to new database \"%s\"\n" , te -> tag ) ;
 _reconnectToDB ( AH , te -> tag ) ;
 ropt -> dbname = pg_strdup ( te -> tag ) ;
 }
 }
 if ( ( reqs & REQ_DATA ) != 0 ) {
 if ( te -> hadDumper ) {
 if ( AH -> PrintTocDataPtr != NULL ) {
 _printTocEntry ( AH , te , true , false ) ;
 if ( strcmp ( te -> desc , "BLOBS" ) == 0 || strcmp ( te -> desc , "BLOB COMMENTS" ) == 0 ) {
 ahlog ( AH , 1 , "processing %s\n" , te -> desc ) ;
 _selectOutputSchema ( AH , "pg_catalog" ) ;
 if ( strcmp ( te -> desc , "BLOB COMMENTS" ) == 0 ) AH -> outputKind = OUTPUT_OTHERDATA ;
 ( * AH -> PrintTocDataPtr ) ( AH , te ) ;
 AH -> outputKind = OUTPUT_SQLCMDS ;
 }
 else {
 _disableTriggersIfNecessary ( AH , te ) ;
 _becomeOwner ( AH , te ) ;
 _selectOutputSchema ( AH , te -> namespace ) ;
 ahlog ( AH , 1 , "processing data for table \"%s.%s\"\n" , te -> namespace , te -> tag ) ;
 if ( is_parallel && te -> created ) {
 StartTransaction ( & AH -> public ) ;
 ahprintf ( AH , "TRUNCATE TABLE %s%s;
\n\n" , ( PQserverVersion ( AH -> connection ) >= 80400 ? "ONLY " : "" ) , fmtId ( te -> tag ) ) ;
 }
 if ( te -> copyStmt && strlen ( te -> copyStmt ) > 0 ) {
 ahprintf ( AH , "%s" , te -> copyStmt ) ;
 AH -> outputKind = OUTPUT_COPYDATA ;
 }
 else AH -> outputKind = OUTPUT_OTHERDATA ;
 ( * AH -> PrintTocDataPtr ) ( AH , te ) ;
 if ( AH -> outputKind == OUTPUT_COPYDATA && RestoringToDB ( AH ) ) EndDBCopyMode ( & AH -> public , te -> tag ) ;
 AH -> outputKind = OUTPUT_SQLCMDS ;
 if ( is_parallel && te -> created ) CommitTransaction ( & AH -> public ) ;
 _enableTriggersIfNecessary ( AH , te ) ;
 }
 }
 }
 else if ( ! defnDumped ) {
 ahlog ( AH , 1 , "executing %s %s\n" , te -> desc , te -> tag ) ;
 _printTocEntry ( AH , te , false , false ) ;
 }
 }
 if ( AH -> public . n_errors > 0 && status == WORKER_OK ) status = WORKER_IGNORED_ERRORS ;
 return status ;
 }