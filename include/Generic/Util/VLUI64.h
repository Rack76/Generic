#ifndef VLUI64_H
#define VLUI64_H

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <functional>
#include <cmath>

namespace Generic {
	class VLUI64
	{
	public:
		VLUI64() {

		}

		VLUI64(std::vector<int> &&signature) {
			for (int id : signature)
			{
				set(id);
			}
		}

		VLUI64(int &&bitPosition) {
			set(std::move(bitPosition));
		}

		VLUI64(std::unordered_map<unsigned int, std::uint64_t>& _words) : words(_words) {

		}

		void set(std::uint64_t bitIndex);
		void clear(std::uint64_t bitIndex);

		VLUI64 operator| (const VLUI64& r) const {
			auto wordsCopy = words;
			auto rWordsCopy = r.words;
			for (auto i = wordsCopy.begin(); i != wordsCopy.end(); i++)
			{
				wordsCopy[i->first] = wordsCopy[i->first] | rWordsCopy[i->first];
			}
			for (auto i = r.words.begin(); i != r.words.end(); i++)
			{
				if (wordsCopy.find(i->first) == wordsCopy.end())
					wordsCopy[i->first] = wordsCopy[i->first] | r.words.at(i->first);
			}

			return VLUI64(wordsCopy);
		}

		VLUI64 operator& (const VLUI64& r) const {
			auto wordsCopy = words;
			auto rWordsCopy = r.words;
			for (auto i = wordsCopy.begin(); i != wordsCopy.end();)
			{
				wordsCopy[i->first] = wordsCopy[i->first] & rWordsCopy[i->first];
				if (wordsCopy[i->first] == 0)
					i = wordsCopy.erase(i);
				else
					i++;
			}
			for (auto i = r.words.begin(); i != r.words.end();)
			{
				if (wordsCopy.find(i->first) == wordsCopy.end())
				{
					wordsCopy[i->first] = wordsCopy[i->first] & r.words.at(i->first);
					if (wordsCopy[i->first] == 0)
						i = rWordsCopy.erase(i);
					else
						i++;
				}
				else
					i++;
			}

			return VLUI64(wordsCopy);
		}

		VLUI64 operator! () const {
			auto wordsCopy = words;
			for (auto i = wordsCopy.begin(); i != wordsCopy.end(); i++)
			{
				wordsCopy[i->first] = ~wordsCopy[i->first];
			}
			return VLUI64(wordsCopy);
		}

		void iterateBits(std::function<void(const VLUI64&, int)> f) const{
			for (auto i = words.begin(); i != words.end(); i++)
			{
				std::uint64_t word = i->second;
				while (word != 0)
				{
					std::uint64_t bitPosition = floor(log2(word));
					f(*this, bitPosition + i->first * 64);
					word -= pow(2, bitPosition);
				}
			}
		}

		bool operator<(const VLUI64& r) const
		{
			auto it1 = std::prev(r.words.end());
			auto it2 = std::prev(words.end());
			if (it1->first == it2->first)
				return it1->second > it2->second;
			else if (it1->first > it2->first)
				return true;
			return false;
		}

		bool isZero() const{
			return words.size() == 0;
		}
	private:
		std::unordered_map<unsigned int, std::uint64_t> words;
	};
}

#endif