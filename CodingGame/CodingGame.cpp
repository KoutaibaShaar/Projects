// CodingGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <sstream>
#include <iomanip>

void plusMinus(std::vector<int> arr) 
{
    int countPos = 0, countNeg = 0, countZero = 0;
    for (int i = 0; i < arr.size(); ++i)
    {
        if (arr[i] > 0)
            ++countPos;
        else if (arr[i] < 0)
            ++countNeg;
        else
            ++countZero;
    }

    double pos = (double)countPos / (double)arr.size();
    double neg = (double)countNeg / (double)arr.size();
    double zero = (double)countZero / (double)arr.size();

    printf("%.6lf \n", pos);
    printf("%.6lf \n", neg);
    printf("%.6lf \n", zero);
}

void Temprature()
{
    int n; // the number of temperatures to analyse
    std::cin >> n; std::cin.ignore();
    int result = 5526;
    for (int i = 0; i < n; i++) {
        int t; // a temperature expressed as an integer ranging from -273 to 5526
        std::cin >> t; std::cin.ignore();
        if (abs(t) < abs(result)) {
            result = t;
        }
        // this case if both are the same absolute value but current one ins a postive, use the positive number
        if (abs(t) == abs(result) && t > 0) {
            result = t;
        }
    }
    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;
    if (result == 5526)
        result = 0;

    std::cout << result << std::endl;
}


void bat_man()
{
    std::string bomb_dir;
    int h, h0 = 0, y0 = 0, x0 = 0, w0, w = 0;
    if (bomb_dir.find("U") != std::string::npos)
    {
        h = y0;
        y0 = floor((y0 + h0) / 2);
    }
    if (bomb_dir.find("D") != std::string::npos)
    {
        h0 = y0;
        y0 = floor((y0 + h) / 2);
    }
    if (bomb_dir.find("R") != std::string::npos)
    {
        w0 = x0;
        x0 = floor((x0 + w) / 2);
    }
    if (bomb_dir.find("L") != std::string::npos)
    {
        w = x0;
        x0 = floor((x0 + w0) / 2);
    }

    // the location of the next window Batman should jump to.
    std::cout << x0 << " " << y0 << std::endl;
}

void ascii_art()
{
    int l = 4;
    int h = 5;
    std::string t;
    //getline(std::cin, t);

    for (int i = 0; i < t.length(); i++) {
        t[i] = toupper(t[i]);
    }

    for (int i = 0; i < h; i++) {
        std::string row;
        //std::getline(std::cin, row);
        std::string line;
        // go through all the letters that make up the input string and compose the correponding ASCII art from the 
        // current ASCII art row[i] - where i is 0 to h
        for (int j = 0; j < t.length(); j++) {
            int ascii_code = t[j] - 65;// this will give the postion of the letter in the alpahbet - so M will be pos 12, A at pos 0
            if (ascii_code < 0 || ascii_code > 26) {
                // NOT in the [a-z] or [A-Z] range, use a control character
                ascii_code = 26;
            }
            // add to current line the correponding parts of row[i] - based on the postion of current letter t[j] (ascii code - 65 to give us postion in the ascii art row)
            // at the end of this loop, line will contains the ascii art for all letters in t for this row: row[i] 
            // every letter section in row has the same width l
            line += row.substr(ascii_code * l, l);
        }
        std::cout << line << std::endl;
    }
}

struct IteratorCurrentAndEnd
{
    bool operator>(const IteratorCurrentAndEnd & that ) const
    {
        return *current > *that.current;
    }
    std::vector<int>::const_iterator current;
    std::vector<int>::const_iterator end;
};

std::vector<int> MergeSortedArrays(std::vector<std::vector<int>>& sorted_arrays)
{
    std::priority_queue<IteratorCurrentAndEnd, std::vector<IteratorCurrentAndEnd>, std::greater<> > min_heap;
    // add the begin and end iterators of each sorted vector to the mini heap
    for (const std::vector<int>& sorted_array : sorted_arrays)
    {
        if (!empty(sorted_array))
        {
            min_heap.emplace(IteratorCurrentAndEnd{cbegin(sorted_array), cend(sorted_array)});
        }
    }

    std::vector<int> res;
    // go through each element in the mini heap (begin and end iterators) and pop off the smallest element
    // placing the begin ietrator (the value it points to) in the result vector, then move one to the next element
    // in the (using next(current)) popped off item, if it exists, add a new item to heap made up of next(current), end) 
    while (!empty(min_heap))
    {
        IteratorCurrentAndEnd smallest_array = min_heap.top();
        min_heap.pop();
        // get the smallest element in the heap (the current element in the vector "*iterator") and store it in the result
        res.emplace_back(*smallest_array.current);
        // now get the next element in the vector that contains the smallest element 
        if (next(smallest_array.current) != smallest_array.end)
        {
            min_heap.emplace(IteratorCurrentAndEnd{ next(smallest_array.current),smallest_array.end });
        }
    }
    return res;
}

/*
    while (!(flipped & 0x0001))
        flipped >>= 1;
*/

std::string unary_code(const std:: string& binaryString)
{

    /*
    * this receives a string of chars "message" and converts it into its binary represenation
    * example "C" ===> 1000011
         for (char& _char : message) {
                binaryString +=bitset<7>(_char).to_string();
    }
        00 means 1 then each 0 is 1 so 00 0000 ==> 1111 
        0 means 0  then each 0 is 0 so 0  0000 ==> 0000 
    */
    std::string codedMessage;
    for (int i = 0; i < binaryString.length(); ++i) {
        if (binaryString[i] == '1') {
            codedMessage += "0 ";
            for (int j = i; binaryString[j] == '1'; j++) {
                i++;
                codedMessage += "0";
            }
        }
        codedMessage += " ";
        if (i < binaryString.length() && binaryString[i] == '0') {
            codedMessage += "00 ";
            for (int j = i; binaryString[j] == '0'; j++) {
                codedMessage += "0";
                // don't increment i as it will be incremented agian back in the main loop
                i = j;
            }
        }
        codedMessage += " ";
    }
    codedMessage.erase(codedMessage.begin(), std::find_if(codedMessage.begin(), codedMessage.end(), [](char ch) { return !std::isspace(ch); }));
    codedMessage.erase(std::find_if(codedMessage.rbegin(), codedMessage.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), codedMessage.end());
    return codedMessage;
}

/*
* for (int i = 0; i < n; i++) {
        string ext; // file extension
        string mt; // MIME type.
        cin >> ext >> mt; cin.ignore();
        std::transform(ext.begin(), ext.end(), ext.begin(),[](unsigned char c){ return std::tolower(c); });
        mimeTable[ext] = mt;
    }
    for (int i = 0; i < q; i++) {
        string fname;
        getline(cin, fname); // One file name per line.
        std::size_t found = fname.find_last_of(".");
        if (found != string::npos) { 
             string ext = fname.substr(found+1);
             std::transform(ext.begin(), ext.end(), ext.begin(),[](unsigned char c){ return std::tolower(c); });
             if(mimeTable.find(ext) != mimeTable.end()) {   
                cout << mimeTable[ext] <<  endl;
             }
             else
                cout << "UNKNOWN" << endl;
        }
        else
            cout << "UNKNOWN" << endl;
    }
*/


std::vector<int> getBytes(const std::string& in)
{
    std::vector<int> result;
    for (int i = 0; i < in.length(); )
    {
        std::string s = in.substr(i, 2);
        std::stringstream ss;
        ss << std::hex << s;
        unsigned n;
        ss >> n;
        result.push_back(n);
        i += 3;
    }

    return result;
}

std::unique_ptr<int[]> getSteps(const std::vector<int>& bytes, int& size)
{
     auto dp = std::unique_ptr<int[]>(new int[bytes.size() * sizeof(int)]());
     for (int i = 0; i < bytes.size(); ++i)
     {
         int byte = bytes[i];
         for (int j = 0; j < sizeof(int); j++)
         {
             // mask the bits and then shift result into position ( << s) so it can be inserted to res
             //int res |= ( ( (byte >> s) & 3) << s);
             int pos = j + (4 * i);
             dp[pos] = ((byte >> (2 * j)) & 3); // shift byte by 0, 2, 4 , 6 and mask - store 2 bits of the byte in j = 0 to j = 3
             //std::cout << dp[pos] << std::endl;
         }
     }
     size = bytes.size() * sizeof(int);
     return dp;
}

std::vector<std::vector<int>> populateGridWithMovements(const std::unique_ptr<int[]>& steps, int size, std::pair<int, int>& end_pos)
{
    std::vector<std::vector<int>> field(9, std::vector<int>(17, 0));
    std::pair<int, int> start_position = { 4, 8 };
    std::map<int, std::pair<int, int>> direction_map = {
                                            {0, {-1, -1}},
                                            {1, {-1, 1}},
                                            {2, {1, -1}},
                                            {3, {1, 1}} };


    auto clip_at_walls = [](int x, int y)
    {
        std::set<std::pair<int, int>> corners = { {0, 16}, {0, 0}, {8, 16}, {8, 0} };
        if (corners.find(std::make_pair(x, y)) != corners.end())
            return std::make_pair(x, y);
        // If on the right edge and the next move is 01, we simply slide one space vertically up
        // or at the top edge (0, 8) an next move is (up right: -1, -1) this would mean new pos is -1, 7
        // so we must remain at postion 0 but move only horizontally
        // we do this by taking std::max(x, 0) which is 0 and then the min between 0 and 8 which is 0
        // thereby remaining at 0. equally of we are at postion 8 and new postion is 9
        // std::max will return 9 but the ouer std::min acts as a ceiling so it return 8
        // inner max acts as a barrier to prevent going below 0 and outer as a barrier going above range 
        return std::make_pair<const int&, const int&>(std::min(std::max(x, 0), 8), std::min(std::max(y, 0), 16));  
    };
    
    std::pair<int, int> pos = start_position;
    for (int i = 0; i < size; ++i)  
    {
        // get new postions/deltas to move to
       int dx, dy;
       std::tie(dx, dy)= direction_map[steps[i]];
       
       // pos has the existing position - add the deltas (based on the current step position) and adjust new pos if required 
       pos = clip_at_walls(pos.first + dx, pos.second + dy);

       // pos now contains new position to move to (x + dx) adjusted so that if at the edge its slides forward
       field[pos.first][pos.second] += 1;
       
       if (i == size - 1)
       {
           // copy last position we moved to
           end_pos.first = pos.first;
           end_pos.second = pos.second;

       }
    }
    return field;
}

void print_randomart(const std::vector<std::vector<int>>& grid, const std::pair<int, int>& end_pos)
{
    // Symbols for the number of times a position is visited by the bishop
    // White space means that the position was never visited
    // S and E are the start and end positions
    std::string symbols = " .o+=*BOX@%&#/^SE";
    std::pair<int, int> start_position = { 4, 8 };
    int start_x, start_y;
    std::tie(start_x, start_y) = start_position;
    std::cout << "+---[CODINGAME]---+\n";
    for (int i = 0; i < grid.size(); ++i)
    {
        std::string row_symbols;
        for (int j = 0; j < grid[0].size(); ++j)
        {
            //std::cout << grid[i][j];
            // wrap-around logic if the number of times visited is greater than 14, 
            // e.g. we use 'o' if a position is visited for 17 times. 17 % 15 = 2
            if(i == start_x && j == start_y)
            {
                row_symbols += "S";
            }
            else if(i == end_pos.first && j == end_pos.second)
            {
                row_symbols += "E";
            }
            else
            {
                row_symbols += symbols[grid[i][j] % 15];
            }
        }
        std::cout << "|" << row_symbols << "|" << std::endl;
    }
    std::cout << "+" << std::setfill('-') << std::setw(19) << "+\n";
}
/*
For example:
4z means zzzz
1{ means {
10= means ==========
5bS means \\\\\ (see Abbreviations list below)
27 means 77
123 means 333333333333
(If a chunk is composed only of numbers, the character is the last digit.)

So if part of the recipe is
2* 15sp 1x 4sQ nl
...that tells you to show
**               x''''
and then go to a new line.


find_if_not
returns last if all of the elements in the range satisfy the condition
if any doesnt, it return the element
for (; first != last; ++first)
        if (!q(*first))
            return first;

    return last;


constexpr bool all_of(InputIt first, InputIt last, UnaryPredicate p)
{
    return std::find_if_not(first, last, p) == last;
}

*/
std::string retroTypeWriterArt(const std::string s)
{

    std::map<std::string, std::string> abbreviations = {
                                            {"sp", " "},
                                            {"bS", "\\"},
                                            {"sQ", "'"},
                                            {"nl", "\n"}
                                                        };



    std::string res;
    if (std::all_of(s.begin(), s.end(), ::isdigit))// str.find_first_not_of("0123456789") == std::string::npos;
    {
        int count = 0;
        for(int i = 0; i < s.length() - 1; ++i)
        {
            count = 10 * count + (s[i] - '0');   
        }
        char c = s[s.length() - 1];
        for(int j = 0; j < count; ++j)
            res += c;

        return res;
    }
    /*
    *while ((end = str.find(" ", start)) != std::string::npos) 
	{
		tokens.push_back(str.substr(start, end - start));
		start = end + 1;
	}
    */

    for (int i = 0; i < s.length() - 1; )
    {
        if (isdigit(s[i]))
        {
            int count = 0;
            while (isdigit(s[i]))
            {
                count = 10 * count + (s[i] - '0');
                i++;
            }
            if (abbreviations.find(s.substr(i, 2)) != abbreviations.end())
            {
                std::string c = abbreviations[s.substr(i, 2)];
                for (int j = 0; j < count; ++j)
                    res += c;
                i = i + 2;
            }
            else
            {
                for (int j = 0; j < count; ++j)
                    res += s[i];
                i = i + 1;
            }
        }
        else
            ++i;// easier than tokaniozing using spaces
    }
    return res;
}


int main()
{
    // 2* 15sp 1x 4sQ nl
    std::cout << retroTypeWriterArt("1sp 1/ 1bS 1_ 1/ 1bS nl 1( 1sp 1o 1. 1o 1sp 1) nl 1sp 1> 1sp 1^ 1sp 1< nl 2sp 3") << std::endl;

    std::vector<int> resq = getBytes("fc:94:b0:c1:e5:b0:98:7c:58:43:99:76:97:ee:9f:b7");
    int size = 0;
    auto steps = getSteps(resq, size);
    std::pair<int, int> end_pos;
    std::vector<std::vector<int>> grid = populateGridWithMovements(steps, size, end_pos);
    print_randomart(grid, end_pos);

    std::string ssssss = unary_code("0100101");

    std::vector<std::vector<int>> v = { {1,2,3},{0, 4, 7},{2, 3, 9} };
    MergeSortedArrays(v);

    int light_x = 36; // the X position of the light of power
    int light_y = 17; // the Y position of the light of power
    int initial_tx; // Thor's starting X position
    int initial_ty; // Thor's starting Y position
    //std::cin >> light_x >> light_y >> initial_tx >> initial_ty; std::cin.ignore();

    // game loop
    //while (1) {
        //int remaining_turns; // The remaining amount of turns Thor can move. Do not remove this line.
        //std::cin >> remaining_turns; std::cin.ignore();
        //int tX = initial_tx;
        //int tY = initial_ty;
        int tX = 18;
        int tY = 18;
        std::string direction;
        // 2 for vertical
        if (tY < light_y)
        {
            direction = "S";
            tY = tY + 1;
        }
        if (tY > light_y)
        {
            direction = "N";
            tY--;
        }
        // 2 for horizontals
        if (tX < light_x)
        {
            direction += "E";
            tX++;
        }
        if (tX > light_x)
        {
            direction += "W";
            tX = tX - 1;
        }
        // A single line providing the move to be made: N NE E SE S SW W or NW
        std::cout << direction << std::endl;
    //}

    /*
    
    while (1) {
        int max = 0;
        int max_index = 0;
        for (int i = 0; i < 8; i++) {
            int mountain_h; // represents the height of one mountain.
            std::cin >> mountain_h; std::cin.ignore();
            if (max < mountain_h) {
                max = mountain_h;
                max_index = i;
            }
        }

        std::cout << max_index << std::endl; // The index of the mountain to fire on.
    }
    std::cout << "Hello World!\n";
    
    */

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
