#include <iostream>
#include <unordered_map>
#include <functional>
#include <limits>
#include <stack>
#include <queue>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
/*
Specify the path of the file to be analyzed in int main for implementation


*/
using namespace std;

struct Node
{
  int data;
  Node *next;

  Node(int value) : data(value), next(nullptr) {}
};
Node *head = nullptr;
struct Vertex
{
  string word;
  int frequency;
  vector<Vertex *> neighbors;
};

struct PatternNode
{
  string pattern;
  vector<PatternNode *> neighbors;
};

void wordCount(const std::string &filePath)
{
  // Create a graph to store the word relationships
  unordered_map<string, vector<string>> wordGraph;

  // Read the file contents line by line
  ifstream file(filePath);
  if (!file.is_open())
  {
    cout << "Unable to open file." << endl;
    return;
  }

  string line;
  while (getline(file, line))
  {
    // Split the line into words
    vector<string> words;
    stringstream ss(line);
    string word;
    while (getline(ss, word, ' '))
    {
      words.push_back(word);
    }

    // Add the words to the graph
    for (size_t i = 0; i < words.size(); i++)
    {
      if (wordGraph.find(words[i]) == wordGraph.end())
      {
        wordGraph[words[i]] = vector<string>();
      }

      for (size_t j = i + 1; j < words.size(); j++)
      {
        wordGraph[words[i]].push_back(words[j]);
      }
    }
  }

  // Calculate word count using BFS
  queue<string> wordQueue;
  unordered_map<string, int> wordCounts;

  for (const auto &pair : wordGraph)
  {
    wordQueue.push(pair.first);
    wordCounts[pair.first] = 0;
  }

  while (!wordQueue.empty())
  {
    string currentWord = wordQueue.front();
    wordQueue.pop();

    for (const string &neighbor : wordGraph[currentWord])
    {
      wordCounts[currentWord]++;

      if (wordCounts.find(neighbor) == wordCounts.end())
      {
        wordQueue.push(neighbor);
      }
    }
  }

  // Display word counts
  cout << "Word Counts:" << endl;
  for (const auto &pair : wordCounts)
  {
    cout << pair.first << ": " << pair.second << endl;
  }
}
void characterCount(const std::string &filePath)
{
  // Create a graph to store the character relationships
  unordered_map<char, vector<char>> characterGraph;

  // Read the file contents character by character
  ifstream file(filePath);
  if (!file.is_open())
  {
    cout << "Unable to open file." << endl;
    return;
  }

  char character;
  while (file.get(character))
  {
    // Add the character to the graph
    if (characterGraph.find(character) == characterGraph.end())
    {
      characterGraph[character] = vector<char>();
    }

    // Check if the next character exists in the file
    if (file.peek() != EOF)
    {
      file.get(character);
      characterGraph[character].push_back(character);
    }
  }

  // Calculate character count using BFS
  queue<char> characterQueue;
  unordered_map<char, int> characterCounts;

  for (const auto &pair : characterGraph)
  {
    characterQueue.push(pair.first);
    characterCounts[pair.first] = 0;
  }

  while (!characterQueue.empty())
  {
    char currentCharacter = characterQueue.front();
    characterQueue.pop();

    for (const char &neighbor : characterGraph[currentCharacter])
    {
      characterCounts[currentCharacter]++;

      if (characterCounts.find(neighbor) == characterCounts.end())
      {
        characterQueue.push(neighbor);
      }
    }
  }

  // Display character counts
  cout << "Character Counts:" << endl;
  for (const auto &pair : characterCounts)
  {
    cout << pair.first << ": " << pair.second << endl;
  }
}
void lineCount(const std::string &filePath)
{
  // Create a graph to store the line relationships
  unordered_map<string, vector<string>> lineGraph;

  // Read the file contents line by line
  ifstream file(filePath);
  if (!file.is_open())
  {
    cout << "Unable to open file." << endl;
    return;
  }

  string line;
  int lineNumber = 0;
  while (getline(file, line))
  {
    // Add the line to the graph
    if (lineGraph.find(line) == lineGraph.end())
    {
      lineGraph[line] = vector<string>();
    }

    // Check if the next line exists in the file
    if (getline(file, line))
    {
      lineGraph[line].push_back(to_string(lineNumber));
      lineNumber++;
    }
  }

  // Calculate line count using BFS
  queue<string> lineQueue;
  unordered_map<string, int> lineCounts;

  for (const auto &pair : lineGraph)
  {
    lineQueue.push(pair.first);
    lineCounts[pair.first] = 0;
  }

  while (!lineQueue.empty())
  {
    string currentLine = lineQueue.front();
    lineQueue.pop();

    for (const string &neighbor : lineGraph[currentLine])
    {
      lineCounts[currentLine]++;

      if (lineCounts.find(neighbor) == lineCounts.end())
      {
        lineQueue.push(neighbor);
      }
    }
  }

  // Display line counts
  cout << "Line Counts:" << endl;
  for (const auto &pair : lineCounts)
  {
    cout << pair.first << ": " << pair.second << endl;
  }
}
void frequencyAnalysis(const std::string &filePath)
{
  // Create a graph to store the word relationships
  unordered_map<string, Vertex *> wordGraph;

  // Read the file contents line by line
  ifstream file(filePath);
  if (!file.is_open())
  {
    cout << "Unable to open file." << endl;
    return;
  }

  string line;
  while (getline(file, line))
  {
    // Split the line into words
    vector<string> words;
    stringstream ss(line);
    string word;
    while (getline(ss, word, ' '))
    {
      words.push_back(word);
    }

    // Add the words to the graph
    for (size_t i = 0; i < words.size(); i++)
    {
      if (wordGraph.find(words[i]) == wordGraph.end())
      {
        wordGraph[words[i]] = new Vertex;
        wordGraph[words[i]]->word = words[i];
      }

      for (size_t j = i + 1; j < words.size(); j++)
      {
        if (wordGraph.find(words[j]) == wordGraph.end())
        {
          wordGraph[words[j]] = new Vertex;
          wordGraph[words[j]]->word = words[j];
        }

        wordGraph[words[i]]->neighbors.push_back(wordGraph[words[j]]);
        wordGraph[words[j]]->neighbors.push_back(wordGraph[words[i]]);
      }
    }
  }

  // Calculate word frequency using BFS
  queue<Vertex *> wordQueue;
  map<string, int> wordCounts;

  for (const auto &pair : wordGraph)
  {
    wordQueue.push(pair.second);
    wordCounts[pair.first] = 0;
  }

  while (!wordQueue.empty())
  {
    Vertex *currentWord = wordQueue.front();
    wordQueue.pop();

    for (const Vertex *neighbor : currentWord->neighbors)
    {
      wordCounts[currentWord->word]++;

      if (wordCounts.find(neighbor->word) == wordCounts.end())
      {
        wordQueue.push(currentWord);
      }
    }
  }

  // Display word counts
  cout << "Word Frequencies:" << endl;
  for (const auto &pair : wordCounts)
  {
    cout << pair.first << ": " << pair.second << endl;
  }
}
void detectEncodingAndType(const std::string &filePath)
{
  // Read the file contents byte by byte
  ifstream file(filePath, ios::binary);
  if (!file.is_open())
  {
    cout << "Unable to open file." << endl;
    return;
  }

  string fileSignature;
  for (int i = 0; i < 8; ++i)
  {
    char byte;
    if (!file.get(byte))
    {
      cout << "Error reading file." << endl;
      return;
    }
    fileSignature += byte;
  }

  // Check for common image file type signatures
  if (fileSignature == "\xFF\xD8\xFF\xE0\x00\x10\x4A\x46")
  {
    cout << "File type: JPEG image" << endl;
  }
  else if (fileSignature == "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A")
  {
    cout << "File type: PNG image" << endl;
  }
  else if (fileSignature.substr(0, 6) == "\x47\x49\x46\x38\x37\x61")
  {
    cout << "File type: GIF87a image" << endl;
  }
  else if (fileSignature.substr(0, 6) == "\x47\x49\x46\x38\x39\x61")
  {
    cout << "File type: GIF89a image" << endl;
  }
  else if (fileSignature.substr(0, 2) == "BM")
  {
    cout << "File type: BMP image" << endl;
  }
  else
  {
    cout << "File type: Unknown or not supported" << endl;
  }
}

int main()
{
  string filePath = "F:\\alokik\\DSA\\neewww\\hello.txt";//Please change the file path as required

  wordCount(filePath);

  characterCount(filePath);

  lineCount(filePath);

  frequencyAnalysis(filePath);

  detectEncodingAndType(filePath);

  return 0;
}
