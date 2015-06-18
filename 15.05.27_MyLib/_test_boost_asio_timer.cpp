/**----------------------------------------------------------------------------
 * _test_boost_asio_timer.cpp
 *-----------------------------------------------------------------------------
 * 
 *-----------------------------------------------------------------------------
 * All rights reserved by Noh,Yonghwan (fixbrain@gmail.com, unsorted@msn.com)
 *-----------------------------------------------------------------------------
 * 2014:12:27 17:44 created
**---------------------------------------------------------------------------*/
#include "stdafx.h"
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>


void asio_on_timer(const boost::system::error_code& error);
bool test_boost_asio_timer_1();

bool test_boost_asio_timer_2();

void asio_on_timer_3(const boost::system::error_code& error, boost::asio::steady_timer& timer);
void set_timer(boost::asio::steady_timer& timer);
bool test_boost_asio_timer_3();
/**
 * @brief	
 * @param	
 * @see		
 * @remarks	
 * @code		
 * @endcode	
 * @return	
**/
bool test_boost_asio_timer()
{
	test_boost_asio_timer_1();
	test_boost_asio_timer_3();
	return true;
}




/**
 * @brief	
**/
void asio_on_timer( const boost::system::error_code& error )
{
	if (!error)		// true if no error
	{
		con_info "1st timer called." con_end
	}
	else
	{
		con_err "error code = %d, msg = %s", error.value(), error.message().c_str() con_end
	}
}

/**
 * @brief	version 1 - �ݹ� �Լ� �̿�
**/
bool test_boost_asio_timer_1()
{
	boost::asio::io_service io_service;

	boost::asio::steady_timer timer(io_service);
	timer.expires_from_now( boost::chrono::seconds(1) );

	timer.async_wait( asio_on_timer );
	io_service.run();
	return true;
}


/**
 * @brief	v2 - lambda �� �̿��� Ÿ�̸�
**/
bool test_boost_asio_timer_2()
{
	boost::asio::io_service io_service;

	boost::asio::steady_timer timer(io_service);
	timer.expires_from_now( boost::chrono::seconds(1) );
	timer.async_wait( 
				[](const boost::system::error_code& error)
				{	
					if (!error)		// true if no error
					{
						con_info "%s, timer expired. ", time(NULL) con_end
					}
					else
					{
						con_err "error code = %d, msg = %s", error.value(), error.message().c_str() con_end
					}
				});
	io_service.run();

	return true;
}


/**
 * @brief	v3 - lamda + Ÿ�̸��̺�Ʈ ��� �߻��ϱ�, ���
**/
void print(const boost::system::error_code& error, boost::asio::steady_timer& timer, int& count)
{
	if (!error)
	{
		if (count < 5)
		{
			std::cout << count << "\n";
			++(count);

			timer.expires_from_now(boost::chrono::seconds(1));
			timer.async_wait(boost::bind(print, boost::asio::placeholders::error, boost::ref(timer), boost::ref(count)));		
			
			// for timer cancel
			//timer.cancel();
		}
	}
	else if (boost::asio::error::operation_aborted == error.value())
	{
		std::cout << "timer canceled." << std::endl;
	}
	
}

bool test_boost_asio_timer_3()
{
	boost::asio::io_service io_service;

	int count = 0;
	boost::asio::steady_timer timer(io_service);
	timer.async_wait(boost::bind(
								print,
								boost::asio::placeholders::error,
								boost::ref(timer),
								boost::ref(count)));
	io_service.run();
	std::cout << "count = " << count << std::endl;
	return true;
}



