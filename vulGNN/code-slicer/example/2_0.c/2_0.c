digraph "Code Property Graph" {
	1 [label="1	int curl_mprintf ( const char * format , ... ) {" shape=box]
	1 -> 4 [color=blue]
	1 -> 5 [color=blue]
	2 [label="2	int retcode ;" shape=box]
	3 [label="3	va_list ap_save ;" shape=box]
	3 -> 4 [color=blue]
	3 -> 5 [color=blue]
	3 -> 6 [color=blue]
	4 [label="4	va_start ( ap_save , format ) ;" shape=box]
	5 [label="5	retcode = dprintf_formatf ( stdout , fputc , format , ap_save ) ;" shape=box]
	5 -> 7 [color=blue]
	6 [label="6	va_end ( ap_save ) ;" shape=box]
	7 [label="7	return retcode ;" shape=box]
}