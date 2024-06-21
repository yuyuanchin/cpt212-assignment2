#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <ctime>

using namespace std;

#define NO_OF_CHARS 256 // Number of ASCII characters

// Function to generate a random string of specified length using given characters
string generateRandomString(const string &characters, int length)
{
	string result;
	int charactersLen = characters.length();
	for (int i = 0; i < length; ++i)
	{
		result += characters[rand() % charactersLen];
	}
	return result;
}

/**
 * @brief Preprocesses the pattern to populate the bad character array.
 *
 * @param str Pattern string
 * @param size Length of the pattern
 * @param badchar Bad character array to be filled
 */
void badCharHeuristic(const string &str, int size, int badchar[NO_OF_CHARS])
{
	// Initialize all occurrences as -1
	for (int i = 0; i < NO_OF_CHARS; i++)
		badchar[i] = -1;

	// Iterate over the pattern and store the last occurrence index of each character
	for (int i = 0; i < size; i++)
		badchar[(int)str[i]] = i;
}

/**
 * @brief Preprocesses the pattern for strong suffix rule.
 *
 * @param shift Array that holds the shift values for the pattern
 * @param bpos Array that holds the border positions for the pattern
 * @param pat Pattern string
 * @param m Length of the pattern
 */
void strongGoodSuffix(int *shift, int *bpos, const char *pat, int m)
{
	int i = m, j = m + 1;
	bpos[i] = j; // Border position for the whole pattern is just beyond the end of the pattern

	// Outer while loop iterates backwards through the pattern
	while (i > 0)
	{
		// Find the rightmost position where characters don't match
		while (j <= m && pat[i - 1] != pat[j - 1])
		{
			// Update shift value if it's not already set
			if (shift[j] == 0)
				shift[j] = j - i;
			j = bpos[j]; // Continue finding the next potential mismatch position
		}
		i--;
		j--;
		bpos[i] = j;
	}
}

/**
 * @brief Preprocesses for case 2 of good suffix heuristic.
 *
 * @param shift Array that holds the shift values for the pattern
 * @param bpos Array that holds the border positions for the pattern
 * @param pat Pattern string
 * @param m Length of the pattern
 */
void goodSuffixCase2(int *shift, int *bpos, const char *pat, int m)
{
	int j = bpos[0]; // Border position for the whole pattern

	// Iterate over each position i in the shift array
	for (int i = 0; i <= m; i++)
	{
		// If shift[i] has not been set by the strong good suffix preprocessing
		if (shift[i] == 0)
			shift[i] = j;
		if (i == j)
			j = bpos[j];
	}
}

// A pattern searching function that uses both bad character and good suffix heuristic of Boyer-Moore Algorithm
void search(const string &txt, const string &pat)
{
	int m = pat.size(); // Length of the pattern
	int n = txt.size(); // Length of the text

	int badchar[NO_OF_CHARS];					 // Array to store the position of last occurrence of a character in the pattern
	badCharHeuristic(pat, m, badchar); // Preprocess bad character heuristic

	int bpos[m + 1], shift[m + 1];		// Arrays for good suffix heuristic
	for (int i = 0; i < (m + 1); i++) // Initialize shift array with zeros
		shift[i] = 0;

	strongGoodSuffix(shift, bpos, pat.c_str(), m); // Preprocess strong good suffix heuristic
	goodSuffixCase2(shift, bpos, pat.c_str(), m);	 // Preprocess case 2 of good suffix heuristic

	int s = 0;								 // s is shift of the pattern with respect to text
	bool patternFound = false; // Flag to check if the pattern is found

	// Outer while loop runs until the pattern can no longer fit in the remaining text
	while (s <= n - m)
	{
		int j = m - 1; // Index of the last character of the pattern

		// Keep reducing index j of pattern while characters of pattern and text are matching at this shift s
		while (j >= 0 && pat[j] == txt[s + j])
			j--;

		// If the pattern is present at the current shift, then index j will become -1 after the above loop
		if (j < 0)
		{
			cout << "Pattern occurs at index = " << s << endl;
			patternFound = true; // Set patternFound to true
			s += shift[0];			 // Shift the pattern using the shift value from the shift table
		}
		// If the pattern is not present at the current shift
		else
		{
			// Combine both heuristics to determine the shift
			s += max(shift[j + 1], j - badchar[(int)txt[s + j]]);
		}
	}

	// If the pattern was not found, print "Pattern not found!"
	if (!patternFound)
	{
		cout << "Pattern not found!" << endl;
	}
}

// Driver code
int main()
{
	int choice;
	cout << "\n============= Boyer-Moore Algorithm ==============" << endl;
	cout << "| 1. View Example of String Matching             |" << endl;
	cout << "| 2. Input Pattern and Text for Search           |" << endl;
	cout << "==================================================" << endl;
	cout << "Enter your choice: ";
	cin >> choice;
	cin.ignore(); // To ignore the newline character left in the buffer after cin

	switch (choice)
	{
	case 1:
	{
		// Example for illustration
		cout << "\n============= Example Searching ==============" << endl;
		string txt = "CABCABABBABCABACBCBABBABCAABBC";
		string pat = "ABABBAB";
		cout << "Text: " << txt << endl;
		cout << "Pattern: " << pat << endl;
		search(txt, pat);
		cout << endl;

		srand(time(0)); // Seed for random number generation

		string patternChars = "AB";
		string textChars = "AB";

		// Generate 5 random patterns and texts and search for the pattern in the text
		for (int i = 1; i <= 5; i++)
		{
			cout << "============ Pattern Searching #" << i << " ============" << endl;

			string pat = generateRandomString(patternChars, 3 + rand() % 2); // Pattern length: 3 or 4
			string txt = generateRandomString(textChars, 30);								 // Text length: 30

			cout << "Generated text: " << txt << endl;
			cout << "Generated pattern: " << pat << endl;

			search(txt, pat);

			cout << endl;
		}

		break;
	}

	case 2:
	{
		string txt, pat;
		cout << "Enter the text: ";
		getline(cin, txt);
		cout << "Enter the pattern to search: ";
		getline(cin, pat);
		search(txt, pat);

		break;
	}

	default:
	{
		cout << "Invalid choice! Exiting..." << endl;
	}
	}

	return 0;
}