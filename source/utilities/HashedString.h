#include <functional>

namespace epsilon
{
	// A simple class to wrap string hashing 
	// which tries to not get in the way
	class HashedString
	{
	public:
		typedef std::hash<std::string> StringHash;

		HashedString() {}
		HashedString(std::string s)
		{
			strVal = s;
			hashVal = hasher(strVal);
		}

		// Assignment from string
		void operator=(std::string newString)
		{
			strVal = newString;
			hashVal = hasher(strVal);
		}

		// Comparison uses the hash
		bool operator==(const HashedString & other) const
		{
			return hashVal == other.hashVal;
		}

		// Comparison to a standard string is slower
		bool operator==(const std::string & other) const
		{
			return hashVal == hasher(other);
		}

		std::string GetString() { return strVal; }
		std::size_t GetHash() { return hashVal; }

		// Utility function to generate a hash from a string
		static std::size_t Hash(std::string str)
		{
			StringHash hasher;
			return hasher(str);
		}

	private:

		std::string strVal;
		std::size_t hashVal;
		StringHash  hasher;
	};
}