#pragma once
#include <string>
#include <unordered_map>
#include <cstdint>

class SymbolTable
{
public:
	void setVariable(const std::string& name, int value)
	{
		table[name] = value;
	}

	int getVariable(const std::string& name)
	{
		if (table.contains(name))
		{
			return table[name];
		}
		return 0; // Default or error value
	}

	bool hasVariable(const std::string& name) const
	{
		return table.contains(name);
	}

private:
	std::unordered_map<std::string, int> table;
};
