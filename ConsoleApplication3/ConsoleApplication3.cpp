// ConsoleApplication3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <functional>
#include <limits.h>
#include <algorithm>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <stack>
#include <queue>
#include <cassert>
#include <numeric>

struct TreeNode {
	int val;
	TreeNode* left;
	TreeNode* right;
	TreeNode() : val(0), left(nullptr), right(nullptr) {}
	TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
	TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
	
};

TreeNode* searchBST(TreeNode* root, int val) 
{
	TreeNode* cur = root;
	while (cur)
	{
		if (cur->val == val)
			return cur;
		else if (cur->val > val)
			cur = cur->left;
		else
			cur = cur->right;
	}

	return cur;
}

std::string mergeAlternately(std::string word1, std::string word2)
{
	int p1 = 0, p2 = 0;
	std::string res;
	while (p1 < word1.length() && p2 < word2.length())
	{
		res += word1[p1++];
		res += word2[p2++];
	}

	while (p1 < word1.length())
	{
		res += word1[p1++];
	}

	while (p2 < word2.length())
	{
		res += word2[p2++];
	}

	return res;
}

/*

while (a% b) > 0
	r = a % mod
	a = b
	b = 2

ret b

if b == 0
	ret a
gcb (a, a%b)

*/

int gcd(int n1, int n2)
{
	if (n2 == 0)
		return n1;
	return gcd(n2, n1 % n2);
}
	
std::string gcdOfStrings(std::string str1, std::string str2) 
{
	/*
	* Input: str1 = "ABCABC", str2 = "ABC"
		Output: "ABC"
	*/

	// concatanation of both strings must be equal so s1+s2 == s2+s1 ; otherwise s2 and s2 couldnt have a common dividor 
	if ((str1 + str2) != (str2 + str1))
		return "";
	int _gcd = gcd(str1.length(), str2.length());
	return str2.substr(0, _gcd);
}

std::vector<bool> kidsWithCandies(std::vector<int>& candies, int extraCandies) 
{
	int max = 0;
	std::vector<bool> res;
	for (int i = 0; i < candies.size(); ++i)
	{
		max = std::max(max, candies[i]);
	}

	for (int i = 0; i < candies.size(); ++i)
	{
		if (candies[i] + extraCandies >= max)
			res.push_back(true);
		else
			res.push_back(false);
	}

	return res;
}

bool canPlaceFlowers(std::vector<int>& flowerbed, int n) 
{
	// return if n or more flowers can be planed - can plant of adjacent elements are 0 ( i-1 && i +1)
	// if i == 0, prev is assumed to be free and if i is at teh end next is assumed to be free 
	int count = 0;
	for (int i = 0; i < flowerbed.size(); ++i)
	{
		// we have a free position
		if (flowerbed[i] == 0)
		{
			// prev is 0 (free) if i (current) is at the start ( i == 0) or previous (i-1) is 0 
			int prev = (i == 0 || flowerbed[i - 1] == 0) ? 0 : 1;
			// next is 0 (free) if i (current) is at the end  ( i ==  size - 1) or next (i-1) is 0 
			int next = (i == flowerbed.size() - 1 || flowerbed[i + 1] == 0) ? 0 : 1;
			if (prev == 0 && next == 0)
			{
				flowerbed[i] = 1;
				count++;
			}
		}
	}

	return count >= n;
}

bool isVowel(char c)
{
	return(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
}

std::string reverseVowels(std::string s) 
{
	std::vector<char> vowels;
	// create a vector of all vowels in the input string 
	for (auto c : s)
	{
		if (isVowel(c)) vowels.push_back(c);
	}
	// reverse the vector of vowels
	int left = 0, right = vowels.size() - 1;
	for (; left < right;)
	{
		std::swap(vowels[left], vowels[right]);
		++left; --right;
	}
	// go through original string and replace each vowels in input string with the correspondig (same position) vowel 
	// in the reversed vowels vector
	int k = 0;
	for (int i = 0; i < s.size(); i++)
	{
		if (isVowel(s[i]))
			s[i] = vowels[k++];
	}
	return s;
}

void reverseString(std::string& s, int left, int right)
{
	for (; left < right;)
	{
		std::swap(s[left], s[right]);
		++left; --right;
	}

}

/*
 String rtrim(const string &str) {
    string s(str);
	//starting at the end of the string, get an iterator to the 
	// first non space char.
	// find will return an iter to d, then erase from d to the end
	abcd_
    s.erase(
        find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
        s.end()
    );
*/

void removeLeadingAndTrailingSpaces(std::string& s)
{
	// ----abcd
	// find return position of a which is teh first non space char
	size_t found = s.find_first_not_of(" ");
	s = s.substr(found);

	// find the last character that is NOT a space
	// abcdef__
	// found will point to the first space at the end of the string
	found = s.find_last_not_of(" ");
	if (found != std::string::npos)
		s.erase(found + 1);
}


std::vector<std::string> split(const std::string& str) 
{
	std::vector<std::string> tokens;
	std::string::size_type start = 0;
	std::string::size_type end = 0;

	while ((end = str.find(" ", start)) != std::string::npos) 
	{
		tokens.push_back(str.substr(start, end - start));
		start = end + 1;
	}

	tokens.push_back(str.substr(start));

	return tokens;
}

std::string reverseWords(std::string s) 
{
	
	removeLeadingAndTrailingSpaces(s);
	
	int left = 0, right = s.length() - 1;
	reverseString(s, left, right);

	//now go through each word (when a "spcae is found" and reverse that word)
	int i = 0, j = 0;
	while (i < s.length())
	{	
		while (s[j] != '\0' && s[j] != ' ') j++;
		// found a space - we have a word now
		reverseString(s, i , j -1);
		// skip existing word
		i=j;
		// skip any further spaces after the previous word - check for eos as we may have reached the end 
		while (s[i] != '\0' && s[i] == ' ') 
		{ 
			i++; 
			//move j to position of i at the start of next word
			j++; 
		}
	}
	// rmove multiple white spaces 
	//std::unique inserts elements that are not repeated consecutively 
	// the lambda checks that both chars are the same and if one is a whitespace meaning we have 2 whitespaces


	/*
	 if (first == last)
        return last;
 
    ForwardIt result = first;
    while (++first != last)
	    // only insert in res if condition is NOT true
		// and if ++result is NOT same as first - this condition is to allow consecutive elems not to be overwritten
		//1223
		// 1 && 2 are not equal; increment res ; is res the same as first? it is so don't write 
		// increment first, so now its at the 2nd 2, first condition will fail and will short cicuit so res wont be incremented
		// and will remain pointing at the 1st 2
		// now move first to 3, 3 is not the same as 2, so execute res++, now res is at the 2nd 2 and now we write 3 in that position
        if (!p(*result, *first) && ++result != first)
            *result = std::move(*first);
 
    return ++result;
	*/
	auto it = std::unique(s.begin(), s.end(),
		[](char const& lhs, char const& rhs) {
			return (lhs == rhs) && (lhs == ' ');
		});
	s.erase(it, s.end());

	return s;
}


void moveZeroes(std::vector<int>& nums) 
{

	/*
		Input: nums = [0,1,0,3,12]
		Output: [1,3,12,0,0]
	*/
	int j = 0;
	// the non zero elements will be placed at the start using index j
	// so at the end of loop j will point to the last non xero elements
	// 1, 3, 12, here
	// fill the rest of the vector wih zeros starting at j
	for(int i = 0; i < nums.size(); i++)
	{
		if (nums[i] != 0 )
		{
			nums[j] = nums[i];
			j++;
		}
	}

	while (j < nums.size())
		nums[j++] = 0;

}

double findMaxAverage(std::vector<int>& nums, int k) 
{
	/*
	Find a contiguous subarray whose length is equal to k that has the maximum average value and return this value. 

	Input: nums = [1,12,-5,-6,50,3], k = 4
	Output: 12.75000
	Explanation: Maximum average is (12 - 5 - 6 + 50) / 4 = 51 / 4 = 12.75
	
	*/

	double max = LLONG_MIN, sum = 0;
	int start = 0;
	for (int end = 0; end < nums.size(); end++)
	{
		sum += nums[end];
		// have we counted k elements
		if (end - start == k - 1)
		{
			// now move window to the right and subtract from sum the removed element
			double average = sum / k;
			max = std::max(max, average);
			sum -= nums[start];
			start++;
		}
	}
	return max;
}

int largestAltitude(std::vector<int>& gain) 
{

	// array gains[] where gain[i] is the gain of altitude between i and i + 1. 
	// altitide[0] is 0

	/*
		Input: gain = [-5,1,5,0,-7]
		Output: 1
		Explanation: The altitudes are [ -5,1,5,0,-7]. The highest is 1.
		altitude = 0 + -5 == -5; max(-5, 0) == 0
		altitude = -5 + 1 == -4;   max(-4, 0) == 0
		altitude = -4 + 5 == 1;   max(1, 0) == 1
		altitude = 1 + 0 == 1;   max(, 1) == 1
		altitude = 1 + -7 == -6;   max(-6, 1) == 1

	*/

	int highest = 0;
	int altitude = 0;

	for (int i = 0; i < gain.size(); i++)
	{
		altitude = altitude + gain[i];
		highest = std::max(altitude, highest);

	}

	return highest;

}

std::vector<std::vector<int>> findDifference(std::vector<int>& nums1, std::vector<int>& nums2) 
{
	std::vector<std::vector<int>> ans;

	/*
	std::unordered_set<int> set1(nums1.begin(), nums1.end());
	std::unordered_set<int> set2;

	for (int i = 0; i < nums2.size(); ++i)
	{
		set1.erase(nums2[i]);
		set2.insert(nums2[i]);
	}

	std::vector<int> ans1(set1.begin(), set1.end());
	ans.push_back(ans1);

	for (int i = 0; i < nums1.size(); ++i)
	{
		set2.erase(nums1[i]);
	}
	std::vector<int> ans2(set2.begin(), set2.end());
	ans.push_back(ans2);

	*/

	// 2nd solution 

	std::unordered_set<int> set2(nums2.begin(), nums2.end());
	std::unordered_set<int> set1(nums1.begin(), nums1.end());
	std::vector<int> ans1;
	for (auto i: set1)
	{
		// are any of the elems in 1 present in set 2?
		if (set2.count(i) == 0)
			ans1.push_back(i);
	}
	ans.push_back(ans1);

	
	std::vector<int> ans2;
	for (auto i : set2)
	{
		// are any of the elems in 2 present in set 1?
		if (set1.count(i) == 0)
			ans2.push_back(i);
	}
	ans.push_back(ans2);

	return ans;
}

bool uniqueOccurrences(std::vector<int>& arr) 
{

	/*
		Input: arr = [1,2,2,1,1,3]
		Output: true
		Explanation: The value 1 has 3 occurrences, 2 has 2 and 3 has 1. No two values have the same number of occurrences.
	*/

	//m = {(elemnetA, #of occurence), (elemnetB, #of occurence)....}

	std::map<int, int> m;
	for (int i = 0; i < arr.size(); i++)
	{
		m[arr[i]]++;
	}

	// insert the count for each element a into a set (in effect set will contain only unique frequencies)
	// // so if the map contains (3, 3, 4, 4), the set will only contain 2.
	// this means the set will conatin only unique values so if the number of elements in 
	// the set is the same as the number of elements in the map the condition will be met 
	std::unordered_set<int> set;
	std::for_each(m.begin(), m.end(), [&set](const std::pair<int, int>& p) { set.insert(p.second); });

	return set.size() == m.size();
}
int compress(std::vector<char>& chars) 
{
	/*
		Input: chars = ["a", "a", "b", "b", "c", "c", "c"]
		Output : Return 6, and the first 6 characters of the input array should be : ["a", "2", "b", "2", "c", "3"]
		Explanation : The groups are "aa", "bb", and "ccc".This compresses to "a2b2c3".
	*/
	
	//aabbccc ===> a2b2c3 ret is 6
	// 
	// 
	// '~' is added  as a marker to the end of the vector otherwise the last set of 
	// chars will not be added to output.
	// for example: cccc: when we loop back to check the next char having processed the previous 3,
	// we will have exceeded the length of the vector so loop will exist.
	// if we add a terminator, we can process all chars and the input will anyway be overwritten 
	// by teh and vector
	chars.push_back('~');

	std::vector<char> ans;
	int total = 1;
	// go through vector and compare next char with previous
	for (int i = 1; i < chars.size(); i++)
	{
		if(chars[i] == chars[i-1])
		{
			++total;
		}
		else 
		{
			// the currrent char i is not the same as last, so add previous char to answer and add the number of 
			// occurances which is stored in total 
			ans.push_back(chars[i-1]);
			if (total != 1)
			{
				// convert from int to string
				std::string len = std::to_string(total);
				// add each char of the string representation of total to ans: total == 12 will result in 1 and 2 being added
				for (auto c : len) ans.push_back(c);
				total = 1;
			}
		}
	}
	chars = ans;
	return ans.size();
}

/*
* std::array<int, 10> s {5, 7, 4, 2, 8, 6, 1, 9, 0, 3};
 
    auto print = [&s](std::string_view const rem)
    {
        for (auto a : s)
            std::cout << a << ' ';
        std::cout << ": " << rem << '\n';
    };
 
    std::sort(s.begin(), s.end());
	// param rem bound to const string literal
	//[&s] captures variable array<int> s
    print("sorted with the default operator<");
*/

std::vector<int> merge(const std::vector<int>& current_interval, const std::vector<int>& next_interval)
{
	return std::vector<int>(current_interval[0], std::max(current_interval[1], next_interval[1]));
}

bool can_merge(const std::vector<int>& current_interval, const std::vector<int>& next_interval)
{
	// if end of current is larger than start of next (intervals are sorted so start of next will be less than start of current )
	return current_interval[1] >= next_interval[0];
}

int eraseOverlapIntervals(std::vector<std::vector<int>>& intervals) 
{
	/*
		Input: intervals = [[1,2],[2,3],[3,4],[1,3]]
		Output: 1
		Explanation: [1,3] can be removed and the rest of the intervals are non-overlapping.
		no overlapping meaning the ranges dont overlap
		example: [1, 3] above overlaps with [1,2] and [2,3] 

		[[1,2],[1,3],[2,3],[3,4]]
	*/

	if (intervals.size() < 2) return 0;
	
	std::sort(intervals.begin(), intervals.end(), [](const std::vector<int>& a, const std::vector<int>& b) { return a[0] < b[0];});
	std::vector<int> current_interval = intervals[0];
	int count = 0;
	for (int i = 1; i < intervals.size(); i++)
	{
		if (can_merge(current_interval, intervals[i]))
		{
			merge(current_interval, intervals[i]);
			++count;
		}
		else
		{
			current_interval = intervals[i];
		}
	}

	return count;

	/*

	// alternative solution
	// sort by end
	// sorting by end means interval with the larger end will be removed 
	// this allow more intervals to fit in 
	// 1-------------5
	//    2-------------7
	// if we removed interval [1,5], other intervals to no overlap must start at 7
	// if however we removed interval [2,7], other intervals to not overlap must start at  5, 6 or 7 which is alarger range than 7
	if (intervals.size() < 2) return 0;
	std::sort(intervals.begin(), intervals.end(), [](const std::vector<int>& a, const std::vector<int>& b) { return a[1] < b[1];});

	int amountRemoved = 0, previousEnd = intervals[0][1];
	for (int i = 1; i < intervals.size(); i++)
	{
		// is there an overlap? start of current less than end of last
		if (intervals[i][0] < previousEnd)
		{
			amountRemoved++;
		}
		else
		{
			previousEnd= intervals[i][1];
		}
	}

	return amountRemoved;
	
	*/
}

int singleNumber(std::vector<int>& nums) 
{
	int result = 0;
	for (int i = 0; i < nums.size(); i++)
	{
		result ^= nums[i];
	}
	return result;
}

/*
int guessNumber(int n) 
{
	int left = 1, right = n;
	
	while (left <= right)
	{
		int middle = left + right / 2;
		int result = guess(middle);
		if (result == 0)
		{
			return middle;
		}
		else if (result < 0)
		{
			right = middle - 1;
		}
		else
		{
			left = middle + 1;
		}
	}
	return -1;
}
*/


class RecentCounter {
	std::queue<int> q;
public:
	/*
	* Adds a new request at time t, where t represents some time in milliseconds, and returns the number 
	  of requests that has happened in the past 3000 milliseconds (including the new request). Specifically, return 
	  the number of requests that have happened in the inclusive range [t - 3000, t].
	*/
	int ping(int t) 
	{
		//add new time 
		q.push(t);
		while (q.front() < t - 3000)
		{
			// keep poping items off the queue that did NOT happen in the last 3000 millisecs (so item is less than t - 3000)
			// example q[3000, 6000, 10000, 105000] t is 11000
			// is 3000 < 11000 - 3000 yes so remove it
			// is 6000 < 11000 - 3000 yes so remove it
			// is 10000 < 11000 - 3000 NO so keep it and exit loop - returning number of elements left on the queue
			// as anything inserted after 10000 will also satisfy the condition of NOt being less than t - 3000
			q.pop();
		}
		return q.size();
	}
};



int maxProfit(std::vector<int>& prices)
{
	int maxProfit = 0;
	int minPrice = INT_MAX;
	for (int i = 0; i < prices.size(); ++i)
	{
		minPrice = std::min(minPrice, prices[i]);
		maxProfit = std::max(maxProfit, prices[i] - minPrice);
	}
	return maxProfit;
}

int mmaxProfit(std::vector<int>& prices)
{
	int profit = 0;
	for (int i = 1; i < prices.size(); ++i)
	{
		if (prices[i] > prices[i - 1])
			profit += (prices[i] - prices[i - 1]);
	}
	return profit;

}

int maxProfit(std::vector<int>& prices, int fee) 
{

	// the maximum profit that can be acheved by buying and sellig 
	int profit = 0;
	int buyPrice = prices[0];
	for (int i = 1; i < prices.size(); ++i)
	{
		// profit is the max between previous profit and new profit (price[i]/sell price minus the buy price minus the fee)
		profit = std::max(profit, prices[i] - buyPrice - fee);
		// min between previous buy price and new buy price/prices[i] minus the profit (use profit to pay for price)
		buyPrice = std::min(buyPrice, prices[i] - profit);
	}
	return profit;
}

int maxSubArray(std::vector<int>& nums) 
{
	int max = nums[0], curSum = 0;
	for (int i = 0; i < nums.size(); ++i)
	{
		if (curSum < 0)
			// reset if accumilator is negative
			curSum = 0;
		curSum += nums[i];
		max = std::max(curSum, max);
	}

	return max;
}

class StockSpanner {
	/*
		* maximum number of consecutive days (starting from that day and going backward) for which the stock price was less
		than or equal to the price of that day.
		*/
	std::vector<int> _spans;
	std::vector<int> _prices;
public:
	StockSpanner() 
	{
		_spans = {1, 1,1,2,1,4};
		_prices = { 100, 80, 60, 70, 60, 75};

	}
	int next(int price) 
	{
		// no need to compare all previous elements until we reach one that is greater than current price.
		// instead we get the previous span and and if its less or equal than current one, we get its span and skip to 
		// that element so to element with index: index - span which saves doing unnecessary comparisons 
		int index = _prices.size() - 1;
		// start at the end of the vector containing all seen prices
		while (index >=0 && _prices[index] <= price)
		{
			// get the span for this price
			int span = _spans[index];
			// the index to go to is index minus span where span indciates how many prices satisfy the condition
			// of days where price is less or equal to current proce 
			//skip to index - span
			index = index - span;
		}

		_prices.push_back(price);
		// span for current price is the size of array - 1 and take away the value of the 
		// index (index is the element where condition is not met) 
		int span = _prices.size() - 1 - index;
		_spans.push_back(span);
		return span;
	}
};
class ss {
public:
	ss(const char* s)
	{
		std::cout << "created string" << std::endl;
		_data = new char[5];
		memcpy(_data, s, 5);
	}
	~ss()
	{
		delete[] _data;
		std::cout << "deleted string" << std::endl;
	}
	ss(const ss& s)
	{
		std::cout << "copied string" << std::endl;
		_data = new char[5];
		memcpy(_data, s._data, 5);
	}
	ss(ss&& s)
	{
		std::cout << "string moved" << std::endl;
		// so no creating of new buffer.....simply re-use the temporary buffer (steal it)
		// NO deep copy - do a shallow copy...
		_data = s._data;
		// the memory pointed to by the rvalue will deleted when destructor is called when temporary dies - we must null the rvalue so
		// // the delete has no impact. Without nulling it, the delete will reclaim the memory which is now owned by 
		// _data causing a crash. 
		s._data = 0;
	}
	
	char* _data;

};

class Entity {
public:
	Entity(const ss& name) : _name(name)
	{
	}
	// we must cast name to ravlue to get it to use the && version of the ss constructor - otherwise
	// it will use the vannila copy constructor in ss ( named objects are treated like lValues)
	Entity(ss&& name) : _name(std::move(name))
	{
	}
	~Entity()
	{
		std::cout << "Entity destroyed" << std::endl;
	}
private:
	ss _name;
};

std::vector<int> topKFrequent(std::vector<int>& nums, int k)
{
	/*
		Input: nums = [1,1,1,2,2,3], k = 2
		Output: [1,2]
	*/

	std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>> > min_heap;
	std::map<int, int> count;

	for (int i = 0; i < nums.size(); ++i)
		count[nums[i]] += 1;

	// top k elements
	for (auto iter = count.begin(); iter != count.end(); ++iter) {
		// the map[key, value] : Key is the element and value is the frequency - so heap is sorted based on frequency (second)
		min_heap.push(std::make_pair(iter->second, iter->first));
		if (min_heap.size() > k)
			min_heap.pop();//pop smallest frequency
	}

	std::vector<int> res(k, 0);
	int index = 0;
	while (!min_heap.empty()) {
		// add top most element (one wih the least frequencies) - one with most frequencies to result - topr().second being the element itself.
		res[index++] = min_heap.top().second;
		min_heap.pop();
	}
	return res;
}

// n is num of ships
// N array of weights on each ship
int minimumMoves(int n, int N[])
{
	int arrSize = sizeof(N);
	int totalWeight = std::accumulate(N, N + arrSize, 0);
	if (totalWeight % n != 0)
		return -1;

	int leftWeight = 0;
	int rightWeight = totalWeight;
	int minMoves = INT_MAX;

	for (int i = 0; i < arrSize; ++i) {
		leftWeight += N[i];
		rightWeight -= N[i];
		int currentMoves = std::abs(leftWeight - rightWeight);
		if (currentMoves < minMoves) {
			minMoves = currentMoves;
		}
	}
	return minMoves;
}

class BigData // inefficient to copy
{
	int data[1024]; /* some raw data */
public:
	explicit BigData(int i = 0) { data[0] = i; /* ... */ }
	operator int() const { return data[0]; }
	BigData& operator=(int i) 
	{ 
		data[0] = i; return *this; 
	}
	/* ... */
};


int main()
{
	std::list<BigData> l(10);
	std::iota(l.begin(), l.end(), -4);

	std::vector<std::list<BigData>::iterator> vvvv(l.size());
	// populate vector with iterators to list l:
	// l.begin(), ++l and so on 
	std::iota(vvvv.begin(), vvvv.end(), l.begin());
	std::cout << *(l.begin()) << std::endl;
	// Vector of iterators (to original data) is used to avoid expensive copying,
	// and because std::shuffle (below) cannot be applied to a std::list directly.

	//std::shuffle(v.begin(), v.end(), std::mt19937 {std::random_device{}()});

	for (auto const& n : vvvv) std::cout << *n << ' ';
	std::cout << '\n';

	int Arr[] = { 10, 20, 30, 40 };
	minimumMoves(4, Arr);

	Entity ee("TEST1");
	return 0;

	StockSpanner ss;
	ss.next(85);

	longestCommonSubsequence("abcde", "ace");

	std::vector<int> o = { 1,1,1,0,0,0,1,1,1,1,0 };
	longestOnes(o, 2);
	
	std::vector<std::vector<int>> vvv = { {1, 2 }, { 1, 3 }, { 2, 3 }, { 3, 4 } };


	eraseOverlapIntervals(vvv);

	std::vector<char> vv = { 'a','a','b','b','c','c','c'};
	compress(vv);
	
	std::vector<int> gains = { -5,1,5,0,-7 };
	largestAltitude(gains);

	std::vector<int> v = { -1 };
	findMaxAverage(v, 1);
	//std::string s("the sky is blue");
	std::string s = " hello world  ";
	reverseWords(s);

	std::string res = mergeAlternately("ab", "pqrs");
    
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
