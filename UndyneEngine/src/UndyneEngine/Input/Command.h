#ifndef ICOMMAND_H
#define ICOMMAND_H
#include "..\Core\Core.h"
#include <glm\vec2.hpp>

namespace UndyneEngine
{
	class UNDYNE_API Command
	{
	public: 
		Command(const Command&) = delete;
		Command(const Command&&) = delete;
		Command& operator=(const Command&) = delete;
		Command& operator=(const Command&&) = delete;
		virtual ~Command() = default; 

		virtual void execute(float value) = 0; 
	protected: 
		Command() = default;
	};

	class UNDYNE_API StickCommand
	{
	public: 
		StickCommand(const StickCommand&) = delete;
		StickCommand(const StickCommand&&) = delete;
		StickCommand& operator=(const StickCommand&) = delete;
		StickCommand& operator=(const StickCommand&&) = delete;
		virtual ~StickCommand() = default;

		virtual void execute(glm::vec2 value) = 0;

	protected: 
		StickCommand() = default; 
	}; 

	class UNDYNE_API TriggerCommand
	{
	public: 
		TriggerCommand(const TriggerCommand&) = delete; 
		TriggerCommand(const TriggerCommand&&) = delete; 
		TriggerCommand& operator=(const TriggerCommand&) = delete; 
		TriggerCommand& operator=(const TriggerCommand&&) = delete; 
		virtual ~TriggerCommand() = default; 

		virtual void execute(float value) = 0; 
	protected: 
		TriggerCommand() = default; 
	};
}



#endif