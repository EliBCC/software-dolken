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
	{

	};

	inline bool
    run()
	{
        PT_BEGIN();

		// Loop for receiving
		while (true)
		{

			// Read to ringbuffer
			MODM_LOG_INFO << "RS485 Example: Reading buffer" << modm::endl;
			uart_empty = PT_CALL(rs485.ReadToBuffer());

			//If queue of read commands is not full and if ringbuffer contains a command
			if(read_queue.isNotFull() && PT_CALL(rs485.GetCommand(read_cmd)))
			{
				MODM_LOG_INFO << "RS485 Example: Added to queue" << modm::endl;
				read_queue.push(read_cmd);
			}
			
			// if there are no more bytes to receive
			if(uart_empty)
			{
				MODM_LOG_INFO << "RS485 Example: Going to write" << modm::endl;
				break;

			}
		}
		
		// loop for transmitting - if there is commands to be transmitted
		while (!write_queue.isEmpty())
		{
			MODM_LOG_INFO << "ADS7828 Example: Reading sensor" << modm::endl;
			
			// Pop command from write queue
			write_cmd = write_queue.get();
			write_queue.pop();

			//Write command
			PT_CALL(rs485.WriteCommand(write_cmd));

		}
	
        PT_END();
    }

	bool GetCommand(rs485::Command_t& cmd)
	{
		// if queue contains elements then return else return false
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
		// if write queue is not full then add command to write queue.
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