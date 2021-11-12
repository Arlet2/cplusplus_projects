#include <iostream>
#include <stack>
using namespace std;
template <typename T>
class BinaryTree
{
    private:
        class Node;
        Node *p_root;
        unsigned int size;
    public:
        BinaryTree()
        {
            p_root = NULL;
            size=0;
        }
        
        static unsigned char get_height(Node& node)
        {
            if(&node == NULL)
                return 0;
            return node.height;
        }
        static void fix_height(Node& node)
        {
            unsigned char hl = get_height(node.get_left());
            unsigned char hr = get_height(node.get_right());
            node.height = max(hl,hr)+1;
        }
        static int bfactor(Node& p)
        {
            return int(get_height(p.get_right())
            -get_height(p.get_left()));
        }
        static Node& rotate_right(Node& node) 
        {
            cout << "ROTATE RIGHT\n";
            Node* new_node = &node.get_left();
            node.set_left(new_node->get_right());
            new_node->set_right(node);

            fix_height(node);
            fix_height(*new_node);

            return *new_node;
        }
        static Node& rotate_left(Node& node) 
        {
            cout << "ROTATE LEFT\n";
            Node* new_node = &node.get_right();
            node.set_right(new_node->get_left());
            new_node->set_left(node);

            fix_height(node);
            fix_height(*new_node);

            return *new_node;
        }
        static Node& balance(Node& node) 
        {
            fix_height(node);
            if(bfactor(node) == 2)
            {
                if(bfactor(node.get_right()) < 0)
                    node.set_right(rotate_right(node.get_right()));
                return rotate_left(node);
            }
            else if(bfactor(node) == -2)
            {
                if(bfactor(node.get_left()) > 0)
                    node.set_left(rotate_left(node.get_left()));
                return rotate_right(node);
            }
            return node;
        }
        void push(T value)
        {
            if(size == 0)
                p_root = new Node(value);
            else
            {
                Node *p_new_node = new Node(value);
                Node *p_go = p_root;
                while(true)
                {
                    if(p_new_node->get_value() < p_go->get_value())
                    {
                        if(&(p_go->get_left()) == NULL)
                        {
                            p_go->set_left(p_new_node->get_this());
                            break;
                        }
                        else
                            p_go = &p_go->get_left();
                    }
                    else if(p_new_node->get_value() > p_go->get_value())
                    {
                        if(&(p_go->get_right()) == NULL)
                        {
                            p_go->set_right(p_new_node->get_this());
                            break;
                        }
                        else
                            p_go = &p_go->get_right();
                    }
                    else
                        break;
                }
            }
            fix_heights();
            p_root = &balance(*p_root);
            ++size;
        }
        void print_tree()
        {
            cout << "SIZE: " << size << endl;
            void (*print)(Node&) = [](Node& node){
                cout << node.get_value() << " " << int(node.height);
                cout << endl;
            };
            rec_order(*p_root,print);
            cout << endl;
        }
        void fix_heights()
        {
            void (*fix)(Node&) = [](Node& node){
                fix_height(node);
            };
            rec_postorder(*p_root,fix);
        }
        void balance_nodes()
        {
            void (*bal)(Node&) = [](Node& node){
                node = balance(node);
            };
            rec_postorder(*p_root,bal);
        }
        Node* search_node(T value) const
        {
            Node* p_go = p_root;
            while(p_go != NULL && p_go->get_value() != value)
            {
                if(p_go->get_value() < value)
                    p_go = &p_go->get_right();
                else
                    p_go = &p_go->get_left();
            }
            return p_go;
        }
        Node* find_min(Node& node) const
        {
            Node* p_go = &node;
            while(&p_go->get_left() != NULL)
                p_go = &p_go->get_left();
            return p_go;
        }
        Node& remove_min(Node& node)
        {
            if(&node.get_left() == NULL)
                return node.get_right();
            node.set_left(remove_min(node.get_left()));
            return balance(node);
        }
        Node* remove_node(Node& p,T value)
        {
            if(&p == NULL)
                return NULL;
            if(value > p.get_value())
                p.set_right(*remove_node(p.get_right(),value));
            else if(value < p.get_value())
                p.set_left(*remove_node(p.get_left(),value));
            else
            {
                --size;
                Node* left = &p.get_left();
                Node* right = &p.get_right();
                delete &p;
                if(&p.get_right() == NULL)
                    return left;
                Node* min = find_min(*right);
                min->set_right(remove_min(*right));
                min->set_left(*left);
                return &balance(*min);
            }
            return &balance(p);
        }
        void pop(T value)
        {
            remove_node(*p_root,value);
            fix_heights();
        }
        void rec_order(Node& node,void (*treatment)(Node&))
        {
            if(&node == NULL)
                return;
            if(&node.get_left() != NULL)
                rec_order(node.get_left(),treatment);
            treatment(node);
            if(&node.get_right() != NULL)
                rec_order(node.get_right(),treatment);
        }
        void rec_postorder(Node& node,void (*treatment)(Node&))
        {
            if(&node.get_left() != NULL)
                rec_postorder(node.get_left(),treatment);
            if(&node.get_right() != NULL)
                rec_postorder(node.get_right(),treatment);
            treatment(node);
        }
        ~BinaryTree()
        {
            void (*delete_node)(Node&) = [](Node& node){
                delete &node;
            };
            rec_postorder(*p_root,delete_node);
        }
};
template <typename T>
class BinaryTree<T> :: Node
{
    private:
        T value;
        Node *p_left;
        Node *p_right;
    public:
        unsigned char height;
        Node()
        {
            p_left = NULL;
            p_right = NULL;
            height = 1;
        }
        Node(T value) : Node()
        {
            this->value = value;
        }
        Node& get_this()
        {
            return *this;
        }
        void set_left(Node &left)
        {
            p_left = &left;
        }
        
        Node& get_left() const
        {
            return *p_left;
        }
        void set_right(Node &right)
        {
            p_right = &right;
        }
        Node& get_right() const
        {
            return *p_right;
        }
        void set_value(T value)
        {
            this->value = value;
        }
        T get_value()
        {
            return value;
        }
        Node& operator =(Node& node)
        {
            value = node.get_value();
            p_left = &node.get_left();
            p_right = &node.get_right();
            return *this;
        }

};

template <typename T>
ostream& operator<< (ostream &out,const BinaryTree<T> bt)
{
    out << 1 << endl;
    return out;
}
int main()
{
    BinaryTree<int> bt;
    bt.push(400);
    bt.push(399);
    bt.push(645);
    bt.push(644);
    bt.push(643);
    bt.push(398);
    bt.push(646);
    bt.push(647);
    bt.print_tree();
    bt.pop(43);
    bt.pop(645);
    bt.print_tree();
    

}