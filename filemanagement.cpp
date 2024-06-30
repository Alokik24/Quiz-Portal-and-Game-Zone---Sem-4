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
#include <cstdlib>

#define FILE_BUFFER_SIZE 1024
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
struct FileNode
{
  string fileName;
  FileNode *left;
  FileNode *right;

  FileNode(const string &name) : fileName(name), left(nullptr), right(nullptr) {}
};
FileNode *searchFileNode(FileNode *root, const std::string &fileName)
{
  if (root == nullptr)
  {
    return nullptr;
  }

  if (root->fileName == fileName)
  {
    return root;
  }
  else if (fileName < root->fileName)
  {
    return searchFileNode(root->left, fileName);
  }
  else
  {
    return searchFileNode(root->right, fileName);
  }
}
FileNode *root = nullptr;
class File
{
public:
  File(const string &filename, int size) : filename(filename), size(size) {}

  // Method to check if the file exists
  bool exists() const
  {
    ifstream fileStream(filename);
    return fileStream.good();
  }

  // Method to read file contents
  string read() const
  {
    ifstream fileStream(filename);
    string fileContent;
    if (fileStream.is_open())
    {
      std::string line;
      while (getline(fileStream, line))
      {
        fileContent += line + "\n";
      }
      fileStream.close();
    }
    return fileContent;
  }

  // Method to write to a file
  void write(const std::string &content)
  {
    std::ofstream fileStream(filename);
    if (fileStream.is_open())
    {
      fileStream << content;
      fileStream.close();
    }
    else
    {
      std::cerr << "Unable to open file for writing: " << filename << std::endl;
    }
  }

  // Additional methods or attributes can be added as needed

private:
  std::string filename;
  int size;
};
struct FileInfo
{
  string fileName;
  size_t fileSize;
  time_t lastWriteTime;
};
struct Directory
{
  string directoryPath;
  unordered_map<string, FileInfo> files;
  vector<Directory> subdirectories;

  explicit Directory(const std::string &path) : directoryPath(path) {}
};
struct DirectoryNode
{
  string directoryName;
};
DirectoryNode *sourceDirectoryNode = nullptr;
Directory rootDirectory("/");

unordered_map<string, string> user_credentials;

void createFile(FileNode *&root)
{
  string fileName;
  string directory;

  cout << "Enter file name: ";
  cin >> fileName;

  cout << "Enter directory path: ";
  cin >> directory;

  string filePath = directory + "/" + fileName;

  ofstream file(filePath); // Creating a file with the specified path

  if (!file.is_open())
  {
    cout << "Failed to create the file." << endl;
    return;
  }

  file.close();

  FileNode *newFileNode = new FileNode(fileName);

  if (root == nullptr)
  {
    root = newFileNode;
    return;
  }

  FileNode *current = root;
  while (true)
  {
    if (fileName < current->fileName)
    {
      if (current->left == nullptr)
      {
        current->left = newFileNode;
        break;
      }
      else
      {
        current = current->left;
      }
    }
    else
    {
      if (current->right == nullptr)
      {
        current->right = newFileNode;
        break;
      }
      else
      {
        current = current->right;
      }
    }
  }
}
void openFile()
{
  string fileName;
  cout << "Enter file name to open: ";
  cin >> fileName;

  FileNode *targetFileNode = searchFileNode(root, fileName);
  if (targetFileNode == nullptr)
  {
    cout << "File not found." << endl;
    return;
  }

  // Open the file using the appropriate editor or application
  cout << "Opening file: " << targetFileNode->fileName << endl;

  // Wrap the file path in double quotes
  string command = "start \"" + targetFileNode->fileName + "\"";
  system(command.c_str());
}
void editFile()
{
  string fileName;
  cout << "Enter file name to edit: ";
  cin >> fileName;

  // Search for the file node in the binary search tree
  FileNode *targetFileNode = searchFileNode(root, fileName);
  if (targetFileNode == nullptr)
  {
    std::cout << "File not found." << std::endl;
    return;
  }

  ifstream inputFile(targetFileNode->fileName);
  if (!inputFile.is_open())
  {
    std::cout << "Error opening file for editing." << std::endl;
    return;
  }

  std::cout << "Current content of the file:\n";
  std::string line;
  while (getline(inputFile, line))
  {
    std::cout << line << std::endl;
  }
  inputFile.close();

  cout << "Enter text to append (enter '#' to stop):" << std::endl;
  string newText;
  cin.ignore(); // Ignore previous '\n'
  while (true)
  {
    std::getline(cin, line);
    if (line == "#")
    {
      break;
    }
    newText += line + "\n";
  }

  ofstream outputFile(targetFileNode->fileName, std::ios_base::app);
  if (!outputFile.is_open())
  {
    std::cout << "Error opening file for editing." << endl;
    return;
  }

  outputFile << newText;
  outputFile.close();

  cout << "File edited successfully." << endl;
}
void createDirectory(Directory &currentDirectory)
{
  string directoryName;
  cout << "Enter directory name: ";
  cin >> directoryName;

  Directory newDirectory(currentDirectory.directoryPath + "/" + directoryName);

  currentDirectory.subdirectories.push_back(newDirectory);
  cout << "Directory created successfully!" << endl;
}

void renameFile(FileNode *&root, const string &fileName, const string &newFileName)
{
  FileNode *targetFileNode = searchFileNode(root, fileName);
  if (targetFileNode == nullptr)
  {
    cout << "File not found." << endl;
    return;
  }

  string sourceFilePath = targetFileNode->fileName;
  string directoryPath = sourceFilePath.substr(0, sourceFilePath.find_last_of("/\\") + 1);
  string destinationFilePath = directoryPath + newFileName;

  if (rename(sourceFilePath.c_str(), destinationFilePath.c_str()) == 0)
  {
    targetFileNode->fileName = destinationFilePath; // Update file node in memory
    cout << "File renamed successfully: " << fileName << " -> " << newFileName << endl;
  }
  else
  {
    std::cout << "Failed to rename file." << std::endl;
  }
}

void registerUser()
{
  string username, password;
  cout << "\nEnter a new username: ";
  cin >> username;

  // Check if the username already exists
  if (user_credentials.find(username) != user_credentials.end())
  {
    cout << "Username already exists. Please choose a different one.\n";
    return;
  }

  cout << "Enter a password: ";
  cin >> password;

  // Add the new user to the credentials map
  user_credentials[username] = password;
  cout << "Registration successful!\n";
}
bool authenticateUser(const string &username, const string &password)
{
  auto it = user_credentials.find(username);
  if (it != user_credentials.end() && it->second == password)
  {
    cout << "\nLogin successful!" << endl;
    return true;
  }
  cout << "Invalid username or password. Please try again." << endl;
  return false;
}
bool fileManagementMenu(FileNode *&root, DirectoryNode *sourceDirectoryNode)
{
  bool returnToMainMenu = false;
  do
  {
    cout << "\nFile Management Menu\n";
    cout << "1. Create File\n";
    cout << "2. Open File\n";
    cout << "3. Edit File\n";
    cout << "4. Create Directory\n";
    cout << "5. Rename File\n";
    cout << "6. Return to Main Menu\n";

    int choice;
    cout << "\nEnter your choice: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
      createFile(root);
      break;
    case 2:
      openFile();
      break;
    case 3:
      editFile();
      break;
    case 4:
      createDirectory(rootDirectory);
      break;
    case 5:
    {
      string fileName, newFileName;
      cout << "Enter file name: ";
      cin >> fileName;
      cout << "Enter new file name: ";
      cin >> newFileName;
      renameFile(root, fileName, newFileName);
      break;
    }
    case 6:
      returnToMainMenu = true;
      break;
    default:
      cout << "Invalid choice. Please enter a valid option." << endl;
      break;
    }
  } while (!returnToMainMenu); // Continue the loop until the flag is set to true

  return returnToMainMenu; // Return whether to go back to the main menu
}

void cleanup()
{
  Node *current = head;
  while (current != nullptr)
  {
    Node *temp = current;
    current = current->next;
    delete temp;
  }
  head = nullptr;
}
void exitProgram()
{
  std::cout << "Exiting the program...\n";
  cleanup();
  exit(0);
}

int main()
{
  int choice;
  std::string username, password;
  Directory rootDirectory("/");

  while (true)
  {
    std::cout << "File Management System\n";
    std::cout << "1. Register\n";
    std::cout << "2. Login\n";
    std::cout << "3. Exit\n";
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    switch (choice)
    {
    case 1:
      registerUser();
      break;
    case 2:
      std::cout << "Enter username: ";
      std::cin >> username;
      std::cout << "Enter password: ";
      std::cin >> password;
      if (authenticateUser(username, password))
      {
        do
        {
          std::cout << "\nFile Management Menu\n";
          std::cout << "1. File Operations\n";
          std::cout << "2. Logout\n";
          std::cout << "Enter your choice: ";
          std::cin >> choice;

          switch (choice)
          {
          case 1:
            if (fileManagementMenu(root, sourceDirectoryNode))
            {
              return 0; // If true, return to main menu
            }
            break;
          case 2:
            std::cout << "Logged out.\n";
            break;
          default:
            std::cout << "Invalid choice. Please enter a valid option." << std::endl;
            break;
          }
        } while (choice != 2);
      }
      else
      {
        std::cout << "Authentication failed. Please try again." << std::endl;
      }
      break;
    case 3:
      exitProgram();
      break;
    default:
      std::cout << "Invalid choice. Please enter a valid option." << std::endl;
      break;
    }
  }

  return 0;
}
