#include <iostream>
#include <list>
#include <unordered_map>
#include <sstream>
#include <regex>
#include <vector>
#include <stack>
#include <queue>

class FileData
{
public:
  std::string data;
  std::vector<std::string> versionHistory;
  std::stack<std::string> undoStack;
  std::queue<std::string> redoQueue;

  FileData() {}

  FileData(const std::string &data) : data(data)
  {
    versionHistory.push_back(data);
  }
};

class FileGraph
{
private:
  std::unordered_map<std::string, std::list<std::string>> graph;
  std::unordered_map<std::string, FileData> fileData;

public:
  void addFile(const std::string &fileName)
  {
    graph[fileName] = std::list<std::string>();
    fileData[fileName] = FileData();
    std::cout << "File '" << fileName << "' added successfully." << std::endl;
  }

  void removeFile(const std::string &fileName)
  {
    graph.erase(fileName);
    fileData.erase(fileName);
    std::cout << "File '" << fileName << "' removed successfully." << std::endl;
  }

  void editFile(const std::string &fileName)
  {
    if (fileData.find(fileName) != fileData.end())
    {
      std::string newData;
      std::cout << "Enter new data for file '" << fileName << "': ";
      std::cin.ignore();
      std::getline(std::cin, newData);

      // Save the current data to the undo stack
      fileData[fileName].undoStack.push(fileData[fileName].data);

      fileData[fileName].data = newData;
      fileData[fileName].versionHistory.push_back(newData);

      // Clear the redo queue when a new edit is made
      while (!fileData[fileName].redoQueue.empty())
      {
        fileData[fileName].redoQueue.pop();
      }

      std::cout << "File data updated successfully. New version created." << std::endl;
    }
    else
    {
      std::cout << "File '" << fileName << "' not found." << std::endl;
    }
  }

  void undoEdit(const std::string &fileName)
  {
    if (fileData.find(fileName) != fileData.end() && !fileData[fileName].undoStack.empty())
    {
      // Move the current data to the redo queue
      fileData[fileName].redoQueue.push(fileData[fileName].data);

      // Pop the previous state from the undo stack
      fileData[fileName].data = fileData[fileName].undoStack.top();
      fileData[fileName].undoStack.pop();

      std::cout << "Undo operation successful." << std::endl;
    }
    else
    {
      std::cout << "Undo not possible for file '" << fileName << "'." << std::endl;
    }
  }

  void redoEdit(const std::string &fileName)
  {
    if (fileData.find(fileName) != fileData.end() && !fileData[fileName].redoQueue.empty())
    {
      // Move the current data to the undo stack
      fileData[fileName].undoStack.push(fileData[fileName].data);

      // Pop the next state from the redo queue
      fileData[fileName].data = fileData[fileName].redoQueue.front();
      fileData[fileName].redoQueue.pop();

      std::cout << "Redo operation successful." << std::endl;
    }
    else
    {
      std::cout << "Redo not possible for file '" << fileName << "'." << std::endl;
    }
  }

  void displayVersionHistory(const std::string &fileName)
  {
    if (fileData.find(fileName) != fileData.end())
    {
      std::cout << "Version history for file '" << fileName << "':\n";
      for (size_t i = 0; i < fileData[fileName].versionHistory.size(); ++i)
      {
        std::cout << "Version " << i + 1 << ":\n"
                  << fileData[fileName].versionHistory[i] << "\n\n";
      }
    }
    else
    {
      std::cout << "File '" << fileName << "' not found." << std::endl;
    }
  }

  void displayFiles()
  {
    std::cout << "Files in the system: ";
    for (const auto &file : graph)
    {
      std::cout << file.first << " ";
    }
    std::cout << std::endl;
  }

  void displayFileData(const std::string &fileName)
  {
    if (fileData.find(fileName) != fileData.end())
    {
      std::cout << "Data in file '" << fileName << "': " << fileData[fileName].data << std::endl;
    }
    else
    {
      std::cout << "File '" << fileName << "' not found." << std::endl;
    }
  }

  void analyzeFileData(const std::string &fileName)
  {
    if (fileData.find(fileName) != fileData.end())
    {
      std::istringstream iss(fileData[fileName].data);
      std::string word;
      int wordCount = 0;
      while (iss >> word)
      {
        ++wordCount;
      }
      std::cout << "Analysis of file '" << fileName << "': Word count = " << wordCount << std::endl;
    }
    else
    {
      std::cout << "File '" << fileName << "' not found." << std::endl;
    }
  }

  void compareFiles(const std::string &fileName1, const std::string &fileName2)
  {
    if (fileData.find(fileName1) != fileData.end() && fileData.find(fileName2) != fileData.end())
    {
      if (fileData[fileName1].data == fileData[fileName2].data)
      {
        std::cout << "Files '" << fileName1 << "' and '" << fileName2 << "' have the same data." << std::endl;
      }
      else
      {
        std::cout << "Files '" << fileName1 << "' and '" << fileName2 << "' have different data." << std::endl;
      }
    }
    else
    {
      std::cout << "One or more files not found." << std::endl;
    }
  }

  void autoComplete(const std::string &fileName, const std::string &partialWord)
  {
    if (fileData.find(fileName) != fileData.end())
    {
      std::istringstream iss(fileData[fileName].data);
      std::string word;
      std::vector<std::string> suggestions;

      while (iss >> word)
      {
        if (word.find(partialWord) == 0)
        {
          suggestions.push_back(word);
        }
      }

      if (!suggestions.empty())
      {
        std::cout << "Auto-complete suggestions for '" << partialWord << "' in file '" << fileName << "': ";
        for (const auto &suggestion : suggestions)
        {
          std::cout << suggestion << " ";
        }
        std::cout << std::endl;
      }
      else
      {
        std::cout << "No suggestions found for '" << partialWord << "' in file '" << fileName << "'." << std::endl;
      }
    }
    else
    {
      std::cout << "File '" << fileName << "' not found." << std::endl;
    }
  }
  void renameFile(const std::string &oldFileName, const std::string &newFileName)
  {
    auto it = fileData.find(oldFileName);
    if (it != fileData.end())
    {
      // Update fileData and graph with the new file name
      fileData[newFileName] = std::move(it->second);
      fileData.erase(it);

      // Update dependencies in the graph
      graph[newFileName] = std::move(graph[oldFileName]);
      graph.erase(oldFileName);

      std::cout << "File '" << oldFileName << "' renamed to '" << newFileName << "' successfully." << std::endl;
    }
    else
    {
      std::cout << "File '" << oldFileName << "' not found." << std::endl;
    }
  }
  void advancedSearch(const std::string &fileName, const std::string &pattern)
  {
    if (fileData.find(fileName) != fileData.end())
    {
      std::istringstream iss(fileData[fileName].data);
      std::string line;
      int lineNumber = 1;

      while (std::getline(iss, line))
      {
        if (std::regex_search(line, std::regex(pattern)))
        {
          std::cout << "Match found in file '" << fileName << "' at line " << lineNumber << ": " << line << std::endl;
        }
        lineNumber++;
      }
    }
    else
    {
      std::cout << "File '" << fileName << "' not found." << std::endl;
    }
  }
};

int main()
{
  FileGraph fileGraph;

  int choice;
  do
  {
    std::cout << "\nMenu:\n";
    std::cout << "1. Add File\n";
    std::cout << "2. Remove File\n";
    std::cout << "3. Edit File\n";
    std::cout << "4. Undo Edit\n";
    std::cout << "5. Redo Edit\n";
    std::cout << "6. Rename File\n";
    std::cout << "7. Display Files\n";
    std::cout << "8. Display File Data\n";
    std::cout << "9. Analyze File Data\n";
    std::cout << "10. Compare Files\n";
    std::cout << "11. Auto-Complete\n";
    std::cout << "12. Advanced Search\n";
    std::cout << "13. Display Version History\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    switch (choice)
    {
    case 1:
    {
      std::string fileName;
      std::cout << "Enter file name: ";
      std::cin >> fileName;
      fileGraph.addFile(fileName);
      break;
    }
    case 2:
    {
      std::string fileName;
      std::cout << "Enter file name: ";
      std::cin >> fileName;
      fileGraph.removeFile(fileName);
      break;
    }
    case 3:
    {
      std::string fileName;
      std::cout << "Enter file name: ";
      std::cin >> fileName;
      fileGraph.editFile(fileName);
      break;
    }
    case 4:
    {
      std::string fileName;
      std::cout << "Enter file name: ";
      std::cin >> fileName;
      fileGraph.undoEdit(fileName);
      break;
    }
    case 5:
    {
      std::string fileName;
      std::cout << "Enter file name: ";
      std::cin >> fileName;
      fileGraph.redoEdit(fileName);
      break;
    }
    case 6:
    {
      std::string oldFileName, newFileName;
      std::cout << "Enter old file name: ";
      std::cin >> oldFileName;
      std::cout << "Enter new file name: ";
      std::cin >> newFileName;
      fileGraph.renameFile(oldFileName, newFileName);
      break;
    }
    case 7:
    {
      fileGraph.displayFiles();
      break;
    }
    case 8:
    {
      std::string fileName;
      std::cout << "Enter file name: ";
      std::cin >> fileName;
      fileGraph.displayFileData(fileName);
      break;
    }
    case 9:
    {
      std::string fileName;
      std::cout << "Enter file name: ";
      std::cin >> fileName;
      fileGraph.analyzeFileData(fileName);
      break;
    }
    case 10:
    {
      std::string fileName1, fileName2;
      std::cout << "Enter first file name: ";
      std::cin >> fileName1;
      std::cout << "Enter second file name: ";
      std::cin >> fileName2;
      fileGraph.compareFiles(fileName1, fileName2);
      break;
    }
    case 11:
    {
      std::string fileName, partialWord;
      std::cout << "Enter file name: ";
      std::cin >> fileName;
      std::cout << "Enter partial word for auto-complete: ";
      std::cin >> partialWord;
      fileGraph.autoComplete(fileName, partialWord);
      break;
    }
    case 12:
    {
      std::string fileName, pattern;
      std::cout << "Enter file name: ";
      std::cin >> fileName;
      std::cout << "Enter search pattern (regular expression): ";
      std::cin.ignore();
      std::getline(std::cin, pattern);
      fileGraph.advancedSearch(fileName, pattern);
      break;
    }
    case 13:
    {
      std::string fileName;
      std::cout << "Enter file name: ";
      std::cin >> fileName;
      fileGraph.displayVersionHistory(fileName);
      break;
    }
    case 0:
      std::cout << "Exiting program.\n";
      break;
    default:
      std::cout << "Invalid choice. Please try again.\n";
    }
  } while (choice != 0);

  return 0;
}
