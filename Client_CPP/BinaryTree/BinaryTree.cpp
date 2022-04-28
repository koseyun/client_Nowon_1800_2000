#include "BinaryTree.h"

Node& BinaryTree::Find(int item)
{
    if (_root == nullptr) return;

    _tmp = _root;

    while (_tmp != nullptr)
    {
        // ã���� �ϴ� ���� ���� ��庸�� ������
        if (_tmp->_data > item)
            _tmp = _tmp->_left;
        // ã���� �ϴ� ���� ���� ��庸�� Ŭ��
        else if (_tmp->_data < item)
            _tmp = _tmp->_right;
        // ã���� �ϴ� ���� ���� ���� ������
        else
            return *_tmp;
    }

    return *_tmp;
}

void BinaryTree::insert(int item)
{
    if (_root == nullptr) {

        _tmp = _root;
        while (_tmp != nullptr)
        {
            // �����ϰ��� �ϴ� ���� ���� ��庸�� ������
            if (_tmp->_data > item)
            {
                // ���� ��忡 ���� �ڽ��� ������
                if (_tmp->_left != nullptr)
                    _tmp = _tmp->_left;
                // ���� ��忡 ���� �ڽ��� ������
                else {
                    _tmp->_left = new Node();
                    _tmp->_left->_data = item;
                    return;
                }
            }
            // �����ϰ��� �ϴ� ���� ���� ��庸�� Ŭ ��
            else if (_tmp->_data < item) {

                // ���� ��忡 ������ �ڽ��� ������
                if (_tmp->_right != nullptr)
                    _tmp = _tmp->_right;
                // ���� ��忡 ������ �ڽ��� ������
                else {
                    _tmp->_right = new Node();
                    _tmp->_right->_data = item;
                    return;
                }
            }
            // �����ϰ��� �ϴ� ���� ���� ���� ���� ��
            else
                throw "�ش� ��尡 �̹� ������";
        }
    }
    else {
        _root = new Node();
        _root->_data = item;
    }
}

bool BinaryTree::Remove(int item)
{
    bool doRemoved = false;
    int dir = 0; // 0 �̸� ���� ����, 1�̸� ������ ����

    if (_root == nullptr)
        return doRemoved;

    _tmp = _root;

    while (_tmp != nullptr)
    {
        // �����Ϸ��� ���� ���� ��� ������ ���� ���
        if (_tmp->_data > item) {
            _tmp2 = _tmp; // �θ��� ������ ����
            _tmp = _tmp->_left; // ������ ���� �ڽ����� �ű�
        }
        // �����Ϸ��� ���� ���� ��� ������ Ŭ ���
        else if (_tmp->_data < item) {
            dir = 1;
            _tmp2 = _tmp; // �θ��� ������ ����
            _tmp = _tmp->_right; // ������ ������ �ڽ����� �ű�
        }
        // �����Ϸ��� ���� ���� ��� ���� ���� ���
        else {
            doRemoved = true;
            break;
        }
        if (doRemoved) {

            // ��������
            // == �� ������ ���װ� ������ ������ true, �ٸ��� false�� ��ȯ��.

            // ���Ǻ� ���������� ( && , || )
            // ������ ����� ���� �������� ����� �����Ǿ��� ��� ���� �񱳿����� ���� �ʰ� ���� ����� ��ȯ��.

            // �����ϰ��� �ϴ� ��忡 �ڽ��� ���� ���
            if (_tmp->_left == nullptr && _tmp->_right == nullptr) {
                // �����ϰ��� �ϴ� ���� �� �θ� ��� ���� ������ ����
                if (_tmp2 != nullptr)
                    if (dir == 0)
                        _tmp2->_left = nullptr;
                    else
                        _tmp2->_right = nullptr;
            }
            // ��� ����
            delete _tmp;
            _tmp = nullptr;
        }
        // �����ϰ��� �ϴ� ��忡 ���� �ڽĸ� ���� ���
        else if ((_tmp->_left != nullptr) && (_tmp->_right == nullptr)) {
            // �����ϰ��� �ϴ� ����� �ڽ� ���� �����ϰ��� �ϴ� ����� �θ��带 ����
            if (_tmp2 != nullptr) {
                if (dir == 0)
                    _tmp2->_left = _tmp->_left;
                else
                    _tmp2->_right = _tmp->_left;
            }
            // ��� ����
            delete _tmp;
            _tmp = nullptr;
        }
        // �����ϰ��� �ϴ� ��忡 ������ �ڽĸ� ���� ���
        else if ((_tmp->_left != nullptr) && (_tmp->_right == nullptr)) {
            // �����ϰ��� �ϴ� ����� �ڽ� ���� �����ϰ��� �ϴ� ����� �θ��带 ����
            if (_tmp2 != nullptr) {
                if (dir == 0)
                    _tmp2->_left = _tmp->_left;
                else
                    _tmp2->_right = _tmp->_right;
            }
            // ��� ����
            delete _tmp;
            _tmp = nullptr;
        }
        // �����ϰ��� �ϴ� ��忡 ����, ������ �ڽ� �� �� ���� ���
        else {

            // �ܸ���� �˻� ( ������ ��带 ��ü�� ��� �˻� )
            Node* leaf = FindVeryLeftLeaf(_tmp->_right, _tmp2);
            _tmp->_data = leaf->_data; // �����ؾ��ϴ� ����� ���� �ܸ���� ������ ����
            _tmp2->_left = nullptr; // �ܸ����� �ܸ���� �θ� ���� ���� ����
            delete leaf; // �ܸ���� ����

        }
    }

    return doRemoved;
}

Node* BinaryTree::FindVeryLeftLeaf(Node* start, Node* parent)
{
    Node* leaf = start;

    while (leaf->_left != nullptr)
    {
        parent = leaf;
        leaf = leaf->_left;
    }
    
    /*if (leaf->_right != nullptr) {
        leaf = leaf->_right;
        while (leaf->_left != nullptr)
        {
            leaf = leaf->_left;
        }
        leaf = FindLeaf(leaf);
    }*/

    return leaf;
}