#ifndef UTIL
#define UTIL

#include <functional>
#include <string>
#include <any>
#include <utility>
#include "Generic/Core/Logger.h"
#include "Generic/Util/RTTI.h"

namespace Generic {
	inline void assertNoAbort(std::function<bool()> condition, std::string&& msg) {
#ifndef NDEBUG
		if (!condition()) {
			Logger::logError(msg);
		}
#endif
	}

	template <typename T, typename ...Types>
	void iterateTypeIds(std::function<void(const int& typeId)> &&f) {
		f(GRTTI::typeId<T>());
		if constexpr (sizeof...(Types) != 0)
			iterateTypeIds<Types...>(std::move(f));
	}

    class SharedRecord
    {
    public:
        void remove(const std::vector<int>& indices, int element)
        {
            SharedRecord* record = this;
            for (auto index = indices.begin(); index != indices.end(); index++)
            {
                if (record->records.find(*index) == record->records.end())
                    return;
                record = &(record->records[*index]);
            }
            if (record->elements.find(element) == record->elements.end())
                return;
            record->elements.erase(element);
        }

        void insert(const std::vector<int>& indices, std::pair<int, int> element)
        {
            SharedRecord* record = this;
            for (auto index = indices.begin(); index != indices.end(); index++)
            {
                record = &((record->records.try_emplace(*index).first)->second);
            }
            record->elements[element.first] = element.second;
        }

        void getElementsRecurse(const std::vector<int>& indices, std::unordered_map<int, int>& _elements, std::unordered_map<int, int>& exclude)
        {
            if (indices.size() == 0)
                return;
            SharedRecord* record = this;
            for (auto index = indices.begin(); index != indices.end(); index++)
            {
                if (record->records.find(*index) == record->records.end())
                    continue;
                record = &(record->records[*index]);
            }
            record->getElements(_elements, std::move(exclude));
        }

    private:
        void getElements(std::unordered_map<int, int>& _elements, std::unordered_map<int, int>&& exclude) {
            for (auto& pair : elements)
            {
                if(exclude.find(pair.first) == exclude.end())
                    _elements[pair.second];
            }
            for (auto& pair : records)
            {
                if (exclude.find(pair.first) != exclude.end())
                    continue;
                pair.second.getElements(_elements, std::move(exclude));
            }
        }

        std::unordered_map<int, int> elements;
        std::unordered_map<int, SharedRecord> records;
    };

    class Record
    {
    public:
        void insert(const std::vector<int>& indices, const int& _element)
        {
            Record* record = this;
            for (auto index = indices.begin(); index != indices.end(); index++)
            {
                record = &((record->records.try_emplace(*index).first)->second);
            }
            record->element = _element;
        }

        void remove(const std::vector<int>& indices, int element)
        {
            Record* record = this;
            for (auto index = indices.begin(); index != indices.end(); index++)
            {
                if (record->records.find(*index) == record->records.end())
                    return;
                record = &(record->records[*index]);
            }
            record->element = -1;
        }

        void getElement(const std::vector<int>& indices, int& _element, std::unordered_map<int, int>&& exclude = {})
        {
            if (indices.size() == 0)
                return;
            Record* record = this;
            for (auto index = indices.begin(); index != indices.end(); index++)
            {
                if (exclude.find(*index) != exclude.end())
                    continue;
                if (record->records.find(*index) == record->records.end())
                {
                    element = -1;
                    return;
                }
                record = &(record->records[*index]);
            }
            _element = record->element;
        }

    private:

        int element = -1;
        std::unordered_map<int, Record> records;
    };

    std::vector<int> merge(std::vector<int>& elements1, std::vector<int>& elements2);

    std::vector<int> mergeSort(const std::vector<int>& _elements);
}

#endif 