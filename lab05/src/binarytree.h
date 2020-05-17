#ifndef BINTREE_H
#define BINTREE_H

#include <memory>
#include <vector>
#include <list>
#include <map>

#include <iostream>
#include <cmath>
#include <algorithm>
#include <queue> 

class Node;

class BinaryTree : public std::unique_ptr<Node>
{
public:
    using std::unique_ptr<Node>::unique_ptr;
    using std::unique_ptr<Node>::operator=;
    friend std::ostream &operator<<(std::ostream &os, const BinaryTree &tree);
    void print(std::ostream &os, int index) const;

    bool build(std::vector<std::string> &questions, std::list<std::string> &animals, std::vector<std::map<std::string, bool>> &answers);
    int height();
    double averageDepth();
    int numberOfLeaves();
    int numberOfSplits();
private:
    bool build(std::vector<std::string> &questions, std::list<std::string> &animals, std::vector<std::map<std::string, bool>> &answers, int i);

private:
};

class Node
{
public:
    std::string value;
    BinaryTree left, right = NULL;

    Node(const std::string &value) {
        this->value = value;
    };
};

// method to print a list of undistinguishable animals
void print_undistinguishable_animals(std::list<std::string> undistinguishable_animals)
{
    std::cout << "Can not distinguish between ";
    for(std::string animal : undistinguishable_animals)
        std::cout << animal << ", ";
    std::cout << std::endl;
}

// this method calls the private build method which has one extra parameter, being the depth, which is 0 at the start of the algorithm
bool BinaryTree::build(std::vector<std::string> &questions, std::list<std::string> &animals, std::vector<std::map<std::string, bool>> &answers)
{
    return this->build(questions, animals, answers, 0);
}

// this method recursively builds a binary tree from questions, animals and answers
bool BinaryTree::build(std::vector<std::string> &questions, std::list<std::string> &animals, std::vector<std::map<std::string, bool>> &answers, int depth)
{
    std::list<std::string> undistinguishable_animals;

    if (animals.size() == 1) {
        // add the animal to the current node
        *this = std::make_unique<Node>(animals.front());
        return true;
    }

    bool left_build_successful = true;
    bool right_build_successful = true;

    // check if the depth hasn't reached the size of the questions vector
    if(depth < questions.size()) {
        *this = std::make_unique<Node>(questions[depth]);

        // split the animals list into two lists, one for the left and one for the right tree
        std::list<std::string> animalsLeft, animalsRight;

        for(std::string animal : animals) {
            if(answers[depth].find(animal)->second)
                animalsLeft.push_back(animal);
            else
                animalsRight.push_back(animal);
        }

        // recursively build the left and right tree
        left_build_successful = this->get()->left.build(questions, animalsLeft, answers, depth+1);
        right_build_successful = this->get()->right.build(questions, animalsRight, answers, depth+1);
    } else if (1 < animals.size()) {
        // animals are undistinguishable if there are more than two animals for one subtree
        for(std::string animal : animals)
            undistinguishable_animals.push_back(animal);
    }

    // print undistinguishable animals if any have been detected and return false
    if(0 < undistinguishable_animals.size()){
        print_undistinguishable_animals(undistinguishable_animals);
        return false;
    }

    // return true or false based on the recursive build of the left and right tree
    return left_build_successful && right_build_successful;
}

int BinaryTree::height()
{
    // recursively call the height method on both left and right side
    int left_height = this->get()->left != NULL ? this->get()->left.height() : 0;
    int right_height = this->get()->right != NULL ? this->get()->right.height() : 0;

    // return the height of the left and right tree and increment with one (the current node)
    return std::max(left_height, right_height) + 1;
}

// this method calculates the average depth of the tree by doing a breath first search 
// this is a non recursive method but by doing a breadth first approach it is more performant 
// than a recursive depth first search, because a depth first approach iterates over certain nodes
// more than once, while this breath first approach doesn't do that
double BinaryTree::averageDepth()
{
    // breath first search by using a queue of binary tree references
    std::queue<BinaryTree*> queue;
    // push the root of the tree on the queue
    queue.push(this);

    // the index can be used to calculate the depth, because we have a non-optimal 
    // complete binary tree where no indices are skipped, this isn't the case in 
    // optimization 1 and 2
    int index = 0;
    double sum_of_leave_depth = 0;
    int number_of_leaves = 0;

    while(!queue.empty()) {
        // check if the current binary tree (~node) is a leave
        if((*queue.front()).get()->left == NULL && (*queue.front()).get()->right == NULL) {
            // calculate the depth from index and add to sum of leave heights
            sum_of_leave_depth += std::floor(std::log2(index+1));
            // keep track of the number of leaves
            number_of_leaves++;
        }

        // push both children on the queue if they exist
        if((*queue.front()).get()->left != NULL)
            queue.push(&((*queue.front()).get()->left));
        if((*queue.front()).get()->right != NULL)
            queue.push(&((*queue.front()).get()->right));

        // remove the processed binary tree (~node) from the queue
        queue.pop();
        index++;
    }

    // return the average leave depth
    return sum_of_leave_depth / number_of_leaves;
}

int BinaryTree::numberOfLeaves()
{
    // if both left and right tree are NULL then this node is a leave
    if(this->get()->left == NULL && this->get()->right == NULL)
        return 1;

    // recursively call the numberOfLeaves method on both left and right side
    int left_number_of_leaves = this->get()->left != NULL ? this->get()->left.numberOfLeaves() : 0;
    int right_number_of_leaves = this->get()->right != NULL ? this->get()->right.numberOfLeaves() : 0;

    return left_number_of_leaves + right_number_of_leaves;
}

int BinaryTree::numberOfSplits()
{
    // if both left and right tree are NULL then this node is not a split
    if(this->get()->left == NULL && this->get()->right == NULL)
        return 0;

    // recursively call the numberOfSplits method on both left and right side
    int left_number_of_splits = this->get()->left != NULL ? this->get()->left.numberOfSplits() : 0;
    int right_number_of_splits = this->get()->right != NULL ? this->get()->right.numberOfSplits() : 0;

    // return the number of splits of the left and right tree and increment with one (the current node)
    return left_number_of_splits + right_number_of_splits + 1;
}

void BinaryTree::print(std::ostream &os, int index) const
{
    // calculate the depth of the current node based on the index
    int depth = std::floor(std::log2(index+1));

    // print indentation and an arrow if needed, the Y/N arrow is printed based on the index being even (N) or odd (Y)
    os << std::string(0 < depth ? depth-1 : 0, '\t') << ((0 < index) ? (index%2 != 0) ? "--Y--> " : "--N--> " : "");
    os << this->get()->value << std::endl;

    // recursively print the left and right tree and pass the index for the left and right node
    if(this->get()->left)
        this->get()->left.print(os, 2*index+1);
    if(this->get()->right)
        this->get()->right.print(os, 2*index+2);
}

std::ostream &operator<<(std::ostream &os, const BinaryTree &tree)
{
    tree.print(os, 0);
    return os;
}

#endif