cmake_minimum_required(VERSION 3.4.1)

project(fifo_test)

set (
		common_src
		fifo/fifo.c
	)
add_executable(fifo_test smaple/test_fifo.c ${common_src})

