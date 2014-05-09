/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "../demo.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * test
 */
static tb_void_t tb_demo_database_sql_test_done(tb_handle_t database, tb_char_t const* sql)
{
	// check
	tb_assert_and_check_return(database && sql);

	// done
	do
	{
		// done sql
		if (!tb_database_sql_done(database, sql))
		{
			// trace
			tb_trace_e("done %s failed, error: %s", sql, tb_state_cstr(tb_database_sql_state(database)));
			break ;
		}

		// load result
//		tb_iterator_t* result = tb_database_sql_result_load(database, tb_true);
		tb_iterator_t* result = tb_database_sql_result_load(database, tb_false);
		tb_check_break(result);

		// trace
		tb_trace_i("==============================================================================");
		tb_trace_i("row: size: %lu", tb_iterator_size(result));

		// walk result
		tb_for_all_if (tb_iterator_t*, row, result, row)
		{
			// trace
			tb_tracef_i("[row: %lu, col: size: %lu]: ", row_itor, tb_iterator_size(row));

			// walk items
			tb_for_all_if (tb_database_sql_value_t*, value, row, value)
			{
				// trace
				tb_tracet_i("[%s:%s] ", tb_database_sql_value_name(value), tb_database_sql_value_text(value));
			}

			// trace
			tb_tracet_i(__tb_newline__);
		}

		// exit result
		tb_database_sql_result_exit(database, result);

	} while (0);
}
static tb_void_t tb_demo_database_sql_test_stmt_done(tb_handle_t database, tb_char_t const* sql)
{
	// check
	tb_assert_and_check_return(database && sql);

	// done
	tb_handle_t stmt = tb_null;
	do
	{
		// init stmt
		if (!(stmt = tb_database_sql_stmt_init(database, sql)))
		{
			// trace
			tb_trace_e("stmt: init %s failed, error: %s", sql, tb_state_cstr(tb_database_sql_state(database)));
			break ;
		}

		// done stmt
		if (!tb_database_sql_stmt_done(database, stmt))
		{
			// trace
			tb_trace_e("stmt: done %s failed, error: %s", sql, tb_state_cstr(tb_database_sql_state(database)));
			break ;
		}

		// load result
//		tb_iterator_t* result = tb_database_sql_result_load(database, tb_true);
		tb_iterator_t* result = tb_database_sql_result_load(database, tb_false);
		tb_check_break(result);

		// trace
		tb_trace_i("==============================================================================");
		tb_trace_i("row: size: %lu", tb_iterator_size(result));

		// walk result
		tb_for_all_if (tb_iterator_t*, row, result, row)
		{
			// trace
			tb_tracef_i("[row: %lu, col: size: %lu]: ", row_itor, tb_iterator_size(row));

			// trace id
			tb_database_sql_value_t const* id = tb_iterator_item(row, 0);
			tb_assert_and_check_break(id);
			tb_tracet_i("[%s:%d] ", tb_database_sql_value_name(id), tb_database_sql_value_int32(id));

#ifdef TB_CONFIG_TYPE_FLOAT
			// trace fval
			tb_database_sql_value_t const* fval = tb_iterator_item(row, 1);
			tb_assert_and_check_break(fval);
			tb_tracet_i("[%s:%f] ", tb_database_sql_value_name(fval), tb_database_sql_value_float(fval));
#endif

			// trace name
			tb_database_sql_value_t const* name = tb_iterator_item(row, 2);
			tb_assert_and_check_break(name);
			tb_tracet_i("[%s:%s] ", tb_database_sql_value_name(name), tb_database_sql_value_text(name));

			// trace data
			tb_database_sql_value_t const* data = tb_iterator_item(row, 3);
			tb_assert_and_check_break(data);
			tb_tracet_i("[%s:%s] ", tb_database_sql_value_name(data), tb_database_sql_value_blob(data));

			// trace number
			tb_database_sql_value_t const* number = tb_iterator_item(row, 4);
			tb_assert_and_check_break(number);
			tb_tracet_i("[%s:%d] ", tb_database_sql_value_name(number), tb_database_sql_value_int32(number));

			// trace snumber
			tb_database_sql_value_t const* snumber = tb_iterator_item(row, 5);
			tb_assert_and_check_break(snumber);
			tb_tracet_i("[%s:%d] ", tb_database_sql_value_name(snumber), tb_database_sql_value_int32(snumber));

			// trace
			tb_tracet_i(__tb_newline__);
		}

		// exit result
		tb_database_sql_result_exit(database, result);

	} while (0);

	// exit stmt
	if (stmt) tb_database_sql_stmt_exit(database, stmt);
}
static tb_void_t tb_demo_database_sql_test_stmt_done_insert(tb_handle_t database, tb_char_t const* sql, tb_char_t const* name, tb_char_t const* data, tb_size_t number, tb_uint16_t snumber)
{
	// check
	tb_assert_and_check_return(database && sql);

	// done
	tb_handle_t stmt = tb_null;
	do
	{
		// init stmt
		if (!(stmt = tb_database_sql_stmt_init(database, sql)))
		{
			// trace
			tb_trace_e("stmt: init %s failed, error: %s", sql, tb_state_cstr(tb_database_sql_state(database)));
			break ;
		}

		// bind stmt
		tb_database_sql_value_t list[4];
		tb_database_sql_value_set_text(&list[0], name, 0);
		tb_database_sql_value_set_blob8(&list[1], (tb_byte_t const*)data, tb_strlen(data) + 1);
		tb_database_sql_value_set_int32(&list[2], number);
		tb_database_sql_value_set_int16(&list[3], snumber);
		if (!tb_database_sql_stmt_bind(database, stmt, list, tb_arrayn(list)))
		{
			// trace
			tb_trace_e("stmt: bind %s failed, error: %s", sql, tb_state_cstr(tb_database_sql_state(database)));
			break ;
		}

		// done stmt
		if (!tb_database_sql_stmt_done(database, stmt))
		{
			// trace
			tb_trace_e("stmt: done %s failed, error: %s", sql, tb_state_cstr(tb_database_sql_state(database)));
			break ;
		}

	} while (0);

	// exit stmt
	if (stmt) tb_database_sql_stmt_exit(database, stmt);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t tb_demo_database_sql_main(tb_int_t argc, tb_char_t** argv)
{
	// init database
	tb_handle_t database = tb_database_sql_init(argv[1]);
	if (database)
	{
		// open database
		if (tb_database_sql_open(database))
		{
			// done tests 
			tb_demo_database_sql_test_done(database, "drop table if exists table1");
			tb_demo_database_sql_test_done(database, "create table table1(id int, name text, number int)");
			tb_demo_database_sql_test_done(database, "insert into table1 values(1, 'name1', 52642)");
			tb_demo_database_sql_test_done(database, "insert into table1 values(2, 'name2', 57127)");
			tb_demo_database_sql_test_done(database, "insert into table1 values(3, 'name3', 9000)");
			tb_demo_database_sql_test_done(database, "insert into table1 values(4, 'name4', 29000)");
			tb_demo_database_sql_test_done(database, "insert into table1 values(5, 'name5', 350000)");
			tb_demo_database_sql_test_done(database, "insert into table1 values(6, 'name6', 21000)");
			tb_demo_database_sql_test_done(database, "insert into table1 values(7, 'name7', 21600)");
			tb_demo_database_sql_test_done(database, "select * from table1");
	
			// done tests 
			tb_demo_database_sql_test_stmt_done(database, "drop table if exists table2");
			tb_demo_database_sql_test_stmt_done(database, "create table table2(id int, fval float, name text, data blob, number int, snumber smallint)");
			tb_demo_database_sql_test_stmt_done_insert(database, "insert into table2 values(1, 3.0, ?, ?, ?, ?)", "name1", "blob_data1", 52642, 2642);
			tb_demo_database_sql_test_stmt_done_insert(database, "insert into table2 values(2, 3.1, ?, ?, ?, ?)", "name2", "blob_data2", 57127, 7127);
			tb_demo_database_sql_test_stmt_done_insert(database, "insert into table2 values(3, 3.14, ?, ?, ?, ?)", "name3", "blob_data3", 9000, 9000);
			tb_demo_database_sql_test_stmt_done_insert(database, "insert into table2 values(4, 3.1415, ?, ?, ?, ?)", "name4", "blob_data4", 29000, 9000);
			tb_demo_database_sql_test_stmt_done_insert(database, "insert into table2 values(5, -3.1, ?, ?, ?, ?)", "name5", "blob_data5", 350000, 5000);
			tb_demo_database_sql_test_stmt_done_insert(database, "insert into table2 values(6, 3.454, ?, ?, ?, ?)", "name6", "blob_data6", 21000, 1000);
			tb_demo_database_sql_test_stmt_done_insert(database, "insert into table2 values(7, 100.098, ?, ?, ?, ?)", "name7", "blob_data7", 21600, 1600);
			tb_demo_database_sql_test_stmt_done(database, "select * from table2");
		}

		// exit database
		tb_database_sql_exit(database);
	}
	return 0;
}