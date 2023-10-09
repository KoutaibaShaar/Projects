// ConsoleApplication2.cpp : This file contains the 'main' function. Program execution begins and ends there.
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
#include <set>
#include <queue>
#include <algorithm>
#include <cassert>


void merge(std::vector<int>& nums1, int m, std::vector<int>& nums2, int n) 
{
    // i will point to end of result array (nums1) which is the next write position 
    int p1 = m - 1, p2 = n - 1, i = m + n - 1;
    // start from the end of both sorted vectors and compare - both are sorted so largest of the 2 largest has to go at the end 
    // write the larger element of the 2 to position i
    // 1 2 3 0 0 0
    // 2 5 6
    // starting at the end, 6 is larger than 3, write 3 at position i (5)
    // decrement p2 so it now points to 5 and leave p1 pointing to 3  
    while (p2 >= 0)
    {
        if (p1 >= 0 && nums1[p1] > nums2[p2])
        {
            // write the larger element num1 and go to next element in num1 leaving pointer 
            // to smaller elemnt in num2 at the same postion 
            nums1[i--] = nums1[p1--];
        }
        else
        {
            nums1[i--] = nums2[p2--];
        }
    }
}

void reverseString(std::vector<char>& s) 
{
    if (s.size() == 0) return;
    int begin = 0, end = s.size() - 1;
    while (begin < end)
    {
        std::swap(s[begin], s[end]);
       ++begin;  --end;
    }
}

int reverse(int x)
{
    /* 
        const int num_of_digits = floor(log(n));
        // of number is 1234, the mask is 1000 so that divinding by mask gives the msd (1) and modding by mask removes msd 
        int msd_mask = pow(10, num_of_digits - 1);
        
        remove msd 
        n = n % msd_mask
        get msd 
        n = n / msd_mask
        
        remove lsd
        n = n / 10;
        get lsd
        n = n % 10;
    */

    long result = 0;
    while (x)
    {
        //get the LSD and make it the MSD in result (to reverse number) 
        result = (result * 10) + (x % 10);
        x /= 10;
    }
    return (result < INT_MIN) || (result > INT_MAX) ? 0 : result;
}

uint32_t reverseBits(uint32_t n) 
{
    unsigned int result = 0, bit = 0;
    for(int i =0; i < 32; ++i)
    {
        bit = n & 1;
        // place bit in the right place (right shift it by number of positions and save in result)
        result|=(bit<<=(31-i));
        n >>= 1;
    }
    return result;

    /*
        n = ((n & 0xffff0000) >> 16) | ((n & 0x0000ffff) << 16)// swap 16 bits at position 
                                                                  //11111111111111110000000000000000 
                                                                  // with ones at 00000000000000001111111111111111
                
        n = ((n & 0xff00ff00) >> 8) | ((n & 0x00ff00ff) << 8) // swap 8 bits at position 1111111100000000 
                                                                //with ones at 0000000011111111
         
        n = ((n & 0xf0f0f0f0) >> 4) | ((n & 0x0f0f0f0f) << 4)//within a byte swap 4 bits at position 11110000 with ones at 00001111
               
        
        n = ((n & 0xcccccccc) >> 2) | ((n & 0x33333333) << 2)//within 1/2 a byte swap 2 bits at position 1100 with ones at 0011
               
        
        n = ((n & 0xaaaaaaaa) >> 1) | ((n & 0x55555555) << 1)//within 1/2 a byte swap 1 bit at position 1010 with ones at 0101
    */
}

std::vector<int> countBits(int n)
{
    /*
        Input: n = 5
        Output: [0,1,1,2,1,2]
        Explanation:
            0 --> 0
            1 --> 1
            2 --> 10
            3 --> 11
            4 --> 100
            5 --> 101
    */

    // for even numbers: the number of bits in number x is the same as the number of bits in x/2 (x>>1)
    // for odd numbers: the number of bits in number x is the same as the number of bits in x/2 (x>>1) plus 1
    // so for odd number we will add one: number % 2 - if even number % 2 is 0 for odd its 1 
    std::vector<int> res;
    res.push_back(0);
    for (int i = 1; i <= n; ++i)
    {
        res.push_back(res[i / 2] + i % 2);
    }
    return res;
}


/*
* 
*       remove lsd
        n = n / 10;
        get lsd
        n = n % 10;
* 
itoa:
    do {  
        s[i++] = n % 10 + '0'; 
    } while ((n /= 10) > 0);   

*/

int ati(const char* a)
{
    int n = 0;
    for (int i = 0; a[i] >= '0' && a[i] <= '9'; ++i)
        n = (10 * n) + (a[i] - '0');

    return n;
}
int myAtoi(std::string s)
{
    if (s.empty()) return 0;

    long int n = 0;
    int i = 0;

    while (i < s.length() && s[i] == ' ') ++i;

    bool negative = (s[i] == '-');
    if (negative) ++i;
    else if (s[i] == '+') ++i;

    for (; s[i] != '\0' && s[i] >= '0' && s[i] <= '9'; ++i)
    {
        n = (10 * n);
        if (n <= INT_MIN || n >= INT_MAX)
            break;
        n = n + (s[i] - '0');
    }

    if (negative)
    {
        n = (-1) * n;
    }

    if (n <= INT_MIN) return INT_MIN;

    if (n >= INT_MAX) return INT_MAX;


    return n;
}


std::vector<std::vector<int>> permute(std::vector<int>& nums) 
{

    // P(1,2 ,3)
    //  [1], P(2,3)
    //      [2,3]   [3, 2]
    //          [2][3] [3][2]

    if (nums.size() <= 1) return{ nums };

    std::vector<std::vector<int>> result;

    for (int i = 0; i < nums.size(); ++i)
    {
        std::vector<int> v(nums.begin(), nums.end());
        // remove first element in the list
        v.erase(v.begin() + i);
        // recure with remaining elements 
        auto res = permute(v);
        for (int j = 0; j < res.size(); ++j)
        {
            //get list of vectors and re-add removed element to the front of each set of vectors 
            std::vector<int> _v = res[j];
            _v.insert(_v.begin(), nums[i]);
            result.push_back(_v);
        }
    }

    return result;

}

int findDuplicate(std::vector<int>& nums) 
{
    // modify the array so each elemnt will be stored at the index based on its elemVal % 5 (5 is the range so index 0 to n-1)
    // The idea is elements which ocur only once will be with a range of 1 + n to n + n
    int n = nums.size();
    int max_idx = 0;
    int curr_max = 0;
    for (int i = 0; i < n; ++i)
    {
        // must mod the elem in case its been added before 
        int id = nums[i] % n;
        // add n to location id - for duplicates n will be added to location id (id being the elemnt itself) at least 2 times 
        nums[id] += n;
    }

    for (int i = 0; i < n; ++i)
    {
        if (nums[i] > curr_max)
        {
            curr_max = nums[i];
            max_idx = i;
        }
        // revert location back to its original value 
        nums[i] %= n;
    }

    return max_idx;
}


bool isPalindrome(std::string s) 
{
    if (s.empty()) return true;

    int i = 0;
    std::string converted;
    //remove spaces, commas and apostrophies and convert any upper case chars to lower case
    for (i = 0; i < s.size(); ++i)
    {
        if (!isalnum(s[i]))
        {
            continue;
        }

        if ((s[i] >= 'A') && (s[i] <= 'Z'))
            converted.push_back(s[i] - ('A' - 'a'));
        else
            converted.push_back(s[i]);
    }

    bool isPalindrome = true;
    int left = 0, right = converted.size() - 1;
    for (; left < right;)
    {
        if (converted[left] != converted[right])
        {
            isPalindrome = false;
            break;
        }

        ++left; --right;
    }

    return isPalindrome;
}

int divide(int dividend, int divisor) 
{

    if (dividend == 1 << 31 && divisor == -1) return INT_MAX;
    // if both are the same sign (+ and +) or (- and -)
    // example if both (dividend >= 0) and (divisor >=0 ) are false as both are negative also return true 
    bool sign = (dividend >= 0) == (divisor >=0 ) ? true : false;
    long _dividend = abs(dividend), _divisor = abs(divisor);
    int quotient = 0;


    // 63 / 2, 4, 8, 16, 32 accum 
    // 63 -> 31 - > 15 -> 7
    // Olog(n)

    // figure out the max accum up front instead of doing it log(N) times
    // outer
    //  10 - 3 >= 0
    //      _dividend - (_divisor << 1 << count) evaluates 10 - 6 as (_divisor << 1 << count) will be 6 
    // 
    while (_dividend - _divisor >= 0)
    {
        int count = 0;
        while (_dividend - (_divisor << 1 << count) >= 0) count++;
        quotient += 1<<count;
        _dividend -= _divisor<<count;
    }
    return sign ? quotient : -quotient;
}



/*
// roll 6-sided dice 20 times
    for (int n=0; n != 20; ++n) {
        int x = 7;
        // instead of mode 7 on the result of rand - keep calling rand() as long as the result is greater than 6 
        // 
        while(x > 6)
            x = 1 + std::rand()/((RAND_MAX + 1u)/6);

         // its biased because twice as many 0's are produced for example:
         // (0 and 6) mod 6 will return 0  

         to discard values that would produce non-uniform duplicates. 
         You calculate the largest multiple of 6 that's less than or equal to RAND_MAX, and whenever rand() 
         returns a value that's greater than or equal to that multiple you reject it and 
         call `rand() again, as many times a needed.

So:

int max = 6 * ((RAND_MAX + 1u) / 6)
int value = rand();
while (value >= max)
    value = rand();

    */

std::vector<int> shuffle(const std::vector<int>& nums) 
{
    std::vector<int> a = nums;
    for (int i = a.size() - 1; i >= 0; i--)
    {
        int index = std::rand() % (i + 1);
        int temp = a[i];
        a[i] = a[index];
        a[index] = temp;
    }    
    return a;
}

int longestOnes(std::vector<int>& nums, int k)
{
    int n = nums.size();
    int window_start = 0, window_end = 0;

    for (; window_end < n; window_end++)
    {
        if (nums[window_end] == 0) k--;
        if (k < 0)
        {
            // if we hit 0's at start of window, we increment k
            if (nums[window_start] == 0)
            {
                k++;
            }
            window_start++;
        }
    }
    return window_end - window_start;
}

int longestCommonSubsequence(std::string text1, std::string text2)
{
    /*
        Input: text1 = "abcde", text2 = "ace"
        Output: 3
        Explanation: The longest common subsequence is "ace" and its length is 3.
    */
    std::vector<std::vector<int>> dp(text1.length() + 1, std::vector<int>(text2.length() + 1, 0));
    // start at the bottom right of the grid
    for (int i = text1.length() - 1; i >= 0; --i)
    {
        for (int j = text2.length() - 1; j >= 0; --j)
        {
            if (text1[i] == text2[j])
            {
                // if chars match, go diagonal
                dp[i][j] = 1 + dp[i + 1][j + 1];
            }
            else
            {
                // select larger value between right and below positions
                // so
                dp[i][j] = std::max(dp[i][j + 1], dp[i + 1][j]);
            }
        }
    }

    return dp[0][0];

}

int longestConsecutive(std::vector<int>& nums) 
{
    std::set<int> s(nums.begin(), nums.end());
    int sequence = 0;

    for (int n : nums)
    {
        // vector: 100, 4, 200, 3 , 2 ,1
        // set: 1, 2, 3 ,4, 100, 200
        // 
        // if n -1 exists, we are not at the start of a new sequence so go to next elem in the input vector
        if (s.find(n-1) != s.end()) continue;
        int cur_seq = 1;
        // start of a new sequence as n -1 does NOT exit in set
        // whilst there exists a value n + 1 in the set (set is sorted and doesn't have dupes), increase sequence counter 
        // so when we hit 1 in the above example, we start a new sequence and try to find n + 1 which is 2 and then 3 and 4
        while (s.find(n + 1) != s.end())
        {
            cur_seq++;
            n++;
        }
        sequence = std::max(sequence, cur_seq);
    }
    return sequence;
}

int longestSubstring(std::string s, int k) 
{
// Longest Substring with At Least K Repeating Characters
// Input: s = "ababbc", k = 2
// Output : 5
// Explanation : The longest substring is "ababb", as 'a' is repeated 2 times and 'b' is repeated 3 times.

    return 0;
}


std::string longestRecurringSubstring(const std::string& str) 
{
    // aabcaabdaab
    // aab repeated 3 times
    // aaaaa
    // 4 because: a is repeated 5 times. aa 4 times. aaa 3 times. aaaa 2 times
    // Input: s = "banana"
    // Output: "ana"
    int n = str.length();
    unsigned int maxLength = 0;
    /*
    // brute force
    std::set<std::string> s;
    for (int i = 0; i < n; ++i)
    {
        for (int j = i; j < n ; ++j)
        {
            std::string sub = str.substr(i, j + 1);
            std::cout << sub << std::endl;
            // must be a repeating sub string so if we havent seen it, we add it and next time
            // we see it, we check its length 
            if (s.find(sub) == s.end())
            {
                s.insert(sub);
            }
            else
                maxLength = std::max(maxLength, sub.length());
        }
    }
    return "";

    */
    std::vector<std::vector<int>> dp(n + 1, std::vector<int>(n + 1, 0));
    int endIndex = 0;
    //grid populuated based on which chars match
    // start matching from i with i + 1
    // example char 'e' at i == 1 will start matching from j = 2 char 't 
  //   n e t a s e t a 
 //j:  0 1 2 3 4 5 6 7 8
 //n 0 0 0 0 0 0 0 0 0 0 
 //e 1 0 0 0 0 0 0 0 0 0 
 //t 2 x 0 0 0 0 0 1 0 0 // str[i - 1] == str[j -1] 'e' where i is 2 and j is 6 - look to the left for both i and j to add up previous matches
 //a 3 x x 0 0 0 0 0 2 0 // here i == 3 and j == 7 the 't' match - go left (1, 5) and add matches so (2, 6) will be assigned 2   
 //s 4 x x x 0 0 0 0 0 3 // here i == 4 and j == 8 the letter 'a' match - go left (2, 6) and add matches so (3, 7) will be assigned 3
 //e 5 x x x x 0 0 0 0 0
 //t 6 x x x x x 0 0 0 0
 //a 7 x x x x x x 0 0 0
 //  8 x x x x x x x 0 0
// i
//     so answer is 3
// 
    // i starts at 1 and j at i + 1 so i is less than j - discount postions where i is greater than j (x)
    // when 2 chars match, go to the left (i-1, j -1) and get the number of matches and add one and store at (i,j)  
    //
    for(int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            if (str[i - 1] == str[j - 1]) {             
                dp[i][j] = dp[i - 1][j - 1] + 1;
                if (dp[i][j] > maxLength) {
                    maxLength = dp[i][j];
                    endIndex = i;
                }
            }
            else {
                // 
                dp[i][j] = 0;
            }
        }
    }
    if (maxLength > 0) {
        return str.substr(endIndex - maxLength, maxLength);
    }
    else {
        return "";
    }

}

std::string longestSubstring(const std::string& s)
{
    //  find the length/string of the longest substring without repeating characters.
    /*
        Input: s = "abcabcbb"
        Output : 3
        Explanation : The answer is "abc", with the length of 3.

        Input: s = "pwwkew"
        Output: 3
        Explanation: The answer is "wke", with the length of 3.
        Notice that the answer must be a substring, "pwke" is a subsequence and not a substring.
    */

    int n = s.length();
    int maxLength = 0;
    int start = 0; // Start index of the longest substring
    std::unordered_map<char, int> charIndexMap;
    int left = 0;
    int right = 0;

    while (right < n) {
        char currentChar = s[right];
        // If the character is already in the map and its index is greater than or equal to the left pointer,
        // move the left pointer to the right of the last occurrence of the character.
        if (charIndexMap.find(currentChar) != charIndexMap.end() && charIndexMap[currentChar] >= left) {
            left = charIndexMap[currentChar] + 1;
        }
        charIndexMap[currentChar] = right; // Update the index of the current character
        // Update maxLength and start index if needed
        if (right - left + 1 > maxLength) {
            maxLength = right - left + 1;
            start = left;
        }
        right++;
    }
    return s.substr(start, maxLength);
}

std::pair<char, int> longest_repeated_character(std::string s) 
{
    std::unordered_map<char, int> char_count;
    char max_char = s[0];
    int max_count = 1;

    for (char c : s) {
        char_count[c]++;
        if (char_count[c] > max_count) {
            max_count = char_count[c];
            max_char = c;
        }
    }

    return std::make_pair(max_char, max_count);
}

std::string decodeString(std::string s)
{
    /*
        push number on a stack and retreive when closing bracket is found, then repeat whatever stringhappens to be in res
        plus the string (if any) thatis on the results stack 
         when a '[' is found, push whatever happens to be in res on teh results stach. so when a '] is found we pop it
         and append to res
    */
    int index = 0;
    std::stack<int> counts;
    std::stack<std::string> result;
    std::string res;

    while (index < s.length())
    {
        if (isdigit(s[index]))
        {
            int count = 0;
            while (isdigit(s[index]))
            {
                // for multi digit numbers : 3000a[2[bbb]]
                count = 10 * count + (s[index] - '0');
                index++;
            }
            counts.push(count);
        }
        else if (s[index] == '[')
        {
            //push what we have seen so far on the stack and when wee see a closing bracket ']', we pop
            // off the stack and add to result 
            result.push(res);
            // must reset result as we dont want to duplicate the output once we pop
            res = "";
            index++;
        }
        else if (s[index] == ']')
        {
            std::string temp(result.top());
            result.pop();
            int count = counts.top();
            counts.pop();
            for (int i = 0; i < count; i++)
                temp += res;
            res = temp;
            index++;
        }
        else
        {
            res+=s[index++];
        }
    }

    return res;
}


int lastStoneWeight(std::vector<int>& stones) 
{
    // max heap - elements sorted from max to min
    std::priority_queue<int> max_heap(stones.begin(), stones.end());

    int first, second;
    int size = stones.size();
    
    while (size > 1)
    {
        first = max_heap.top();
        max_heap.pop();
        second = max_heap.top();
        max_heap.pop();
        size = size - 2;
        if (first - second > 0)
        {
            max_heap.push(first - second);
            size = size + 1;
        }
    }

    if (!max_heap.empty())
    {
        return max_heap.top();
    }

    return 0;

}


std::vector<std::vector<int>> findDifference(std::vector<int>& nums1, std::vector<int>& nums2) 
{
    std::vector<std::vector<int>> result;

    // result[0] is the list of elements that are in num1 but not in num2: for each element in s1; if s2 doesnt contain elem, add to 1
    // result[1] is the list of elements that are in num2 but not in num1: for each element in s2; if s1 doesnt contain elem, add to 2
    return result;
}

int main()
{
    std::string serr = longestRecurringSubstring("netaseta");

    // left to right so 3<<1 is calculated first then << 2
    std::cout << (3 << 1 << 2) << std::endl;

    std::vector<int> vec{ 100, 4, 200, 3, 2, 1};
    longestConsecutive(vec);

    decodeString("3[a]2[bc]");

    std::string ser = longestSubstring("pwwkew");

    
    std::cout << divide(10, 3) << std::endl;;

    isPalindrome(" A man, a plan, a canal: Panama");
    int res = myAtoi("-48");
    std::cout << "Hello World!\n";
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
