// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
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



class A {
public:
	A() {}
	~A() 
	{
		std::cout << "~A()" << std::endl;  
		//throw; 
	}
};

class B {
public:
	B() 
	{
		std::cout << "B()" << std::endl;
	}
	~B()
	{
		std::cout << "~B()" << std::endl;
	}
};

void fo()
{
	A a;
	std::cout << "fo" << std::endl;
	throw "exception";
}

bool handleNI() try
{
	A a;
}
catch (const std::exception& ex)
{	
	throw ex;
}
catch (...)
{
	throw;
}


template<typename T> struct Callback {
	
	class Thang {
	public:
		Thang() { std::cout << "Thang()" << std::endl; }
	};
	
	std::shared_ptr<Thang> sp_;

	template<typename T>
	void forward(std::weak_ptr<Thang> const& wp, T const&	p1)
	{
		std::shared_ptr<Thang> sp;
		int j;
		struct LocalScopeEnd
		{			
			~LocalScopeEnd()
			{
				std::cout << "~LocalScope() " << i << std::endl;
			}

			std::shared_ptr<Thang>& sp_;
			int& i;
		} sa = { sp, j}; //assign sp (a local shared pointer) to sa of type local scope, once sa is out of scope, the destructor will be called - this allows 
		                 // for custom destruction of shared pointers - the destructor of the LocalScopeEnd will use data member sp_
	}

public:
	template<typename T>
	std::function< void() > proxy(T const & p1)
	{
        // returns a function that is bound to the args below  
		return std::bind(&Callback::forward<T>, this, std::weak_ptr<Thang>(sp_), std::cref<T>(p1));		
	}
};

class foo {
public:
	foo() : i(100) { std::cout << "foo()" << std::endl; }
	~foo() { std::cout << "~foo()" << std::endl; }
	int i;
};

Callback<foo> m_dscb;

template<typename T>
class Test {
public:
	struct Thang
	{
		Thang(std::function < void(T const&) > cb) : cb_(cb) {}
		std::function < void(T const&) > cb_;
	};
	std::shared_ptr<Thang> sp_;
	
	void cb(std::function < void(T const&) > f)
	{
		sp_ = std::make_shared<Thang>(f);
	}
};

void cb__(int i)
{
	std::cout << i << std::endl;
}

// Has a constructor
struct TempData2
{
	//TempData2(double minimum, double maximum, double cur, int id) { std::cout << "CON" << std::endl; }
	int stationId;
	double current;
	double maxTemp;
	double minTemp;
};

struct SomeStruct
{};

std::vector<int> twoSum(std::vector<int>& nums, int target) 
{
	std::vector<int> res;
	
	if (nums.size() == 0)
		return res;

	/*
    for (int i = 0; i < nums.size(); ++i)
	{
		for (int j = i + 1; j < nums.size(); ++j)
		{
			if (nums[i] + nums[j] == target)
			{
				res = { i, j };
				return res;
			}
		}
	}
	*/
	// store the element at position i as the key and its index i as the value
	// the idea is to search for (target - currentValue) - if found the sum is currValue + the existing stored Value in the map
	// // we store teh lem vaue as the key so we can search for target - curValue 
	// if found we return the current index i and teh index of the found item which is the 
	// value associated with the key (target - nums{index} in the map 
	std::unordered_map<int, int> map;
	for (int index = 0; index < nums.size(); index++) {
		if (map.find(target - nums[index]) != map.end()) {
			return { index, map[target - nums[index]] };
		}
		map[nums[index]] = index;
	}


	return res;
}

int romanToInt(std::string s)
{
	int num = 0;
	enum Roman : char { I = 'I', V = 'V', X = 'X', L = 'L', C = 'C', D = 'D', M = 'M' };
	std::map<Roman, int> convert =
	{
		{I,   1},
		{ V , 5 },
		{ X , 10 },
		{ L , 50 },
		{ C , 100 },
		{ D , 500 },
		{ M , 1000 }
	};

	char previous = '/0';
	for (int i = 0; i < s.length(); ++i)
	{
		auto   it = convert.find(Roman(s[i]));
		assert(it != convert.end());
		
		switch (previous)
		{
		case 'I':
			if (s[i] == 'V')
				num += 3;
			else if (s[i] == 'X')
				num += 8;
			else
				num += it->second;
			break;
		case 'C':
			if (s[i] == 'M')
				num += 800;
			else if (s[i] == 'D')
				num += 300;
			else
				num += it->second;
			break;
		case 'X':
			if (s[i] == 'C')
				num += 80;
			else if (s[i] == 'L')
				num += 30;
			else 
				num += it->second;
			break;
		default:
			num += it->second;
		}
		previous = s[i];
	}
	
	return num;
}

// int(*arr)[8]; // A pointer to an array of integers
// void(*foo)(int);
// A reference to an array of type T and length N.
template<typename T, size_t N>
size_t ARRAYS(T(&a)[N])
{
	return N;
}

std::string longestCommonPrefix(std::vector<std::string>& strs) 
{
	int size = strs.size();
	if (size == 1)
		return strs[0];

	std::sort(strs.begin(), strs.end());
	
	// compare first string and last string character by character 
	// since the array is sorted, common characters among the first and last element will be common among all the elements of the array.
	std::string res;
	int length = strs[0].size();
	for (int i = 0; i < length; i++) {
		if (strs[0][i] == strs[size - 1][i]) {
			res += strs[0][i];
		}
		else {
			break;
		}
	}
	return res;
}

bool isValid(std::string s) 
{

	int size = s.size();
	std::stack<char> stack;

	for (int i = 0; i < size; ++i) 
	{
		switch (s[i])
		{
		case '(':
			stack.push(')');
			break;
		case '{':
			stack.push('}');
			break;
		case '[':
			stack.push(']');
			break;
		default:
			if (stack.empty() || (stack.top() != s[i])) {
				stack.pop();
				return false;
			}
			else
				stack.pop();
		}		
	}
	return stack.empty();
}

std::vector<int> runningSum(std::vector<int>& nums) 
{
	std::vector<int> res;
	int size = nums.size();
	int acum = 0;
	for (int i = 0; i < size; ++i)
	{
		acum += nums[i];
		res.push_back(acum);
	}
	return res;
}

int binaryGap(int n)
{
	std::vector<int> v;

	while (n > 0)
	{
		v.push_back(n % 2);
		n /=2;
	}

	int maxb = 0, count = 0;
	// start from end of vector (low end last in vector)
	for(int i = v.size() -1; i >=0 ; --i)
	{
		if (v[i] == 0) count++;
		else if (v[i] == 1) { maxb = std::max(maxb, count); count = 0; }

	}

	return maxb;

}

int CountConformingBitmasks(int A, int B, int C)
{
	int nA = 0;
	int nB = 0;
	int nC = 0;

	int nAorB = 0;
	int nAorC = 0;
	int nBorC = 0;
	int nAorBorC = 0;

	int AorB = A | B;
	int AorC = A | C;
	int BorC = B | C;
	int AorBorC = A | B | C;

	// each digit zero within the number binary representation generates 2 possibilities (0 and 1)
	// thus, we need only count the number of zeroes and get # possibilities as (2 power number_of_zeroes)
	// the only tricky is avoid duplicates. In order to avoid it, we can use the basic set relation:
	// #(A or B) = #(A) + #(B) - #(A and B)

	for (int i = 0; i < 30; i++) {

		// shift right A by i and check if the rightmost digit is 0
		if (((A >> i) & 0x01) == 0) nA++;
		if (((B >> i) & 0x01) == 0) nB++;
		if (((C >> i) & 0x01) == 0) nC++;

		if (((AorB >> i) & 0x01) == 0) nAorB++;
		if (((AorC >> i) & 0x01) == 0) nAorC++;
		if (((BorC >> i) & 0x01) == 0) nBorC++;

		if (((AorBorC >> i) & 0x01) == 0) nAorBorC++;

	}

	// result = + #(possibilities of A) + #(possibilities of B) + #(possibilities of C) 
	//          - #(possibilities of A or B) - #(possibilities of B or C) - #(possibilities of A or C)
	//          + #(possibilities of A or B or C)

	//int result = (1 << nA) + (1 << nB) + (1 << nC) - (1 << nAorB) - (1 << nAorC) - (1 << nBorC) + (1 << nAorBorC);

	// reordering the sum up in order to avoid overflow
	int result = (1 << nA) - (1 << nAorB) - (1 << nAorC) - (1 << nBorC) + (1 << nB) + (1 << nC) + (1 << nAorBorC);

	return result;
}

int pivotIndex(std::vector<int>& nums) 
{
	int size = nums.size();
	int totalsum = 0;
	for (int i = 0; i < size; ++i)
	{
		totalsum += nums[i];
	}

	int leftsum = 0;
	for (int i = 0; i < size; ++i)
	{
		if (i != 0) leftsum += nums[i - 1];
		// subtract current number and what we have seen so far form total and if that is equal to what we have seen then found 
		if (totalsum - leftsum - nums[i] == leftsum)
			return i;
	}

	return -1;
}

int removeDuplicates(std::vector<int>& nums) 
{
	if (nums.size() == 1)
		return 0;

	int index = 0, last = nums.size(),  res = 0;
	while (++index != last)
		if (!(nums[res] == nums[index]) && ++res != index)
			nums[res] = nums[index];

	return ++res;
}


template<int N> 
void f1()
{
	f1<N - 1>();
	std::cout << N << '\n';
}
template<> 
void f1<1>()
{
	std::cout << 1 << '\n';
}

bool isIsomorphic(std::string s, std::string t) 
{
	if (s.length() != t.length())
		return false;

	std::unordered_map<char, char> sMt; std::unordered_map<char, char> tMs;
	for (int i = 0; i < s.length(); i++) {
		sMt[s[i]] = t[i];
		tMs[t[i]] = s[i];
	}
	for (int i = 0; i < s.length(); i++) {
		if ((tMs[t[i]] == s[i] && sMt[s[i]] == t[i])) continue; // checks if mapping in either way is same
		else return false;
	}
	
	return true;

}

// A subsequence of a string is a new string that is formed from the original string by 
// deleting some (can be none) of the characters without disturbing the relative positions of the remaining characters. (i.e., "ace" is a subsequence of "abcde" while "aec" is not).
bool isSubsequence(std::string s, std::string t) 
{
	int p1 = 0, p2 = 0;
	while (p1 < s.length() && p2 < t.length())
	{
		if (s[p1] == t[p2])
		{
			++p1; 
			++p2;
		}
		else
			++p2;
	}
	if (p1 == s.length())
		return true;

	return false;
}

class Str {
private:
	Str(const char* in) : str(in) {}
	const char* str;
public:
	operator const char*() { return str; }
	static Str M;
};

Str Str::M = "aaaa";

void remove(Str str)
{
	int i;
}


struct ListNode {
	int val;
	ListNode *next;
	ListNode() : val(0), next(nullptr) {}
	ListNode(int x) : val(x), next(nullptr) {}
	ListNode(int x, ListNode *next) : val(x), next(next) {}
	
};

ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) 
{
	if (!list1 && !list2)
		return nullptr;
	else if (!list1)
		return list2;
	else if (!list2)
		return list1;


	ListNode* res = nullptr, *start = nullptr;
	int count = 0;
	while (list1 && list2)
	{		
		if (list1->val < list2->val)
		{
			if (count == 0)
			{
				res = new ListNode(list1->val);
				list1 = list1->next;
				start = res;
			}			
			else
			{
				res->next = new ListNode(list1->val);
				list1 = list1->next;
				res = res->next;				
				
			}
		}
		else if (list1->val > list2->val)
		{
			if (count == 0)
			{
				res = new ListNode(list2->val);
				list2 = list2->next;
				start = res;
			}
			else
			{
				res->next = new ListNode(list2->val);
				list2 = list2->next;
				res = res->next;

			}
		}
		else
		{
			//order doesnt matter - both are the same 
			if (count)
			{
				res->next = new ListNode(list1->val);
				res->next->next = new ListNode(list2->val);
				res = res->next->next;
			}
			else
			{
				res = new ListNode(list1->val);
				res->next = new ListNode(list2->val);
				start = res;
				res = res->next;
			}
			list1 = list1->next;
			list2 = list2->next;
		}
		++count;
	}

	if (list1)
	{		
		while (list1)
		{
			res->next = new ListNode(list1->val);			
			list1 = list1->next;
			res = res->next;
		}
	}
	else if (list2)
	{
		while (list2)
		{
			
			res->next = new ListNode(list2->val);
			list2 = list2->next;
			res = res->next;
		}
	}

	return start;
}

ListNode* reverseList(ListNode* head)
{
	ListNode* previous = 0, *tmp = 0;

	while (head) {
		tmp = head->next; // save next node
		head->next = previous; // next now points to previous
		previous = head; // save current node in previous
		head = tmp;// move to next node
	}
	head = previous;// because head will be null (head = next)
	return previous;
}

ListNode* getIntersectionNode(ListNode* headA, ListNode* headB)
{
	ListNode* a = headA, * b = headB;
	if (!headA && !headB) return nullptr;

	while (a && b)
	{

		a = a->next; b->next;
	}
}


int sstrStr(std::string haystack, std::string needle) 
{
	int p1 = 0, p2 = 0, offset = 0;
	while (p1 < haystack.length() && p2 < needle.length())
	{
		if (haystack[p1] == needle[p2])
		{
			//save position where start of match was found
			offset = p1;
			++p1, ++p2;
			for (;  ( (p1 < haystack.length() ) && (p2 < needle.length() )) && (haystack[p1] == needle[p2]); ++p1, ++p2);
			// if we have reached the end - must have matched all chars in needle inside haystack
			if (p2 == needle.length())
				return offset;
			else
			{//partial match
				//reset needle
				p2 = 0;
				// resume haystack from position after the earlier match
				p1 = offset + 1;
			}
		}
		else
		{
			//reset needle
			p2 = 0;
			++p1;

		}
	}
	return -1;
}

ListNode* middleNode(ListNode* head) 
{
	ListNode* slow = head, *fast = head;
	
	while (fast && fast->next)
	{
		slow = slow->next;
		fast = fast->next->next;
	}
	return slow;

}

ListNode *detectCycle(ListNode *head)
{
	ListNode* slow = head, *fast = head;
	bool found = false;
	while (fast && fast->next)
	{
		slow = slow->next;
		fast = fast->next->next;
		if (slow == fast)
		{
			found = true;
			break;
		}
	}
	
	if (found)
	{
		slow = head;
		while (slow != fast)
		{
			slow = slow->next;
			fast = fast->next;
		}
		return slow;
	}

	return nullptr;
}

void generate(int ones, int zeroes, std::string str, int len)
{
	// If length of current string becomes same as desired length
	if (len == str.length())
	{
		std::cout << str << "  ";
		return;
	}

	// Append a 1 and recur
	generate(ones + 1, zeroes, str + "1", len);

	// If there are more 1's, append a 0 as well, and recur
	if (ones > zeroes)
		generate(ones, zeroes + 1, str + "0", len);
}



int expand(std::string s, int left, int right)
{
	while (left >= 0 && right < s.length() && s[left] == s[right])
	{
		--left;
		++right;
	}

	return right - left - 1;
}

int longestPalindrome(std::string s) 
{
	int start = 0, end = 0;
	for (int i = 0; i < s.length(); ++i)
	{
		int odd = expand(s, i, i);// start from middle 
		int even = expand(s, i, i + 1);// start from middle and middle + 1
		int len = std::max(odd, even);
		if (len > end - start)
		{
			// we have foound a new longgest palindrome - move starting postion to the left of 
			// index i (which is the center of new longgest palindrome) and end to the right
			// start and end define the boundaries of teh palindrome 
			start = i - (len -1) / 2;
			end = i + len / 2;
		}
	}
	return end - start + 1;
}

class Node {
public:
	int val;
	std::vector<Node*> children;

	Node() {}

	Node(int _val) {
		val = _val;
	}

	Node(int _val, std::vector<Node*> _children) {
		val = _val;
		children = _children;
	}
};


//DFS
void preOderHelper(Node* node, std::vector<int>& result )
{	
	result.push_back(node->val);
	
	if(node->children.empty()) return;
	
	for(int i =0; i < node->children.size(); ++i)
		preOderHelper(node->children[i], result);
}
std::vector<int> preorder(Node* root) 
{
	std::vector<int> result;
	if (root == nullptr) return result;
	preOderHelper(root, result);
	return result;
}

 struct TreeNode 
 {
	int val;
	TreeNode* left;
	TreeNode* right;
	TreeNode() : val(0), left(nullptr), right(nullptr) {}
	TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
	TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
	
};


//BFS
std::vector<std::vector<int>> levelOrder(TreeNode* root) 
{
	std::vector<std::vector<int>> result;
	
	if (root == nullptr) return result;
	
	std::queue<TreeNode*> q;
	q.push(root);

	while (!q.empty())
	{
		int count = q.size();
		std::vector<int> level_nodes;
		for (int i = 0; i < count; ++i)
		{
			TreeNode* node = q.front();
			q.pop();
			if (node->left) q.push(node->left);
			if (node->right) q.push(node->right);
			level_nodes.push_back(node->val);
		}
		result.push_back(level_nodes);
	}	
	
	return result;

}


void inorder(TreeNode* root, std::vector<int>& nodes)
{
	if (!root) return;

	inorder(root->left, nodes);
	nodes.push_back(root->val);
	inorder(root->right, nodes);
}

std::vector<int> inorderTraversal(TreeNode* root) 
{
	std::vector<int> nodes;
	inorder(root, nodes);
	return nodes;
}

bool isBST(TreeNode* root, TreeNode* left, TreeNode* right) 
{
	if (!root)
		return true;


	if ((left && root->val <= left->val) || (right && root->val >= right->val))
		return false;
    // when going right,max remains the same and minumim is the parent value: parent < value < root (same as the parent's max)
	// when going left, min remains the same (root) and max changes to the parent's max
    // value: root (same minimum as the parent) < value < parent  
	return isBST(root->left, left, root) && isBST(root->right, root, right);
}

bool isValidBST(TreeNode* root) 
{
	return isBST(root, NULL, NULL);
}

bool isBST_V2(TreeNode* root, long min, long max)
{
	if (!root) return true;


	if (root->val <= min || root->val >= max)
		return false;
	
	// when going left, min remains the same (root) and max changes to the parent's max
	// root (same minimum as the parent) < value < parent  
	bool left = isBST_V2(root->left, min, root->val);
	if (left)
	{
		// when going right,max remains the same and minumim is the parent's minimum
		// parent < value < root (same as the parent's max)
		bool right = isBST_V2(root->right, root->val, max);
		return right;
	}

	return false;
}

bool isValidBST_V2(TreeNode* root)
{
	return isBST_V2(root, INT_MIN, INT_MAX);
}

int search(std::vector<int>& nums, int target) {

	int left = 0;
	int right = nums.size() - 1;

	while (left <= right)
	{
		int middle = (left + right) / 2;
		if (nums[middle] == target)
			return middle;
		if (target > nums[middle])
			left = middle + 1; // now search from middle to end
		else
			right = middle - 1; //now search from beginning to middle
	}
	return -1;
}

/*
int firstBadVersion(int n) 
{
	int left = 0;
	int right = n;
	int result = n;

	while (left <= right)
	{
		int middle = (left + right) / 2;
		if (isBadVersion(middle))
		{
			result = middle;
			right = middle -1 
		}
		else
			left = middle + 1; // now search from middle to end		
	}
	return result;

}
*/



TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q)
{
	if (p->val < root->val && q->val < root->val)
	{
		return lowestCommonAncestor(root->left, p, q);
	}
	
	if (p->val > root->val && q->val > root->val)
	{
		return lowestCommonAncestor(root->right, p, q);
	}

	return root;
}


int maxDepth(TreeNode* root) 
{
	if(root == nullptr) return 0;
	return std::max(maxDepth(root->left) + 1, maxDepth(root->right) + 1);
}


void findLeaves(TreeNode* root, std::vector<int>& leaves)
{
	if (root == nullptr) return;

	// no left  right ptrs, so we have reached a leaf
	if (root->left == nullptr && root->right == nullptr)
	{
		leaves.push_back(root->val);
		return;
	}
	findLeaves(root->left, leaves);
	findLeaves(root->right, leaves);
}

bool leafSimilar(TreeNode* root1, TreeNode* root2) 
{

	//Two binary trees are considered leaf-similar if their leaf value sequence is the same.

	std::vector<int> leaves1, leaves2;

	findLeaves(root1, leaves1);
	findLeaves(root2, leaves2);

	if (leaves1.size() != leaves2.size()) return false;

	for (int i = 0; i < leaves1.size(); i++)
	{
		if(leaves1[i] != leaves2[i])
		{
			return false;
		}
	}
	return true;
}


int solution(const std::vector<int>& input) 
{
	std::unordered_set<int> s(input.begin(), input.end());
	int max_value = 0;
	for (auto n : s) {
		if ((abs(n) > max_value) && (s.count(-n) != 0)) 
		{
			max_value = n;
		}
	}
	return max_value;
}

/*
void quicksort (int[] a, int lo, int hi)
	{

//  lo is the lower index, hi is the upper index
//  of the region of array a that is to be sorted

	int i = lo;
	int j = hi;
	int h;

	int x = a[ ( lo + hi ) / 2 ];

	//  partition
	do
	{
		while (a[i] < x) i++;	// find 1st element greater then x
		while (a[j] > x) j--;	// find last element less than x

		if ( i <= j)
		{
			h = a[i]; a[i] = a[j]; a[j] = h; //swap a[i] and a[j]
			i++; j--;
		}

	} while ( i <= j );

	//  recursion
	if (lo < j) quicksort(a, lo, j);
	if (j < hi) quicksort(a, j + 1, hi);
}

*/

int findKthLargest(std::vector<int>& nums, int k) 
{
	//sort array and then return index at length - k
	return 0;

}

void fill(std::vector<std::vector<int>>& image, int sr, int sc, int color, int newColor)
{
	// ensure the position required does indeed contain the color
	if ( sr < 0 || sc < 0 || sr >= image.size() || sc >= image[sr].size() || image[sr][sc] != color)
	{
		return;
	}

	image[sr][sc] = newColor;
	// do the row before
	fill(image, sr - 1, sc, color, newColor);
	// the row after
	fill(image, sr + 1, sc, color, newColor);
	// column before
	fill(image, sr, sc -1, color, newColor);
	// column after
	fill(image, sr, sc + 1, color, newColor);
}

std::vector<std::vector<int>> floodFill(std::vector<std::vector<int>>& image, int sr, int sc, int color) 
{
	if (image[sr][sc] == color)
	{
		return image;
	}

	fill(image, sr, sc, image[sr][sc], color);

	return image;
}

void mark_current_island(std::vector<std::vector<char>>& grid, int x, int y, int rows, int cols)
{
	if (x < 0 || y < 0 || x >= rows || y >= cols || grid[x][y] != '1') return;
	grid[x][y] = '2'; // visited
	// down
	mark_current_island(grid, x + 1, y, rows, cols);
	// right
	mark_current_island(grid, x, y + 1, rows, cols);
	// top
	mark_current_island(grid, x -1, y, rows, cols);
	// left
	mark_current_island(grid, x , y - 1, rows, cols);

}

int numIslands(std::vector<std::vector<char>>& grid) 
{
	int rows = grid.size();
	if (rows == 0) return 0;
	int cols = grid[0].size();

	int no_of_islands = 0;
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < cols; ++j)
		{
			if (grid[i][j] == '1')
			{
				mark_current_island(grid, i, j, rows, cols);
				++no_of_islands;
			}
		}
	}

	return no_of_islands;

}

int tribonacci(int n) 
{
	
	if (n == 0)
		return 0;
	else if (n <= 2)
		return 1;

	//return tribonacci(n - 1) + tribonacci(n - 2) + tribonacci(n - 3);

	std::unique_ptr<int[]> dp(new int[n + 1]);
	dp[0] = 0; dp[1] = 1; dp[2] = 1;
	for (int i = 3; i <=n; i++)
	{
		dp[i] = dp[i - 1] + dp[i - 2] + dp[i - 3];
	}
	return dp[n];
}

int fib(int n)
{
	if (n < 2)
		return n;
	return fib(n - 1) + fib(n - 2);
	/*
		int first = 0, second = 1 total = 0;
		for (int i = 2; i <= n; ++i)
		{
			total = first + second;
			first = second;
			second = total;
		}
		return total;
	*/
}

int climbStairs(int n) 
{
	if(n == 1) return 1;

	int first = 1;
	int second = 2;
	
	for (int i = 3; i <= n; i++)
	{
		int third = first + second;
		first = second;
		second = third;
	}

	return second;

/*
	std::vector<int> steps(n +1, 0);
	steps[1] = 1;
	steps[2] = 2;

	for (int i = 3; i <= n; i++)
	{
		steps[i] = steps[i - 2] + steps[i - 1];
	}
	return steps[n];
*/

}

int minCostClimbingStairs(std::vector<int>& cost) 
{
	// start from the end
	// [10, 15, 20] 0
	// start at end 
	// can do one to 2 jummps - result is either form start or start + 1;
	// 2 jummps from i will cost: cost[i] + cost[i +1]
	// a double jummp from i will cost: cost[i] + cost[i+2] 
	// so will take the minimun of those 2 values 
	// min (cost[i] + cost[i +1], cost[i] + cost[i +2])
	// remove cost[i] as its in both expressions
	// min (cost[i+1],cost[i+2]) and add result to the cost of i
	// 
	cost.push_back(0);
	for (int i = cost.size() - 1; i >= 0; i--)
	{
		// a single jump (i and then i + 1) or a double jump so i and then i + 2
		// std::min(cost[i] + cost[i +1], cost[i] + cost[i +2])
		cost[i] = std::min(cost[i+1], cost[i+2]);
	}

	return std::min(cost[0], cost[1]);
}


/*
* bool is_first_same_as_last(int a, int b)
{
      // check if LSN are the same 
      if (a % 10 != b% 10)
          return false;
      const int no_digits_a = static_cast<int>(log10(a));
      const int no_digits_b = static_cast<int>(log10(b));
      // get the MSDs for both (divide by 10 to the number of digits)
      return (a / pow(10, no_digits_a)) == (b / pow(10, no_digits_b));
}

// largest 2 numbers in a set where  first and last digits are the same 

12 -> [12, 132, 13452]

37 -> [37, 387]

60 -> [650, 67890]

int max = 0;
for (int i = 0; i < array.size()-1; ++i )
{
    //compare each element with all other elements in the array 
    for (int j = i + 1; j < array.size(); ++j)
    {
       if (is_first_same_as_last(array[i], array[j]))
       {
           max = std::max(max, array[i] + array[j] )
       }
  }
}
return max;


std::unordered_map<int, std::set>

int GetFirstAndLastKey(int);

for (auto i : array)
{
    // partition array into buckets keyed by first and last number
    my_map[GetFirstAndLastKey(i)].insert(i);
}
for (const auto& [key, value] : my_map)
{
    if (value.size() < 2)
		continue;
   // update max by adding the last 2 numbers which will be the largest as the set is sorted - no need to traverse the whole set  
    max = std::max(max, *value.rbegin() + *(value.rbegin() + 1);
}
return max;

###############
so say u had these numbers in one group start with 1 and end with 2
[13452, 12362, 12, 1482]

if we say find 2 numbers in this cohort that there sum is the max of any other 2 numbers
so in the brute force, we would check the following combinations
[13452 and 12362] [13452 and 12] [13452 and 1482] [12362 and 12] [12362 and 1482] [12, 1482]

we find the local max which will be the set [13452 and 12362]

if u sort these numbers
u will find 12362 13452 will be last and second last hence the either behind rbegin and rbegin + 1

so its the max within a bucket
yes
so we go to next bucket and do same in the end we have local maximums per bucket then we want the global max the max of all local max

*/

std::vector<int> findAnagrams(std::string s, std::string p) 
{
	
	
	if (s.size() < p.size() ) return {};
	
	std::vector<int> result;
	std::vector<int> shash(26, 0);
	std::vector<int> phash(26, 0);

	for (int i = 0; i < p.size(); ++i)
	{
		// build frequency for a window the size of string p
		phash[p[i] - 'a']++;
		shash[s[i] - 'a']++;
	}

	if (phash == shash) //if both hashes are the ame then its an anagram - hash for "abc" and "cba" will be the same 
	{
		result.push_back(0);
	}
	// start in s from end of last window (the size of p)
	for (int i = p.size(); i < s.size() ; i++) //shash changes as we slide through the s string 
	{
		//cbaeXXXXXX
		// add 'e' and remove 'c' 
		// add frequency to next character in s (after last window) 
		shash[s[i] - 'a']++;
		// remove frequncy of the 1st character in previous window 
		// so now the hash of s contains the "bae" and we compare with p's hash and so on as we slide the window comparing
		// p's hash with s's hash (s' hash will be adjusted each iteraton to contain the next letter and remove first lettet) 
		shash[s[i - p.size()] - 'a']--;

		if (phash == shash) 
		{
			result.push_back(i +1 - p.size());
		}
	}

	return result;
}

int uniquePaths(int m, int n) 
{

	std::vector<std::vector<int>> dp(m, std::vector<int>(n, 1));
	
	// set bottom row to 1 - can only go right ... 
	for (int i = 0; i < m; i++)
	{
		dp[m-1][i] = 1;			
	}

	// set last coloumn to 1 - can only go down ... 
	for (int i = 0; i < n; i++)
	{
		dp[i][n-1] = 1;
	}

	for (int i = 1; i < m; i++)
	{
		for (int j = 1; j < n; j++)
		{
			// above and left 
			dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
		}
	}

	return dp[m - 1][n - 1];

}

int characterReplacement(std::string s, int k) 
{
	/*
		get the length of the string containing teh same letter having replaced up to k letters
		ABAB: k = 2
		AAAA replace the 2 B's

		AAABABBA: k =1
		BBBB replace the A with a B so 4

		k =2
		AABABBA
		^ 
		 ^
		  ^
		   ^
		    ^
			 ^

		{A :1}
		maxLen = 0
		maxCount = 0

		{A :2}
		maxLen = 1
		maxCount = 2

		{A :2, B:1}
		maxLen = 3
		maxCount = 2

		{A :3, B:1}
		maxLen = 3
		maxCount = 3
		while ( window_end - window_start + 1) - maxCount > k) is now FALSE

		when false, dont shrink our window
		if true, shrink window by popping off start of frequency array from the start and move window 

		{A :3, B:2}
		maxLen = 4
		maxCount = 3

		{A :3, B:3}
		maxLen = 5
		maxCount = 3
		while ( window_end - window_start + 1) - maxCount > k) is now FALSE so pop off freq array 
		{A: 2, B:3}
		and move window 
		AABABBA
		 ^
		     ^

		check if most frequent (max_count) is bigger than frequency of current character 
		max_count = std::max(max_count, char_count[s[window_end] - 'a']);

		
		number of char replacement is less than or equal to k we continue
		while ( window_end - window_start + 1) - maxCount > k)

	*/

	int n = s.length();
	int window_start = 0;
	int max_length = 0;
	int max_count = 0; //repeating character 
	int char_count[26] = {};

	for (int window_end = 0; window_end < n; window_end++)
	{
		//counter for each letter
		//char_count[s[window_end] - 'A'] = char_count[s[window_end] - 'A'] + 1;
		char_count[s[window_end] - 'A']++;

		max_count = std::max(max_count, char_count[s[window_end] - 'A']);

		int window_size = window_end - window_start + 1;
		// subtract the length of the string with maximum number of repeating letters will give the number of 
		// letters than need to change 
		int replacement_count = window_size - max_count;

		if (replacement_count > k)
		{
			// invalid window as we now have more mismatched letters than k (the number of chars we can change) 
			// must sliede left hand side by one and pop off the letter at the start of window 
			char_count[s[window_start] - 'A'] = char_count[s[window_start] - 'A'] - 1;
			window_start++;
		}
		else
		{
			max_length = std::max(max_length, window_size);
		}
	}

	return max_length;

}

bool backspaceCompare(std::string s, std::string t) 
{
	std::string res1 = "", res2 = "";
	int i = 0;

	while(s[i] != '\0')
	{
		if (s[i] >='a' && s[i] <= 'z')
			res1 += s[i];
		else if (!res1.empty())
			// if its a '#', delete last character in result 
			res1.pop_back();
		++i;
	}
	
	i = 0;
	while(t[i] != '\0')
	{
		if (t[i] >= 'a' && t[i] <= 'z')
			res2 += t[i];
		else if (!res2.empty())
			// if its a '#', delete last character in result 
			res2.pop_back();

		++i;
	}

	if (res1.compare(res2) == 0)
		return true;

	return false;
}

std::string getHint(std::string secret, std::string guess) 
{
	int bulls = 0, cows = 0;
	std::vector<int> s(10, 0);
	std::vector<int> g(10, 0);

	for (int i = 0; i < secret.length(); i++)
	{
		if (secret[i] == guess[i])
		{
			++bulls;
		}
		else
		{
			s[secret[i] - '0']++;
			g[guess[i] - '0']++;
		}
	}

	for (int i = 0; i < 10; i++)
	{
		cows += std::min(s[i], g[i]);
	}

	std::string res = "";
	res += std::to_string(bulls);
	res += "A";
	res += std::to_string(cows);
	res += "B";

	return res;

}

int main()
{
	tribonacci(35);

	std::vector<int> vec = { 5, 3, 4, 8 };
	twoSum(vec, 12);
	binaryGap(128);

	bool bol = backspaceCompare("ab#c", "ad#c");
	int iiii = characterReplacement("ABAA", 0);


	std::vector<int> ro = findAnagrams("cbaebabacd", "abc");
	std::cout << ro[0] << " " << ro[1] << std::endl;

	int out = longestPalindrome("abccccdd");
		

	std::string sttr = "";
	generate(0, 0, sttr, 4);
	return 0;


	//int q = sstrStr("mississippi", "issip");
	//int q = sstrStr("sadbutsad", "sad");

	ListNode* l1 = new ListNode(3);
	ListNode* l2 = new ListNode(2);
	l1->next = l2;
	ListNode* l3 = new ListNode(0);
	ListNode* l4 = new ListNode(4);
	l2->next = l3;
	l3->next = l4;
	l4->next = l2;

	detectCycle(l1);


	//ListNode* ll4 = new ListNode(4);
	//ListNode* ll3 = new ListNode(3, ll4);
	ListNode* ll1 = new ListNode(1);


	mergeTwoLists(l1, ll1);
	
	
	remove("vvvv");
	std::vector<int> vv = { 1};		
	int o = removeDuplicates(vv);

	f1<10>();

	std::vector<int> v = { 3, 1, 2, 10, 1 };
	std::vector<int> r = runningSum(v);


	std::cout << isValid("()") << std::endl ;

	std::vector<std::string> strs = { "flower", "flow", "flight" };
	std::cout << longestCommonPrefix(strs) << std::endl;
	

	int a[] = { 1,2,3,4,5,6 };
	std::cout << ARRAYS(a) << std::endl;

	//romanToInt("MCMXCIV");
	romanToInt("LVIII");

	std::vector<int> nums = { 3, 2, 4 };
	std::vector<int> res = twoSum(nums, 6);
	std::cout << res[0] << " " << res[1] << std::endl;

	const int const * ppp = 0;
	
	std::shared_ptr<foo> const sp = std::make_shared<foo>();
	std::shared_ptr<foo> sp_ = sp;

	return 0;

	int x = 8, y = -1000;
	if( x = y)
	  std::cout << "ZZZZZ" << std::endl;

	char c = 1202;
	std::cout << CHAR_MIN << " " << c << std::endl;


	TempData2 td_default {1, 2, 3, 4 };

	Test<int> t;
	// member function cb takes a function that returns void and takes a single argument of the same type T as the object Test<int> so an int 
	t.cb(cb__);

	//#################################

	foo f;
	if (std::function < void() > fn = m_dscb.proxy(f))
	{
		fn();
	}


	try {
	  B b;
	  fo();
	}
	catch (...)
	{
		std::cout << "here" << std::endl;
	}
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
