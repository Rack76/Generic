#include <vector>

namespace Gen
{
    std::vector<int> merge(std::vector<int>& elements1, std::vector<int>& elements2)
    {
        int counter1 = 0;
        int counter2 = 0;

        std::vector<int> returnValue;

        if (elements1.back() < elements2.front())
        {
            elements1.insert(elements1.end(), elements2.begin(), elements2.end());
            return elements1;
        }
        else if (elements2.back() < elements1.front())
        {
            elements2.insert(elements2.end(), elements1.begin(), elements1.end());
            return elements2;
        }

        while (counter1 != elements1.size() && counter2 != elements2.size())
        {
            if (elements1[counter1] < elements2[counter2])
            {
                returnValue.push_back(elements1[counter1]);
                counter1++;
            }
            else
            {
                returnValue.push_back(elements2[counter2]);
                counter2++;
            }
        }

        if (counter1 == elements1.size())
            for (int i = counter2; i < elements2.size(); i++)
            {
                returnValue.push_back(elements2[counter2]);
                counter2++;
            }
        else
            for (int i = counter1; i < elements1.size(); i++)
            {
                returnValue.push_back(elements1[counter1]);
                counter1++;
            }

        return returnValue;
    }

    std::vector<int> mergeSort(const std::vector<int>&& _elements)
    {
        std::vector<int> elements = _elements;

        if (elements.size() == 2)
        {
            if (elements[0] > elements[1])
            {
                int temp = elements[0];
                elements[0] = elements[1];
                elements[1] = temp;
            }
            return elements;
        }
        else if (elements.size() == 1)
            return elements;

        int mid = elements.size() / 2;

        std::vector<int> elements1;
        for (int i = 0; i < mid; i++)
        {
            elements1.push_back(elements[i]);
        }

        std::vector<int> elements2;
        for (int i = mid; i < elements.size(); i++)
        {
            elements2.push_back(elements[i]);
        }

        elements1 = mergeSort(std::move(elements1));
        elements2 = mergeSort(std::move(elements2));

        return merge(elements1, elements2);
    }
}