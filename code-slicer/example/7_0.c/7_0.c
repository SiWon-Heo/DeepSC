digraph "Code Property Graph" {
	1 [label="1	int curl_msprintf ( char * buffer , const char * format , ... ) {" shape=box]
	1 -> 4 [color=blue]
	1 -> 5 [color=blue]
	1 -> 7 [color=blue]
	2 [label="2	va_list ap_save ;" shape=box]
	2 -> 4 [color=blue]
	2 -> 5 [color=blue]
	2 -> 6 [color=blue]
	3 [label="3	int retcode ;" shape=box]
	4 [label="4	va_start ( ap_save , format ) ;" shape=box]
	5 [label="5	retcode = dprintf_formatf ( & buffer , storebuffer , format , ap_save ) ;" shape=box]
	5 -> 8 [color=blue]
	6 [label="6	va_end ( ap_save ) ;" shape=box]
	7 [label="7	* buffer = 0 ;" shape=box]
	8 [label="8	return retcode ;" shape=box]
}
