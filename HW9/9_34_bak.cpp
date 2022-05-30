#include <iostream>

#define TOTAL_PAGE_FRAMES 7

using namespace std;

class link_list
{
public:
	link_list(int value) : val(value) {
		next == NULL;
	};
	link_list() {
		next == NULL;
	};
	int val;
	link_list * next;
};

class page_algo
{
public:
	page_algo(int frame){
		frame_num = frame;
		page_fault = 0; // set page fault initial val to 0
		store_num = 0;
		head = tail = NULL;
	}
protected:
	int frame_num;
	bool check[TOTAL_PAGE_FRAMES + 1] = {false};
	// set all pages false initially (not in the store)
	int page_fault;
	int store_num;
	link_list store;
	link_list *head, *tail;
};

class fifo : public page_algo
{
public:
	fifo(int frame) : page_algo(frame){};

	void insert(int input) {
		if (check[input] == false) {
			page_fault++;
			check[input] = true;
			link_list *newNode = new link_list(input);
			if (head == NULL) {	//initialize head and tail
				head = tail = newNode;
			} else {
				tail->next = newNode;
				tail = tail->next;
			}
			if (++store_num > frame_num) delete_node();
		}
	}
	void delete_node() {
		link_list *cur = head;
		head = head->next;
		check[cur->val] = false;
		delete cur;
		cur = NULL;
		store_num--;
	}
	void show_fifo(){
		link_list *cur = head;
		cout << "show_fifo:" << endl;
		while (cur != NULL) {
			cout << cur->val << " ";
			cur = cur->next;
		}
		cout << endl << "page faults: " << page_fault << endl;
	}
};

class lru : public page_algo
{
public:
	lru(int frame) : page_algo(frame){};

	void move_node(int input) {
		link_list *cur = head, *pre = head;
		if (cur->val == input) { // if the input is the head
			head = head->next;
		}
		while (cur->val != input){
			pre = cur;
			cur = cur->next;
		}
		pre->next = cur->next;
		tail->next = cur;	// add cur to the end
		tail = tail->next;
		cur->next = NULL;	// cur turn to tail
	}
	void insert(int input) {
		if (check[input] == false) {
			page_fault++;
			check[input] = true;
			link_list *newNode = new link_list(input);
			if (head == NULL) {	//initialize head and tail
				head = tail = newNode;
			} else {
				tail->next = newNode;
				tail = tail->next;
			}
			if (++store_num > frame_num) delete_node();
		} else if (input != tail->val) {
			move_node(input);	// switch the order
		}
	}
	void delete_node() {
		link_list *cur = head;
		head = head->next;
		check[cur->val] = false;
		delete cur;
		cur = NULL;
		store_num--;
	}
	void show_lru(){
		link_list *cur = head;
		cout << "show_lru:" << endl;
		while (cur != NULL) {
			cout << cur->val << " ";
			cur = cur->next;
		}
		cout << endl << "page faults: " << page_fault << endl;
	}
};

int main(int argc, char *argv[]) {

	fifo fifo_(stoi(argv[1]));
	lru lru_(stoi(argv[1]));
	int i = 0;
	while (argv[2][i] != '\0') {
		int input = argv[2][i] - '0';
		fifo_.insert(input);
		lru_.insert(input);
		i++;
	}
	// for (int i = 0; i < 4; i++) {
	// 	fifo_.insert(i);
	// 	lru_.insert(i);
	// }
	// fifo_.insert(1);
	// lru_.insert(1);
	fifo_.show_fifo();
	lru_.show_lru();


	return 0;
}