#include "BTree.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <tuple>

#include "Application.h"

#undef main

#ifdef _WIN32
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>				
#endif 

using namespace std;

void Quick_sort(int arr[], int l, int r)
{
	if (l >= r) return;
	bool flag = false;
	int tl = l, tr = r, key = arr[l];
	while (tl < tr)
	{
		if (!flag)
		{
			if (arr[tr] < key)
			{
				arr[tl] = arr[tr];
				flag = true;
			}
			else
				tr--;
		}
		else
		{
			if (arr[tl] > key)
			{
				arr[tr] = arr[tl];
				flag = false;
			}
			else
				tl++;
		}
	}
	arr[tr] = key;
	Quick_sort(arr, l, tr - 1);
	Quick_sort(arr, tr + 1, r);
}

template<typename T>
bool down_sort(T f1, T f2)
{
	return f1 < f2;
}

template<typename T>
bool up_sort(T f1, T f2)
{
	return f1 > f2;
}

template<typename T>
mdlist<T>* createMdlistNode(T data)
{
	return new mdlist<T>(data);
}

template<typename T>
mdlist<T>* createMdlistNode()
{
	return new mdlist<T>();
}

template<typename T>
BTree<T>* createTreeHead(T data, int depth)
{
	return new BTree<T>(data,depth);
}

template<typename T>
void addTreeNode(BTree<T>* head, std::vector<T>& vec, int idx)
{
	static int cur_depth = 1;

	if (head == nullptr)
		return;

	BTree<T>* node = nullptr;
	if (head->left == nullptr && cur_depth != head->depth)
	{
		node = new BTree<T>(vec[idx++], head->depth);
		node->parent = head;
		node->depth = head->depth;
		head->left = node;
		cur_depth++;
		addTreeNode(head->left, vec, idx);
	}
	else if (head->right == nullptr && cur_depth != head->depth)
	{
		node = new BTree<T>(vec[idx++], head->depth);
		node->parent = head;
		node->depth = head->depth;
		head->right = node;
		cur_depth++;
		addTreeNode(head->right, vec, idx);
	}
	else
	{
		cur_depth--;
		addTreeNode(head->parent, vec, idx);
	}
}

template<typename T>
void showTreeFront(BTree<T>* head)
{
	if (head == nullptr) return;
	cout << head->data << " ";
	if (head->left) showTreeFront(head->left);
	if (head->right) showTreeFront(head->right);
}

template<typename T>
bool findNode(BTree<T>* head, T data)
{
	if (head->data == data)
		return true;

	if (head->left)  
		if (findNode(head->left, data)) return true;
	if (head->right)
		if (findNode(head->right, data)) return true;

	return false;
}

template<typename T>
bool TreeRemoveOne(BTree<T>* head, T data)
{
	if (head->data == data)
		return true;

	if (head->left)
	{
		if (findNode(head->left, data)) 
		{
			
		}
	}
		
	if (head->right)
	{
		if (findNode(head->right, data))
		{
			
		}
	}
		
}

double triangle(double a, double b, double c) {
	double s = (a + b + c) / 2;
	if (a + b <= c || a + c <= b || b + c <= a) {
		throw 1.0;        //语句throw抛出double异常
	}
	return sqrt(s * (s - a) * (s - b) * (s - c));
}

/*
void destoryMdlistHead(mdlist* h)
{
	
}

void headInsert(mdlist** h, int data)
{
	mdlist* node = createMdlistNode(data);
	
	if ((*h)->next)
	{
		mdlist* temp = (*h)->next;

		(*h)->next = node;
		node->next = temp;

		temp->back = node;
		node->back = (*h);
	}
	else
	{
		(*h)->next = node;
		node->back = (*h);
	}
}

void tailInsert(mdlist** h, int data)
{
	mdlist* node = createMdlistNode(data);

	mdlist* t = (*h)->next;
	if (!t)
	{
		(*h)->next = node;
		node->back = (*h);
	}
	else
	{
		while (t->next)
		{
			t = t->next;
		}
		t->next = node;
		node->back = t;
	}	
}

void posInsert(mdlist** h, int idx, int data)
{
	mdlist* t = (*h)->next;
	int i = 1;

	if (idx == 0)
	{
		headInsert(h, data);
		return;
	}

	if (idx == mdlist_size(h))
	{
		tailInsert(h, data);
		return;
	}
 	
	mdlist* node = createMdlistNode(data);
	while (i != idx)
	{
		i++;
		t = t->next;
	}

	auto t1 = t->next;

	t->next = node;
	node->next = t1;
	t1->back = node;
	node->back = t;
}

void removeOne(mdlist** h, int data)
{

}

void removeAt(mdlist** h, int idx)
{
}

int findOne(mdlist** h, int data)
{
	return 0;
}

int mdlist_size(mdlist** h)
{
	int idx = 0;
	mdlist* t = (*h)->next;
	if (!t) return idx;

	while (t)
	{
		t = t->next;
		idx++;
	}
	return idx;
}

void showMdlist(mdlist** h)
{
	mdlist* t = (*h)->next;
	if (!t) return;

	while (t)
	{
		cout << t->data << " ";
		t = t->next;
	}
	cout << "\n";
}

void showMdlist_tail(mdlist** t)
{

}*/

void sort_mdlist()
{
}

template<typename T>
const mdlist<T>* mcontainer<T>::font() const
{
	return head;
}

template<typename T>
const mdlist<T>* mcontainer<T>::end() const
{
	return tail;
}

template<typename T>
void mcontainer<T>::push_front(int data)
{
	mdlist<T>* node = createMdlistNode<T>(data);
	mdlist<T>* t = head->next;
	head->next = node;
	node->next = t;
	t->back = node;
	node->back = head;
}

template<typename T>
void mcontainer<T>::push_back(int data)
{
	mdlist<T>* node = createMdlistNode<T>(data);
	mdlist<T>* t = tail->back;
	t->next = node;
	node->next = tail;
	tail->back = node;
	node->back = t;
}

template<typename T>
void mcontainer<T>::removeOne(int data)
{
	if (isEmpty()) return;

	mdlist<T>* t = head->next;
	while (t)
	{
		if (t->data == data)
		{
			t->back->next = t->next;
			t->next->back = t->back;
			delete t;
			break;
		}
		t = t->next;
	}
}

template<typename T>
void mcontainer<T>::removeAt(int idx)
{
	if (isEmpty()) return;

	int i = 0;
	mdlist<T>* t = head->next;
	while (i != idx)
	{
		if (i++ == idx)
		{
			t->back->next = t->next;
			t->next->back = t->back;
			delete t;
			break;
		}
		t = t->next;
	}
}

template<typename T>
int mcontainer<T>::size()const
{
	int idx = 0;
	mdlist<T>* t = head->next;
	if (t == tail) return idx;

	while (t != tail)
	{
		t = t->next;
		idx++;
	}
	return idx;
}

template<typename T>
bool mcontainer<T>::isEmpty()
{
	if (size() == 0)
		return true;
	else return false;
}

template<typename T>
bool mcontainer<T>::findOne(int data)
{
	bubble_sort<T>(*this, up_sort);
	
	int mid = -1;
	int l = 0, r = size();
	while (l <= r)
	{
		mid = (r + l) / 2;
		if (value(mid) > data)
		{
			r = mid - 1;
		}
		else if (value(mid) < data)
		{
			l = mid + 1;
		}
		else return true;
	}
	return false;
}

template<typename T>
mdlist<T>* mcontainer<T>::operator[](int idx)
{
	assert(0 <= idx && idx <= this->size() - 1);

	int i = 0;
	mdlist<T>* t = head->next;
	while (i != idx)
	{
		t = t->next;
		i++;
	}
	return t;
}

template<typename T>
T& mcontainer<T>::value(int idx)
{
	return operator[](idx)->data;
}

template<typename T>
void mcontainer<T>::SortMdlist()
{
}

template<typename T>
void mcontainer<T>::exchange(int first, int second)
{
	auto f = operator[](first);
	auto s = operator[](second);

	if (f->next == s)
	{
		auto f0 = f->back;
		auto s1 = s->next;

		f0->next = s;
		s->next = f;
		f->next = s1;
		s1->back = f;
		f->back = s;
		s->back = f0;
	}
	else
	{
		auto f0 = f->back;
		auto f1 = f->next;

		auto s0 = s->back;
		auto s1 = s->next;

		f0->next = s;
		s->next = f1;
		f1->back = s;
		s->back = f0;

		s0->next = f;
		f->next = s1;
		s1->back = f;
		f->back = s0;
	}
}

template<typename T>
void show_container(const mcontainer<T>& c)
{
	mdlist<T>* t = c.font()->next;
	
	while (t != c.end())
	{
		cout << t->data << " ";
		t = t->next;
	}
	cout << "\n";
}

template<typename T>
void bubble_sort(mcontainer<T>& c, sort_ptr<T> p) {
	for (int i = 0; i < c.size(); ++i)
	{
		for (int j = i + 1; j < c.size(); ++j)
		{
			if (p(c[i]->data, c[j]->data))
			{
				c.exchange(i, j);
			}
		}
	}
}

void test() {
	
	mcontainer<char> c;
	sort_ptr<char> p = down_sort<char>;
	c.push_back('a');
	c.push_back('b');
	c.push_back('c');
	c.push_back('d');
	c.push_front('r');
	c.push_front('q');
	std::cout << "size:" << c.size() << "\n";
	std::cout << "data:" << c[1]->data << ":" << c.value(1) << "\n";
	c.exchange(0, 1);
	bubble_sort<char>(c, p);
	show_container(c);
	c.removeOne(4);
	show_container(c);

	std::cout << c.findOne('a');
}

int add(int a, int b)
{
	return a + b;
}	

//windows下的UDP通信
void udp_server()
{
	//初始化WinSock库
	WSAStartup(MAKEWORD(2, 2), new WSADATA);
	//创建套接字
	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	//绑定
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(12345);
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(s, (sockaddr*)&addr, sizeof(addr));
	//接收数据
	char buf[1024];
	sockaddr_in client_addr;
	int len = sizeof(client_addr);
	recvfrom(s, buf, sizeof(buf), 0, (sockaddr*)&client_addr, &len);
	//发送数据
	sendto(s, buf, strlen(buf) + 1, 0, (sockaddr*)&client_addr, len);
	//关闭套接字
	closesocket(s);
	//清理WinSock库
	WSACleanup();
}

class Dialog :public std::enable_shared_from_this<Dialog>
{
public:
	Dialog() {
		this->num;
		std::cout << "Dialog construction\n";
	}
	~Dialog() {
		this->num;
		std::cout << "~Dialog destruction\n";
	};
	void active()
	{
		this->num;
		std::cout << "dialog active\n";
	}
	void close()
	{
		this->num;
		std::cout << "dialog close\n";
	}

	//获取当前对象的shared_ptr
	std::shared_ptr<Dialog> getSelf()
	{ 
		return shared_from_this();
	}

	int num = 0;
};



void onDoubleClickMap(std::map<int, std::unique_ptr<Dialog>>& userDialogs,int id)
{
#if CXX_STANDARD >= 17
	//try_emplace的返回值是一个std::pair<T1, T2>类型，其中T2是一个bool类型表示元素是否成功插入map中，
	//T1是一个map的迭代器，如果插入成功，则返回指向插入位置的元素的迭代器，如果插入失败，则返回map中已存在的相同key元素的迭代器。
	auto [iter,inserted] = userDialogs.try_emplace(id,nullptr);
	if (inserted)
	{
		iter->second = std::move(std::make_unique<Dialog>());
	}
	iter->second->active();
#else
	auto it = userDialogs.find(id);
	if (it != userDialogs.end())
	{
		it->second->active();
	}
	else
	{
		auto ptr = std::unique_ptr<Dialog>();
		ptr->active();
		userDialogs.insert(std::make_pair(id, std::move(ptr)));
	}
#endif
}

struct MyStruct
{
public:
	MyStruct() :d() {
		std::cout << "MyStruct construction\n";
	}
	~MyStruct() {
		std::cout << "~MyStruct destruction\n";
	}
private:
	Dialog d;
};


void test_shared_ptr(std::shared_ptr<Dialog>& ptr)
{
	std::shared_ptr<Dialog> s_ptr = ptr;
	std::cout << "use count:" << ptr.use_count() << "\n";
	ptr.reset();
	std::cout << "use count:" << s_ptr.use_count() << "\n";
}

int main()
{
	{
		/*int coreCount = std::thread::hardware_concurrency();
		threadPool pool(coreCount);
		std::mutex mutex;

		auto fu1 = pool.Enqueue(add, 1, 2);
		auto fu2 = pool.Enqueue(add, 3, 4);
		auto fu3 = pool.Enqueue(add, 5, 6);
		auto fu4 = pool.Enqueue(add, 7, 8);
		auto fu5 = pool.Enqueue(add, 9, 10);*/
	}
	//cout << fu1.get() << fu2.get() << fu3.get() << fu4.get() << fu5.get();

	//C++14元组
	std::tuple<string, string, int> t("hello", "world", 1);
	//C++17结构化绑定
#if CXX_STANDARD >= 17
	auto [a, b, c] = t;
#endif // 
	{
		std::map<int, std::unique_ptr<Dialog>> userDialogs;
		onDoubleClickMap(userDialogs,120);
		onDoubleClickMap(userDialogs,121);
		onDoubleClickMap(userDialogs,120);
		//insert_or_assign方法，如果map中指定的key不存在则插入，存在则更新其value值的情形
		userDialogs.insert_or_assign(12, std::make_unique<Dialog>());
	}
	{
		//智能指针，自定义删除器
		auto deleter = [](Dialog *d)->void{
			d->close();
			delete d;
		};

		std::unique_ptr<Dialog, void(*)(Dialog *d)> ptr(new Dialog(), deleter);
		//std::unique_ptr<Dialog, decltype(deleter)> ptr1(new Dialog(), deleter);
		std::shared_ptr<Dialog> s_ptr(std::make_shared<Dialog>());
		//自定义删除器

		FILE* ffp;
		fopen_s(&ffp, "./test.txt", "rb");
		std::shared_ptr<FILE> de_ptr(ffp, [](FILE* fp) {if(fp)fclose(fp);});

		/*err C2280:“std::shared_ptr<Dialog,std::default_delete<Dialog>>::shared_ptr(const std::shared_ptr<Dialog,std::default_delete<Dialog>> &)”:尝试引用已删除的函数
		auto dd = new Dialog();
		std::shared_ptr<Dialog> s_ptr1(dd);
		std::shared_ptr<Dialog> s_ptr2(dd);		//自定义删除
		std::shared_ptr<Dialog> s_ptr3(dd);		//自定义删除
		std::shared_ptr<Dialog> s_ptr4(dd);		//自定义删除
		std::shared_ptr<Dialog> s_ptr5(dd);		//自定义删除
		std::cout << "use count:" << s_ptr1.use_count() << " " << s_ptr2.use_count() << " " << s_ptr3.use_count() << " " << s_ptr4.use_count()<<" " << s_ptr5.use_count();*/
		auto ts_ptr = s_ptr.get();
	}

	{
		cout << "---------------------------\n";
		std::shared_ptr<Dialog> s_ptr = std::make_shared<Dialog>();
		s_ptr.reset();
		cout << "---------------------------\n";
		std::unique_ptr<Dialog> u_ptr = std::make_unique<Dialog>();
		//u_ptr.reset();
		
		std::unique_ptr<Dialog> tu_ptr;
		tu_ptr.swap(u_ptr);

		std::shared_ptr<Dialog> ts_ptr = std::move(tu_ptr);
	}

	{
		cout << "---------------------------\n";
		std::shared_ptr<Dialog> s_ptr = std::make_shared<Dialog>();
		test_shared_ptr(s_ptr);
	}
	

	{
		fightMediator mediator;
		player p(&mediator);
		monster m(&mediator);

		charactor* c0 = &p;
		charactor* c1 = &m;
		mediator.addCharactor(&p);
		mediator.addCharactor(&m);
		p.attack(&m);
	}
	//AB a{ 1,2,3,4,5,6 };

	//system("pause");

	Application::GetInstance().Run();
	return 0;
}