#ifndef OPTIMIZATION2_H
#define OPTIMIZATION2_H

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

// method to get the index of the question/answer with the highest information gain for the remaining animals
int get_max_information_gain_index(std::vector<std::map<std::string, bool>> &answers, std::list<std::string> &animals) {
    std::vector<double> information_gain;
    
    for(std::map<std::string, bool> question_answer : answers) {
        double count_true = 0;
        double count_false = 0;

        // count the animals for the left (true) and right (false) subtree
        for(std::string animal : animals) {
            if(question_answer.find(animal) == question_answer.end()) continue;
            if(question_answer.find(animal)->second)
                count_true++;
            else
                count_false++;
        }

        int total = count_true+count_false;
        double rel_count_true = count_true/total;
        double rel_count_false = count_false/total;

        double gain = -(rel_count_true * std::log2(rel_count_true) + rel_count_false * std::log2(rel_count_false));
        gain = std::isnan(gain) ? 0 : gain;
        information_gain.push_back(gain);
        
        // the max information gain is 1, so if this occurs return this as the max index
        if(gain == 1) return information_gain.size() - 1;
    }

    // get the maximum information gain and return its index
    auto max_it = std::max_element(information_gain.begin(), information_gain.end());
    int max_index = max_it - information_gain.begin();

    return max_index;
}

// this method recursively builds a binary tree from questions, animals and answers
bool BinaryTree::build(std::vector<std::string> &questions, std::list<std::string> &animals, std::vector<std::map<std::string, bool>> &answers)
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
    if(questions.size() != 0) {
        int index = get_max_information_gain_index(answers, animals);
        *this = std::make_unique<Node>(questions[index]);

        // split the animals list into two lists, one for the left and one for the right tree
        std::list<std::string> animalsLeft, animalsRight;

        for(std::string animal : animals) {
            if(answers[index].find(animal)->second)
                animalsLeft.push_back(animal);
            else
                animalsRight.push_back(animal);
        }

        // remove the current question/answer from its vector to pass to the left and right subtree
        std::vector<std::string> questions_copy(questions);
        std::vector<std::map<std::string, bool>> answers_copy(answers);
        questions_copy.erase(questions_copy.begin() + index);
        answers_copy.erase(answers_copy.begin() + index);

        // recursively build the left and right tree
        left_build_successful = this->get()->left.build(questions_copy, animalsLeft, answers_copy);
        right_build_successful = this->get()->right.build(questions_copy, animalsRight, answers_copy);
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
    // breath first search by using a queue of pairs of binary tree references and integers (depth)
    // the index can't be used here to calculate the depth because it isn't always a complete binary tree anymore as it used to be before optimalisation
    std::queue<std::pair<BinaryTree*, int>> queue;
    // push the root of the tree on the queue
    queue.push(std::make_pair(this, 0));

    double sum_of_leave_depth = 0;
    int number_of_leaves = 0;

    while(!queue.empty()) {
        int depth = queue.front().second;
        // check if the current binary tree (~node) is a leaf
        if((*queue.front().first).get()->left == NULL && (*queue.front().first).get()->right == NULL) {
            sum_of_leave_depth += depth;
            // keep track of the number of leaves
            number_of_leaves++;
        }

        // push both children on the queue if they exist
        if((*queue.front().first).get()->left != NULL)
            queue.push(std::make_pair(&((*queue.front().first).get()->left), depth+1));
        if((*queue.front().first).get()->right != NULL)
            queue.push(std::make_pair(&((*queue.front().first).get()->right), depth+1));

        // remove the processed binary tree (~node) from the queue
        queue.pop();
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