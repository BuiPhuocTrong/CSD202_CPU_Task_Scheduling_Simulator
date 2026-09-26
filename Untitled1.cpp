#include <iostream>

using namespace std;

class Node {
public:	
	int data;
	Node * next;
	Node (int data){
		this -> data = data;
		next = nullptr;
	}	
};

class CLL{
public:
	Node * tail;
	bool isEmpty (){
		return tail == 0;
	}
	
	CLL(){
		tail = nullptr;	
	}
	void addFirst(int data){
		Node *t = new Node (data);
		if (isEmpty()){
			tail = t;
			t->next = tail;
			return;
		}
		t->next = tail -> next;
		tail->next = t;
	}
	void addLast(int data){
		Node *t = new Node (data);
		if (isEmpty()){
			tail = t;
			t->next = tail;
			return;
		}
		t->next = tail -> next;
		tail->next = t;
		tail = t;
	}
	
	void display (){
		Node *temp = tail;
		do{
			cout << temp->next->data << " ";
			temp = temp->next;
		}while (temp != tail);
	}
	
	
};

int main() {
	CLL cll;
	cll.addFirst (4);
	cll.addFirst (3);
	cll.addFirst (2);
	cll.addFirst (1);
	
	cll.display ();
	
}