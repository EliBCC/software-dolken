/*
* Written by Nicolai Dynweber Bruhn
*/

#include <modm/processing/protothread.hpp>
#include <modm/processing/timer/timeout.hpp>
#include <modm/container/queue.hpp>
#include <modm/container/linked_list.hpp>
#include <modm/container/deque.hpp>
#include <common/board.hpp>
#include <common/rs485.hpp>

#undef	MODM_LOG_LEVEL
#define	MODM_LOG_LEVEL modm::log::INFO

class Rs485ExampleThread : public modm::pt::Protothread, private modm::NestedResumable<1>
{
public:
	inline Rs485ExampleThread() : rs485()
	{};

	inline bool
    run()
	{
        PT_BEGIN();

		while (true)
		{
			MODM_LOG_INFO << "RS485 Example: Reading buffer" << modm::endl;
			uart_empty = PT_CALL(rs485.ReadToBuffer());

			if(read_queue.isNotFull() && GetCommand(read_cmd))
			{
				MODM_LOG_INFO << "RS485 Example: Added to queue" << modm::endl;
				read_queue.push(read_cmd);
			}

			if(uart_empty)
			{
				MODM_LOG_INFO << "RS485 Example: Going to write" << modm::endl;
				break;

			}
		}
		
		while (!write_queue.isEmpty())
		{
			MODM_LOG_INFO << "ADS7828 Example: Reading sensor" << modm::endl;
			write_cmd = write_queue.get();
			write_queue.pop();
			rs485.WriteCommand(write_cmd);

		}
	
        PT_END();
    }

	bool GetCommand(rs485::Command_t& cmd)
	{
		if(!read_queue.isEmpty())
		{
			cmd = read_queue.get();
			read_queue.pop();
			return true;
		}
		return false;
	}

	bool AddToWriteQueue(rs485::Command_t& cmd)
	{
		if(write_queue.isNotFull())
		{
			write_queue.push(cmd);
			return true;
		}
		return false;
	}

private:
	rs485::Rs485 rs485;

	rs485::Command_t read_cmd;
	rs485::Command_t write_cmd;
	bool uart_empty;
	modm::Queue<rs485::Command_t, modm::BoundedDeque<rs485::Command_t, 256> > read_queue;
	modm::Queue<rs485::Command_t, modm::BoundedDeque<rs485::Command_t, 256> > write_queue;
};