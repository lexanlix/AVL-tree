#include <iostream>
#include <fstream>
#include <string>

using namespace std;


struct TreeNode
{
	TreeNode(string m, int n);
	~TreeNode() {}

	TreeNode* NodeRight,
		* NodeLeft,
		* ParentNode;

	void GetInfo() { cout << "\n|" << nodeValue << "| " << nodeMean; }

	string nodeMean;
	int nodeValue;
};

TreeNode::TreeNode(string m, int n)
{
	nodeMean = m;
	nodeValue = n;
}

TreeNode* StartNode;

void Output();
void StepOutput(TreeNode* curr);
void AddToStart(string mean, int value);
int Height(TreeNode* current);

void Output()
{
	if (StartNode == NULL)
	{
		cout << "\nError. Tree is not created!\n";
		return;
	}
	StepOutput(StartNode); cout << "\n";
}

void StepOutput(TreeNode* curr) // Рекурсивно выводит значения всех узлов от меньшего к большему
{
	TreeNode* ukazatel = curr;
	if (ukazatel->NodeLeft != NULL)
		StepOutput(ukazatel->NodeLeft);
	
	cout << "\n" << ukazatel->nodeMean;
	
	if (ukazatel->NodeRight != NULL)
		StepOutput(ukazatel->NodeRight);

	return;
}

void AddToStart(string mean, int value)
{
	TreeNode* temp = new TreeNode(mean, value);
	StartNode = temp;
	temp->NodeRight = NULL;
	temp->NodeLeft = NULL;
	temp->ParentNode = NULL;
}

int Height(TreeNode* current)
{
	if (current == NULL)
		return(0);
	int LeftHeight = 0, RightHeight = 0;
	if (current->NodeLeft != NULL)
		LeftHeight = Height(current->NodeLeft);
	if (current->NodeRight != NULL)
		RightHeight = Height(current->NodeRight);
	if ((current->NodeLeft == NULL) and (current->NodeRight == NULL))
		return(1);
	if (LeftHeight > RightHeight)
		return(LeftHeight + 1);
	else
		return(RightHeight + 1);
}

struct Tree
{
	Tree() {}
	~Tree() {}

	void Init();
	void Read();
	TreeNode * Search(TreeNode* current, int mean);
	void Add(TreeNode* current, string mean, int value);
	void DestroyNode(TreeNode* current, int mean);
	void Balancing(TreeNode* current);
};

void Tree::Init()
{
	StartNode = NULL;
}

void Tree::Read()
{
	string line;
	ifstream file("C:\\Users\\Алексей\\source\\repos\\Kursovaya\\Data.txt", ios::in);   //"C:\\Users\\Алексей\\source\\repos\\Kursovaya\\Data.txt"

	if (!file.is_open())
		cout << "\nFile can not to be open!\n";
	else
	{
		while (!file.eof())
		{
			getline(file, line);
			if (line == "")
				break;
			string small = line.substr(0, 3);  // Возвращает строке small подстроку строки line
			int number = stoi(small);          // Подстроку small преобразовывает в тип int

			cout << "\n" << line;

			if (StartNode == NULL)
				AddToStart(line, number);
			else
				Add(StartNode, line, number);
		}
	}
	file.close();
};

TreeNode * Tree::Search(TreeNode* current, int mean)
{
	if (current == NULL)
		return(current);
	if (current->nodeValue == mean)
		return(current);
	if (current->nodeValue > mean)
		Search(current->NodeLeft, mean);
	else
		Search(current->NodeRight, mean);
}

void Tree::Add(TreeNode* current, string mean, int value)
{
	if (current->nodeValue == value)
	{
		cout << "\nThis info is in Tree already.\n";
		return;
	}

	if (current->nodeValue < value)
		if (current->NodeRight == NULL)
		{
			TreeNode* temp = new TreeNode(mean, value);
			current->NodeRight = temp;
			temp->NodeRight = NULL;
			temp->NodeLeft = NULL;
			temp->ParentNode = current;
		}
		else
			Add(current->NodeRight, mean, value);
	else
		if (current->NodeLeft == NULL)
		{
			TreeNode* temp = new TreeNode(mean, value);
			current->NodeLeft = temp;
			temp->NodeRight = NULL;
			temp->NodeLeft = NULL;
			temp->ParentNode = current;
		}
		else
			Add(current->NodeLeft, mean, value);

	Balancing(current);
}

void Tree::DestroyNode(TreeNode* current, int mean)
{
	if (current == NULL)
		return;
	if (current->nodeValue > mean)
		DestroyNode(current->NodeLeft, mean);
	else
		DestroyNode(current->NodeRight, mean);
	if (current->nodeValue == mean)
	{
		if (current->NodeLeft == NULL)
		{
			if (current->NodeRight == NULL)
			{
				if (current->ParentNode == NULL)		//Если это единственный узел
				{
					StartNode = NULL;
					delete current;
					current = NULL;
				}
				else
				{
					if (current->nodeValue < current->ParentNode->nodeValue)
						current->ParentNode->NodeLeft = NULL;
					else
						current->ParentNode->NodeRight = NULL;
					current->ParentNode = NULL;
					delete current;
					current = NULL;
				}
			}
			else
			{
				TreeNode* temp = current->NodeRight;
				if (current->ParentNode == NULL)		//Если current - стартовый узел
				{
					StartNode = temp;
					temp->ParentNode = NULL;
					current->NodeRight = NULL;
					delete current;
					current = NULL;
				}
				else
				{
					if (current->nodeValue < current->ParentNode->nodeValue)
						current->ParentNode->NodeLeft = temp;
					else
						current->ParentNode->NodeRight = temp;
					temp->ParentNode = current->ParentNode;

					current->NodeRight = NULL;
					current->ParentNode = NULL;
					delete current;
					current = NULL;
				}
			}
		}
		else
		{
			TreeNode* tempLeft = current->NodeLeft;
			while (tempLeft->NodeRight != NULL)
				tempLeft = tempLeft->NodeRight;

			if (tempLeft->nodeValue < tempLeft->ParentNode->nodeValue)
			{
				if (current->ParentNode == NULL)		//Если current - стартовый узел
				{
					StartNode = tempLeft;
					tempLeft->ParentNode = NULL;
					tempLeft->NodeRight = current->NodeRight;
					if (tempLeft->NodeRight != 0)
						tempLeft->NodeRight->ParentNode = tempLeft;
					current->NodeRight = NULL;
					current->NodeLeft = NULL;
					delete current;
					current = NULL;
				}
				else
				{
					if (current->nodeValue < current->ParentNode->nodeValue)
						current->ParentNode->NodeLeft = tempLeft;
					else
						current->ParentNode->NodeRight = tempLeft;
					tempLeft->ParentNode = current->ParentNode;
					tempLeft->NodeRight = current->NodeRight;
					if (tempLeft->NodeRight != NULL)
						tempLeft->NodeRight->ParentNode = tempLeft;		//Переназначаем у ребенка этого узла родителя(этот узел)
					current->ParentNode = NULL;
					current->NodeLeft = NULL;
					current->NodeRight = NULL;
					delete current;
					current = NULL;
				}
			}
			else
			{
				if (tempLeft->NodeLeft == NULL)
				{
					tempLeft->ParentNode->NodeRight = NULL;
					if (current->ParentNode == NULL)		//Если current - стартовый узел
					{
						StartNode = tempLeft;
						tempLeft->ParentNode = NULL;
						tempLeft->NodeLeft = current->NodeLeft;
						tempLeft->NodeLeft->ParentNode = tempLeft;
						tempLeft->NodeRight = current->NodeRight;
						if (tempLeft->NodeRight != 0)
							tempLeft->NodeRight->ParentNode = tempLeft;		//Переназначаем у ребенка этого узла родителя(этот узел)
						current->NodeLeft = NULL;
						current->NodeRight = NULL;
						delete current;
						current = NULL;
					}
					else
					{
						if (current->nodeValue < current->ParentNode->nodeValue)
							current->ParentNode->NodeLeft = tempLeft;
						else
							current->ParentNode->NodeRight = tempLeft;

						tempLeft->ParentNode = current->ParentNode;
						tempLeft->NodeRight = current->NodeRight;
						if (tempLeft->NodeRight != 0)
							tempLeft->NodeRight->ParentNode = tempLeft;		//Переназначаем у детей этого узла родителя(этот узел)
						tempLeft->NodeLeft = current->NodeLeft;
						tempLeft->NodeLeft->ParentNode = tempLeft;		//Переназначаем у детей этого узла родителя(этот узел)
						current->ParentNode = NULL;
						current->NodeLeft = NULL;
						current->NodeRight = NULL;
						delete current;
						current = NULL;
					}
				}
				else
				{
					tempLeft->ParentNode->NodeRight = tempLeft->NodeLeft; // Перевешиваем
					tempLeft->NodeLeft->ParentNode = tempLeft->ParentNode;  //		z

					if (current->ParentNode == NULL)		//Если current - стартовый узел
					{
						StartNode = tempLeft;
						tempLeft->ParentNode = NULL;
						tempLeft->NodeLeft = current->NodeLeft;
						tempLeft->NodeLeft->ParentNode = tempLeft;
						tempLeft->NodeRight = current->NodeRight;
						if (tempLeft->NodeRight != 0)
							tempLeft->NodeRight->ParentNode = tempLeft;
						current->NodeLeft = NULL;
						current->NodeRight = NULL;
						delete current;
						current = NULL;
					}
					else
					{
						tempLeft->ParentNode = current->ParentNode;		// y подсоединяем
						if (current->nodeValue < current->ParentNode->nodeValue)	// к родителю х
							current->ParentNode->NodeLeft = tempLeft;
						else
							current->ParentNode->NodeRight = tempLeft;
						tempLeft->NodeLeft = current->NodeLeft;
						tempLeft->NodeLeft->ParentNode = tempLeft;		//Переназначаем у детей этого узла родителя(этот узел)
						tempLeft->NodeRight = current->NodeRight;
						if (tempLeft->NodeRight != 0)
							tempLeft->NodeRight->ParentNode = tempLeft;		//Переназначаем у детей этого узла родителя(этот узел)
						current->ParentNode = NULL;
						current->NodeLeft = NULL;
						current->NodeRight = NULL;
						delete current;
						current = NULL;
					}
				}
			}
		}
	}
	Balancing(current);
}

void Tree::Balancing(TreeNode* current)
{
	if (current == NULL)	//Удаленные узлы не балансируем
		return;
	if ((abs(Height(current->NodeLeft) - Height(current->NodeRight))) <= 1)
		return;
	else
	{
		if (Height(current->NodeLeft) > Height(current->NodeRight))		 // L - повороты
		{
			TreeNode* temp = current->NodeLeft;
			if (Height(temp->NodeLeft) > Height(temp->NodeRight))
			{																		// LL-поворот(с обоими случаями)
				if (current->ParentNode != NULL)
				{
					if (current->nodeValue > current->ParentNode->nodeValue)
						current->ParentNode->NodeRight = temp;
					else
						current->ParentNode->NodeLeft = temp;
				}
				else
					StartNode = temp;
				temp->ParentNode = current->ParentNode;
				current->NodeLeft = temp->NodeRight;
				if (current->NodeLeft != NULL)
					current->NodeLeft->ParentNode = current;
				temp->NodeRight = current;
				current->ParentNode = temp;
			}
			else
			{																		// LR - поворот
				temp = temp->NodeRight;
				if (current->ParentNode != NULL)
				{
					if (current->nodeValue > current->ParentNode->nodeValue)
						current->ParentNode->NodeRight = temp;
					else
						current->ParentNode->NodeLeft = temp;
				}
				else
					StartNode = temp;
				temp->ParentNode->NodeRight = temp->NodeLeft;
				if (temp->NodeLeft != NULL)
					temp->NodeLeft->ParentNode = temp->ParentNode;
				temp->NodeLeft = temp->ParentNode;
				temp->NodeLeft->ParentNode = temp;
				temp->ParentNode = current->ParentNode;
				current->NodeLeft = temp->NodeRight;
				if (temp->NodeRight != NULL)
					current->NodeLeft->ParentNode = current;
				temp->NodeRight = current;
				temp->NodeRight->ParentNode = temp;

			}
		}
		else															// R - повороты
		{
			TreeNode* temp = current->NodeRight;

			if (Height(temp->NodeRight) > Height(temp->NodeLeft))
			{																		// RR-поворот(с обоими случаями)
				if (current->ParentNode != NULL)
				{
					if (current->nodeValue > current->ParentNode->nodeValue)
						current->ParentNode->NodeRight = temp;
					else
						current->ParentNode->NodeLeft = temp;
				}
				else
					StartNode = temp;
				temp->ParentNode = current->ParentNode;
				current->NodeRight = temp->NodeLeft;
				if (current->NodeRight != NULL)
					current->NodeRight->ParentNode = current;
				temp->NodeLeft = current;
				current->ParentNode = temp;
			}
			else
			{																		// RL - поворот
				temp = temp->NodeLeft;
				if (current->ParentNode != NULL)
				{
					if (current->nodeValue > current->ParentNode->nodeValue)
						current->ParentNode->NodeRight = temp;
					else
						current->ParentNode->NodeLeft = temp;
				}
				else
					StartNode = temp;
				temp->ParentNode->NodeLeft = temp->NodeRight;
				if (temp->NodeRight != NULL)
					temp->NodeRight->ParentNode = temp->ParentNode;
				temp->NodeRight = temp->ParentNode;
				temp->NodeRight->ParentNode = temp;
				temp->ParentNode = current->ParentNode;
				current->NodeRight = temp->NodeLeft;
				if (temp->NodeLeft != NULL)
					current->NodeRight->ParentNode = current;
				temp->NodeLeft = current;
				temp->NodeLeft->ParentNode = temp;
			}
		}
	}
}


int main()
{
	int act;
	int number;
	string info, infoSmall, changeNumberStr, newSmallInfo, newInfo;
	int deleteNumber, changeNumber;
	TreeNode* poisk = NULL,
		* deleteNode = NULL,
		* changeNode = NULL;
	Tree AVL;
	AVL.Init();

	cout << "\n\nReading file <Data.txt>..";
	AVL.Read(); cout << "\n\n";

	
	for( ; ; )
	{
		cout << "\nWhat will you do? Enter the one:\n(1) Search\n(2) Input\n(3) Delete\n(4) Change info\n(5) Output(from min to max)\n(6) Exit\n\n";
		cin >> act;
		 
		switch (act)
		{
		case 1:
			int num;
			cout << "Performing search..\n";
			if (StartNode == NULL)
				cout << "\nError. Tree is not created.\n";
			else
			{
				cout << "\nInput number: ";
				cin >> num;

				poisk = AVL.Search(StartNode, num);
				if (poisk == NULL)
					cout << "\nThe element (" << num << ") is not found.\n";
				else
				{
					cout << "\nSearching element is:\n" << poisk->nodeMean << "\n";
					cout << "\nHeight of this node is " << Height(poisk) << "\n";
				}
			}
			break;
		
		case 2:
			cout << "Performing input..\n";
			cout << "\nInput number and information (num Name | phone):\n";
			cin.ignore(32767, '\n');		// Позволяет следующему оператору cin игнорировать введенные ранее пробелы и нормально осуществлять ввод.
			getline(cin, info);

			infoSmall = info.substr(0, 3);  // Возвращает строке infoSmall подстроку строки info
			number = stoi(infoSmall);

			if (StartNode == NULL)
				AddToStart(info, number);
			else
				AVL.Add(StartNode, info, number);
			break;
		
		case 3:
			cout << "Performing delete..\n";
			if (StartNode == NULL)
				cout << "\nError. Tree is not created.\n";
			else
			{
				cout << "\nInput number what you want to delete: ";
				cin >> deleteNumber;

				deleteNode = AVL.Search(StartNode, deleteNumber);
				if (deleteNode == NULL)
					cout << "\nThe element (" << deleteNumber << ") is not found.\n";
				else
				{
					AVL.DestroyNode(StartNode, deleteNumber);

					cout << "\nSuccessfully!\n";
					Output();
				}
			}
			break;
		
		case 4:
			cout << "Performing change..\n";
			if (StartNode == NULL)
				cout << "\nError. Tree is not created.\n";
			else
			{
				cout << "\nInput number what you want to change: ";
				cin.ignore(32767, '\n');		// Позволяет следующему оператору cin игнорировать введенные ранее пробелы и нормально осуществлять ввод.
				cin >> changeNumberStr;
				changeNumber = stoi(changeNumberStr);

				changeNode = AVL.Search(StartNode, changeNumber);
				if (changeNode == NULL)
					cout << "\nThe element (" << changeNumber << ") is not found.\n";
				else
				{
					cout << "\nInput new information (Name | phone): ";
					cin.ignore(32767, '\n');		// Позволяет следующему оператору cin игнорировать введенные ранее пробелы и нормально осуществлять ввод.
					getline(cin, newSmallInfo);
					newInfo = changeNumberStr + " " + newSmallInfo;

					changeNode->nodeMean = newInfo;

					cout << "\nNow information is:\n" << changeNode->nodeMean << "\n";
				}
			}
			break;

		case 5:
			cout << "Performing output..\n";
			Output();
			break;

		case 6:
			break;

		default:
			cout << "Incorrect input!\n";
			break;
		}

		if (act == 6)
			break;
	} 
	
	return(0);
}