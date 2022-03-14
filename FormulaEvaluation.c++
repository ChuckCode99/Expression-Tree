#include <iostream>
#include <string>
#include <algorithm>
#include <utility>
#include <memory>
#include <vector>
#include <map>

class operation_tree
{
private:
    std::map<std::string, int> OperatorPriority
    {
        {"(" , 0},
        {"+" , 1},
        {"-" , 1},
        {"*" , 2},
        {"/" , 2},
        {"^" , 3}
    };

    struct node;

    std::shared_ptr<node> HeadNode;
    std::vector<node> NodeStack;
    std::vector<std::string> ElementStack;

    void print2DUtil (std::shared_ptr<node> root, int space);
    
public:
    operation_tree (std::string Input);

    void printNode (node N);
    
    // Wrapper over print2DUtil()
    void print2D () { print2DUtil(HeadNode, 0); } // Pass initial space count as 0
};


int main ()
{
    std::string Input {};

    std::getline(std::cin, Input);

    Input.erase(std::remove(Input.begin(), Input.end(), ' '), Input.end());

    Input = "(" + Input + ")";

    operation_tree T (Input);

    T.print2D();

    return 0;
}

struct operation_tree::node
{
    std::string Data;
    std::pair<std::shared_ptr<node>, std::shared_ptr<node>> Children;

    std::shared_ptr<node> RightChild = Children.first;
    std::shared_ptr<node> LeftChild  = Children.second;

    node () = default;

    node (std::string D)
        : Data(D), RightChild(nullptr), LeftChild(nullptr) {}

    node (char D)
        : RightChild(nullptr), LeftChild(nullptr)
    {
        std::string tmp {D};
        Data = tmp;
    }

    node (std::string D, std::shared_ptr<node> RC, std::shared_ptr<node> LC)
        : Data(D), RightChild(RC), LeftChild(LC) {}
    
    node (char D, std::shared_ptr<node> RC, std::shared_ptr<node> LC)
        : RightChild(RC), LeftChild(LC)
    {
        std::string tmp {D};
        Data = tmp;
    }
};

operation_tree::operation_tree (std::string Input)
{
    for (const auto& character : Input)
    {
        // std::cout << "Character: " << character << std::endl; // debugging

        // Pushing open parenthesis into the stack
        if(std::string tmp; character == '(')
        {
            ElementStack.push_back(tmp = character); // to convert char to std::string
            goto HERE;// continue;
        }

        // Checking for closed parenthesis to pop everything from the stack till open parenthesis
        if (std::string tmp; character == ')')
        {
            while (true)
            {
                if (ElementStack.back() == "(")
                {
                    ElementStack.pop_back();
                    break;
                }
                else
                {
                    std::shared_ptr<node> Left  = std::make_shared<node>(NodeStack.back());
                    NodeStack.pop_back();
                    
                    std::shared_ptr<node> Right = std::make_shared<node>(NodeStack.back());
                    NodeStack.pop_back();

                    NodeStack.push_back(node(ElementStack.back(), Right, Left));

                    ElementStack.pop_back(); 
                }
                
            }

            goto HERE;// continue;
        }

        // Checking that if the character is an operator {+,-,*,/,^}
        if (std::string tmp; !std::isalpha(character) && !std::isdigit(character))
        {
            // std::cout << "Operation Stack: " << ElementStack.back() << ", " << OperatorPriority[ElementStack.back()] << " | " << "Node Stack: " << character << ", " << OperatorPriority[tmp = character] << std::endl; // debugging
            if (OperatorPriority[ElementStack.back()] < OperatorPriority[tmp = character] || ElementStack.empty())
            {
                ElementStack.push_back(tmp = character);
            }
            else
            {
                while (OperatorPriority[ElementStack.back()] >= OperatorPriority[tmp = character] || ElementStack.back() != "(")
                {
                    std::shared_ptr<node> Left  = std::make_shared<node>(NodeStack.back());
                    NodeStack.pop_back();

                    std::shared_ptr<node> Right = std::make_shared<node>(NodeStack.back());
                    NodeStack.pop_back();

                    NodeStack.push_back(node(ElementStack.back(), Right, Left));

                    ElementStack.pop_back();
                }

                ElementStack.push_back(tmp = character);
            }

            goto HERE;// continue;
        }

        // Checking for everything else
        if (std::isalpha(character) || std::isdigit(character))
        {
            NodeStack.push_back(node(character));
            goto HERE;// continue;
        }

        HERE:
        // debugging stuff
        // std::cout << "C: ";
        // for (const auto& C : ElementStack)
        //     std::cout << C << " ";
        // std::cout << std::endl;
        // std::cout << "N: ";
        // for (const auto& N : NodeStack)
        //     std::cout << N.Data << "| ";
        // std::cout << std::endl;
        continue;
    }

    std::cout << "Node stack: ";
    for (const auto& N : NodeStack)
        std::cout << N.Data << " ";
    std::cout << std::endl;

    HeadNode = std::make_shared<node>(NodeStack.back());
}

void operation_tree::printNode (node N)
{
    if (!N.RightChild && !N.LeftChild)
        std::cout << "(" << "null" << ", " <<  N.Data<< ", " << "null" << ")" << std::endl;
    else if (!N.RightChild)
        std::cout << "(" << "null" << ", " <<  N.Data<< ", " << N.LeftChild->Data << ")" << std::endl;
    else if (!N.LeftChild)
        std::cout << "(" << N.RightChild->Data << ", " <<  N.Data<< ", " << "null" << ")" << std::endl;
    else
        std::cout << "(" << N.RightChild->Data << ", " <<  N.Data<< ", " << N.LeftChild->Data << ")" << std::endl;
}

void operation_tree::print2DUtil (std::shared_ptr<node> root, int space)
{
    const int COUNT {10};

    // Base case
    if (root == nullptr)
        return ;

    // Increase distance between levels
    space += COUNT;

    // Process right child first
    print2DUtil(root->RightChild, space);

    // Print current node after space count
    std:: cout<< std::endl;

    for (int i = COUNT; i < space; i++)
        std::cout << " ";

    printNode(*root);

    // Process left child
    print2DUtil(root->LeftChild, space);
}