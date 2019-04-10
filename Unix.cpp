#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

class Node
{
  private:
    string Name;
    int diskUsage; //store the disk usage per node
    Node *rightChild;
    Node *leftChild;
    bool isDirectory;//tells whether the node is a file or a directory

  public:
    Node(bool isDirectory)
    {
        isDirectory = isDirectory;
        diskUsage = 0;
        rightChild = NULL;
        leftChild = NULL;
    }
    bool Node_isDirectory()
    {
        return isDirectory;
    }
    friend class FileSystem;
};

//=====================================================================================================
//=====================================================================================================
class FileSystem
{
  public:
    //=======================================
    FileSystem();//constructor
    //=======================================
    Node *createNode(string inputName, bool isDirectory); //creaters a node
    //=======================================
    vector<string> ReadCommands();//reads commands from the given file
    //=======================================
    void mkdir(string DirectoryName);//creates a directory
    //=======================================
    void createFile(string FileName);//creates a file
    //=======================================
    void CallFunction(string command, string name);//calls the function that require a file/directory name
    //=========================================================
    void CallFunction(string command);//calls the functions that don't require a file/directory name and prints out errors when commands are called without specifying a file/directory name
    //=========================================================
    void ProcessCommands(vector<string> commands); //checks the strings and checks which command to call
    //=========================================================
    void changeDirectory_toRoot() //changes current directory to the root directory
    {
        currentDirectory = rootDirectory;
    }
    //=======================================
    string pwd()//prints the name of the current directory
    {
        if (currentDirectory != NULL)
        {
            return currentDirectory->Name;
        }
        return "Error:No Directory\n";
    }
    //=======================================
    int quota()//returns the disk usage of the current directory
    {
        return nodeSize(currentDirectory);
    }

    //=========================================================
    //=========================================================
    void ls()//prints the contents of the current directory
    {
        cout << endl
             << "Current directory: " << endl;
        printDirectory(currentDirectory);
        cout << "_____________________" << endl;
    }
    //=========================================================
    //=========================================================
    void printDirectory(Node *node)//prints the contents of the directory
    {
        if (node != NULL)
        {
            cout << node->Name << " " << node->Node_isDirectory() << endl;

            printDirectory(node->leftChild);

            printDirectory(node->rightChild);
        }
    }
    //========================================================
    //========================================================
    int nodeSize(Node *node)//computes the disk usage
    {
        int sum = 0;
        if (node == NULL)
        {
            return 0;
        }
        else if (node->isDirectory)
        {
            sum = 3;
        }
        else
        {
            sum = 7;
        }
        return sum + nodeSize(node->leftChild) + nodeSize(node->rightChild);
    }

    //=========================================================
    //=========================================================
    void printDirectoryAsTree(Node *node, vector<string> &input)
    {
        input.push_back(node->Name);
        if (node->leftChild != NULL)
        {
            input.push_back(node->leftChild->Name);
        }

        if (node->rightChild != NULL)
        {
            input.push_back(node->rightChild->Name);
        }
    }
    //=========================================================
    //=========================================================
    void lsAsTree()//prints the contents of the directory in the form of a tree
    {
        vector<string> contents;
        printDirectoryAsTree(currentDirectory, contents);
        cout << "Contents of the directory as a tree structure: " << endl;
        if (contents.size() == 3)
        {
            string space = "";
            string branch = "";
            for (int i = 0; i < contents[0].size(); i++)
            {
                space += " ";
                branch += "_";
            }
            cout << endl
                 << space << space << " ___" << contents[1] << endl;

            cout << contents[0] << branch << "|" << endl;

            cout << space << space << "|__" << contents[2] << endl
                 << endl;
        }

        else if (contents.size() == 2)
        {
            string space = "";
            string branch = "";
            for (int i = 0; i < contents[0].size(); i++)
            {
                space += " ";
                branch += "_";
            }

            cout << contents[0] << branch << endl;

            cout << space << space << "|__" << contents[1] << endl
                 << endl;
        }
        else
        {
            cout << contents[0] << endl;
        }
    }
    //=========================================================
    //=======================================

    void changeDirectory(string fileName)//changes the directory to a specified directory
    {
        if (currentDirectory->leftChild != NULL)
        {
            if (currentDirectory->leftChild->Name == fileName)
            {
                currentDirectory = currentDirectory->leftChild;
                return;
            }
        }

        if (currentDirectory->rightChild != NULL)
        {
            if (currentDirectory->rightChild->Name == fileName)
            {
                currentDirectory = currentDirectory->leftChild;
                return;
            }
        }
        currentDirectory = currentDirectory->rightChild;

        cout << "error:no such directory in current directory\n";
    }
    //=======================================
    //=======================================
    void removeNode(Node *&node)
    {
        if (node != NULL)
        {
            removeNode(node->leftChild);
            removeNode(node->rightChild);
            delete node;
            node = NULL;
        }
    }
    //==================================================================================
    void rm(string File_DirectoryName)//deletes specified file/directory
    {
        if (currentDirectory->leftChild != NULL)
        {
            if (currentDirectory->leftChild->Name == File_DirectoryName)
            {
                removeNode(currentDirectory->leftChild);
                return;
            }
        }

        if (currentDirectory->rightChild != NULL)
        {
            if (currentDirectory->rightChild->Name == File_DirectoryName)
            {

                removeNode(currentDirectory->rightChild);
                return;
            }
        }

        cout << "error:no such file or directory in current directory\n";
    }

  private:
    Node *currentDirectory;
    Node *rootDirectory;
};
/*=======================================
    FUNCTUONS
=======================================*/
FileSystem::FileSystem()
{
    Node *Root = new Node(true);
    rootDirectory = Root;
    rootDirectory->Name = "/";
    currentDirectory = Root;
    currentDirectory->diskUsage += 3;
}
//=====================================================================================================
//=====================================================================================================
Node *FileSystem::createNode(string inputName, bool isDirectory)
{
    Node *tempNode = new Node(isDirectory);

    tempNode->Name = inputName;
    tempNode->rightChild = NULL;
    tempNode->leftChild = NULL;
    tempNode->Parent_Node = currentDirectory; //the parent is the current directory
    tempNode->isDirectory = isDirectory;

    return tempNode;
}
//=====================================================================================================
//=====================================================================================================
void FileSystem::mkdir(string DirectoryName)
{

    Node *tempNode = createNode(DirectoryName, true);

    if (currentDirectory->leftChild == NULL)
    {                                           //If we dont have a directory
        currentDirectory->leftChild = tempNode; //we create one
    }

    else if (currentDirectory->rightChild == NULL)
    {
        currentDirectory->rightChild = tempNode; //we create one
    }

    else
    {
        cout << "error: capacity exceeded.\n";
    }
}
//=====================================================================================================
//=====================================================================================================
void FileSystem::createFile(string FileName)
{

    Node *tempNode = createNode(FileName, false);

    if (currentDirectory->leftChild == NULL)
    {                                           //If we dont have a directory
        currentDirectory->leftChild = tempNode; //we create one
        cout << FileName << " created.\n";
    }

    else if (currentDirectory->rightChild == NULL)
    {
        currentDirectory->rightChild = tempNode; //we create one
        cout << FileName << " created.\n";
    }

    else
    {
        cout << "error: capacity exceeded.\n";
    }
}
//=====================================================================================================
//=====================================================================================================
void FileSystem::CallFunction(string command, string name)
{
    // cout<<"Command "<<command<<endl;

    if (command == "cd")
    {
        changeDirectory(name);
    }
    else if (command == "createfile")
    {
        createFile(name);
    }
    else if (command == "mkdir")
    {
        mkdir(name);
    }
    else if (command == "rm")
    {
        rm(name);
    }

    else
    {
        cout << command << " :command not found" << endl;
    }
}
//=====================================================================================================
//=====================================================================================================
void FileSystem::CallFunction(string command)
{
    // cout<<"Command "<<command<<endl;
    if (command == "cd")
    {
        changeDirectory_toRoot();
    }
    else if (command == "createfile")
    {
        cout << "error: cannot create file" << endl;
    }
    else if (command == "mkdir")
    {
        cout << "error: cannot create directory" << endl;
    }
    else if (command == "rm")
    {
        cout << "error: no such file or directory" << endl;
    }
    else if (command == "ls")
    {
        ls();
    }
    else if (command == "ls -i")
    {
        lsAsTree();
    }
    else if (command == "pwd")
    {
        cout << "Name of current directory: " << pwd() << endl;
    }
    else if (command == "quota")
    {
        cout << pwd() << "'s current disk usage is: " << quota() << " bytes." << endl
             << endl;
    }
    else
    {
        cout << command << " :command not found" << endl;
    }
}

//=====================================================================================================
//=====================================================================================================
vector<string> FileSystem::ReadCommands()
{
    vector<string> commands;
    fstream commandFile;

    commandFile.open("commands.txt");
    string line;
    while (!commandFile.eof())
    {
        getline(commandFile, line);
        commands.push_back(line);
    }
    commandFile.close();
    return commands;
}
//=====================================================================================================
//=====================================================================================================
void FileSystem::ProcessCommands(vector<string> commands)
{
    for (int i = 0; i < commands.size(); i++)
    {
        string command = "";
        string param1 = "";
        for (int j = 0; j < commands[i].length(); j++)
        {
            if (commands[i][j + 1] == '-' && commands[i][j + 2] == 'i')
            {
                command = commands[i].substr(0, j + 3);
                break;
            }
            else if (commands[i][j] == ' ')
            {
                command = commands[i].substr(0, j);
                if (commands[i].length() > j + 1)
                {

                    param1 = commands[i].substr(j + 1, commands[i].length() - 1);
                }
                break;
            }
            else
            {
                command += commands[i][j];
            }
        }
        // cout<<command<<endl;
        if (param1.length() == 0)
        {
            CallFunction(command);
        }
        else
        {
            CallFunction(command, param1);
        }
        cout << endl;
    }
}
//=====================================================================================================
//=====================================================================================================

//===================================================================
int main()
{
    FileSystem test;
    cout << ">Unix.exe commands.txt" << endl;
    test.ProcessCommands(test.ReadCommands());
}